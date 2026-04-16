"""
EE 5393 HW#1 - Full Solution Verification
Problem 1: Bernstein polynomial synthesis
Problem 2: Probability transformation circuits
"""

from fractions import Fraction
from math import comb
from collections import deque

# ─────────────────────────────────────────────────────────
# PROBLEM 1: Bernstein Polynomial Synthesis
# ─────────────────────────────────────────────────────────

def bernstein_coeffs(power_coeffs):
    """
    Convert power-form coefficients a[0..n] to Bernstein coefficients b[0..n]
    using: b_{i,n} = sum_{j=0}^{i} C(i,j)/C(n,j) * a_j
    """
    n = len(power_coeffs) - 1
    bern = []
    for i in range(n + 1):
        b = Fraction(0)
        for j in range(i + 1):
            b += Fraction(comb(i, j), comb(n, j)) * Fraction(power_coeffs[j])
        bern.append(b)
    return bern

def eval_poly(coeffs, x):
    """Evaluate power-form polynomial at x."""
    return sum(c * x**i for i, c in enumerate(coeffs))

def eval_bernstein(bern, x):
    """Evaluate Bernstein polynomial at x."""
    n = len(bern) - 1
    return sum(bern[i] * comb(n, i) * x**i * (1-x)**(n-i) for i in range(n+1))

print("=" * 65)
print("PROBLEM 1(a):  f(x) = x - x²/4")
print("=" * 65)
# a0=0, a1=1, a2=-1/4
a_1a = [Fraction(0), Fraction(1), Fraction(-1, 4)]
b_1a = bernstein_coeffs(a_1a)
print(f"Power-form coefficients: {[str(c) for c in a_1a]}")
print(f"Bernstein coefficients (degree 2):")
for i, b in enumerate(b_1a):
    print(f"  β_{i} = {b} = {float(b):.4f}  {'✓ in [0,1]' if 0 <= b <= 1 else '✗ OUT OF RANGE'}")
print(f"Degree elevation needed: {'No' if all(0 <= b <= 1 for b in b_1a) else 'Yes'}")

# Verify at test points
print("\nVerification:")
for x_val in [0, 0.25, 0.5, 0.75, 1.0]:
    fval = x_val - x_val**2/4
    bval = sum(float(b_1a[i]) * comb(2,i) * x_val**i * (1-x_val)**(2-i) for i in range(3))
    print(f"  f({x_val}) = {fval:.6f}, Bernstein = {bval:.6f}, match = {abs(fval-bval)<1e-9}")

print("\nCircuit description:")
print("  2 independent x-streams → bit counter → 3-way MUX")
print(f"  count=0 selects: constant 0  (β₀ = {b_1a[0]})")
print(f"  count=1 selects: stream P=1/2 (β₁ = {b_1a[1]})")
print(f"  count=2 selects: stream P=3/4 (β₂ = {b_1a[2]})")

print("\n" + "=" * 65)
print("PROBLEM 1(b):  Approximate cos(x), x ∈ [0,1]")
print("=" * 65)
import math
# Use degree-2 approximation: cos(x) ≈ 1 - x²/2
# Power coefficients: a0=1, a1=0, a2=-1/2
a_1b = [Fraction(1), Fraction(0), Fraction(-1, 2)]
b_1b = bernstein_coeffs(a_1b)
print("Approximation used: cos(x) ≈ 1 - x²/2  (degree-2 Maclaurin)")
print(f"Bernstein coefficients (degree 2):")
for i, b in enumerate(b_1b):
    print(f"  β_{i} = {b} = {float(b):.4f}  {'✓ in [0,1]' if 0 <= b <= 1 else '✗ OUT OF RANGE'}")

print("\nVerification (approx vs exact):")
for x_val in [0, 0.25, 0.5, 0.75, 1.0]:
    approx = 1 - x_val**2/2
    exact  = math.cos(x_val)
    bval   = sum(float(b_1b[i]) * comb(2,i) * x_val**i * (1-x_val)**(2-i) for i in range(3))
    print(f"  x={x_val}: exact={exact:.5f}, approx(1-x²/2)={approx:.5f}, Bernstein={bval:.5f}")

print("\nCircuit description:")
print("  2 independent x-streams → bit counter → 3-way MUX")
print(f"  count=0 selects: constant 1  (β₀={b_1b[0]})")
print(f"  count=1 selects: constant 1  (β₁={b_1b[1]})")
print(f"  count=2 selects: stream P=1/2 (β₂={b_1b[2]})")

print("\n" + "=" * 65)
print("PROBLEM 1(c): p(t) = 31t⁵/32 + 5t⁴/32 - 5t³/8 + 5t²/4 - 5t/4 + 1/2")
print("=" * 65)
# a0=1/2, a1=-5/4, a2=5/4, a3=-5/8, a4=5/32, a5=31/32
a_1c = [Fraction(1,2), Fraction(-5,4), Fraction(5,4),
        Fraction(-5,8), Fraction(5,32), Fraction(31,32)]
b_1c = bernstein_coeffs(a_1c)
print("Power-form coefficients:")
for i, a in enumerate(a_1c):
    print(f"  a_{i} = {a}")
print(f"\nBernstein coefficients (degree 5):")
all_valid = True
for i, b in enumerate(b_1c):
    valid = 0 <= b <= 1
    if not valid: all_valid = False
    print(f"  β_{i} = {b} = {float(b):.6f}  {'✓' if valid else '✗'}")
print(f"\nAll coefficients in [0,1]: {'YES - no degree elevation needed' if all_valid else 'NO - elevation required'}")

print("\nDemonstration at test points:")
test_pts = [0, 0.25, 0.5, 0.75, 1.0]
for t in test_pts:
    poly = sum(float(a_1c[i]) * t**i for i in range(6))
    bern = sum(float(b_1c[i]) * comb(5,i) * t**i * (1-t)**(5-i) for i in range(6))
    print(f"  t={t:.2f}: p(t) = {poly:.6f}, Bernstein circuit output ≈ {bern:.6f}")

print("\nCircuit description:")
print("  5 independent t-streams → bit counter (0..5) → 6-way MUX")
for i, b in enumerate(b_1c):
    src = f"constant {float(b):.4f}" if b in (0, 1) else f"stream P={b} = {float(b):.5f}"
    print(f"  count={i} selects: {src}  (β_{i}={b})")

# ─────────────────────────────────────────────────────────
# PROBLEM 2(a): AND/NOT circuits from {0.4, 0.5}
# ─────────────────────────────────────────────────────────

print("\n" + "=" * 65)
print("PROBLEM 2(a): Targets from S = {0.4, 0.5}")
print("=" * 65)

def find_circuit(target, max_depth=8):
    """
    BFS to find shortest circuit reaching target using AND and NOT
    from sources {0.4, 0.5} with arbitrary copies of each.
    Returns (value, description_string)
    """
    target = round(target, 7)
    # BFS: state = set of reachable values
    # Each node: (value, expression_string)
    # sources are infinitely available
    sources = {0.4: "0.4", 0.5: "0.5"}
    
    # Round to 7 decimal places for comparison
    def r(x): return round(x, 7)
    
    # BFS queue: list of (value, description) reachable so far
    # We build up reachable set layer by layer
    reachable = {r(0.4): "0.4", r(0.5): "0.5", r(0.6): "NOT(0.4)", r(0.5): "0.5"}
    # Compute a few basics
    base = {}
    base[r(0.4)] = "0.4"
    base[r(0.5)] = "0.5"
    base[r(0.6)] = "NOT(0.4)"
    # 0.5 NOT = 0.5 (same)
    base[r(0.2)] = "AND(0.4, 0.5)"
    base[r(0.8)] = "NOT(AND(0.4, 0.5))"
    base[r(0.3)] = "AND(0.5, NOT(0.4))"
    base[r(0.7)] = "NOT(AND(0.5, NOT(0.4)))"
    base[r(0.25)] = "AND(0.5, 0.5)"
    base[r(0.75)] = "NOT(AND(0.5, 0.5))"
    base[r(0.16)] = "AND(0.4, 0.4)"
    base[r(0.84)] = "NOT(AND(0.4, 0.4))"
    
    # Check if target already in base
    if r(target) in base:
        return base[r(target)]
    
    # Layer 2: AND and NOT of base values
    layer2 = {}
    vals = list(base.keys())
    for i, v1 in enumerate(vals):
        nv = r(1 - v1)
        if nv not in base and nv not in layer2:
            layer2[nv] = f"NOT({base[v1]})"
        for j, v2 in enumerate(vals):
            av = r(v1 * v2)
            if av not in base and av not in layer2:
                layer2[av] = f"AND({base[v1]}, {base[v2]})"
            nav = r(1 - v1 * v2)
            if nav not in base and nav not in layer2:
                layer2[nav] = f"NOT(AND({base[v1]}, {base[v2]}))"
    
    all_known = {**base, **layer2}
    if r(target) in all_known:
        return all_known[r(target)]
    
    # Layer 3
    layer3 = {}
    vals2 = list(all_known.keys())
    for v1 in vals2:
        for v2 in vals2:
            av = r(v1 * v2)
            nav = r(1 - v1 * v2)
            if av not in all_known and av not in layer3:
                layer3[av] = f"AND({all_known[v1]}, {all_known[v2]})"
            if nav not in all_known and nav not in layer3:
                layer3[nav] = f"NOT(AND({all_known[v1]}, {all_known[v2]}))"
        nv = r(1 - v1)
        if nv not in all_known and nv not in layer3:
            layer3[nv] = f"NOT({all_known[v1]})"
    
    all_known2 = {**all_known, **layer3}
    if r(target) in all_known2:
        return all_known2[r(target)]
    
    return "Not found in depth-3 search"

# For the cascade algorithm from slides, implement directly
def cascade_circuit(target):
    """
    Implement the cascade algorithm from the slides.
    Returns step-by-step derivation.
    """
    # Use the decimal digit approach: express as continued AND/NOT chain
    target = round(target, 7)
    
    # The algorithm: 
    # p > 0.5: NOT(AND(0.4, NOT_inner)) or NOT(AND(0.5, NOT_inner))
    # p < 0.5: AND(0.4, ...) or AND(0.5, ...)
    # Adapted from the slides
    
    ops = []
    p = target
    steps = []
    
    for _ in range(12):
        if abs(p - round(p, 0)) < 1e-7:
            break
        if p > 0.5:
            steps.append(('NOT', None))
            p = 1 - p
        elif p <= 0.5:
            # Try dividing by 0.5 or 0.4
            p05 = p / 0.5
            p04 = p / 0.4
            if abs(p05 - round(p05, 7)) < abs(p04 - round(p04, 7)) and p05 <= 1:
                steps.append(('AND', 0.5))
                p = p05
            elif p04 <= 1:
                steps.append(('AND', 0.4))
                p = p04
            else:
                steps.append(('NOT', None))
                p = 1 - p
    
    return steps

print("\nTarget: 0.8881188")
print("Target: 0.2119209")
print("Target: 0.5555555")

# Build systematic circuits using the cascade approach from slides
# For these specific values, we trace the algorithm

targets_2a = [
    (0.8881188, "i"),
    (0.2119209, "ii"),
    (0.5555555, "iii"),
]

# Manual construction using AND/NOT cascade
def build_cascade(target, verbose=True):
    """Build the cascade circuit step by step."""
    t = target
    steps = []
    
    for iteration in range(20):
        t = round(t, 9)
        if abs(t - round(t)) < 1e-7:
            break
        
        if t > 0.5:
            steps.append(('NOT', t, 1-t))
            t = 1 - t
        else:
            # Try ×0.5 or ×0.4
            if abs(round(t/0.5, 9) - round(round(t/0.5,9))) > 1e-8 and t/0.5 <= 1:
                new_t = round(t/0.5, 9)
                steps.append(('AND_0.5', t, new_t))
                t = new_t
            elif t/0.4 <= 1:
                new_t = round(t/0.4, 9)
                steps.append(('AND_0.4', t, new_t))
                t = new_t
            else:
                steps.append(('NOT', t, 1-t))
                t = 1 - t
    
    return steps, t

for target, label in targets_2a:
    print(f"\n{label}. Target = {target}")
    steps, final = build_cascade(target)
    print(f"   Cascade steps (reverse reading gives circuit):")
    for step in steps:
        if step[0] == 'NOT':
            print(f"     {step[1]:.7f} → NOT → {step[2]:.7f}")
        else:
            gate = '×0.5' if '0.5' in step[0] else '×0.4'
            print(f"     {step[1]:.7f} → AND({gate[1:]}) → {step[2]:.7f}")
    print(f"   Terminal value reached: {final:.7f}")

# For clean circuit descriptions, use these known constructions:
print("\n" + "-"*55)
print("CLEAN CIRCUIT CONSTRUCTIONS:")

# 0.5555555 ≈ 5/9:  Using repeated NOT/AND
# Let's find this by building from simple operations
def explore(depth=4):
    """Enumerate all probabilities reachable within given depth."""
    from fractions import Fraction
    
    sources = {Fraction(2,5): "0.4", Fraction(1,2): "0.5"}
    reachable = dict(sources)
    
    for d in range(depth):
        new_vals = {}
        curr_vals = list(reachable.items())
        
        for v1, e1 in curr_vals:
            # NOT
            nv = 1 - v1
            if nv not in reachable and nv not in new_vals:
                new_vals[nv] = f"NOT({e1})"
        
        for v1, e1 in curr_vals:
            for v2, e2 in curr_vals:
                # AND
                av = v1 * v2
                if av not in reachable and av not in new_vals:
                    new_vals[av] = f"AND({e1},{e2})"
        
        reachable.update(new_vals)
    
    return reachable

print("\nSearching for exact or near circuits using Fraction arithmetic...")
reachable = explore(depth=3)

# Find closest to each target
for target, label in targets_2a:
    tf = Fraction(target).limit_denominator(10**8)
    if tf in reachable:
        print(f"\n{label}. {target}: EXACT: {reachable[tf]}")
    else:
        # Find closest
        closest = min(reachable.keys(), key=lambda x: abs(float(x) - target))
        diff = abs(float(closest) - target)
        print(f"\n{label}. {target}: Closest exact = {float(closest):.7f} "
              f"(diff={diff:.2e}): {reachable[closest]}")
        # Additional step to reach exact target
        # For 5/9: need AND with something; 
        # 5/9 = AND(5/9's_source, ...)

print("\n" + "=" * 65)
print("PROBLEM 2(b): Targets from S = {0.5}")
print("=" * 65)

def binary_prob_circuit(binary_str):
    """
    Implement binary probability 0.b1b2...bn using only {0.5}.
    Algorithm (from slides): scan bits right-to-left.
    - bit=1: p_new = NOT(AND(0.5, NOT(p_prev))) = 0.5 + 0.5*p_prev
    - bit=0: p_new = AND(0.5, p_prev) = 0.5 * p_prev
    Start from p_prev = 0 (before MSB, but after LSB)
    """
    # Parse binary fraction: 0.b1b2...bn
    bits = [int(b) for b in binary_str]  # e.g. [1,0,1,1,1,1,1]
    
    print(f"\n  Binary: 0.{''.join(str(b) for b in bits)}")
    decimal = sum(b * (0.5)**(i+1) for i, b in enumerate(bits))
    print(f"  Decimal value: {decimal:.7f} = {sum(b*2**(len(bits)-1-i) for i,b in enumerate(bits))}/{2**len(bits)}")
    
    # Build circuit reading bits from RIGHT (LSB) to LEFT (MSB)
    p = 0.0
    expr = "0"
    
    print(f"  Circuit construction (right-to-left, LSB first):")
    for i, bit in enumerate(reversed(bits)):
        old_p = p
        old_expr = expr
        if bit == 1:
            p = 0.5 + 0.5 * p
            expr = f"NOT(AND(0.5, NOT({old_expr})))"
            op = "bit=1: NOT(AND(0.5, NOT(·)))"
        else:
            p = 0.5 * p
            expr = f"AND(0.5, {old_expr})"
            op = "bit=0: AND(0.5, ·)"
        bit_pos = len(bits) - i  # MSB=1, LSB=n
        print(f"    b_{bit_pos} = {bit}: {old_p:.6f} → {op} → {p:.6f}")
    
    print(f"  Final: p = {p:.7f}")
    print(f"  Circuit: {expr}")
    return p, expr

print("\ni.  0.1011111₂")
p1, e1 = binary_prob_circuit([1,0,1,1,1,1,1])

print("\nii.  0.1101111₂")
p2, e2 = binary_prob_circuit([1,1,0,1,1,1,1])

print("\niii. 0.1010111₂")
p3, e3 = binary_prob_circuit([1,0,1,0,1,1,1])

print("\n" + "=" * 65)
print("Summary of binary probability circuits")
print("=" * 65)
for label, bits, p, e in [
    ("i",   [1,0,1,1,1,1,1], p1, e1),
    ("ii",  [1,1,0,1,1,1,1], p2, e2),
    ("iii", [1,0,1,0,1,1,1], p3, e3),
]:
    decimal = sum(b * 0.5**(i+1) for i,b in enumerate(bits))
    numer   = sum(b * 2**(len(bits)-1-i) for i,b in enumerate(bits))
    denom   = 2**len(bits)
    print(f"  {label}. 0.{''.join(str(b) for b in bits)}₂ = {numer}/{denom} = {decimal:.7f}, circuit output = {p:.7f}")
