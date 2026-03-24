# EE 5393 Homework #1 — Build & Run Instructions

## Prerequisites

- Windows machine with **WSL** (Ubuntu) installed
- `g++` and `make` available inside WSL (run `sudo apt install g++ make` if not)

## How to Run

Open PowerShell and type `wsl` to enter the Linux shell, then run the commands below.

> **Note:** If `make` says "Nothing to be done" or gives errors, run `make clean` first to remove old build files, then run `make` again.

---

### Problem 1 — Analyzing Chemical Reaction Networks

```bash
cd /mnt/c/Users/User/OneDrive/Desktop/aleae/problem1
make

# 1(a): Threshold probabilities from state [110, 26, 55]
./aleae p1a.in p1a.r 100000 -1 0

# 1(b): Mean & variance after 7 steps from state [9, 8, 7]
./aleae p1b.in p1b.r 100000 -1 0 7
```

---

### Problem 2 — Lambda Phage Stealth vs. Hijack

```bash
cd /mnt/c/Users/User/OneDrive/Desktop/aleae/problem2
make

# Sweep MOI = 1..10 (100 trials takes ~2 min, increase for better accuracy)
./aleae lambda.in lambda.r 100
```

---

### Problem 3 — Synthesizing Chemical Reaction Networks

```bash
cd /mnt/c/Users/User/OneDrive/Desktop/aleae/problem3
make

# 3(a): Z = X * log2(Y)  |  X=5, Y=16, expected Z=20
./aleae p3a.in p3a.r 1000 100 0

# 3(b): Y = 2^(log2(X))  |  X=8, expected Y=8
./aleae p3b.in p3b.r 1000 500 0
```

---

### When finished, exit WSL

```bash
exit
```
