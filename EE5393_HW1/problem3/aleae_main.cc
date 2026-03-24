// Problem 3: Synthesizing Chemical Reaction Networks
// 3(a): Z = X * log2(Y)     -- run with: ./aleae p3a.in p3a.r <trials> <time> 0
// 3(b): Y = 2^(log2(X))     -- run with: ./aleae p3b.in p3b.r <trials> <time> 0

#include "aleae.h"

int main(int argc, char **argv) 
{
   if (argc < 6 || argc > 6) {
      cerr << "usage: " << argv[0] << " <state> <reactions> <trials> <time> <verbosity>" << endl;
      cerr << "  Problem 3(a): ./aleae p3a.in p3a.r 1000 100 0" << endl;
      cerr << "  Problem 3(b): ./aleae p3b.in p3b.r 1000 500 0" << endl;
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

   unsigned trials = atoi(argv[3]);

   stoch_param_t param;
   param.time_lt    = atof(argv[4]);
   param.print      = atoi(argv[5]);
   param.step_limit = 0;

   stoch_stats_t stats;

   cout << setiosflags(ios::fixed) << setprecision(4);
   cout << "Initial Quantities:" << endl;
   for (unsigned i = 0; i < biocr.N.size(); i++) {
      if (S[i] > 0) {
         cout << "  " << biocr.N[i] << " = " << S[i] << endl;
      }
   }
   cout << endl;
   cout << "Reactions:" << endl;
   aleae_reactions_out(biocr.N, biocr.R);
   cout << endl;

   struct timeval start1;
   gettimeofday(&start1, NULL);

   vector<double> sum(S.size(), 0.0);
   vector<double> sum_sq(S.size(), 0.0);

   for (unsigned n = 0; n < trials; n++) {

      if (param.print & PRINT_TRIALS) {
         cout << "trial " << n << endl;
      }

      vector<unsigned> I = S;
      stats.event_ct = 0;
      stats.time     = 0;

      aleae_stoch(biocr, param, I, stats);

      for (unsigned i = 0; i < S.size(); i++) {
         sum[i]    += I[i];
         sum_sq[i] += (double)I[i] * I[i];
      }
   }

   cout << "===== Results (" << trials << " trials, time limit = " 
        << param.time_lt << ") =====" << endl;
   cout << endl;

   cout << setw(12) << "Species" << setw(12) << "Initial" 
        << setw(12) << "Mean" << setw(14) << "Variance" << endl;
   cout << string(50, '-') << endl;

   for (unsigned i = 0; i < S.size(); i++) {
      double mean = sum[i] / (double)trials;
      double var  = sum_sq[i] / (double)trials - mean * mean;
      cout << setw(12) << biocr.N[i] 
           << setw(12) << S[i]
           << setw(12) << mean 
           << setw(14) << var << endl;
   }

   struct timeval end1;
   gettimeofday(&end1, NULL);
   unsigned micro1 = end1.tv_sec*1000000 + end1.tv_usec 
                    - start1.tv_sec*1000000 + start1.tv_usec;
   cout << endl;
   cout << "Total runtime: " << micro1/1000000 << "." << (micro1 % 1000000)/1000 << "s" << endl;

   return 0;
}
