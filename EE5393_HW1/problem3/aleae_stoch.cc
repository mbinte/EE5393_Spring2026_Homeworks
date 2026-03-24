#include "aleae.h"

void
aleae_stoch(const biocr_t           biocr,
            const stoch_param_t     param,
                  vector<unsigned> &S,
                  stoch_stats_t    &stats)
{
   const vector<thresh_t> &T = biocr.T;
   const vector<react_t > &R = biocr.R;

   bool   done = false; 
   while (!done 
          && (param.time_lt < 0 || stats.time < param.time_lt)
          && (param.step_limit == 0 || stats.event_ct < param.step_limit)) {

      vector<double> a(R.size());
      double         A = 0;

      if (param.print & PRINT_STATES) {
         cout << "S [";
         for (unsigned j = 0; j < S.size(); j++) {
            cout << setw(3) << S[j];
            if (j < S.size() - 1) cout << ", ";
         }
         cout << "]" << endl;
      }
      
      done = true;
      for (unsigned i = 0; i < R.size(); i++) {
         
         a[i] = R[i].rate;
         
         bool possible = true;
         for (unsigned j = 0; possible && j < R[i].react.size(); j++) {
            
            unsigned x = S[R[i].react[j].first];
            unsigned c = R[i].react[j].second;
            
            if (x < c) {
               possible = false;
               a[i] = 0;
            } else {
               a[i] *= choose(x, c);
            }
         }
         if (possible) done = false;
         A += a[i];
      }

      if (param.print & PRINT_TRACE) {
         cout << "Pr [";
         for (unsigned j = 0; j < a.size(); j++) {
            cout << setw(3) << a[j];
            if (j < a.size() - 1) cout << ", ";
         }
         cout << "]" << endl;
      }
      
      if (done) {
         if (param.print & PRINT_TERMINAL) {
            cout << "no further reactions are possible" << endl;
         }
      } else {
         
         unsigned    i;
         double s = 0;
         double r = rand()/(double)RAND_MAX;
         
         bool found = false;
         for (unsigned j = 0; !found && j < a.size(); j++) {
            s += a[j]/A;
            if (s >= r) {
               found = true;
               i     = j;
            }
         }
         assert(found);
         if (param.print & PRINT_TRACE) {
            cout << "chose R" << i << endl;
         }

         if (param.time_lt >= 0) {
            r  = rand()/(double)RAND_MAX;
            stats.time += (-1/A)*log(r);
         }
         
         for (unsigned j = 0; j < R[i].delta.size(); j++) {
            unsigned k = R[i].delta[j].first;
            S[k] += R[i].delta[j].second;
         }
         
         for (unsigned i = 0; !done && i < T.size(); i++) {
            bool exceeds = false;
            switch(T[i].c) {
            case THRESH_LT:
                 if (S[T[i].i] < T[i].t) exceeds = true; 
                 break;
            case THRESH_LE:
                 if (S[T[i].i] <= T[i].t) exceeds = true; 
                 break;
            case THRESH_GE:
                 if (S[T[i].i] >= T[i].t) exceeds = true; 
                 break;
            case THRESH_GT:
                 if (S[T[i].i] > T[i].t) exceeds = true; 
                 break;
            default:
                 cerr << "error: invalid threshold code" << endl;
                 exit(1);
           }
           if (exceeds) {
               done = true;
               if (param.print & PRINT_STATES) {
                  cout << "S [";
                  for (unsigned j = 0; j < S.size(); j++) {
                     cout << setw(3) << S[j];
                     if (j < S.size() - 1) cout << ", ";
                  }
                  cout << "]" << endl;
               }
               if (param.print & PRINT_TERMINAL) {
                  cout << "state exceeds a threshold" << endl;
               }
            }
         }

         stats.event_ct++;
      }
   }
}
