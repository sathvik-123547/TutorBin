<h1 align="center">Finite Element Method — Coursework Report</h1>

<p align="center">
<b>Module:</b> Finite Element Method<br>
<b>P-number example used:</b> P----075 &rarr; X = 5<br>
<b>Coursework weighting:</b> 18% of total module mark (9% per question)
</p>

---

## Table of Contents

<table>
<tr><td><b>Section</b></td><td><b>Topic</b></td><td><b>Marks</b></td></tr>
<tr><td colspan="3"><b>Question 1 — 2D Truss Analysis</b></td></tr>
<tr><td>1.1</td><td>Problem Definition</td><td>—</td></tr>
<tr><td>1.2</td><td>Numerical Analysis</td><td>3 / 9</td></tr>
<tr><td>1.3</td><td>Critical Analysis (a–g)</td><td>6 / 9</td></tr>
<tr><td colspan="3"><b>Question 2 — 2D Plane Stress / CST Analysis</b></td></tr>
<tr><td>2.1</td><td>Problem Definition</td><td>—</td></tr>
<tr><td>2.2</td><td>Boundary Condition Verification</td><td>2 / 9</td></tr>
<tr><td>2.3</td><td>Stress State Assessment</td><td>2 / 9</td></tr>
<tr><td>2.4</td><td>Load Representation</td><td>2 / 9</td></tr>
<tr><td>2.5</td><td>FEM Solution and Interpretation</td><td>3 / 9</td></tr>
</table>

---

## Nomenclature

| Symbol | Description | Unit |
|:------:|:------------|:----:|
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
| c, s | cos θ, sin θ (direction cosines) | — |
| **K** | Global stiffness matrix | N/mm |
| **k** | Element stiffness matrix | N/mm |
| **B** | Strain–displacement matrix | 1/mm |
| **D** | Material constitutive matrix | MPa |
| **F** | Global force vector | N |
| **U** | Global displacement vector | mm |
| DOF | Degree of freedom | — |
| CST | Constant Strain Triangle | — |

---

<h1>Question 1 — 2D Truss Analysis</h1>

## 1.1 Problem Definition

### 1.1.1 Structural Description

A 2D truss structure comprising **nine bar elements** and **six nodes**.

### 1.1.2 Structural Diagram

<table><tr><td>

```
         W=12.5kN
            <--- 3 ----------- 5 ----------- 6
                /|\            |`.          .'|
              /  | \     [5]   |  `.  [9] .' |
            / [3]|  \          | [7]`.  .'   |
          /      |   \   [6]   |     .`x     |
        /        | [4]\       |   .'   `.   |
       2 --------+---- 4      | .'  [8]  `. |
        \  [4]  /      ^      |'           `.|
     [2] \    /     P=15kN    5 ----------- 6
     ^     \/                                 ^
   P=15kN   \   [1]                       [PIN]
              \                          (u6=v6=0)
               1
            [ROLLER]
           (v1=0, u1=free)

     |--2.4m--|--2.4m--|--2.4m--|
```

</td></tr></table>

### 1.1.3 Given Data

| Parameter | Value |
|:----------|:-----:|
| Young's modulus, E | 30,000 MPa |
| Cross-sectional area, A | 20,000 mm² |
| Vertical load, P = (10 + X) | **15 kN** (nodes 2 & 4, downward) |
| Horizontal load, W = (7.5 + X) | **12.5 kN** (node 3, leftward) |
| Node 1 support | Roller (restrains v₁ only) |
| Node 6 support | Pin (restrains u₆ and v₆) |
| Axial rigidity, EA | 6 × 10⁸ N |

### 1.1.4 Node Coordinates

| Node | x (mm) | y (mm) | Description |
|:----:|:------:|:------:|:------------|
| 1 | 0 | 0 | Bottom-left, roller support |
| 2 | 2,400 | 1,800 | Lower chord, loaded (P↓) |
| 3 | 2,400 | 5,400 | Upper chord, loaded (W←) |
| 4 | 4,800 | 1,800 | Lower chord, loaded (P↓) |
| 5 | 4,800 | 5,400 | Upper chord |
| 6 | 7,200 | 5,400 | Upper chord, pin support |

### 1.1.5 Element Connectivity

| Element | From → To | Orientation |
|:-------:|:---------:|:------------|
| 1 | 1 → 2 | Inclined (lower-left diagonal) |
| 2 | 1 → 4 | Inclined (long diagonal) |
| 3 | 2 → 3 | Vertical |
| 4 | 2 → 4 | Horizontal (lower chord) |
| 5 | 3 → 4 | Inclined (upper-to-lower diagonal) |
| 6 | 3 → 5 | Horizontal (upper chord) |
| 7 | 4 → 5 | Vertical |
| 8 | 5 → 6 | Horizontal (upper chord) |
| 9 | 4 → 6 | Inclined (lower-to-upper diagonal) |

---

## 1.2 Numerical Analysis (3 marks)

### 1.2.1 Element Type and Degrees of Freedom

> **Element type:** 2-node linear bar (truss) element — a 1D element embedded in 2D space.  
> Each element transmits only **axial force**; no bending, shear, or torsion.

**Degrees of freedom:**
- Per node: 2 (horizontal u, vertical v)
- Total nodes: 6
- **Total DOFs: 12**

<table>
<tr><th>Node</th><th>1</th><th>2</th><th>3</th><th>4</th><th>5</th><th>6</th></tr>
<tr><td><b>DOF u</b></td><td>0</td><td>2</td><td>4</td><td>6</td><td>8</td><td>10</td></tr>
<tr><td><b>DOF v</b></td><td>1</td><td>3</td><td>5</td><td>7</td><td>9</td><td>11</td></tr>
</table>

*(0-indexed numbering used throughout)*

### 1.2.2 Element Geometry Calculations

For each element connecting node *i* to node *j*:

$$L = \sqrt{(\Delta x)^2 + (\Delta y)^2}, \qquad c = \frac{\Delta x}{L}, \qquad s = \frac{\Delta y}{L}$$

| El. | From→To | Δx (mm) | Δy (mm) | L (mm) | cos θ | sin θ | EA/L (N/mm) |
|:---:|:-------:|--------:|--------:|-------:|------:|------:|------------:|
| 1 | 1→2 | 2,400 | 1,800 | 3,000.00 | 0.8000 | 0.6000 | 200,000 |
| 2 | 1→4 | 4,800 | 1,800 | 5,126.40 | 0.9363 | 0.3511 | 117,041 |
| 3 | 2→3 | 0 | 3,600 | 3,600.00 | 0.0000 | 1.0000 | 166,667 |
| 4 | 2→4 | 2,400 | 0 | 2,400.00 | 1.0000 | 0.0000 | 250,000 |
| 5 | 3→4 | 2,400 | −3,600 | 4,326.66 | 0.5547 | −0.8321 | 138,675 |
| 6 | 3→5 | 2,400 | 0 | 2,400.00 | 1.0000 | 0.0000 | 250,000 |
| 7 | 4→5 | 0 | 3,600 | 3,600.00 | 0.0000 | 1.0000 | 166,667 |
| 8 | 5→6 | 2,400 | 0 | 2,400.00 | 1.0000 | 0.0000 | 250,000 |
| 9 | 4→6 | 2,400 | 3,600 | 4,326.66 | 0.5547 | 0.8321 | 138,675 |

### 1.2.3 Element Stiffness Matrix — General Form

For a 2D truss element, the **4×4 stiffness matrix** in global coordinates:

$$\mathbf{k}^{(e)} = \frac{EA}{L} \begin{bmatrix} c^2 & cs & -c^2 & -cs \\\\ cs & s^2 & -cs & -s^2 \\\\ -c^2 & -cs & c^2 & cs \\\\ -cs & -s^2 & cs & s^2 \end{bmatrix}$$

This maps to global DOFs $[2i, \ 2i{+}1, \ 2j, \ 2j{+}1]$ for element connecting nodes *i* and *j*.

### 1.2.4 Worked Example — Element 1 (Nodes 1→2, Inclined)

> c = 0.8, &ensp; s = 0.6, &ensp; EA/L = 200,000 N/mm

$$\mathbf{k}^{(1)} = 200\text{,}000 \begin{bmatrix} 0.64 & 0.48 & -0.64 & -0.48 \\\\ 0.48 & 0.36 & -0.48 & -0.36 \\\\ -0.64 & -0.48 & 0.64 & 0.48 \\\\ -0.48 & -0.36 & 0.48 & 0.36 \end{bmatrix}$$

<table>
<tr><th></th><th>u₁</th><th>v₁</th><th>u₂</th><th>v₂</th></tr>
<tr><td><b>u₁</b></td><td align="right">128,000</td><td align="right">96,000</td><td align="right">−128,000</td><td align="right">−96,000</td></tr>
<tr><td><b>v₁</b></td><td align="right">96,000</td><td align="right">72,000</td><td align="right">−96,000</td><td align="right">−72,000</td></tr>
<tr><td><b>u₂</b></td><td align="right">−128,000</td><td align="right">−96,000</td><td align="right">128,000</td><td align="right">96,000</td></tr>
<tr><td><b>v₂</b></td><td align="right">−96,000</td><td align="right">−72,000</td><td align="right">96,000</td><td align="right">72,000</td></tr>
</table>

**Maps to global DOFs:** {0, 1, 2, 3}

### 1.2.5 Worked Example — Element 4 (Nodes 2→4, Horizontal)

> c = 1.0, &ensp; s = 0.0, &ensp; EA/L = 250,000 N/mm

<table>
<tr><th></th><th>u₂</th><th>v₂</th><th>u₄</th><th>v₄</th></tr>
<tr><td><b>u₂</b></td><td align="right">250,000</td><td align="right">0</td><td align="right">−250,000</td><td align="right">0</td></tr>
<tr><td><b>v₂</b></td><td align="right">0</td><td align="right">0</td><td align="right">0</td><td align="right">0</td></tr>
<tr><td><b>u₄</b></td><td align="right">−250,000</td><td align="right">0</td><td align="right">250,000</td><td align="right">0</td></tr>
<tr><td><b>v₄</b></td><td align="right">0</td><td align="right">0</td><td align="right">0</td><td align="right">0</td></tr>
</table>

**Maps to global DOFs:** {2, 3, 6, 7}

> Note: For a horizontal element (s = 0), only the u–u stiffness terms are nonzero. The v DOFs are decoupled — the element has no stiffness in the transverse direction.

### 1.2.6 Worked Example — Element 3 (Nodes 2→3, Vertical)

> c = 0.0, &ensp; s = 1.0, &ensp; EA/L = 166,667 N/mm

<table>
<tr><th></th><th>u₂</th><th>v₂</th><th>u₃</th><th>v₃</th></tr>
<tr><td><b>u₂</b></td><td align="right">0</td><td align="right">0</td><td align="right">0</td><td align="right">0</td></tr>
<tr><td><b>v₂</b></td><td align="right">0</td><td align="right">166,667</td><td align="right">0</td><td align="right">−166,667</td></tr>
<tr><td><b>u₃</b></td><td align="right">0</td><td align="right">0</td><td align="right">0</td><td align="right">0</td></tr>
<tr><td><b>v₃</b></td><td align="right">0</td><td align="right">−166,667</td><td align="right">0</td><td align="right">166,667</td></tr>
</table>

**Maps to global DOFs:** {2, 3, 4, 5}

> Note: For a vertical element (c = 0), only the v–v stiffness terms are nonzero — the mirror image of the horizontal case.

### 1.2.7 Global Stiffness Matrix Assembly

The 12×12 global stiffness matrix is assembled by the **direct stiffness method:**

$$\mathbf{K} = \sum_{e=1}^{9} \mathbf{A}^{(e)T}\,\mathbf{k}^{(e)}\,\mathbf{A}^{(e)}$$

**Assembly map** — which global DOFs each element contributes to:

| Element | Node i | Node j | Global DOFs (0-indexed) |
|:-------:|:------:|:------:|:-----------------------:|
| 1 | 1 | 2 | 0, 1, 2, 3 |
| 2 | 1 | 4 | 0, 1, 6, 7 |
| 3 | 2 | 3 | 2, 3, 4, 5 |
| 4 | 2 | 4 | 2, 3, 6, 7 |
| 5 | 3 | 4 | 4, 5, 6, 7 |
| 6 | 3 | 5 | 4, 5, 8, 9 |
| 7 | 4 | 5 | 6, 7, 8, 9 |
| 8 | 5 | 6 | 8, 9, 10, 11 |
| 9 | 4 | 6 | 6, 7, 10, 11 |

<details>
<summary><b>Click to expand: Full 12×12 Global Stiffness Matrix K (kN/mm)</b></summary>

<table>
<tr><th></th><th>u₁</th><th>v₁</th><th>u₂</th><th>v₂</th><th>u₃</th><th>v₃</th><th>u₄</th><th>v₄</th><th>u₅</th><th>v₅</th><th>u₆</th><th>v₆</th></tr>
<tr><td><b>u₁</b></td><td>230.61</td><td>134.48</td><td>−128.00</td><td>−96.00</td><td>0</td><td>0</td><td>−102.61</td><td>−38.48</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
<tr><td><b>v₁</b></td><td>134.48</td><td>86.43</td><td>−96.00</td><td>−72.00</td><td>0</td><td>0</td><td>−38.48</td><td>−14.43</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
<tr><td><b>u₂</b></td><td>−128.00</td><td>−96.00</td><td>378.00</td><td>96.00</td><td>0</td><td>0</td><td>−250.00</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
<tr><td><b>v₂</b></td><td>−96.00</td><td>−72.00</td><td>96.00</td><td>238.67</td><td>0</td><td>−166.67</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
<tr><td><b>u₃</b></td><td>0</td><td>0</td><td>0</td><td>0</td><td>292.67</td><td>−64.00</td><td>−42.67</td><td>64.00</td><td>−250.00</td><td>0</td><td>0</td><td>0</td></tr>
<tr><td><b>v₃</b></td><td>0</td><td>0</td><td>0</td><td>−166.67</td><td>−64.00</td><td>262.67</td><td>64.00</td><td>−96.01</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
<tr><td><b>u₄</b></td><td>−102.61</td><td>−38.48</td><td>−250.00</td><td>0</td><td>−42.67</td><td>64.00</td><td>437.95</td><td>38.48</td><td>0</td><td>0</td><td>−42.67</td><td>−64.00</td></tr>
<tr><td><b>v₄</b></td><td>−38.48</td><td>−14.43</td><td>0</td><td>0</td><td>64.00</td><td>−96.01</td><td>38.48</td><td>373.11</td><td>0</td><td>−166.67</td><td>−64.00</td><td>−96.01</td></tr>
<tr><td><b>u₅</b></td><td>0</td><td>0</td><td>0</td><td>0</td><td>−250.00</td><td>0</td><td>0</td><td>0</td><td>500.00</td><td>0</td><td>−250.00</td><td>0</td></tr>
<tr><td><b>v₅</b></td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>−166.67</td><td>0</td><td>166.67</td><td>0</td><td>0</td></tr>
<tr><td><b>u₆</b></td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>−42.67</td><td>−64.00</td><td>−250.00</td><td>0</td><td>292.67</td><td>64.00</td></tr>
<tr><td><b>v₆</b></td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>−64.00</td><td>−96.01</td><td>0</td><td>0</td><td>64.00</td><td>96.01</td></tr>
</table>

> **Properties:** Symmetric ( $K_{ij} = K_{ji}$ ), positive semi-definite before BCs, sparse (many zeros reflecting that non-adjacent nodes share no direct stiffness).

</details>

### 1.2.8 Boundary Conditions

<table>
<tr>
<td width="50%">

**Node 1 — ROLLER**
- u₁ = **free**
- v₁ = **0** (constrained)
- Provides reaction: R₁y
- DOF 1 eliminated

</td>
<td width="50%">

**Node 6 — PIN**
- u₆ = **0** (constrained)
- v₆ = **0** (constrained)
- Provides reactions: R₆x, R₆y
- DOFs 10, 11 eliminated

</td>
</tr>
</table>

> **Constrained DOFs:** {1, 10, 11}  
> **Free DOFs:** {0, 2, 3, 4, 5, 6, 7, 8, 9}  
> **Reduced system size:** 9 × 9

### 1.2.9 Applied Load Vector

| DOF | Node | Direction | Applied Load | Notes |
|:---:|:----:|:---------:|:------------:|:------|
| 0 | 1 | u | 0 | |
| 1 | 1 | v | — | *constrained* |
| 2 | 2 | u | 0 | |
| 3 | 2 | v | **−15,000 N** | P downward |
| 4 | 3 | u | **−12,500 N** | W leftward |
| 5 | 3 | v | 0 | |
| 6 | 4 | u | 0 | |
| 7 | 4 | v | **−15,000 N** | P downward |
| 8 | 5 | u | 0 | |
| 9 | 5 | v | 0 | |
| 10 | 6 | u | — | *constrained* |
| 11 | 6 | v | — | *constrained* |

> **Sign convention:** positive = rightward (u) and upward (v).

### 1.2.10 Solution of the Reduced System

Partitioning into free (f) and constrained (c) DOFs:

$$\mathbf{K}_{ff}\,\mathbf{U}_f = \mathbf{F}_f \qquad \Longrightarrow \qquad \mathbf{U}_f = \mathbf{K}_{ff}^{-1}\,\mathbf{F}_f$$

The 9×9 system is solved numerically. Constrained DOFs are set to zero.

### 1.2.11 Nodal Displacements — Results

| Node | u (mm) | v (mm) | Notes |
|:----:|-------:|-------:|:------|
| 1 | **+0.5300** | 0.0000 | v₁ = 0 (roller BC); shifts rightward |
| 2 | +1.4645 | **−1.6627** | Large vertical deflection |
| 3 | −0.0200 | −1.7527 | Largest total deflection |
| 4 | +1.3045 | −1.0259 | Loaded node |
| 5 | −0.0100 | −1.0259 | Same v as node 4 |
| 6 | 0.0000 | 0.0000 | Pin BC |

<table><tr><td>

**Deformed shape (exaggerated ×1000):**
```
  Undeformed (----)              Deformed (====)

  3 ----------- 5 ----------- 6      3'========= 5'========== 6
  |              |              |      '           '           |
  |              |              |       '          '           |
  2 ----------- 4              |       2'======= 4'          |
  |            /               |       |        /             |
  |          /                 |       v v    /               |
  |        /                   |     (nodes 2,4 drop most)   |
  1                            6       1'-->                  6
 ===                         xxx     (shifts right)         xxx
```

</td></tr></table>

### 1.2.12 Reaction Forces

| Node | Direction | Reaction Force |
|:----:|:---------:|:--------------:|
| 1 | Fy (vertical) | **+15,000 N = +15.00 kN** ↑ |
| 6 | Fx (horizontal) | **+12,500 N = +12.50 kN** → |
| 6 | Fy (vertical) | **+15,000 N = +15.00 kN** ↑ |

**Equilibrium verification:**

| Condition | Calculation | Result |
|:---------:|:------------|:------:|
| ΣFx = 0 | −12,500 + 12,500 | **= 0 ✓** |
| ΣFy = 0 | +15,000 + 15,000 − 15,000 − 15,000 | **= 0 ✓** |
| ΣM₁ = 0 | (−15000)(2400) + (−12500)(5400) + (−15000)(4800) + (12500)(5400) + (15000)(7200) | **= 0 ✓** |

> All three static equilibrium equations are satisfied exactly, confirming solution correctness.

### 1.2.13 Element Strains, Stresses, and Internal Forces

**Strain** for each element:

$$\varepsilon^{(e)} = \frac{c(u_j - u_i) + s(v_j - v_i)}{L}$$

**Stress:** $\sigma^{(e)} = E \cdot \varepsilon^{(e)}$ &emsp; **Force:** $F^{(e)} = \sigma^{(e)} \cdot A$

| El. | Nodes | Force (kN) | Strain (×10⁻⁵) | Stress (MPa) | State |
|:---:|:-----:|-----------:|----------------:|-------------:|:------|
| **1** | 1→2 | **−50.00** | −8.333 | **−2.500** | 🔴 Compression |
| 2 | 1→4 | +42.72 | +7.120 | +2.136 | 🟢 Tension |
| 3 | 2→3 | −15.00 | −2.500 | −0.750 | 🔴 Compression |
| **4** | 2→4 | **−40.00** | −6.667 | **−2.000** | 🔴 Compression |
| 5 | 3→4 | +18.03 | +3.005 | +0.901 | 🟢 Tension |
| 6 | 3→5 | +2.50 | +0.417 | +0.125 | 🟢 Tension |
| 7 | 4→5 | **0.00** | 0.000 | 0.000 | ⚪ Zero-force |
| 8 | 5→6 | +2.50 | +0.417 | +0.125 | 🟢 Tension |
| 9 | 4→6 | +18.03 | +3.005 | +0.901 | 🟢 Tension |

<table><tr><td>

**Element force summary diagram:**
```
          3 --[+2.5T]--- 5 --[+2.5T]--- 6
         /|              |             /|
       /  |            [ZERO]        /  |
  [-15C]  | [+18T]      |    [+18T]/   |
    /     |              |        /     |
  2 -----[-40C]----- 4  |      /      |
   \                  / \     /       |
    \ [-50C]        /    \  /        |
     \            /       \/         |
      \         /   [+42.7T]        |
       \      /                     |
        1  (ROLLER)              (PIN) 6
```

</td></tr></table>

> **Key observations:**
> - **Element 1** carries the highest force (50.0 kN compression) and stress (2.50 MPa) — it is the **most critical element**.
> - **Element 7** is a **zero-force member** — nodes 4 and 5 have identical vertical displacements.
> - Lower chord (El 1, 4) is in **compression**; diagonals (El 2, 5, 9) are in **tension** — consistent with downward loading.
> - All stresses ≪ 250 MPa (typical steel yield), confirming linear elastic behaviour.

---

## 1.3 Critical Analysis (6 marks)

### 1.3(a) Reconstruct the FEM model and justify all modelling assumptions — *1 mark*

The FEM model rests on seven assumptions, each justified below:

| # | Assumption | Justification |
|:-:|:-----------|:--------------|
| 1 | **Bar/truss element (axial only)** | Problem specifies pin joints → no moment transfer. All loads at nodes → no transverse member loading. Standard 2-DOF/node truss element is appropriate. |
| 2 | **Linear elastic material** | Max stress = 2.50 MPa, which is < 1% of steel yield (~250 MPa). Material remains firmly in the elastic regime. σ = Eε is valid. |
| 3 | **Small displacements** | Max displacement = 1.75 mm vs shortest element = 2,400 mm → ratio = 7.3 × 10⁻⁴. Deformed ≈ undeformed geometry. No geometric nonlinearity needed. |
| 4 | **2D planar analysis** | All nodes, elements, loads lie in one plane. No out-of-plane effects. 2 DOFs/node sufficient. |
| 5 | **Concentrated nodal loads** | P and W applied at specific nodes. No distributed loads → no work-equivalent transformation needed. |
| 6 | **Homogeneous, isotropic, uniform** | All elements share same E and A. Prescribed by problem statement. |
| 7 | **Self-weight neglected** | ⚠️ For a steel member with A = 20,000 mm² and ρ = 7,850 kg/m³, the longest element (El 2, L = 5.13 m) weighs ~7.9 kN — comparable to P = 15 kN. This introduces a **non-trivial** error and should be acknowledged. |

### 1.3(b) Predict sign and relative magnitude of nodal displacements — *1 mark*

*Without relying on the full numerical solution:*

**Horizontal displacements (u):**

The horizontal load W = 12.5 kN acts leftward at node 3. Since node 6 is pinned, R₆ₓ = +12.5 kN (rightward) to satisfy ΣFx = 0. Node 1 (roller) is free in x.

The diagonal members (El 2, El 9) connect the top chord to the bottom. Their tension pulls node 1 rightward → **u₁ > 0**. Nodes 2 and 4, at mid-height, also move rightward: **u₂ > 0, u₄ > 0**. Node 3 receives W directly leftward but is connected to rightward-moving nodes below → u₃ ≈ 0, slightly negative. Similarly u₅ ≈ 0.

**Vertical displacements (v):**

v₁ = 0 (roller BC), v₆ = 0 (pin BC). Nodes 2 and 4 receive P = 15 kN downward → **v₂ < 0, v₄ < 0**. Nodes 3 and 5 are pulled down through vertical members → **v₃ < 0, v₅ < 0**. Node 2 sits at the base of the tallest sub-structure above the roller → expect |v₂| to be large. Node 3 (above node 2) should have comparable deflection.

**Relative magnitudes:** |v₃| > |v₂| > |v₄| = |v₅|

> **Validation from numerical solution:** All signs match predictions. Computed: v₃ = −1.753 mm (largest), v₂ = −1.663 mm, v₄ = v₅ = −1.026 mm. ✓ Confirmed.

### 1.3(c) Most critical element in terms of stress — *1 mark*

> **Most critical: Element 1 (nodes 1→2)** — |σ| = **2.500 MPa** (compression), F = **−50.0 kN**

**Physical and structural reasoning (not merely numerical ranking):**

1. **Load path convergence at node 1:** The roller at node 1 provides R₁y = 15 kN upward. This reaction must be delivered through elements 1 and 2 — the only members connected to node 1. Element 1 provides the most direct path from the loaded node 2 to the support.

2. **Equivalent beam analogy:** Treating the truss as a simply-supported beam (roller + pin), the lower chord acts as the compression flange. Element 1 is part of this chord nearest the roller, where combined vertical load effects create the highest demand.

3. **Geometric amplification:** Element 1 is inclined at 36.87°. Its axial force must provide both horizontal and vertical equilibrium components at node 2 — requiring a larger axial force than a purely vertical or horizontal member.

4. **Compression → buckling risk:** Beyond highest stress, Element 1 is in compression and therefore susceptible to **Euler buckling** — an instability failure mode not captured by linear FEM. This makes it doubly critical.

### 1.3(d) Uncertainties in boundary conditions — *1 mark*

**Roller at Node 1:**

| Uncertainty | Effect |
|:------------|:-------|
| Friction (μ ≠ 0) | Horizontal reaction develops: Rₓ = μ × 15 kN. For μ = 0.1, this is 1.5 kN (12% of W), reducing R₆ₓ and redistributing forces in chord members. |
| Support settlement | For statically determinate trusses, settlements change displacements but not internal forces. However, if the roller direction is misaligned, the structure gains/loses a DOF — fundamentally changing the problem. |
| Direction misalignment (α = 5°) | Roller reaction gains horizontal component R₁y sin 5° ≈ 1.3 kN — comparable to forces in El 6 and El 8 (2.5 kN each). |

**Pin at Node 6:**

| Uncertainty | Effect |
|:------------|:-------|
| Compliance (finite stiffness) | Effective spring BC instead of rigid. Displacements near node 6 increase; forces in El 8, El 9 change. |
| Clearance / play | Nonlinear contact behaviour; structure must displace before constraint engages. |

> **Key insight:** The structure is **statically determinate** (3 reactions, 3 equilibrium equations). Internal forces are theoretically independent of support stiffness — but **reaction forces** and **displacements** are highly sensitive to BC uncertainties. Elements near supports (El 1, 2, 8, 9) are the most affected.

### 1.3(e) One modelling assumption whose violation would invalidate results — *0.75 mark*

> **Critical assumption: All joints are frictionless pins (no moment transfer)**

If joints are actually **rigid** (welded/bolted with moment capacity), the structure becomes a **frame**, and the truss element formulation is **fundamentally invalid:**

<table>
<tr>
<td width="50%">

**Pin joint (assumed):**
- Free rotation at connections
- Transmits: axial force only
- DOFs per node: 2 (u, v)
- Element type: Bar

</td>
<td width="50%">

**Rigid joint (if violated):**
- Fixed rotation at connections
- Transmits: axial + shear + **bending moment**
- DOFs per node: 3 (u, v, **θ**)
- Element type: Beam/Frame

</td>
</tr>
</table>

**Consequences:**
1. **Element formulation error:** Entire stiffness matrix wrong — 12×12 system should be 18×18.
2. **Bending stress dominance:** For Element 1, a modest bending moment M = 5 kN·m would produce σ_b ≈ 10.7 MPa — over **4× the computed axial stress**.
3. **Stiffness change:** Pin-jointed truss is more flexible than a rigid frame → truss model overestimates displacements.

> This is the most critical assumption because it fundamentally defines the structural behaviour and element formulation, yet it depends on physical construction details that cannot be verified from the model alone.

### 1.3(f) Most critical element under 50% reduction of P — *0.75 mark*

With P reduced to 7.5 kN while W remains at 12.5 kN.

**Superposition analysis** (valid for linear systems):

| | Load Case 1 (P only) | Load Case 2 (W only) | Original (LC1+LC2) | Modified (0.5×LC1+LC2) |
|:--|:--:|:--:|:--:|:--:|
| **Contribution to El 1** | −R₁y/sin θ = −15.0/0.6 = **−25.0 kN** | Total − LC1 = −50.0 − (−25.0) = **−25.0 kN** | **−50.0 kN** | 0.5(−25) + (−25) = **−37.5 kN** |
| **Contribution to El 4** | Dominant (horizontal chord) | Secondary | −40.0 kN | ≈ **−20 kN** |

**Conclusion:** Element 1 remains the most critical at ≈ −37.5 kN because:
1. It carries roughly **equal contributions** from both load cases
2. The **horizontal load W (unchanged)** continues to drive large compressive force through it
3. Its **compression state** also makes it the most buckling-susceptible member

### 1.3(g) Least trustworthy result — *0.5 mark*

> **Least trustworthy: displacement u₃ = −0.020 mm**

| Reason | Explanation |
|:-------|:------------|
| **Smallest magnitude** | 0.020 mm is the smallest non-zero result — three orders of magnitude below element lengths. At this scale, modelling errors dominate. |
| **Cancellation of opposing effects** | Node 3 experiences W pushing leftward, but diagonals pulling rightward. The near-zero result reflects near-cancellation — a 10% error in either component could flip the sign. |
| **Self-weight sensitivity** | Including self-weight would add vertical loads that change El 3's force and thus node 3's displacement. Since u₃ ≈ 0, even small absolute changes produce large relative errors. |
| **Experimentally unverifiable** | 0.020 mm requires micrometer-level instrumentation. Thermal expansion and vibration would mask this measurement. |

---

<h1>Question 2 — 2D Plane Stress / CST Analysis</h1>

## 2.1 Problem Definition

### 2.1.1 Structural Description

A rectangular plate modelled with **four linear triangular (CST) elements** and **five nodes**. Fixed on the left edge, subject to uniform traction on the right edge.

### 2.1.2 Structural Diagram

<table><tr><td>

```
  y
  ^
  |
 10 +--  2 ========================== 3 --> P = 2175 psi
  |    /  \  .                     /  \|    (uniform traction)
  |   /     \    .   El 4       /      \|
  |  / El 1    \      .     /    El 3   \|
  5 +      5 *   \       . /             +--> P
  |  \ (10,5)    /      /  .             |
  |   \     /       /        .   El 3    |
  |    \  /    /   El 2         .        |
  |     \/  /                      .     |
  0 +--  1 ========================== 4 --> P
  |
  +----+----------+---------+----------+--> x
       0         10                   20

  [##] = Fixed wall (u=v=0 at nodes 1 and 2)
   *   = Interior node 5 at (10, 5)
```

</td></tr></table>

> **P-number example:** P---175 → X = 1175, so P = 1000 + 1175 = **2,175 psi**

### 2.1.3 Given Data

| Parameter | Value |
|:----------|:-----:|
| Uniform traction P | **2,175 psi** |
| Plate dimensions | 20 in × 10 in |
| Thickness t | 1 in |
| Young's modulus E | 30 × 10⁶ psi |
| Poisson's ratio ν | 0.3 |

### 2.1.4 Nodes and Elements

<table>
<tr>
<td>

| Node | x (in) | y (in) |
|:----:|:------:|:------:|
| 1 | 0 | 0 |
| 2 | 0 | 10 |
| 3 | 20 | 10 |
| 4 | 20 | 0 |
| 5 | 10 | 5 |

</td>
<td>

| Element | Nodes | Area (in²) |
|:-------:|:-----:|:----------:|
| 1 | 1, 5, 2 | 50 |
| 2 | 1, 4, 5 | 50 |
| 3 | 4, 3, 5 | 50 |
| 4 | 2, 5, 3 | 50 |

</td>
</tr>
</table>

### 2.1.5 DOF Numbering

| Node | 1 | 2 | 3 | 4 | 5 |
|:-----|:-:|:-:|:-:|:-:|:-:|
| **DOF u** | 0 | 2 | 4 | 6 | 8 |
| **DOF v** | 1 | 3 | 5 | 7 | 9 |

> **Total DOFs:** 10 &emsp;|&emsp; ⚠️ *"Not all modelling choices implied by the figure are physically or mathematically correct"* — issues are identified and corrected below.

---

## 2.2 Boundary Condition Verification (2 marks)

### 2.2.1 Identify incorrect or incomplete boundary conditions

The left edge (x = 0) shows wall/hatching at nodes 1 and 2, indicating a fixed boundary.

**Potential issues:**

1. If the figure shows node 1 as a **pin** and node 2 as a **roller** (constraining only v₂), this gives only **3 constrained DOFs** — the minimum to prevent rigid body motion, but it does **not** represent a wall. The top-left corner could slide horizontally, producing non-physical shearing.

2. If any constraint is applied at **node 5** (interior), this is physically incorrect — there is no physical support at the plate centre.

### 2.2.2 Propose the correct boundary conditions

<table>
<tr>
<td width="50%">

**✓ Correct (wall boundary):**
- Node 1: u₁ = 0, v₁ = 0
- Node 2: u₂ = 0, v₂ = 0
- **4 constrained DOFs, 6 free**

</td>
<td width="50%">

**✗ Incorrect (pin-roller):**
- Node 1: u₁ = 0, v₁ = 0
- Node 2: v₂ = 0 only
- **3 constrained DOFs, 7 free**

</td>
</tr>
</table>

### 2.2.3 Consequences of incorrect boundary conditions

| Aspect | Effect of incorrect BC |
|:-------|:-----------------------|
| **Stiffness matrix** | May become singular if rigid body motion is not prevented → unsolvable system |
| **Displacements** | Non-physical patterns; top-left corner slides horizontally under traction → shearing instead of pure tension |
| **Reactions** | All horizontal reaction goes to pin node only → artificial stress concentration |
| **Stresses** | Incorrect displacements propagate through ε = B·u and σ = D·ε → qualitatively wrong stress field |

---

## 2.3 Stress State Assessment (2 marks)

### 2.3.1 Correct assumption: **Plane Stress**

### 2.3.2 Justification

| Criterion | Assessment |
|:----------|:-----------|
| **Geometry** | Thin plate: t = 1 in vs L = 20 in, H = 10 in → t/L = **0.05 ≪ 1** |
| **Loading** | All forces in the x-y plane. No out-of-plane loads. |
| **Surfaces** | Top and bottom (z = ±½) are **traction-free** → σ_z = τ_xz = τ_yz = 0 |
| **Conclusion** | This is the defining condition for **plane stress** |

**Plane stress constitutive matrix:**

$$\mathbf{D}\_{\text{stress}} = \frac{E}{1-\nu^2} \begin{bmatrix} 1 & \nu & 0 \\\\ \nu & 1 & 0 \\\\ 0 & 0 & \frac{1-\nu}{2} \end{bmatrix}$$

Substituting E = 30 × 10⁶ psi, ν = 0.3:

<table>
<tr><th></th><th>ε_x</th><th>ε_y</th><th>γ_xy</th></tr>
<tr><td><b>σ_x</b></td><td align="right">32.967 × 10⁶</td><td align="right">9.890 × 10⁶</td><td align="right">0</td></tr>
<tr><td><b>σ_y</b></td><td align="right">9.890 × 10⁶</td><td align="right">32.967 × 10⁶</td><td align="right">0</td></tr>
<tr><td><b>τ_xy</b></td><td align="right">0</td><td align="right">0</td><td align="right">11.538 × 10⁶</td></tr>
</table>

### 2.3.3 Comparison: Plane Strain vs. Plane Stress

**Plane strain** assumes ε_z = 0 (appropriate for very thick/long bodies in z).

| Quantity | Plane Stress | Plane Strain | Ratio |
|:---------|:-------------|:-------------|:-----:|
| D₁₁ (effective modulus) | 32.97 × 10⁶ | 40.38 × 10⁶ | **1.22** |
| D₁₂ (coupling term) | 9.89 × 10⁶ | 17.31 × 10⁶ | **1.75** |
| D₃₃ (shear modulus) | 11.54 × 10⁶ | 11.54 × 10⁶ | 1.00 |

**If plane strain were incorrectly used:**

| Impact | Description |
|:-------|:------------|
| **Displacements** | ~9% smaller (stiffer system). Ratio ≈ (1 − ν²) = 0.91 |
| **Stresses** | Similar σ_x, but a fictitious σ_z = ν(σ_x + σ_y) ≈ 652 psi would appear |
| **Design consequence** | Underestimates displacements, produces spurious through-thickness stresses |

---

## 2.4 Load Representation (2 marks)

### 2.4.1 Derive equivalent nodal forces

Right boundary (x = 20): uniform traction P = 2,175 psi in x-direction. Edge connects nodes 4 (bottom) to 3 (top), length = 10 in.

**Consistent load vector** from work-equivalence:

$$\mathbf{f} = \int_{\Gamma_t} \mathbf{N}^T\,\mathbf{t}\;d\Gamma$$

For uniform traction q = P × t = 2,175 × 1 = 2,175 lb/in on a linear edge of length L = 10 in:

$$f_{x,3} = q \cdot \frac{L}{2} = 2{,}175 \times 5 = \mathbf{10{,}875\text{ lb}}$$

$$f_{x,4} = q \cdot \frac{L}{2} = 2{,}175 \times 5 = \mathbf{10{,}875\text{ lb}}$$

> **Total force** = 10,875 + 10,875 = 21,750 lb = P × t × L = 2,175 × 1 × 10 ✓

### 2.4.2 Why naive force lumping is incorrect

For **uniform traction on a linear edge**, lumping coincidentally gives the same result (equal split). However, this is **not generally correct:**

| Scenario | Consistent (correct) | Naive lumping | Error |
|:---------|:---------------------|:--------------|:-----:|
| Uniform traction, linear edge | qL/2 at each node | qL/2 at each node | 0% (coincidence) |
| **Linear traction**, linear edge | L(2q₃+q₄)/6, L(q₃+2q₄)/6 | q₃L/2, q₄L/2 | **significant** |
| Uniform traction, **quadratic edge** | Ratio 1:4:1 | Ratio 1:1:1 | **300% at mid-node** |

**Fundamental reason:** Lumping does not satisfy **work equivalence** — the work done by lumped forces ≠ work done by the actual traction through the interpolated displacement field.

### 2.4.3 Impact of improper lumping on stresses

- Incorrect nodal forces → wrong displacements → wrong strains and stresses throughout the mesh
- Over-loading one node creates **artificial stress concentrations** in connected elements
- **Moment equilibrium** violated in the weak sense → spurious shear stresses
- Slower **convergence** with mesh refinement compared to consistent loads

---

## 2.5 FEM Solution and Interpretation (3 marks)

### 2.5.1 Model Summary

| Aspect | Choice | Justification |
|:-------|:-------|:--------------|
| Element type | CST (3-node triangle) | Given by problem |
| Stress state | Plane stress | Thin plate, t/L = 0.05 (§2.3) |
| BCs | u₁ = v₁ = u₂ = v₂ = 0 | Wall boundary (§2.2) |
| Loads | F₃ₓ = F₄ₓ = 10,875 lb | Consistent formulation (§2.4) |

### 2.5.2 CST Element Formulation

For triangular element with vertices (x₁,y₁), (x₂,y₂), (x₃,y₃):

**Area:**

$$A_e = \tfrac{1}{2}\,|x_1(y_2 - y_3) + x_2(y_3 - y_1) + x_3(y_1 - y_2)|$$

**Strain-displacement matrix B** (3×6, constant over element):

$$\mathbf{B} = \frac{1}{2A_e} \begin{bmatrix} b_1 & 0 & b_2 & 0 & b_3 & 0 \\\\ 0 & c_1 & 0 & c_2 & 0 & c_3 \\\\ c_1 & b_1 & c_2 & b_2 & c_3 & b_3 \end{bmatrix}$$

where $b_i = y_j - y_k$ , $c_i = x_k - x_j$ (cyclic permutation).

**Element stiffness:** $\mathbf{k}^{(e)} = t \cdot A_e \cdot \mathbf{B}^T\mathbf{D}\mathbf{B}$

### 2.5.3 Worked Example — Element 1 (Nodes 1, 5, 2)

Vertices: (0, 0), (10, 5), (0, 10) &emsp; Area = 50 in²

| | b | c |
|:--:|:--:|:--:|
| Node 1 | y₅ − y₂ = 5 − 10 = **−5** | x₂ − x₅ = 0 − 10 = **−10** |
| Node 5 | y₂ − y₁ = 10 − 0 = **10** | x₁ − x₂ = 0 − 0 = **0** |
| Node 2 | y₁ − y₅ = 0 − 5 = **−5** | x₅ − x₁ = 10 − 0 = **10** |

$$\mathbf{B}_1 = \frac{1}{100}\begin{bmatrix} -5 & 0 & 10 & 0 & -5 & 0 \\\\ 0 & -10 & 0 & 0 & 0 & 10 \\\\ -10 & -5 & 0 & 10 & 10 & -5 \end{bmatrix}$$

### 2.5.4 Worked Example — Element 2 (Nodes 1, 4, 5)

Vertices: (0, 0), (20, 0), (10, 5) &emsp; Area = 50 in²

| | b | c |
|:--:|:--:|:--:|
| Node 1 | y₄ − y₅ = 0 − 5 = **−5** | x₅ − x₄ = 10 − 20 = **−10** |
| Node 4 | y₅ − y₁ = 5 − 0 = **5** | x₁ − x₅ = 0 − 10 = **−10** |
| Node 5 | y₁ − y₄ = 0 − 0 = **0** | x₄ − x₁ = 20 − 0 = **20** |

$$\mathbf{B}_2 = \frac{1}{100}\begin{bmatrix} -5 & 0 & 5 & 0 & 0 & 0 \\\\ 0 & -10 & 0 & -10 & 0 & 20 \\\\ -10 & -5 & -10 & 5 & 20 & 0 \end{bmatrix}$$

### 2.5.5 Nodal Displacements — Results

| Node | u (in) | v (in) | Notes |
|:----:|-------:|-------:|:------|
| 1 | 0 | 0 | Fixed (BC) |
| 2 | 0 | 0 | Fixed (BC) |
| 3 | **+0.001406** | −0.000145 | Right edge, extends right |
| 4 | **+0.001406** | +0.000145 | Right edge, extends right |
| 5 | +0.000664 | 0.000000 | Centre — half of right-edge u |

> **Physical interpretation:**
> - u₃ = u₄ → right edge translates uniformly (consistent with uniform traction)
> - v₃ = −v₄ → **Poisson contraction** (plate narrows as it stretches)
> - u₅ ≈ u₃/2 → linear variation confirms near-uniform strain
> - **Analytical:** u = PL/E = 2175 × 20 / 30×10⁶ = 0.00145 in → FEM gives 0.00141 in (**3% error**, acceptable for 4-element mesh)

### 2.5.6 Reaction Forces

| Node | Fx (lb) | Fy (lb) |
|:----:|--------:|--------:|
| 1 | **−10,875** | −4,355 |
| 2 | **−10,875** | +4,355 |

| Check | Result |
|:------|:------:|
| ΣFx = (−10,875) + (−10,875) + 10,875 + 10,875 | **= 0 ✓** |
| ΣFy = (−4,355) + (+4,355) | **= 0 ✓** |

> The non-zero Fy reactions arise because the fixed wall prevents Poisson contraction at x = 0. The anti-symmetric values (±4,355 lb) confirm geometric symmetry about y = 5.

### 2.5.7 Element Stresses

| Element | Nodes | σ_x (psi) | σ_y (psi) | τ_xy (psi) |
|:-------:|:-----:|----------:|----------:|-----------:|
| 1 | 1, 5, 2 | 2,189 | 657 | ≈ 0 |
| 2 | 1, 4, 5 | 2,175 | 218 | −7.0 |
| 3 | 4, 3, 5 | 2,161 | −221 | ≈ 0 |
| 4 | 2, 5, 3 | 2,175 | 218 | +7.0 |

**Comparison with analytical solution** (uniform tension P):

| Quantity | Analytical | FEM Range | Max Error |
|:---------|:-----------|:----------|:---------:|
| σ_x | 2,175 psi | 2,161 – 2,189 psi | **0.6%** ✓ |
| σ_y | 0 psi | −221 to +657 psi | ⚠️ **significant** |
| τ_xy | 0 psi | −7 to +7 psi | small |

> **Note on σ_y error:** The non-zero σ_y is **partially real** — the fixed BCs at x = 0 prevent Poisson contraction, generating genuine transverse stress near the wall. The error is that the coarse mesh smears this localized boundary effect across entire elements, producing σ_y ≠ 0 even far from the wall (Element 3 at x = 20).

### 2.5.8 Mesh Dependence and Adequacy

**Is the stress field mesh-dependent?**

> **Yes.** CST elements produce **constant stress within each element** — a piecewise step function. Stress jumps discontinuously between adjacent elements. This is intrinsic to the CST formulation (constant B matrix → constant strain → constant stress).

**Is this mesh adequate for stress prediction?**

> **No.** Four CST elements are grossly insufficient:

| Issue | Explanation |
|:------|:------------|
| **Only 6 free DOFs** | Extremely limited capability to represent the displacement field |
| **σ_y error ≈ 30%** | Analytical σ_y = 0 away from wall, but FEM gives up to 657 psi |
| **Cannot resolve boundary layer** | Fixed edge creates stress concentration that decays over ~10 in — the mesh has no refinement there |
| **Convergence rate** | CST converges at O(h¹) — the slowest of any practical element type |

**Recommended improvements:**

| Improvement | Benefit |
|:------------|:--------|
| Refine to 8×4 CST mesh (64 elements) | σ_y error < 5% away from boundary |
| Use 6-node quadratic triangles (LST) | O(h²) convergence, much better accuracy |
| Use bilinear quad (Q4) elements | Fewer elements needed, passes patch test |
| Grade mesh toward fixed boundary | Captures boundary stress concentration |

---

<p align="center"><i>End of Coursework Report</i></p>
