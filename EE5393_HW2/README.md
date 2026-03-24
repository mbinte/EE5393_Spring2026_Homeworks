# EE 5393 — Homework 2

## Build

From the `vanilla/` directory:

```bash
make
```

This produces the `aleae` executable.

---

## Problem 1: Fibonacci (12 steps)

### Files

| File | Description |
|------|-------------|
| `fib12_01.in` | Species & initial conditions for starting values A=0, B=1 |
| `fib12_01.r`  | Reactions (same logic as fib12_37.r) |
| `fib12_37.in` | Species & initial conditions for starting values A=3, B=7 |
| `fib12_37.r`  | Reactions (same logic as fib12_01.r) |

### How to run

```bash
./aleae fib12_01.in fib12_01.r 200 -1 0
./aleae fib12_37.in fib12_37.r 200 -1 0
```

### Design

Each Fibonacci step computes `A_new = B_old`, `B_new = A_old + B_old` using 4 sequential phases:

| Phase | Reaction | Purpose |
|-------|----------|---------|
| 1 | A → TA | Save old A into temp TA |
| 2 | B → A + C | New A = old B; also copy old B into C |
| 3 | TA → C | Add old A into C, so C = old A + old B |
| 4 | C → B | New B = old A + old B |

12 Fibonacci steps × 4 phases = 48 phase tokens (S0 through S47).
Simulation stops when S48 ≥ 1.

Phase-advance reactions have rate 1 (slow), while transfer reactions have rate 10000 (fast).
This ensures each transfer completes before the next phase begins.

### Expected outputs

| Starting values | A after 12 steps | B after 12 steps |
|-----------------|-------------------|-------------------|
| A=0, B=1        | **144**           | 233               |
| A=3, B=7        | **1275**          | 2063              |

Look at the `avg [...]` line in Aleae's output. The first value is A, the second is B.

---

## Problem 2: Biquad Filter (5 cycles)

### Files

| File | Description |
|------|-------------|
| `biquad.in` | Species & initial conditions (inputs scaled ×16) |
| `biquad.r`  | Reactions for 5 filter cycles |

### How to run

```bash
./aleae biquad.in biquad.r 5 -1 0
```

(Per the professor's correction: run 5 times, not 10.)

### Design

The biquad filter implements the recurrence:

```
y[n] = ( x[n] + x[n-1] + x[n-2] + y[n-1] + y[n-2] ) / 8
```

All signal values are **scaled by ×16** so that the divide-by-8 step produces
exact integers (no fractional rounding in Aleae's integer molecule counts).

Each of the 5 cycles uses **5 sequential phases**:

| Phase | Purpose |
|-------|---------|
| Accumulate | Load input Xin, add x[n] + x[n-1] + x[n-2] + y[n-1] + y[n-2] into SUM |
| Divide     | SUM ÷ 8 → Y (consume 8 SUM molecules, produce 1 Y molecule) |
| Copy       | Y → Ycopy + Ykeep (destructive split into two separate species) |
| Output + Shift | Ycopy → Yout; Ykeep → Y1; shift delay registers (X0k→X1, X1k→X2, Y1k→Y2) |
| Cleanup    | Consume leftover X2k, Y2k, SUM remainder, and any residuals |

5 cycles × 5 phases = 25 phase tokens (C0 through C24).
Simulation stops when C25 ≥ 1.

### Reading the output

The `avg [...]` line reports average molecule counts. The species order matches `biquad.in`.
Find the Yout1–Yout5 values and **divide each by 16** to get the actual filter output:

| Cycle | X (input) | Expected Yout (×16) | **y = Yout / 16** |
|-------|-----------|---------------------|--------------------|
| 1     | 100       | 200                 | **12.50**          |
| 2     | 5         | 235                 | **14.69**          |
| 3     | 500       | 1264                | **79.00**          |
| 4     | 20        | 1237                | **77.31**          |
| 5     | 250       | 1852                | **115.75**         |

---

## Aleae command reference

```
./aleae <file1> <file2> <trials> <time> <verbosity>
```

| Argument | Meaning |
|----------|---------|
| `file1`  | `.in` file — species names, initial quantities, stopping thresholds |
| `file2`  | `.r` file — reactions (reactants, products, rate constants) |
| `trials` | Number of independent simulation runs |
| `time`   | Time limit per trial (`-1` = no limit, run until a threshold is hit) |
| `verbosity` | 0 = summary only, 15 = full trace of every reaction |
