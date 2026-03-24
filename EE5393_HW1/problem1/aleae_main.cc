// Problem 1: Analyzing Chemical Reaction Networks
// 1(a): Estimate Pr(C1), Pr(C2), Pr(C3) with thresholds
// 1(b): Mean and variance after 7 steps (pass step count as 6th argument)

#include "aleae.h"

int main(int argc, char **argv) 
{
   if (argc < 6 || argc > 7) {
      cerr << "usage: " << argv[0] << " <state> <reactions> <trials> <time> <verbosity> [<steps>]" << endl;
      cerr << "  For Problem 1(a): ./aleae p1a.in p1a.r 100000 -1 0" << endl;
      cerr << "  For Problem 1(b): ./aleae p1b.in p1b.r 100000 -1 0 7" << endl;
      exit(1);
   } 

   srand(time(NULL));

   ifstream file1(argv[1]);
   if (!file1) {
      cerr << "could not open file " << argv[1] << endl;
      exit(1);
   } 

   ifstream file2(argv[2]);
   if (!file2) {
      cerr << "could not open file " << argv[2] << endl;
      exit(1);
   } 

   vector<unsigned> S;
   biocr_t          biocr;
   
   if (!aleae_initial_in(file1, biocr.N, S, biocr.T)) {
      cerr << "error reading initial state from file " << argv[1] << endl;
      exit(1);
   }

   if (!aleae_reactions_in(file2, biocr.R, biocr.N)) {
      cerr << "error reading reactions from file " << argv[2] << endl;
      exit(1);
   }

   unsigned event_ct = 0;
   double   sim_time = 0;

   cout << setiosflags(ios::fixed) << setprecision(4);
   cout << "Initial Quantities and Thresholds" << endl;
   aleae_initial_out  (biocr.N, S, biocr.T);
   cout << endl;
   cout << "Reactions" << endl;
   aleae_reactions_out(biocr.N, biocr.R);
   cout << endl;

   struct timeval start1;
   gettimeofday(&start1, NULL);

   vector<thresh_t> &T = biocr.T;
   vector<string >  &N = biocr.N;
   vector<unsigned>  F(T.size(), 0);
   unsigned          trials = atoi(argv[3]);

   stoch_param_t param;
   param.time_lt    = atof(argv[4]);
   param.print      = atoi(argv[5]);
   param.step_limit = (argc >= 7) ? atoi(argv[6]) : 0;

   stoch_stats_t stats;

   // Accumulators for mean and variance
   vector<double> sum(S.size(), 0.0);
   vector<double> sum_sq(S.size(), 0.0);

   for (unsigned n = 0; n < trials; n++) {
      
      if (param.print & PRINT_TRIALS) {
         cout << "trial " << n << endl;
      }

      vector<unsigned> I = S;
      stats.event_ct     = 0;
      stats.time         = 0;
            
      aleae_stoch(biocr, param, I, stats);
            
      event_ct += stats.event_ct;
      sim_time += stats.time;
            
      for (unsigned i = 0; i < S.size(); i++) {
         sum[i]    += I[i];
         sum_sq[i] += (double)I[i] * I[i];
      }
            
      for (unsigned i = 0; i < T.size(); i++) {
         switch(T[i].c) {
         case THRESH_LT:
           if (I[T[i].i] <  T[i].t) F[i]++;
           break;
         case THRESH_LE:
           if (I[T[i].i] <= T[i].t) F[i]++;
           break;
         case THRESH_GE:
           if (I[T[i].i] >= T[i].t) F[i]++;
           break;
         case THRESH_GT:
           if (I[T[i].i] >  T[i].t) F[i]++;
           break;
         default:
           cout << "error: invalid threshold code" << endl;
           exit(1);
         }  
      }
   }
         
   cout << endl << "===== Simulation Results (" << trials << " trials) =====" << endl;
   cout << endl;

   // Print averages
   cout << "Average final state: [";
   for (unsigned i = 0; i < S.size(); i++) {
      cout << sum[i]/(double)trials;
      if (i < S.size() - 1) cout << ", ";
   }
   cout << "]" << endl;

   // Print threshold probabilities (Problem 1a)
   if (T.size() > 0) {
      cout << endl << "Threshold Probabilities:" << endl;
      for (unsigned i = 0; i < T.size(); i++) {
         cout << "  " << N[T[i].i];
         switch(T[i].c) {
         case THRESH_LT: cout << " <  "; break;
         case THRESH_LE: cout << " <= "; break;
         case THRESH_GE: cout << " >= "; break;
         case THRESH_GT: cout << " >  "; break;
         }  
         cout << T[i].t << ": " << F[i] << "/" << trials
              << " (" << (F[i]/(double)trials)*100 << "%)" << endl;
      }
   }

   // Print mean and variance (especially useful for Problem 1b)
   if (param.step_limit > 0) {
      cout << endl << "Mean and Variance after " << param.step_limit << " steps:" << endl;
      for (unsigned i = 0; i < S.size(); i++) {
         double mean = sum[i] / (double)trials;
         double var  = sum_sq[i] / (double)trials - mean * mean;
         cout << "  " << N[i] << ": mean = " << mean << ", variance = " << var << endl;
      }
   }

   cout << endl;
   cout << "avg events/trial: " << event_ct/(double)trials << endl;

   struct timeval end1;
   gettimeofday(&end1, NULL);
   unsigned micro1 = end1.tv_sec*1000000 + end1.tv_usec - start1.tv_sec*1000000 + start1.tv_usec;
   cout << "total runtime: " << micro1/1000000 << "." << (micro1 % 1000000)/1000 << "s" << endl;

   return 0;
}
