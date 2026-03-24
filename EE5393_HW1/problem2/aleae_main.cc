// Problem 2: Lambda Phage - Stealth vs. Hijack Mode
// Sweeps MOI = 1..10 and reports probability of each outcome.

#include "aleae.h"

int main(int argc, char **argv) 
{
   if (argc < 4 || argc > 5) {
      cerr << "usage: " << argv[0] << " <state> <reactions> <trials> [<verbosity>]" << endl;
      cerr << "  Example: ./aleae lambda.in lambda.r 10000" << endl;
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
   param.time_lt    = -1;  // no time limit: simulation runs until threshold
   param.print      = (argc >= 5) ? atoi(argv[4]) : 0;
   param.step_limit = 0;

   vector<thresh_t> &T = biocr.T;
   vector<string>   &N = biocr.N;

   // Find the MOI species index
   int moi_index = -1;
   for (unsigned i = 0; i < biocr.N.size(); i++) {
      if (biocr.N[i] == "MOI") {
         moi_index = i;
         break;
      }
   }
   if (moi_index < 0) {
      cerr << "error: species 'MOI' not found in " << argv[1] << endl;
      exit(1);
   }

   cout << setiosflags(ios::fixed) << setprecision(4);
   cout << "==========================================================" << endl;
   cout << "Lambda Phage: Stealth vs. Hijack Mode" << endl;
   cout << "  Stealth mode: cI2 > 145" << endl;
   cout << "  Hijack  mode: Cro2 > 55" << endl;
   cout << "  Trials per MOI value: " << trials << endl;
   cout << "==========================================================" << endl;
   cout << endl;

   // Print header
   cout << setw(5) << "MOI";
   for (unsigned i = 0; i < T.size(); i++) {
      string label = N[T[i].i];
      switch(T[i].c) {
      case THRESH_LT: label += " < ";  break;
      case THRESH_LE: label += " <= "; break;
      case THRESH_GE: label += " >= "; break;
      case THRESH_GT: label += " > ";  break;
      }
      label += to_string(T[i].t);
      cout << setw(20) << label;
   }
   cout << endl;
   cout << string(5 + 20*T.size(), '-') << endl;

   struct timeval start_all;
   gettimeofday(&start_all, NULL);

   // Sweep MOI = 1 .. 10
   for (unsigned moi = 1; moi <= 10; moi++) {

      vector<unsigned> F(T.size(), 0);

      for (unsigned n = 0; n < trials; n++) {

         vector<unsigned> I = S;
         I[moi_index] = moi;

         stoch_stats_t stats;
         stats.event_ct = 0;
         stats.time     = 0;

         aleae_stoch(biocr, param, I, stats);

         // Check which thresholds were exceeded in final state
         for (unsigned i = 0; i < T.size(); i++) {
            bool exceeded = false;
            switch(T[i].c) {
            case THRESH_LT: if (I[T[i].i] <  T[i].t) exceeded = true; break;
            case THRESH_LE: if (I[T[i].i] <= T[i].t) exceeded = true; break;
            case THRESH_GE: if (I[T[i].i] >= T[i].t) exceeded = true; break;
            case THRESH_GT: if (I[T[i].i] >  T[i].t) exceeded = true; break;
            }
            if (exceeded) F[i]++;
         }
      }

      // Print row for this MOI value
      cout << setw(5) << moi;
      for (unsigned i = 0; i < T.size(); i++) {
         double pct = (F[i]/(double)trials)*100.0;
         cout << setw(17) << pct << "%  ";
      }
      cout << endl;
   }

   struct timeval end_all;
   gettimeofday(&end_all, NULL);
   unsigned micro = end_all.tv_sec*1000000 + end_all.tv_usec 
                  - start_all.tv_sec*1000000 + start_all.tv_usec;
   cout << endl;
   cout << "Total runtime: " << micro/1000000 << "." << (micro % 1000000)/1000 << "s" << endl;

   return 0;
}
