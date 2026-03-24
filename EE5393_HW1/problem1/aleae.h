#ifndef __ALEAE_H
#define __ALEAE_H

#include <cstdlib>
#include <cmath>
#include <ctime>
#include <sys/time.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <utility>
#include <algorithm>

#include <queue>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <stack>
#include <assert.h>

using namespace std;

#include "comb.h"

#define MAXLINE 1024

class react_t 
{
public:
   vector< pair<unsigned, unsigned> > react;
   vector< pair<unsigned, int     > > delta;
   vector< unsigned                 > depend;
   double                             rate;
};

#define THRESH_LT 0
#define THRESH_LE 1
#define THRESH_GE 2
#define THRESH_GT 3

class thresh_t 
{
public:
   unsigned i;
   unsigned t;
   unsigned c;
};

class biocr_t 
{
public:
   vector<string>   N;
   vector<react_t>  R;
   vector<thresh_t> T;
};

#define PRINT_TRIALS     1   
#define PRINT_TERMINAL   2   
#define PRINT_TRACE      4 
#define PRINT_STATES     8 

class stoch_param_t
{
public:
   double   time_lt;
   unsigned print;
   unsigned step_limit;  // 0 = no limit; >0 = stop after this many steps
};

class stoch_stats_t
{
public:
   unsigned event_ct;
   double   time;
   thresh_t F;
};

bool                                         
aleae_initial_in(ifstream           &file,
                 vector  <string  > &N, 
                 vector  <unsigned> &S, 
                 vector  <thresh_t> &T);

bool                                         
aleae_reactions_in(      ifstream        &file, 
                         vector<react_t> &R,
                   const vector<string > &N);   

void
aleae_initial_out(const vector<string  > &N,
                  const vector<unsigned> &S,
                  const vector<thresh_t> &T);

ostream &operator<<(ostream &os, const vector<unsigned> &S);

void
aleae_reactions_out(const vector<string > &N,
                    const vector<react_t> &R); 

void
aleae_stoch(const biocr_t        biocr, 
            const stoch_param_t  param, 
                  vector<unsigned> &S,     
                  stoch_stats_t    &stats); 

#endif
