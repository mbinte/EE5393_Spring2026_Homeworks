#!/bin/bash
# Problem 1: Analyzing Chemical Reaction Networks
# Reactions:
#   R1: 2X1 + X2 -> 4X3,  k1 = 1
#   R2: X1 + 2X3 -> 3X2,  k2 = 2
#   R3: X2 + X3  -> 2X1,  k3 = 3

echo "=============================================="
echo "Problem 1(a): Estimate Pr(C1), Pr(C2), Pr(C3)"
echo "  Initial state: S = [X1=110, X2=26, X3=55]"
echo "  C1: X1 >= 150"
echo "  C2: X2 < 10"
echo "  C3: X3 > 100"
echo "=============================================="
echo ""
./aleae p1a.in p1a.r 100000 -1 0

echo ""
echo ""
echo "=============================================="
echo "Problem 1(b): Mean and Variance after 7 steps"
echo "  Initial state: S = [X1=9, X2=8, X3=7]"
echo "  Running 100000 trials, each for exactly 7 steps"
echo "=============================================="
echo ""
./aleae p1b.in p1b.r 100000 -1 0 7
