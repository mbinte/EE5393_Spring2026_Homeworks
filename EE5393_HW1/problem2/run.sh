#!/bin/bash
# Problem 2: Lambda Phage - Stealth vs. Hijack Mode
# Sweeps MOI = 1..10 and reports probability of each outcome.
# Stealth mode: cI2 > 145
# Hijack  mode: Cro2 > 55
#
# The lambda model is complex (~60 species, ~118 reactions), so each
# trial takes significant time. Adjust trial count based on patience:
#   100 trials  -> ~2 minutes   (rough estimates)
#   500 trials  -> ~10 minutes  (decent estimates)
#   1000 trials -> ~20 minutes  (good estimates)

echo "Running lambda phage simulation..."
echo ""
./aleae lambda.in lambda.r 100
