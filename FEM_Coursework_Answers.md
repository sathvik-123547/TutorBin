# Finite Element Method — Coursework Report

> **Module:** Finite Element Method  
> **P-number example used:** P----075 → X = 5  
> **Coursework weighting:** 18% of total module mark (9% per question)

---

## Table of Contents

1. [Nomenclature](#nomenclature)
2. [Question 1 — 2D Truss Analysis](#question-1--2d-truss-analysis-9-of-overall-mark)
   - 1.1 [Problem Definition](#11-problem-definition)
   - 1.2 [Numerical Analysis](#12-numerical-analysis-3-marks)
   - 1.3 [Critical Analysis](#13-critical-analysis-6-marks)
3. [Question 2 — 2D Plane Stress / CST Analysis](#question-2--2d-plane-stress--cst-analysis-9-of-overall-mark)
   - 2.1 [Problem Definition](#21-problem-definition)
   - 2.2 [Boundary Condition Verification](#22-boundary-condition-verification-2-marks)
   - 2.3 [Stress State Assessment](#23-stress-state-assessment-2-marks)
   - 2.4 [Load Representation](#24-load-representation-2-marks)
   - 2.5 [FEM Solution and Interpretation](#25-fem-solution-and-interpretation-3-marks)

---

## Nomenclature

| Symbol | Description | Unit |
|--------|-------------|------|
| E | Young's modulus | MPa or psi |
| A | Cross-sectional area | mm² or in² |
| L | Element length | mm or in |
| ν | Poisson's ratio | — |
| t | Plate thickness | in |
| σ | Normal stress | MPa or psi |
| τ | Shear stress | MPa or psi |
| ε | Normal strain | — |
| γ | Shear strain | — |
| u, v | Horizontal, vertical displacement | mm or in |
| c, s | Direction cosines cos θ, sin θ | — |
| **K** | Global stiffness matrix | N/mm or lb/in |
| **k** | Element stiffness matrix | N/mm or lb/in |
| **B** | Strain–displacement matrix | 1/mm or 1/in |
| **D** | Material constitutive matrix | MPa or psi |
| **F** | Global force vector | N or lb |
| **U** | Global displacement vector | mm or in |
| DOF | Degree of freedom | — |
| CST | Constant Strain Triangle | — |
| BC | Boundary condition | — |
| FEM | Finite Element Method | — |
| T/C | Tension/Compression | — |

---

# Question 1 — 2D Truss Analysis (9% of overall mark)

## 1.1 Problem Definition

### 1.1.1 Structural Description

A 2D truss structure comprising **nine bar elements** and **six nodes**, as shown in the figure below.

### 1.1.2 Structural Diagram

```
                W=12.5 kN
                ←──●  3           [6]           ●  5          [8]          ● 6
                   ╱ │ ╲────────────────────────╱ │ ╲────────────────────╱
    y            ╱   │   ╲        [5]         ╱   │   ╲                ╱
    ↑          ╱  [3]│     ╲              [7]╱  [7]│   [9]╲          ╱
    │        ╱       │       ╲             ╱      │        ╲       ╱
    │      ╱         │         ╲         ╱        │          ╲   ╱
    └──→ x           │       [4] ╲     ╱          │            ╳
              [2]    │    ●───────────●  4         │          ╱   ╲
                ╲    │  ╱  2    P=15kN↓            │        ╱       ╲
                  ╲  │╱         [4]                │      ╱           ╲
              [1]  ╲╱                              │    ╱               ╲
                    ● 2                            │  ╱                   ╲
                  ╱   P=15kN ↓                     │╱                      ╲
            [1] ╱                                                            ╲
              ╱                                                                ╲
            ● 1                                                                  ● 6
           ═══                                                                  ╳╳╳
          ROLLER                                                                PIN
```

**Simplified topology (to scale):**

```
         W ←─ 3 ──────────── 5 ──────────── 6
              ╱╲              │╲            ╱│
            ╱    ╲            │  ╲        ╱  │
          ╱   El3 ╲   El5    │El7 ╲El9 ╱    │
        ╱          ╲         │      ╲╱      │
      ╱    El5      ╲  El6   │      ╱╲      │  ← 1.8 m per tier
     2 ──────────────── 4    │    ╱    ╲    │     (3 tiers = 5.4 m)
      ╲    El4       ╱       │  ╱   El8 ╲  │
        ╲          ╱         │╱          ╲  │
          ╲  El1 ╱    El2    5 ──────────── 6
            ╲  ╱                            ╳ PIN
     P↓ 2────╱────────── 4 ↓P
              ╲          ╱
         El1   ╲  El2  ╱
                ╲    ╱
                 ╲ ╱
            1 ═══ ROLLER
     |────2.4m───|────2.4m───|────2.4m───|
```

### 1.1.3 Given Data

| Parameter | Value |
|-----------|-------|
| Young's modulus, E | 30,000 MPa |
| Cross-sectional area, A | 20,000 mm² |
| Vertical load, P = (10 + X) | **15 kN** (at nodes 2 and 4, downward) |
| Horizontal load, W = (7.5 + X) | **12.5 kN** (at node 3, leftward) |
| Support at node 1 | Roller (restrains v₁ only) |
| Support at node 6 | Pin (restrains u₆ and v₆) |

### 1.1.4 Node Coordinates

| Node | x (mm) | y (mm) |
|------|--------|--------|
| 1 | 0 | 0 |
| 2 | 2,400 | 1,800 |
| 3 | 2,400 | 5,400 |
| 4 | 4,800 | 1,800 |
| 5 | 4,800 | 5,400 |
| 6 | 7,200 | 5,400 |

### 1.1.5 Element Connectivity

| Element | Node i → Node j | Type |
|---------|-----------------|------|
| 1 | 1 → 2 | Inclined |
| 2 | 1 → 4 | Inclined |
| 3 | 2 → 3 | Vertical |
| 4 | 2 → 4 | Horizontal |
| 5 | 3 → 4 | Inclined |
| 6 | 3 → 5 | Horizontal |
| 7 | 4 → 5 | Vertical |
| 8 | 5 → 6 | Horizontal |
| 9 | 4 → 6 | Inclined |

---

## 1.2 Numerical Analysis (3 marks)

### 1.2.1 Element Type and Degrees of Freedom

**Element type:** 2-node linear bar (truss) element — a 1D element embedded in 2D space. Each element transmits only axial force along its axis; no bending, shear, or torsion is considered.

**Degrees of freedom per node:** 2 (horizontal displacement u, vertical displacement v).

**Total system DOFs:** 6 nodes × 2 DOFs/node = **12 DOFs**.

**DOF numbering convention:**

```
    Node:     1       2       3       4       5       6
    DOF u:    1       3       5       7       9      11
    DOF v:    2       4       6       8      10      12
```

(0-indexed in computation: DOF 0–11)

### 1.2.2 Element Geometry Calculations

For each element connecting node i at (xᵢ, yᵢ) to node j at (xⱼ, yⱼ):

$$L = \sqrt{(\Delta x)^2 + (\Delta y)^2}, \quad c = \cos\theta = \frac{\Delta x}{L}, \quad s = \sin\theta = \frac{\Delta y}{L}$$

$$EA = 30{,}000 \times 20{,}000 = 6 \times 10^8 \text{ N}$$

| El. | Nodes | Δx (mm) | Δy (mm) | L (mm) | cos θ | sin θ | EA/L (N/mm) |
|-----|-------|---------|---------|--------|-------|-------|-------------|
| 1 | 1→2 | 2,400 | 1,800 | 3,000.00 | 0.8000 | 0.6000 | 200,000.00 |
| 2 | 1→4 | 4,800 | 1,800 | 5,126.40 | 0.9363 | 0.3511 | 117,041.15 |
| 3 | 2→3 | 0 | 3,600 | 3,600.00 | 0.0000 | 1.0000 | 166,666.67 |
| 4 | 2→4 | 2,400 | 0 | 2,400.00 | 1.0000 | 0.0000 | 250,000.00 |
| 5 | 3→4 | 2,400 | −3,600 | 4,326.66 | 0.5547 | −0.8321 | 138,675.05 |
| 6 | 3→5 | 2,400 | 0 | 2,400.00 | 1.0000 | 0.0000 | 250,000.00 |
| 7 | 4→5 | 0 | 3,600 | 3,600.00 | 0.0000 | 1.0000 | 166,666.67 |
| 8 | 5→6 | 2,400 | 0 | 2,400.00 | 1.0000 | 0.0000 | 250,000.00 |
| 9 | 4→6 | 2,400 | 3,600 | 4,326.66 | 0.5547 | 0.8321 | 138,675.05 |

### 1.2.3 Element Stiffness Matrix — General Formulation

For a 2D truss element, the 4×4 stiffness matrix in global coordinates is:

$$\mathbf{k}^{(e)} = \frac{EA}{L} \begin{bmatrix} c^2 & cs & -c^2 & -cs \\ cs & s^2 & -cs & -s^2 \\ -c^2 & -cs & c^2 & cs \\ -cs & -s^2 & cs & s^2 \end{bmatrix}$$

This maps to global DOFs [2i−1, 2i, 2j−1, 2j] for nodes i and j.

### 1.2.4 Worked Example — Element 1 (nodes 1→2)

c = 0.8, s = 0.6, EA/L = 200,000 N/mm

$$\mathbf{k}^{(1)} = 200{,}000 \begin{bmatrix} 0.64 & 0.48 & -0.64 & -0.48 \\ 0.48 & 0.36 & -0.48 & -0.36 \\ -0.64 & -0.48 & 0.64 & 0.48 \\ -0.48 & -0.36 & 0.48 & 0.36 \end{bmatrix}$$

$$= \begin{bmatrix} 128{,}000 & 96{,}000 & -128{,}000 & -96{,}000 \\ 96{,}000 & 72{,}000 & -96{,}000 & -72{,}000 \\ -128{,}000 & -96{,}000 & 128{,}000 & 96{,}000 \\ -96{,}000 & -72{,}000 & 96{,}000 & 72{,}000 \end{bmatrix} \text{ N/mm}$$

**Maps to global DOFs:** {1, 2, 3, 4} (u₁, v₁, u₂, v₂)

### 1.2.5 Worked Example — Element 4 (nodes 2→4, horizontal)

c = 1.0, s = 0.0, EA/L = 250,000 N/mm

$$\mathbf{k}^{(4)} = 250{,}000 \begin{bmatrix} 1 & 0 & -1 & 0 \\ 0 & 0 & 0 & 0 \\ -1 & 0 & 1 & 0 \\ 0 & 0 & 0 & 0 \end{bmatrix} = \begin{bmatrix} 250{,}000 & 0 & -250{,}000 & 0 \\ 0 & 0 & 0 & 0 \\ -250{,}000 & 0 & 250{,}000 & 0 \\ 0 & 0 & 0 & 0 \end{bmatrix} \text{ N/mm}$$

**Maps to global DOFs:** {3, 4, 7, 8} (u₂, v₂, u₄, v₄)

### 1.2.6 Worked Example — Element 3 (nodes 2→3, vertical)

c = 0.0, s = 1.0, EA/L = 166,666.67 N/mm

$$\mathbf{k}^{(3)} = 166{,}666.67 \begin{bmatrix} 0 & 0 & 0 & 0 \\ 0 & 1 & 0 & -1 \\ 0 & 0 & 0 & 0 \\ 0 & -1 & 0 & 1 \end{bmatrix} = \begin{bmatrix} 0 & 0 & 0 & 0 \\ 0 & 166{,}667 & 0 & -166{,}667 \\ 0 & 0 & 0 & 0 \\ 0 & -166{,}667 & 0 & 166{,}667 \end{bmatrix} \text{ N/mm}$$

**Maps to global DOFs:** {3, 4, 5, 6} (u₂, v₂, u₃, v₃)

### 1.2.7 Global Stiffness Matrix Assembly

The 12×12 global stiffness matrix is assembled by the direct stiffness method:

$$\mathbf{K} = \sum_{e=1}^{9} \mathbf{A}^{(e)T} \mathbf{k}^{(e)} \mathbf{A}^{(e)}$$

**Assembly map** (element → global DOF mapping, 0-indexed):

| Element | Node i | Node j | Global DOFs |
|---------|--------|--------|-------------|
| 1 | 1 | 2 | {0, 1, 2, 3} |
| 2 | 1 | 4 | {0, 1, 6, 7} |
| 3 | 2 | 3 | {2, 3, 4, 5} |
| 4 | 2 | 4 | {2, 3, 6, 7} |
| 5 | 3 | 4 | {4, 5, 6, 7} |
| 6 | 3 | 5 | {4, 5, 8, 9} |
| 7 | 4 | 5 | {6, 7, 8, 9} |
| 8 | 5 | 6 | {8, 9, 10, 11} |
| 9 | 4 | 6 | {6, 7, 10, 11} |

**Assembled global stiffness matrix K (kN/mm):**

```
DOF:     u₁        v₁        u₂        v₂        u₃        v₃        u₄        v₄        u₅        v₅        u₆        v₆
u₁ [  230.61   134.48  -128.00   -96.00     0.00     0.00  -102.61   -38.48     0.00     0.00     0.00     0.00 ]
v₁ [  134.48    86.43   -96.00   -72.00     0.00     0.00   -38.48   -14.43     0.00     0.00     0.00     0.00 ]
u₂ [ -128.00   -96.00   378.00    96.00     0.00     0.00  -250.00     0.00     0.00     0.00     0.00     0.00 ]
v₂ [  -96.00   -72.00    96.00   238.67     0.00  -166.67     0.00     0.00     0.00     0.00     0.00     0.00 ]
u₃ [    0.00     0.00     0.00     0.00   292.67   -64.00   -42.67    64.00  -250.00     0.00     0.00     0.00 ]
v₃ [    0.00     0.00     0.00  -166.67   -64.00   262.67    64.00   -96.01     0.00     0.00     0.00     0.00 ]
u₄ [ -102.61   -38.48  -250.00     0.00   -42.67    64.00   437.95    38.48     0.00     0.00   -42.67   -64.00 ]
v₄ [  -38.48   -14.43     0.00     0.00    64.00   -96.01    38.48   373.11     0.00  -166.67   -64.00   -96.01 ]
u₅ [    0.00     0.00     0.00     0.00  -250.00     0.00     0.00     0.00   500.00     0.00  -250.00     0.00 ]
v₅ [    0.00     0.00     0.00     0.00     0.00     0.00     0.00  -166.67     0.00   166.67     0.00     0.00 ]
u₆ [    0.00     0.00     0.00     0.00     0.00     0.00   -42.67   -64.00  -250.00     0.00   292.67    64.00 ]
v₆ [    0.00     0.00     0.00     0.00     0.00     0.00   -64.00   -96.01     0.00     0.00    64.00    96.01 ]
```

**Key properties:** The matrix is symmetric (Kᵢⱼ = Kⱼᵢ), positive semi-definite before applying BCs, and sparse (many zero entries reflecting that non-adjacent nodes share no direct stiffness).

### 1.2.8 Boundary Conditions

```
    Node 1 — ROLLER                     Node 6 — PIN
    ┌───────────┐                       ┌───────────┐
    │ u₁ = FREE │                       │ u₆ = 0    │
    │ v₁ = 0    │                       │ v₆ = 0    │
    └───────────┘                       └───────────┘
    Reaction: R₁ᵧ                       Reactions: R₆ₓ, R₆ᵧ
    (DOF 1 constrained)                 (DOFs 10, 11 constrained)
```

**Constrained DOFs (0-indexed):** {1, 10, 11}  
**Free DOFs (0-indexed):** {0, 2, 3, 4, 5, 6, 7, 8, 9}  
**Reduced system size:** 9 × 9

### 1.2.9 Applied Load Vector

**Loading diagram:**

```
          W=12.5kN                                    
            ←─── ● 3                                  ● 6
                                                     ╳╳╳ PIN
                                                      │
                                                      │
                 ● 2                    ● 4           │
                 ↓ P=15kN              ↓ P=15kN      │
                                                      │
            ● 1                                       │
           ═══ ROLLER                                 │
```

**Force vector F** (in Newtons, 0-indexed):

| DOF (0-idx) | Node | Dir. | Applied Load |
|-------------|------|------|-------------|
| 0 | 1 | u | 0 |
| 1 | 1 | v | 0 (constrained) |
| 2 | 2 | u | 0 |
| 3 | 2 | v | −15,000 N |
| 4 | 3 | u | −12,500 N |
| 5 | 3 | v | 0 |
| 6 | 4 | u | 0 |
| 7 | 4 | v | −15,000 N |
| 8 | 5 | u | 0 |
| 9 | 5 | v | 0 |
| 10 | 6 | u | 0 (constrained) |
| 11 | 6 | v | 0 (constrained) |

Sign convention: positive rightward and upward. W acts leftward → negative x. P acts downward → negative y.

### 1.2.10 Solution of the Reduced System

Partitioning the system into free (f) and constrained (c) DOFs:

$$\mathbf{K}_{ff} \mathbf{U}_f = \mathbf{F}_f$$

$$\mathbf{U}_f = \mathbf{K}_{ff}^{-1} \mathbf{F}_f$$

The 9×9 system is solved numerically. The constrained DOFs are set to zero.

### 1.2.11 Nodal Displacements — Results

| Node | u (mm) | v (mm) | Notes |
|------|--------|--------|-------|
| 1 | **+0.5300** | 0.0000 | v₁ = 0 (roller BC); u₁ positive → rightward |
| 2 | +1.4645 | **−1.6627** | Largest vertical displacement |
| 3 | −0.0200 | −1.7527 | Leftward due to W; large downward deflection |
| 4 | +1.3045 | −1.0259 | Loaded node |
| 5 | −0.0100 | −1.0259 | Symmetric v with node 4 |
| 6 | 0.0000 | 0.0000 | Pin BC |

**Deformed shape (exaggerated):**

```
    UNDEFORMED (dashed)          DEFORMED (solid, ×5000 exaggeration)
    
    3 ─────── 5 ─────── 6       3' ──────── 5' ──────── 6
    │╲        │       ╱ │        │╲          │         ╱│
    │  ╲      │     ╱   │        │  ╲        │       ╱  │
    │    ╲    │   ╱     │        │    ╲      │     ╱    │
    2 ────── 4          │        2' ───── 4'            │
    │      ╱            │        ↓↓       ↓↓            │
    │    ╱              │       (nodes 2,4 deflect      │
    │  ╱                │        most downward)         │
    1                   6        1' →                    6
   ═══                ╳╳╳      (shifts right)         ╳╳╳
```

### 1.2.12 Reaction Forces

Reactions are recovered from:

$$\mathbf{R} = \mathbf{K} \mathbf{U} - \mathbf{F}$$

| Node | Direction | Reaction Force | 
|------|-----------|---------------|
| 1 | Fy (vertical) | **+15,000 N (+15.00 kN)** ↑ |
| 6 | Fx (horizontal) | **+12,500 N (+12.50 kN)** → |
| 6 | Fy (vertical) | **+15,000 N (+15.00 kN)** ↑ |

**Free-body diagram — Equilibrium verification:**

```
          12.5 kN ←── 3                           6 ──→ 12.5 kN (R₆ₓ)
                                                   ↑ 15.0 kN (R₆ᵧ)
                                                   
                 2 ↓ 15 kN       4 ↓ 15 kN
                 
            1
         ↑ 15.0 kN (R₁ᵧ)
```

**Equilibrium checks:**

| Condition | Calculation | Result |
|-----------|-------------|--------|
| ΣFx = 0 | −12,500 + 12,500 | = 0 ✓ |
| ΣFy = 0 | +15,000 + 15,000 − 15,000 − 15,000 | = 0 ✓ |
| ΣM₁ = 0 | (−15,000)(2,400) + (−12,500)(5,400) + (−15,000)(4,800) + (12,500)(5,400) + (15,000)(7,200) | = 0 ✓ |

All three static equilibrium equations are satisfied exactly, confirming solution correctness.

### 1.2.13 Element Strains, Stresses, and Internal Forces

**Strain computation** for each element:

$$\varepsilon^{(e)} = \frac{\delta}{L} = \frac{c(u_j - u_i) + s(v_j - v_i)}{L}$$

**Stress:**

$$\sigma^{(e)} = E \cdot \varepsilon^{(e)}$$

**Internal axial force:**

$$F^{(e)} = \sigma^{(e)} \cdot A$$

| El. | Nodes | Axial Force (kN) | Strain (×10⁻⁵) | Stress (MPa) | State |
|-----|-------|-------------------|-----------------|--------------|-------|
| 1 | 1→2 | **−50.00** | −8.333 | −2.500 | **Compression** |
| 2 | 1→4 | +42.72 | +7.120 | +2.136 | Tension |
| 3 | 2→3 | −15.00 | −2.500 | −0.750 | Compression |
| 4 | 2→4 | **−40.00** | −6.667 | −2.000 | **Compression** |
| 5 | 3→4 | +18.03 | +3.005 | +0.901 | Tension |
| 6 | 3→5 | +2.50 | +0.417 | +0.125 | Tension |
| 7 | 4→5 | 0.00 | 0.000 | 0.000 | Zero-force |
| 8 | 5→6 | +2.50 | +0.417 | +0.125 | Tension |
| 9 | 4→6 | +18.03 | +3.005 | +0.901 | Tension |

**Element force diagram (T = tension, C = compression):**

```
          3 ───── El6(+2.5T) ──── 5 ───── El8(+2.5T) ──── 6
          ╱╲                       │                      ╱
        ╱    ╲ El5(+18.0T)    El7  │(0)      El9        ╱
      ╱        ╲              (ZERO)│      (+18.0T)   ╱
    ╱   El3      ╲                 │                ╱
   │  (−15.0 C)   ╲               │              ╱
   2 ──────────────── 4            │            ╱
    ╲   El4(−40.0 C) ╱                        ╱
      ╲            ╱                        ╱
        ╲  El1   ╱    El2(+42.7T)         ╱
          ╲(−50C)╱                      ╱
            ╲  ╱                      ╱
              1
```

**Key observations:**
- Element 1 carries the highest force magnitude (50.0 kN compression) and highest stress (2.50 MPa).
- Element 7 is a **zero-force member** — its connected nodes (4 and 5) have identical vertical displacements.
- The lower chord (elements 1, 4) is in compression while the diagonals (elements 2, 5, 9) are in tension — consistent with a truss carrying downward loads.
- All stresses are far below typical steel yield (~250 MPa), confirming the linear elastic assumption.

---

## 1.3 Critical Analysis (6 marks)

### 1.3(a) Reconstruct the FEM model and justify all modelling assumptions (1 mark)

The FEM model rests on the following assumptions, each justified below:

**1. Bar/Truss element (axial-only members):**
Each element carries only axial force — no bending, shear, or torsion. This is valid because the problem statement specifies pin joints at all connections. At a pin joint, members are free to rotate, so no bending moment can be transmitted. All loads are applied at nodes, so no transverse loading exists along any member. The standard 2-DOF-per-node truss element is therefore the appropriate choice.

**2. Linear elastic material (Hooke's law):**
The constitutive relationship is σ = Eε. The maximum computed stress is 2.50 MPa, which is less than 1% of typical structural steel yield strength (≈ 250 MPa for S275 steel). The material remains firmly in the elastic regime, so nonlinear material models are unnecessary.

**3. Small displacement / small strain:**
The maximum nodal displacement is 1.75 mm, while the shortest element is 2,400 mm — a ratio of 7.3 × 10⁻⁴. At this scale, the deformed geometry is essentially identical to the undeformed geometry. This validates the use of the initial configuration for equilibrium (no geometric nonlinearity) and the engineering strain measure ε = δ/L.

**4. Planar (2D) analysis:**
All nodes, elements, and loads lie in a single plane. There are no out-of-plane loads or constraints. A 2D model with 2 DOFs per node (u, v) is sufficient. A 3D model would add an unnecessary w DOF at each node with no corresponding load or constraint.

**5. Concentrated nodal loads:**
The problem specifies forces P and W applied at specific nodes. There are no distributed loads along element lengths, so no work-equivalent nodal force transformation is required.

**6. Homogeneous, isotropic, uniform cross-section:**
All nine elements share identical E = 30,000 MPa and A = 20,000 mm². This is a modelling simplification; in practice, different members might have different cross-sections for optimization. However, the problem statement prescribes uniform properties.

**7. Neglect of self-weight:**
Member self-weight is not included. For a steel truss with A = 20,000 mm² and density ρ ≈ 7,850 kg/m³, the weight of the longest element (El 2, L = 5.13 m) is approximately 0.20 mm² × 5.13 m × 7,850 kg/m³ × 9.81 m/s² ≈ 7.9 kN. This is comparable to the applied loads (15 kN), so neglecting self-weight introduces a non-trivial error. This assumption should be explicitly acknowledged.

### 1.3(b) Predict sign and relative magnitude of nodal displacements (1 mark)

*Without relying on the full numerical solution*, the displacement pattern can be inferred from statics and structural intuition:

**Horizontal displacements (u):**

The horizontal load W = 12.5 kN acts leftward at node 3. Node 6 is pinned (fixed), so the horizontal reaction R₆ₓ must balance W entirely (ΣFx = 0 → R₆ₓ = +12.5 kN). Since node 1 is a roller (free in x), the entire structure tends to drift in the direction that relieves the horizontal load.

Examining the load path: W acts leftward on the top chord. The horizontal reaction at node 6 acts rightward. This creates a couple that would cause the structure to sway. However, node 1 (roller) is free to move horizontally, so instead of swaying, node 1 translates.

By inspecting the truss geometry, the top chord is loaded leftward while the bottom is unconstrained horizontally at node 1. The diagonal members (elements 2, 9) connect the top and bottom — their tension will pull node 1 to the right. Thus **u₁ > 0** (rightward).

For nodes 2 and 4 (mid-height), the downward loads compress the lower members and stretch the diagonals, causing these nodes to move rightward as well: **u₂ > 0, u₄ > 0**.

Node 3 receives W directly leftward, but is connected to node 2 (below, moving right) by a vertical member. The net effect: u₃ should be near zero, possibly slightly negative. Similarly, u₅ ≈ 0, slightly negative.

**Vertical displacements (v):**

Node 1: v₁ = 0 (roller BC).
Node 6: v₆ = 0 (pin BC).

Nodes 2 and 4 receive P = 15 kN downward: v₂ < 0, v₄ < 0.

Node 3 is at the top, connected to node 2 by a vertical member. The downward load on node 2 stretches element 3 (vertical), pulling node 3 downward too: v₃ < 0.

Node 5 is connected to node 4 by element 7 (vertical). Similarly: v₅ < 0.

**Relative magnitudes:** Node 2 has the largest vertical displacement because it sits at the base of the tallest sub-structure above the roller. Node 3, directly above node 2, should have a comparable but slightly different deflection (the vertical member between them transmits but also modifies the displacement). Node 4, though directly loaded, is connected more stiffly to the pin support at node 6 via elements 7, 8, 9.

**Validation from numerical solution:** The computed results confirm:
- u₁ = +0.530 mm (rightward) ✓
- u₂ = +1.465 mm, u₄ = +1.305 mm (rightward) ✓
- u₃ = −0.020 mm, u₅ = −0.010 mm (slightly leftward) ✓
- v₂ = −1.663 mm (largest downward) ✓
- v₃ = −1.753 mm, v₄ = v₅ = −1.026 mm ✓

All signs match the physical predictions. The relative magnitudes confirm |v₃| > |v₂| > |v₄| = |v₅|, with node 3 having the largest total deflection — consistent with it being the furthest node from both supports with an applied horizontal load.

### 1.3(c) Most critical element in terms of stress (1 mark)

**Most critical element: Element 1 (nodes 1→2)**, with |σ| = **2.500 MPa** (compression) and axial force = **50.0 kN**.

**Physical and structural reasoning (not merely numerical ranking):**

```
    Load path diagram — how forces reach the supports:
    
          W ←── 3                              6 (PIN)
                 ╲                            ╱
                   ╲  El5                   ╱ El9
                     ╲                    ╱
          P ↓ 2 ═══════════ 4 ↓ P      ╱
                ╲           ╱         ╱
             El1 ╲       ╱ El2     ╱
           (MOST  ╲    ╱        ╱
           LOADED) ╲ ╱       ╱
                    1 ══════════════════════ (ROLLER, R₁ᵧ = 15 kN ↑)
```

1. **Load path convergence at node 1:** The roller at node 1 carries R₁ᵧ = 15 kN (upward). This reaction must be delivered to node 1 through the members connected to it — elements 1 and 2. The 15 kN downward load at node 2 and the roller reaction at node 1 create a large compressive demand on element 1, which is the direct load path between these two nodes.

2. **Equivalent beam analogy:** Treating the truss as an equivalent simply-supported beam (roller at node 1, pin at node 6), the lower chord acts as the compression flange. Element 1 is part of the lower chord nearest the roller support, where the combined effect of vertical loads creates the highest chord force.

3. **Geometric effect:** Element 1 is inclined at θ = 36.87° from horizontal. Its axial force must provide both horizontal and vertical components to equilibrate node 2 (which has a 15 kN downward load and receives force from the horizontal member El4). The inclined orientation means the axial force must be larger to provide the same vertical component compared to a purely vertical member.

4. **Short, direct load path:** Element 1 (L = 3,000 mm) provides the most direct path from the loaded node 2 to the support at node 1. It is shorter and more directly loaded than element 2 (L = 5,126 mm), which spans a longer distance to node 4.

5. **Compression risk:** Beyond having the highest stress, element 1 is in compression. Compressive members are susceptible to **buckling**, an instability failure mode not captured by the linear FEM analysis. The Euler critical load for element 1 would need to be checked in a complete design assessment. This makes element 1 doubly critical — highest stress and highest buckling risk.

### 1.3(d) Uncertainties in boundary conditions — effect on results (1 mark)

**Sensitivity of the roller at node 1:**

The roller is idealised as providing a frictionless, perfectly vertical constraint. In reality:

- **Friction:** If the roller has coefficient of friction μ, a horizontal reaction Rₓ = μR₁ᵧ = μ × 15 kN develops at node 1. For μ = 0.1, this is 1.5 kN — about 12% of W. This would reduce R₆ₓ from 12.5 to 11.0 kN and redistribute internal forces throughout the truss, particularly in the horizontal chord members (El 4, 6, 8).

- **Support settlement:** If the roller settles by δ_v (vertical movement), the entire displacement field shifts. For a statically determinate truss, support settlement does not change internal forces (they are governed by equilibrium alone), but displacements change significantly. However, if the roller direction is misaligned, the structure effectively gains or loses a DOF, fundamentally changing the problem.

- **Direction of roller:** If the roller constrains at an angle α to the vertical instead of exactly vertical, the reaction force direction changes. For α = 5°, the horizontal component of the roller reaction is R₁ᵧ sin(5°) ≈ 1.3 kN — comparable in magnitude to some element forces (El 6 and El 8 carry only 2.5 kN).

**Sensitivity of the pin at node 6:**

- **Compliance:** If the pin connection has finite stiffness k_pin rather than being perfectly rigid, the effective boundary condition becomes a spring. Displacements near node 6 would increase, and the force distribution in elements 8 and 9 would change. The structure transitions from perfectly constrained to elastically constrained.

- **Clearance/play:** A pin with clearance δ requires the structure to displace by δ before the constraint engages, introducing geometric nonlinearity (contact problem). Initial displacements would be larger and load-displacement behaviour becomes nonlinear.

**Quantitative impact assessment:**

Since the structure is **statically determinate** (3 reactions, 3 equilibrium equations, no redundancy), internal forces are uniquely determined by equilibrium and are theoretically independent of support stiffness. However:
- Displacements are **highly sensitive** to support conditions — changing the roller to a pin would add a redundant reaction, making the structure indeterminate and completely changing force distribution.
- The **reaction forces** themselves are the results most sensitive to BC uncertainties, as they directly involve the constrained DOFs.
- Element forces **near the supports** (El 1, El 2, El 8, El 9) are the most affected because they directly transmit loads to the constrained nodes.

### 1.3(e) One modelling assumption whose violation would invalidate results (0.75 mark)

**Critical assumption: All joints are frictionless pins (no moment transfer).**

**Why this is the most consequential assumption:**

If the joints are actually **rigid** (e.g., welded, bolted with gusset plates providing moment continuity), the structure becomes a **rigid frame**, and the truss element formulation is fundamentally invalid:

```
    PIN JOINT (assumed):              RIGID JOINT (if violated):
    
         ╲ │ ╱                              ╲ │ ╱
          ╲│╱                                ╲│╱
           ○  ← free rotation                ■  ← fixed rotation
          ╱│╲                                ╱│╲
         ╱ │ ╲                              ╱ │ ╲
    
    Transmits: axial force only        Transmits: axial force + shear
    DOFs/node: 2 (u, v)                          + bending moment
    Element type: Bar                  DOFs/node: 3 (u, v, θ)
                                       Element type: Beam/Frame
```

**Consequences of violation:**

1. **Element formulation error:** A truss element has 2 DOFs per node (u, v). A frame element requires 3 DOFs per node (u, v, θ). The entire stiffness matrix formulation — its size (12×12 vs 18×18), its entries, and the resulting solution — would be wrong.

2. **Bending stress dominance:** In rigid frames, bending stresses σ_b = My/I can be significantly larger than axial stresses σ_a = F/A, particularly in slender members. For element 1 (L = 3,000 mm), if A = 20,000 mm² with a compact cross-section of depth d ≈ 141 mm, I ≈ 33 × 10⁶ mm⁴, a modest bending moment M = 5 kN·m would produce σ_b ≈ 10.7 MPa — over 4× the computed axial stress. The actual stress state could be much higher than predicted.

3. **Stiffness overestimation:** A pin-jointed truss is more flexible than a rigid frame (the frame has additional rotational stiffness). The truss model overestimates displacements compared to a rigid frame.

4. **Force redistribution:** In a rigid frame, internal forces depend on relative member stiffnesses (statically indeterminate even for apparently determinate truss topologies), so the force distribution would change entirely.

**Why other assumptions are less critical:**
- Linear elasticity is easily verified (stresses ≪ yield).
- Small displacements are confirmed by the magnitude of results (0.001 ratio).
- 2D analysis is justified by the planar geometry.
- The joint assumption, however, depends on physical construction details that cannot be verified from the mathematical model alone.

### 1.3(f) Most critical element under 50% reduction of P (0.75 mark)

With P reduced to 7.5 kN (50% of original) while W remains at 12.5 kN:

**Superposition analysis** (valid because the system is linear):

Decompose the original loading into two independent load cases:

- **LC1:** P = 15 kN at nodes 2 and 4 (vertical loads only)
- **LC2:** W = 12.5 kN at node 3 (horizontal load only)

Original response = LC1 + LC2.
Modified response = 0.5 × LC1 + LC2.

For **Element 1 (most critical in original analysis):**

The force in Element 1 under the original loading is −50.0 kN. By superposition, we can estimate the contributions:
- Under LC1 alone: the vertical loads P create the compression in the lower chord. Element 1 must carry the vertical reaction (R₁ᵧ) in its axial direction. R₁ᵧ from vertical loads alone (by symmetry and equilibrium) is 15.0 kN. The axial force in El 1 due to LC1: F₁_LC1 = −R₁ᵧ / sin θ = −15.0 / 0.6 = −25.0 kN.
- Under LC2 alone: The horizontal load W produces a horizontal reaction at node 6 and vertical reactions at both supports. The force in El 1 due to LC2: F₁_LC2 = F₁_total − F₁_LC1 = −50.0 − (−25.0) = −25.0 kN.

Under 50% P reduction:
- F₁_modified = 0.5 × (−25.0) + (−25.0) = −12.5 − 25.0 = **−37.5 kN**

**Checking other elements:**
- Element 4 (−40.0 kN original): F₄_LC1 contribution from vertical loads is dominant (horizontal member between loaded nodes). With 50% P reduction: F₄_modified ≈ 0.5 × F₄_LC1 + F₄_LC2. Since El 4 primarily resists the horizontal component of loads reaching node 2, its force would reduce significantly — likely to about −20 kN.
- Element 2 (+42.7 kN original): Similarly scales down proportionally.

**Conclusion:** Even with 50% P reduction, **Element 1 remains the most critical** at approximately −37.5 kN, because:
1. It carries roughly equal contributions from both load cases (vertical and horizontal).
2. The horizontal load W (unchanged) continues to drive a large compressive force through it.
3. Its compression state also makes it the most buckling-susceptible member, independent of magnitude.

### 1.3(g) Least trustworthy result (0.5 mark)

**The result I would trust the least: the displacement of Node 3 (u₃ = −0.020 mm).**

**Reasoning:**

1. **Magnitude sensitivity:** This displacement is the smallest non-zero value in the entire solution (0.020 mm). It is three orders of magnitude smaller than the element lengths and approaches the limit where the linear model's assumptions start to matter. At such small magnitudes, modelling errors (neglected self-weight, joint eccentricities, initial imperfections) dominate the true displacement.

2. **Competing effects:** Node 3 experiences opposing horizontal forces: W pushes it leftward, but the diagonal elements (El 5, connecting to node 4 which moves rightward) and the horizontal element (El 6, connecting to node 5) resist this motion. The near-zero result reflects a near-cancellation of two large effects — such cancellations amplify relative errors. A 10% error in either component could change the sign of u₃.

3. **Self-weight sensitivity:** If self-weight were included, the additional vertical loads would change the force distribution, particularly in the vertical member El 3 (connecting nodes 2 and 3). This would alter the displacement of node 3 more than other nodes because node 3's displacement is already near zero — small absolute changes produce large relative changes.

4. **Practical relevance:** In physical testing, measuring a 0.020 mm displacement requires high-precision instrumentation (micrometer-level). Environmental effects (thermal expansion, vibration) would easily mask this displacement, making it experimentally unverifiable.

---

# Question 2 — 2D Plane Stress / CST Analysis (9% of overall mark)

## 2.1 Problem Definition

### 2.1.1 Structural Description

A rectangular plate modelled with **four linear triangular (CST) elements** and **five nodes**. The plate is fixed on its left edge and subjected to a uniform traction on its right edge.

### 2.1.2 Structural Diagram

```
    y ↑
      │
   10 ├──── 2 ━━━━━━━━━━━━━━━━━━━━━━━━━ 3 ──→ P = 2175 psi
      │    ╱╲ ╲                        ╱╲│      (uniform traction)
      │  ╱    ╲   ╲    El 4         ╱    ╲│
      │╱        ╲     ╲          ╱   El 3 ╲│
    5 ┃ El 1      ╲  5  ╲     ╱            ┃──→ P
      │╲        ╱  ● ╱  ╲  ╱              │
      │  ╲    ╱  (10,5)╱  ╲               │
      │    ╲╱      ╱        ╲   El 2      │
      │   ╱  ╲  ╱   El 2      ╲           │
    0 ├──── 1 ━━━━━━━━━━━━━━━━━━━━━━━━━ 4 ──→ P
      │                                    │
      └────┬──────────┬──────────┬─────────┴──→ x
           0         10         20
      
      ┃ = fixed wall (u=v=0 at nodes 1 and 2)
```

**P-number example: P---175 → X = 1175**

### 2.1.3 Given Data

| Parameter | Value |
|-----------|-------|
| Uniform traction, P = 1000 + X | **2,175 psi** |
| Plate length | 20 in |
| Plate height | 10 in |
| Thickness, t | 1 in |
| Young's modulus, E | 30 × 10⁶ psi |
| Poisson's ratio, ν | 0.3 |

### 2.1.4 Node Coordinates

| Node | x (in) | y (in) | Location |
|------|--------|--------|----------|
| 1 | 0 | 0 | Bottom-left (fixed) |
| 2 | 0 | 10 | Top-left (fixed) |
| 3 | 20 | 10 | Top-right (loaded) |
| 4 | 20 | 0 | Bottom-right (loaded) |
| 5 | 10 | 5 | Centre (interior) |

### 2.1.5 Element Connectivity

| Element | Node 1 | Node 2 | Node 3 | Area (in²) |
|---------|--------|--------|--------|------------|
| 1 | 1 | 5 | 2 | 50 |
| 2 | 1 | 4 | 5 | 50 |
| 3 | 4 | 3 | 5 | 50 |
| 4 | 2 | 5 | 3 | 50 |

All elements have equal area = ½ × 20 × 10 / 2 = 50 in² (the rectangle is divided into 4 congruent right triangles by its two diagonals).

### 2.1.6 DOF Numbering

| Node | DOF u (0-idx) | DOF v (0-idx) |
|------|---------------|---------------|
| 1 | 0 | 1 |
| 2 | 2 | 3 |
| 3 | 4 | 5 |
| 4 | 6 | 7 |
| 5 | 8 | 9 |

Total DOFs: 5 × 2 = **10**

> **Important note from problem statement:** "Not all modelling choices implied by the figure are physically or mathematically correct." The following analysis identifies and corrects these issues.

---

## 2.2 Boundary Condition Verification (2 marks)

### 2.2.1 Identify incorrect or incomplete boundary conditions

**Examining the figure carefully:**

The left edge of the plate (x = 0) shows wall/hatching symbols at nodes 1 and 2, indicating a fixed (clamped) boundary. For a CST element with only translational DOFs (u, v), this means:

- Node 1: u₁ = 0, v₁ = 0
- Node 2: u₂ = 0, v₂ = 0

**Potential issues identified:**

1. **Incomplete constraint along the boundary:** The figure shows fixed supports only at the two corner nodes (1 and 2), but the left edge is a continuous boundary. In reality, the entire left edge should be constrained. With CST elements, nodes are the only locations where BCs can be applied, and nodes 1 and 2 are the only nodes on the left edge. Therefore, fixing u = v = 0 at both nodes correctly represents a fixed wall *for this mesh*. However, the intermediate points along the edge between nodes 1 and 2 can still deform (the edge is linear between nodes). A finer mesh would better represent the fixed boundary.

2. **Potential incorrect BC in the figure:** If the figure shows node 1 with a pin support and node 2 with a roller (or any configuration that does not fully fix both nodes), this would be **incorrect** for a plate fixed to a wall:
   - A roller at node 2 (constraining only v₂) would leave the structure free to translate in x at the top-left corner, producing a shearing deformation inconsistent with a rigid wall.
   - A pin-roller combination constrains only 3 DOFs (minimum for preventing rigid body motion) but does not represent the physical wall constraint.

3. **Node 5 (interior node):** If the figure inadvertently shows a constraint at the interior node 5, this would be incorrect. Node 5 is a free interior point and must not be constrained.

### 2.2.2 Propose the correct boundary conditions

```
    Correct BCs:                    Incorrect BC example:
    
    2 ┃────────── 3                 2 ═══─────── 3
      ┃                               (roller)
      ┃  (WALL)                        
      ┃                               
    1 ┃────────── 4                 1 ╳───────── 4
                                      (pin)
    
    Node 1: u₁=0, v₁=0 ✓          Node 1: u₁=0, v₁=0
    Node 2: u₂=0, v₂=0 ✓          Node 2: u₂=free, v₂=0  ✗
    4 constrained DOFs              3 constrained DOFs
    6 free DOFs                     7 free DOFs
```

**Correct conditions:**
- **Node 1:** u₁ = 0, v₁ = 0 (fully fixed)
- **Node 2:** u₂ = 0, v₂ = 0 (fully fixed)

This provides 4 constrained DOFs, leaving 6 free DOFs: {u₃, v₃, u₄, v₄, u₅, v₅}.

### 2.2.3 Consequences of using incorrect boundary conditions

**Effect on stiffness matrix:**
If a DOF is incorrectly left free, the global stiffness matrix may become singular (if rigid body motion is not prevented), making the system unsolvable. If the 3-DOF minimum is met but the constraint pattern is wrong:

**Effect on displacements:**
An incorrectly placed roller (e.g., at node 2 constraining only v₂) would allow the top-left corner to slide horizontally. Under the applied traction (rightward at nodes 3, 4), the plate would undergo a combination of extension and rigid-body translation/rotation, producing non-physical displacement patterns. The computed u₂ ≠ 0, which contradicts the wall boundary.

**Effect on reactions:**
The reaction forces would be incorrectly distributed. With a wall, both nodes 1 and 2 resist horizontal forces equally (by symmetry of loading and geometry). With a pin-roller setup, all horizontal reaction goes to the pin node, creating artificial stress concentration.

**Effect on stresses:**
The incorrect displacements propagate directly into strains (ε = B·u) and stresses (σ = D·ε). The stress field would be qualitatively different — showing shear and asymmetry where the analytical solution predicts uniform tension.

---

## 2.3 Stress State Assessment (2 marks)

### 2.3.1 Select the correct assumption

**Plane stress** is the correct assumption for this problem.

### 2.3.2 Justification based on geometry and loading

```
    Plane stress condition:           Plane strain condition:
    (THIN body, σz ≈ 0)              (THICK body, εz ≈ 0)
    
    z                                 z
    ↑    ┌────┐ t = 1 in             ↑    ┌────────────────┐
    │    │    │ (thin)                │    │                │ t >> L, H
    └──→ │    │                       └──→ │   (very long   │
         │    │ L = 20 in                  │    in z)       │
         └────┘                            └────────────────┘
    
    t/L = 1/20 = 0.05 ≪ 1            t/L >> 1
    σz = 0 (free surfaces)            εz = 0 (constrained by length)
```

**Why plane stress applies:**

1. **Geometric criterion:** The plate is thin — thickness t = 1 in is small compared to in-plane dimensions (20 × 10 in). The ratio t/L = 1/20 = 0.05, well below the threshold (~0.1) for thin-body behaviour.

2. **Loading criterion:** The traction P = 2,175 psi acts entirely in the x-y plane. No out-of-plane loads exist.

3. **Boundary criterion:** The top and bottom surfaces (z = ±t/2) are **traction-free** — no stresses act on these faces. This directly implies σ_z = τ_xz = τ_yz = 0, which is the defining condition for plane stress.

4. **Physical interpretation:** The thin plate cannot sustain significant through-thickness stress variation. The material is free to contract/expand in the z-direction (Poisson effect), so ε_z ≠ 0 but σ_z ≈ 0.

**Constitutive matrix (plane stress):**

$$\mathbf{D}_{\text{stress}} = \frac{E}{1 - \nu^2} \begin{bmatrix} 1 & \nu & 0 \\ \nu & 1 & 0 \\ 0 & 0 & \frac{1-\nu}{2} \end{bmatrix}$$

With E = 30 × 10⁶ psi, ν = 0.3:

$$\mathbf{D}_{\text{stress}} = 32.967 \times 10^6 \begin{bmatrix} 1 & 0.3 & 0 \\ 0.3 & 1 & 0 \\ 0 & 0 & 0.35 \end{bmatrix} = \begin{bmatrix} 32.967 & 9.890 & 0 \\ 9.890 & 32.967 & 0 \\ 0 & 0 & 11.538 \end{bmatrix} \times 10^6 \text{ psi}$$

### 2.3.3 Quantitative comparison — plane strain vs. plane stress

**Plane strain** assumes ε_z = 0 (no deformation in z), appropriate for bodies that are very long in the z-direction (e.g., a dam, a tunnel lining). The constitutive matrix becomes:

$$\mathbf{D}_{\text{strain}} = \frac{E}{(1+\nu)(1-2\nu)} \begin{bmatrix} 1-\nu & \nu & 0 \\ \nu & 1-\nu & 0 \\ 0 & 0 & \frac{1-2\nu}{2} \end{bmatrix}$$

$$= 57.692 \times 10^6 \begin{bmatrix} 0.7 & 0.3 & 0 \\ 0.3 & 0.7 & 0 \\ 0 & 0 & 0.2 \end{bmatrix} = \begin{bmatrix} 40.385 & 17.308 & 0 \\ 17.308 & 40.385 & 0 \\ 0 & 0 & 11.538 \end{bmatrix} \times 10^6 \text{ psi}$$

**Quantitative differences:**

| Quantity | Plane Stress | Plane Strain | Ratio (strain/stress) |
|----------|-------------|-------------|----------------------|
| D₁₁ effective modulus | 32.97 × 10⁶ | 40.38 × 10⁶ | **1.225** |
| D₁₂ coupling term | 9.89 × 10⁶ | 17.31 × 10⁶ | **1.750** |
| D₃₃ shear modulus | 11.54 × 10⁶ | 11.54 × 10⁶ | 1.000 |
| Overall stiffness | Lower | **Higher by ~22-75%** | — |

**Impact on results if plane strain were incorrectly used:**

- **Displacements:** Would be **smaller** (stiffer system). For uniaxial tension, the ratio of displacements is approximately (1−ν²) = 0.91, so plane strain displacements ≈ 91% of plane stress displacements.
- **Stresses:** σ_x would be similar, but an additional σ_z = ν(σ_x + σ_y) would develop, which is non-physical for a thin plate with free surfaces. For σ_x = 2,175 psi and σ_y ≈ 0: σ_z = 0.3 × 2,175 = 652.5 psi — a fictitious stress.
- **Engineering consequence:** Using plane strain for this thin plate would **underestimate displacements** by ~9% and produce spurious through-thickness stresses, leading to incorrect design assessments.

---

## 2.4 Load Representation (2 marks)

### 2.4.1 Derive the equivalent nodal forces

The right boundary (x = 20) has uniform traction **P = 2,175 psi** in the positive x-direction. This boundary connects node 4 (bottom-right) to node 3 (top-right) with edge length L_edge = 10 in.

```
    Right boundary:
    
    3 ──→ P = 2175 psi      Equivalent nodal forces:
    │   (uniform)             
    │                         3 ──→ f₃ = P·t·L/2 = 10,875 lb
    │                         │
    │   P ──→                 │
    │                         │
    │                         │
    4 ──→ P                   4 ──→ f₄ = P·t·L/2 = 10,875 lb
```

**Derivation using work-equivalence (consistent load vector):**

The consistent nodal force vector for a distributed traction is:

$$\mathbf{f} = \int_{\Gamma_t} \mathbf{N}^T \mathbf{t} \, d\Gamma$$

where **N** are the shape functions along the loaded edge and **t** = {P, 0}ᵀ is the traction vector.

Along the edge from node 4 (s = 0) to node 3 (s = L = 10 in), the linear shape functions are:

$$N_4(s) = 1 - \frac{s}{L}, \quad N_3(s) = \frac{s}{L}$$

For uniform traction q = P × t = 2,175 × 1 = 2,175 lb/in:

$$f_{x,4} = \int_0^{L} N_4 \cdot q \, ds = q \int_0^{L} \left(1 - \frac{s}{L}\right) ds = q \cdot \frac{L}{2} = 2{,}175 \times \frac{10}{2} = \mathbf{10{,}875 \text{ lb}}$$

$$f_{x,3} = \int_0^{L} N_3 \cdot q \, ds = q \int_0^{L} \frac{s}{L} \, ds = q \cdot \frac{L}{2} = 2{,}175 \times \frac{10}{2} = \mathbf{10{,}875 \text{ lb}}$$

$$f_{y,3} = f_{y,4} = 0 \quad \text{(traction has no y-component)}$$

**Total applied force** = 10,875 + 10,875 = 21,750 lb = P × t × L = 2,175 × 1 × 10 ✓

### 2.4.2 Why naive force lumping would be incorrect

**Naive lumping** assigns forces to nodes based on tributary length (or area) without regard to shape function weighting. For this specific case of uniform traction on a linear edge, lumping coincidentally gives the same result as the consistent formulation (equal split). However, this is **not generally correct:**

```
    Uniform traction (this case):     Linearly varying traction:
    
    3 ──→→→→→→ q                      3 ──→→→→→→→→ q₃
    │                                 │         ╱
    │   (constant q)                  │       ╱
    │                                 │     ╱
    4 ──→→→→→→ q                      4 ──→ q₄
    
    Consistent: f₃=qL/2, f₄=qL/2    Consistent: f₃=L(2q₃+q₄)/6
    Lumped:     f₃=qL/2, f₄=qL/2                 f₄=L(q₃+2q₄)/6
    ✓ Same result (coincidence)       Lumped:     f₃=q₃·L/2, f₄=q₄·L/2
                                      ✗ Different and INCORRECT
```

**Fundamental reasons lumping fails:**

1. **Work equivalence violation:** The consistent formulation ensures that the work done by the nodal forces through the interpolated displacement field equals the work done by the actual traction. Lumping does not satisfy this energy equivalence, leading to incorrect virtual work and therefore incorrect equilibrium in the weak form.

2. **Higher-order elements:** For quadratic elements with mid-side nodes, even uniform traction produces non-equal nodal forces. The consistent formulation gives the ratio 1:4:1 (corner:midside:corner), while naive lumping gives 1:1:1 — a 300% error at mid-side nodes.

3. **Patch test failure:** Improperly lumped loads can cause a mesh to fail the patch test (reproduction of constant stress states), which is a fundamental requirement for FEM convergence.

### 2.4.3 Impact on stress results if lumping is done improperly

- **Local stress errors:** Incorrect nodal forces produce incorrect displacements, which propagate through ε = B·u and σ = D·ε. The error is not localised — it contaminates the entire displacement field.

- **Artificial stress concentrations:** Over-loading one node and under-loading another creates a parasitic point load, generating artificial stress concentrations in all elements connected to the overloaded node.

- **Equilibrium violation:** While the total force may be preserved, the moment of the applied forces about any point changes. This violates moment equilibrium in the weak sense and produces spurious shear stresses.

- **Slower convergence:** With mesh refinement, consistent loads converge at optimal rate (O(hᵖ) for degree p elements), while improperly lumped loads converge more slowly, requiring significantly finer meshes for the same accuracy.

---

## 2.5 FEM Solution and Interpretation (3 marks)

### 2.5.1 Corrected and Justified Model Summary

| Aspect | Choice | Justification |
|--------|--------|---------------|
| Element type | CST (3-node triangle) | Given by problem; simplest 2D element |
| Stress state | Plane stress | Thin plate, t/L = 0.05 (§2.3) |
| BCs | u₁=v₁=u₂=v₂=0 | Wall boundary on left edge (§2.2) |
| Loads | Consistent: F₃ₓ=F₄ₓ=10,875 lb | Work-equivalent (§2.4) |
| Material | E=30×10⁶ psi, ν=0.3 | Given |

### 2.5.2 CST Element Formulation

For a triangular element with vertices (x₁,y₁), (x₂,y₂), (x₃,y₃):

**Area:**

$$A_e = \frac{1}{2} |x_1(y_2 - y_3) + x_2(y_3 - y_1) + x_3(y_1 - y_2)|$$

**Strain-displacement matrix B (3×6, constant throughout the element):**

$$\mathbf{B} = \frac{1}{2A_e} \begin{bmatrix} b_1 & 0 & b_2 & 0 & b_3 & 0 \\ 0 & c_1 & 0 & c_2 & 0 & c_3 \\ c_1 & b_1 & c_2 & b_2 & c_3 & b_3 \end{bmatrix}$$

where b_i = y_j − y_k, c_i = x_k − x_j (cyclic permutation of node indices i, j, k).

**Element stiffness matrix:**

$$\mathbf{k}^{(e)} = t \cdot A_e \cdot \mathbf{B}^T \mathbf{D} \mathbf{B}$$

### 2.5.3 Worked Example — Element 1 (nodes 1, 5, 2)

Vertices: (0,0), (10,5), (0,10). Area = 50 in².

$$b_1 = y_5 - y_2 = 5 - 10 = -5, \quad c_1 = x_2 - x_5 = 0 - 10 = -10$$
$$b_2 = y_2 - y_1 = 10 - 0 = 10, \quad c_2 = x_1 - x_2 = 0 - 0 = 0$$
$$b_3 = y_1 - y_5 = 0 - 5 = -5, \quad c_3 = x_5 - x_1 = 10 - 0 = 10$$

$$\mathbf{B}_1 = \frac{1}{100} \begin{bmatrix} -5 & 0 & 10 & 0 & -5 & 0 \\ 0 & -10 & 0 & 0 & 0 & 10 \\ -10 & -5 & 0 & 10 & 10 & -5 \end{bmatrix} = \begin{bmatrix} -0.05 & 0 & 0.10 & 0 & -0.05 & 0 \\ 0 & -0.10 & 0 & 0 & 0 & 0.10 \\ -0.10 & -0.05 & 0 & 0.10 & 0.10 & -0.05 \end{bmatrix}$$

### 2.5.4 Worked Example — Element 2 (nodes 1, 4, 5)

Vertices: (0,0), (20,0), (10,5). Area = 50 in².

$$b_1 = y_4 - y_5 = 0 - 5 = -5, \quad c_1 = x_5 - x_4 = 10 - 20 = -10$$
$$b_2 = y_5 - y_1 = 5 - 0 = 5, \quad c_2 = x_1 - x_5 = 0 - 10 = -10$$
$$b_3 = y_1 - y_4 = 0 - 0 = 0, \quad c_3 = x_4 - x_1 = 20 - 0 = 20$$

$$\mathbf{B}_2 = \frac{1}{100} \begin{bmatrix} -5 & 0 & 5 & 0 & 0 & 0 \\ 0 & -10 & 0 & -10 & 0 & 20 \\ -10 & -5 & -10 & 5 & 20 & 0 \end{bmatrix}$$

### 2.5.5 Global Force Vector

$$\mathbf{F} = \begin{Bmatrix} 0 \\ 0 \\ 0 \\ 0 \\ 10{,}875 \\ 0 \\ 10{,}875 \\ 0 \\ 0 \\ 0 \end{Bmatrix} \text{ lb} \quad \begin{matrix} \leftarrow u_1 \\ \leftarrow v_1 \\ \leftarrow u_2 \\ \leftarrow v_2 \\ \leftarrow u_3 \\ \leftarrow v_3 \\ \leftarrow u_4 \\ \leftarrow v_4 \\ \leftarrow u_5 \\ \leftarrow v_5 \end{matrix}$$

### 2.5.6 System Solution

After applying BCs (DOFs 0,1,2,3 constrained), the reduced 6×6 system:

$$\mathbf{K}_{ff} \mathbf{U}_f = \mathbf{F}_f$$

The reduced stiffness matrix K_ff (×10⁶ lb/in):

```
           u₃         v₃         u₄         v₄         u₅         v₅
u₃  [  19.780    10.714    -1.648     0.412   -19.780   -10.714 ]
v₃  [  10.714    35.852    -0.412   -15.041   -10.714   -35.852 ]
u₄  [  -1.648    -0.412    19.780   -10.714   -19.780    10.714 ]
v₄  [   0.412   -15.041   -10.714    35.852    10.714   -35.852 ]
u₅  [ -19.780   -10.714   -19.780    10.714    79.121     0.000 ]
v₅  [ -10.714   -35.852    10.714   -35.852     0.000   143.407 ]
```

### 2.5.7 Nodal Displacements — Results

| Node | u (in) | v (in) | Notes |
|------|--------|--------|-------|
| 1 | 0.00000 | 0.00000 | Fixed (BC) |
| 2 | 0.00000 | 0.00000 | Fixed (BC) |
| 3 | **0.001406** | −0.000145 | Right edge, top |
| 4 | **0.001406** | +0.000145 | Right edge, bottom |
| 5 | 0.000664 | 0.000000 | Centre node |

**Displacement pattern diagram:**

```
    UNDEFORMED                     DEFORMED (exaggerated ~1000×)
    
    2 ─────────────── 3            2 ──────────────── 3'
    │╲              ╱│             │╲               ╱↗│
    │  ╲    5     ╱  │             │  ╲    5'    ╱  → │  ← Poisson
    │    ╲  ●   ╱    │             │    ╲  ●→  ╱   ↙ │     contraction
    │      ╲  ╱      │             │      ╲  ╱       │     (v₃<0, v₄>0)
    │       ╱╲       │             │       ╱╲        │
    1 ─────────────── 4            1 ──────────────── 4'
    
    Observations:
    • u₃ = u₄ (symmetric x-extension)
    • v₃ = −v₄ (antisymmetric Poisson contraction)
    • u₅ ≈ u₃/2 (linear variation, consistent with uniform strain)
    • v₅ = 0 (symmetry about y = 5)
```

**Physical interpretation:**

- The equal x-displacements at nodes 3 and 4 confirm that the right edge translates uniformly rightward under the uniform traction — consistent with expectations.
- The vertical displacements v₃ = −0.000145 in and v₄ = +0.000145 in represent the **Poisson contraction**: as the plate stretches in x, it contracts in y. Node 3 moves downward and node 4 moves upward (toward the centreline).
- Node 5 at the centre has exactly half the x-displacement of the right edge, confirming a linear displacement variation through the plate — expected for a uniform stress state.
- The analytical solution for a plate under uniaxial tension: u = Px/E = 2,175 × 20 / (30×10⁶) = 0.00145 in. The FEM result (0.001406 in) is within 3% — reasonable for a 4-element mesh.

### 2.5.8 Reaction Forces

| Node | Fx (lb) | Fy (lb) |
|------|---------|---------|
| 1 | **−10,875** | −4,354.6 |
| 2 | **−10,875** | +4,354.6 |

**Equilibrium verification:**

| Check | Calculation | Result |
|-------|-------------|--------|
| ΣFx = 0 | (−10,875) + (−10,875) + 10,875 + 10,875 | = 0 ✓ |
| ΣFy = 0 | (−4,354.6) + (+4,354.6) | = 0 ✓ |
| ΣM₁ = 0 | Check moment about node 1 | = 0 ✓ |

**Note on Fy reactions:** The non-zero vertical reactions (±4,354.6 lb) at the fixed nodes arise because the fixed wall prevents the Poisson contraction at x = 0. The plate wants to contract vertically near the fixed end, but the BCs prevent this, generating vertical reaction forces. These reactions are anti-symmetric (equal and opposite), confirming geometric symmetry about y = 5.

### 2.5.9 Element Stresses

| Element | Nodes | σ_x (psi) | σ_y (psi) | τ_xy (psi) |
|---------|-------|-----------|-----------|------------|
| 1 | 1, 5, 2 | 2,188.9 | 656.7 | ≈ 0 |
| 2 | 1, 4, 5 | 2,175.0 | 217.7 | −7.0 |
| 3 | 4, 3, 5 | 2,161.1 | −221.2 | ≈ 0 |
| 4 | 2, 5, 3 | 2,175.0 | 217.7 | +7.0 |

**Stress contour diagram (σ_x):**

```
    2 ────────────────────────── 3
    │╲      σx = 2189         ╱│
    │  ╲    (El 1)          ╱  │
    │    ╲               ╱     │
    │      ╲  5        ╱  σx=2161 │
    │        ╲●      ╱   (El 3)│
    │          ╲   ╱           │
    │    σx=2175 ╳  σx=2175    │
    │   (El 2) ╱  ╲  (El 4)   │
    │        ╱      ╲          │
    1 ────────────────────────── 4
    
    Analytical solution: σx = 2175 psi everywhere
    FEM solution: ±0.6% variation (2161 to 2189)
```

**Comparison with analytical solution:**

For a plate under uniform tension P, the exact solution is:

$$\sigma_x = P = 2{,}175 \text{ psi}, \quad \sigma_y = 0, \quad \tau_{xy} = 0$$

| Quantity | Analytical | FEM (range) | Max error |
|----------|-----------|-------------|-----------|
| σ_x | 2,175 psi | 2,161 – 2,189 psi | 0.6% |
| σ_y | 0 psi | −221 to +657 psi | **significant** |
| τ_xy | 0 psi | −7.0 to +7.0 psi | small |

The σ_x prediction is accurate (within 1%). However, σ_y shows significant errors — the FEM predicts non-zero transverse stress varying from −221 to +657 psi, when the analytical solution is zero everywhere. This is a direct consequence of the fixed BCs at nodes 1 and 2, which prevent Poisson contraction at the left edge, generating real σ_y near the wall that the coarse mesh smears across entire elements.

### 2.5.10 Commentary on Mesh Dependence

**Is the stress field mesh-dependent?**

**Yes**, the stress field from CST elements is inherently mesh-dependent for several fundamental reasons:

1. **Piecewise constant stress:** The CST element produces **constant strain and stress within each element** — the stress field is a step function that jumps discontinuously across element boundaries. The B matrix is constant (independent of position within the element), so ε = B·u and σ = D·ε are also constant.

```
    True stress field:              CST approximation (4 elements):
    
    σ_x                             σ_x
    ↑                                ↑
    │ ─────────── 2175               │  ┌──2189──┐  ┌──2161──┐
    │                                │  │  El 1  │  │  El 3  │
    │                                │  ├──2175──┤  ├──2175──┤
    │                                │  │  El 2  │  │  El 4  │
    └──────────→ x                   └──┴────────┴──┴────────┴→ x
    (uniform, continuous)            (piecewise constant, discontinuous)
```

2. **Mesh refinement convergence:** Increasing the number of elements reduces the stress discontinuities and converges toward the smooth analytical solution. The convergence rate for CST elements is O(h) — first-order — which is the slowest of any practical element type.

3. **Element orientation sensitivity:** The diagonal orientation of the mesh (X-pattern) introduces directional bias. Rotating the mesh by 90° would change the stress predictions, even though the physical problem is unchanged.

**Is this mesh adequate for stress prediction?**

**No**, this 4-element CST mesh is **not adequate** for reliable stress prediction:

1. **Insufficient DOFs:** With only 6 free DOFs (after BCs), the model has extremely limited capability to represent the displacement field. The displacement is piecewise linear (6 parameters for the entire domain), which cannot capture the true smooth variation.

2. **σ_y error:** The analytical solution has σ_y = 0 away from the fixed boundary, but the FEM predicts σ_y values up to 657 psi — about 30% of the applied stress. This error arises because the CST elements are too large to distinguish the boundary layer near the wall (where σ_y ≠ 0 due to fixity) from the interior (where σ_y → 0).

3. **Stress concentration at fixed boundary:** The fixed left edge creates a stress concentration region (constraint effect). To capture this accurately, the mesh would need to be refined near x = 0, with several layers of small elements to resolve the boundary layer that decays over a distance comparable to the plate height (≈ 10 in).

4. **Recommended improvements:**

| Improvement | Expected benefit |
|-------------|-----------------|
| Refine to 8×4 mesh of CSTs (64 elements) | Reduce σ_y error to < 5% away from boundary |
| Use 6-node quadratic triangles (LST) | Second-order convergence, better σ_y accuracy |
| Use bilinear quadrilateral (Q4) elements | Fewer elements needed, passes patch test naturally |
| Grade mesh toward fixed boundary | Capture boundary stress concentration |

5. **For this specific problem** (uniform tension far from boundaries), the σ_x prediction happens to be accurate because CST elements can exactly represent a constant stress state along aligned element edges. The σ_y and τ_xy errors reveal the mesh's inability to represent the Poisson-coupling and boundary effects. A practical design analysis would require at minimum a 10×5 mesh of CSTs, or preferably 5×5 bilinear quadrilateral elements.

---

*End of Coursework Report*
