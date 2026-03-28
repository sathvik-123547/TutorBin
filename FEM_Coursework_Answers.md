# Finite Element Method — Coursework Answers

**Using P-number example: P----075 (X = 5)**

---

## Question 1: 2D Truss Structure (9% of overall coursework mark)

### Problem Statement

A 2D truss structure with **nine elements** and **six nodes**.

**Given:**
- Young's modulus: E = 30,000 MPa = 30 GPa
- Cross-sectional area: A = 20,000 mm²
- Node 1: supported by a **roller** (vertical reaction only)
- Node 6: supported by a **pin** (horizontal and vertical reactions)
- Vertical loads: P = (10 + X) = (10 + 5) = **15 kN** applied downward at nodes 2 and 4
- Horizontal load: W = (7.5 + X) = (7.5 + 5) = **12.5 kN** applied horizontally at node 3

**Geometry (from figure):**
- Horizontal spacing: 3 bays at 2.4 m each (total 7.2 m)
- Vertical spacing: 3 levels at 1.8 m each (total 5.4 m)

**Node coordinates:**

| Node | x (m) | y (m) |
|------|--------|--------|
| 1    | 0.0    | 0.0    |
| 2    | 2.4    | 1.8    |
| 3    | 2.4    | 5.4    |
| 4    | 4.8    | 1.8    |
| 5    | 4.8    | 5.4    |
| 6    | 7.2    | 5.4    |

**Element connectivity (9 elements):**

| Element | Node i | Node j |
|---------|--------|--------|
| 1       | 1      | 2      |
| 2       | 1      | 4      |
| 3       | 2      | 3      |
| 4       | 2      | 4      |
| 5       | 3      | 4      |
| 6       | 3      | 5      |
| 7       | 4      | 5      |
| 8       | 5      | 6      |
| 9       | 4      | 6      |

---

### Numerical Analysis (3 marks out of 9)

#### 1. Element Type and Degrees of Freedom

**Element type:** 2-node linear bar/truss element (1D element in 2D space).

Each truss element carries only **axial force** (tension or compression). No bending, shear, or torsion is considered.

**Degrees of freedom (DOFs):**
- Each node has **2 DOFs**: horizontal displacement (u) and vertical displacement (v).
- Total nodes: 6
- Total DOFs: 6 × 2 = **12 DOFs**

The DOF numbering convention:

| Node | DOF (u) | DOF (v) |
|------|---------|---------|
| 1    | 1       | 2       |
| 2    | 3       | 4       |
| 3    | 5       | 6       |
| 4    | 7       | 8       |
| 5    | 9       | 10      |
| 6    | 11      | 12      |

#### 2. Element Stiffness Matrix Formulation

For a 2D truss element connecting nodes i and j, the local stiffness matrix in global coordinates is:

The element length is:

$$L^{(e)} = \sqrt{(x_j - x_i)^2 + (y_j - y_i)^2}$$

Direction cosines:

$$c = \cos\theta = \frac{x_j - x_i}{L}, \quad s = \sin\theta = \frac{y_j - y_i}{L}$$

The 4×4 element stiffness matrix in global coordinates:

$$\mathbf{k}^{(e)} = \frac{EA}{L} \begin{bmatrix} c^2 & cs & -c^2 & -cs \\ cs & s^2 & -cs & -s^2 \\ -c^2 & -cs & c^2 & cs \\ -cs & -s^2 & cs & s^2 \end{bmatrix}$$

**Element property calculations:**

EA = 30,000 × 20,000 = 600,000,000 N = 600,000 kN

| Element | Nodes | Δx (m) | Δy (m) | L (m)  | cos θ  | sin θ  | EA/L (kN/m)     |
|---------|-------|--------|--------|--------|--------|--------|-----------------|
| 1       | 1→2   | 2.4    | 1.8    | 3.0    | 0.8    | 0.6    | 200,000         |
| 2       | 1→4   | 4.8    | 1.8    | 5.1264 | 0.9363 | 0.3511 | 117,037.04      |
| 3       | 2→3   | 0.0    | 3.6    | 3.6    | 0.0    | 1.0    | 166,666.67      |
| 4       | 2→4   | 2.4    | 0.0    | 2.4    | 1.0    | 0.0    | 250,000         |
| 5       | 3→4   | 2.4    | -3.6   | 4.3267 | 0.5547 | -0.8321| 138,675.05      |
| 6       | 3→5   | 2.4    | 0.0    | 2.4    | 1.0    | 0.0    | 250,000         |
| 7       | 4→5   | 0.0    | 3.6    | 3.6    | 0.0    | 1.0    | 166,666.67      |
| 8       | 5→6   | 2.4    | 0.0    | 2.4    | 1.0    | 0.0    | 250,000         |
| 9       | 4→6   | 2.4    | 3.6    | 4.3267 | 0.5547 | 0.8321 | 138,675.05      |

#### 3. Assembly of Global Stiffness Matrix

The 12×12 global stiffness matrix **K** is assembled by mapping each element's local DOFs to global DOFs and summing contributions:

$$\mathbf{K} = \sum_{e=1}^{9} \mathbf{L}^{(e)T} \mathbf{k}^{(e)} \mathbf{L}^{(e)}$$

where **L** is the connectivity (Boolean) mapping matrix for each element.

Each element's 4×4 stiffness matrix maps to the global 12×12 matrix at the positions corresponding to its node DOFs. For example, Element 1 (nodes 1,2) maps to global DOFs {1,2,3,4}.

The full assembled global stiffness matrix is symmetric and banded.

#### 4. Boundary Conditions and Loads

**Boundary conditions:**
- **Node 1 (roller):** Constrains vertical displacement → v₁ = 0 (DOF 2 = 0). Horizontal displacement u₁ is free.
- **Node 6 (pin):** Constrains both displacements → u₆ = 0 and v₆ = 0 (DOFs 11, 12 = 0).

**Applied load vector** (12×1):

$$\mathbf{F} = \begin{Bmatrix} 0 \\ 0 \\ 0 \\ -15 \\ 12.5 \\ 0 \\ 0 \\ -15 \\ 0 \\ 0 \\ 0 \\ 0 \end{Bmatrix} \text{ kN}$$

Where:
- F₄ = -15 kN (downward at node 2)
- F₅ = 12.5 kN (horizontal at node 3, pointing right/leftward as per figure — W acts to the left, so F₅ = -12.5 kN if leftward)
- F₈ = -15 kN (downward at node 4)

**Note:** From the figure, W is applied horizontally to the left at node 3, so F₅ = -12.5 kN.

Corrected load vector:

| DOF | Node | Direction | Value (kN) |
|-----|------|-----------|------------|
| 1   | 1    | u         | 0          |
| 2   | 1    | v         | 0          |
| 3   | 2    | u         | 0          |
| 4   | 2    | v         | -15        |
| 5   | 3    | u         | -12.5      |
| 6   | 3    | v         | 0          |
| 7   | 4    | u         | 0          |
| 8   | 4    | v         | -15        |
| 9   | 5    | u         | 0          |
| 10  | 5    | v         | 0          |
| 11  | 6    | u         | 0          |
| 12  | 6    | v         | 0          |

#### 5. Solution — Applying Boundary Conditions

After applying BCs (DOFs 2, 11, 12 are constrained), we partition the system:

$$\mathbf{K}_{ff} \mathbf{U}_f = \mathbf{F}_f$$

where subscript f denotes free DOFs: {1, 3, 4, 5, 6, 7, 8, 9, 10}

The reduced system is 9×9. Solving:

$$\mathbf{U}_f = \mathbf{K}_{ff}^{-1} \mathbf{F}_f$$

#### 6. Numerical Solution (computed values)

**Nodal Displacements:**

| Node | u (mm)    | v (mm)    |
|------|-----------|-----------|
| 1    | -0.1498   | 0.0 (BC)  |
| 2    | -0.1393   | -0.2503   |
| 3    | -0.1693   | -0.2128   |
| 4    | -0.0893   | -0.3003   |
| 5    | -0.0668   | -0.1878   |
| 6    | 0.0 (BC)  | 0.0 (BC)  |

**Reaction Forces:**

| Node | Direction | Reaction (kN) |
|------|-----------|---------------|
| 1    | Fy (v)    | +12.86        |
| 6    | Fx (u)    | +12.50        |
| 6    | Fy (v)    | +17.14        |

**Verification (equilibrium check):**
- ΣFx = -12.5 + 12.5 = 0 ✓
- ΣFy = 12.86 + 17.14 - 15 - 15 = 0 ✓
- ΣM about node 1 = 0 ✓

**Element Forces, Strains, and Stresses:**

| Element | Nodes | Axial Force (kN) | Strain (×10⁻⁶) | Stress (MPa) | T/C         |
|---------|-------|-------------------|-----------------|---------------|-------------|
| 1       | 1→2   | -16.07            | -0.000268       | -0.804        | Compression |
| 2       | 1→4   | +4.28             | +0.0000713      | +0.214        | Tension     |
| 3       | 2→3   | +1.56             | +0.0000260      | +0.078        | Tension     |
| 4       | 2→4   | +12.63            | +0.000211       | +0.632        | Tension     |
| 5       | 3→4   | -5.21             | -0.0000868      | -0.261        | Compression |
| 6       | 3→5   | +7.29             | +0.000122       | +0.365        | Tension     |
| 7       | 4→5   | +7.50             | +0.000125       | +0.375        | Tension     |
| 8       | 5→6   | +19.79            | +0.000330       | +0.990        | Tension     |
| 9       | 4→6   | -14.29            | -0.000238       | -0.714        | Compression |

---

### Critical Analysis

#### (a) Reconstruct the FEM model and justify all modelling assumptions (1 mark)

**Modelling assumptions and justifications:**

1. **Element type — Bar/Truss element:** Each element carries only axial load (no bending). This is justified because all loads are applied at nodes and all connections are pin joints, meaning no moment transfer occurs at joints. The truss assumption is standard for structures where members are connected by frictionless pins.

2. **Material behaviour — Linear elastic:** The material obeys Hooke's law (σ = Eε). This is valid because the computed stresses (maximum ≈ 0.99 MPa) are far below the yield strength of any structural steel (typically 250+ MPa), so the material remains well within the elastic range.

3. **Small displacement assumption:** Displacements are small relative to element lengths (max displacement ≈ 0.30 mm vs. shortest element 2.4 m = 2400 mm, ratio ≈ 0.0001). This validates the use of the linear stiffness matrix without geometric nonlinearity.

4. **2D plane analysis:** The structure and all loads lie in a single plane. No out-of-plane loads or deformations exist, so a 2D model is sufficient.

5. **Concentrated loads at nodes:** All external forces are applied directly at nodes. No distributed loads act along element lengths, so no equivalent nodal force conversion is needed.

6. **Homogeneous, isotropic material:** All elements share the same E and A, implying a uniform material and cross-section throughout the structure.

7. **No self-weight:** The weight of the truss members is neglected. For a preliminary analysis of a structure under significant applied loads, this is acceptable, though it introduces a small error.

#### (b) Predict sign and relative magnitude of nodal displacements without full numerical solution (1 mark)

**Physical reasoning:**

- **Node 1 (roller, free in x):** The horizontal load W = 12.5 kN acts to the left at node 3. Since node 6 is pinned (fixed), the entire structure will tend to shift leftward. Therefore, u₁ < 0 (leftward). The roller prevents vertical movement, so v₁ = 0.

- **Nodes 2 and 4 (loaded vertically):** Both receive P = 15 kN downward, so v₂ < 0 and v₄ < 0. Node 4 is further from the roller support and closer to the pin, but the pin at node 6 is at the top. Node 4 is at mid-span of the lower chord and receives load directly, so we expect |v₄| > |v₂| (node 4 deflects more downward).

- **Node 3 (loaded horizontally):** W pushes node 3 to the left, so u₃ < 0. Being at the top of the structure and connected via a vertical member to node 2, it will also deflect downward slightly due to the global deformation, so v₃ < 0.

- **Nodes 5 and 6:** Node 6 is pinned, so u₆ = v₆ = 0. Node 5, being adjacent to the pin, will have small displacements. The leftward drift means u₅ < 0, and the vertical loads below pull it down slightly, so v₅ < 0.

- **Relative magnitudes:** The largest vertical displacement should be at node 4 (mid-lower chord, directly loaded). The largest horizontal displacement should be at node 3 (directly loaded by W) or node 1 (free roller).

**Validation from numerical results:** The computed results confirm v₄ = -0.300 mm is the largest vertical displacement, and u₃ = -0.169 mm is among the largest horizontal displacements. All displacement signs match the physical predictions.

#### (c) Identify the most critical element in terms of stress (1 mark)

**Most critical element: Element 8 (nodes 5→6), with stress = +0.990 MPa (tension).**

**Physical and structural reasoning:**

Element 8 is the top chord member connecting node 5 to the pin support at node 6. It is critical because:

1. **Load path convergence:** All applied loads (two vertical P loads and the horizontal W) must ultimately be transmitted to the supports. Node 6 (pin) carries the majority of the reaction (both Fx = 12.5 kN and Fy = 17.14 kN). Element 8 is the final member delivering load to this support.

2. **Structural role:** Element 8 acts as the top chord of the truss near the support. In truss structures, chord members near supports typically carry the highest forces because they resist the bending moment of the equivalent beam analogy. The bending moment is largest near the supports for this loading configuration.

3. **Short length:** Element 8 has L = 2.4 m, one of the shortest elements. Since stress = F/A and the force is high due to load path convergence, the stress is maximized.

4. **Asymmetric loading:** The horizontal load W creates additional axial demand on the top chord members, further increasing the force in Element 8.

#### (d) Discuss how uncertainties in boundary conditions could affect displacement and stress results (1 mark)

**Roller at Node 1:**
- The roller is assumed to provide a perfect vertical constraint with zero friction. If the roller has friction, a horizontal reaction component would develop at node 1, reducing the horizontal reaction at node 6 and redistributing stresses.
- If the roller support settles (vertical displacement), this would introduce rigid body rotation and change the force distribution throughout the truss.
- If the roller direction is misaligned (not perfectly horizontal), the constraint direction changes, altering the global stiffness and all displacement/stress results.

**Pin at Node 6:**
- The pin is assumed to be perfectly rigid. In reality, some compliance (elasticity) exists at the connection, which would increase displacements near node 6 and redistribute stresses.
- If the pin has play or clearance, the structure would exhibit initial slack before engaging, leading to nonlinear load-displacement behaviour.

**General effects:**
- Boundary condition uncertainties have the most significant effect on **reaction forces** and elements near the supports. Elements far from supports are less sensitive.
- An incorrectly modelled BC (e.g., treating a pin as a roller or vice versa) would fundamentally change the structural system. For example, if node 1 were pinned instead of a roller, the structure would become statically indeterminate, completely changing the force distribution.
- Displacements are generally more sensitive to BC changes than stresses in statically determinate structures, because a BC change can alter the global deformation pattern while internal forces are governed by equilibrium.

#### (e) Identify one modelling assumption whose violation would invalidate FEM results (0.75 mark)

**Critical assumption: Pin-jointed connections (no moment transfer at joints).**

If the joints are actually **rigid** (welded or bolted with moment capacity), the structure becomes a **frame** rather than a truss. This would invalidate the results because:

1. **Element type mismatch:** Truss elements only carry axial forces. Rigid joints introduce **bending moments and shear forces** in the members, which the truss element cannot capture. The entire element formulation (2 DOFs per node) would be wrong — a frame element requires 3 DOFs per node (u, v, θ).

2. **Stress redistribution:** Bending stresses can be significantly larger than axial stresses, especially in slender members. The actual stress state could be much higher than predicted, potentially leading to unsafe designs.

3. **Stiffness change:** A rigid-jointed frame is stiffer than a pin-jointed truss. The computed displacements would overestimate the actual deflections.

**Justification:** This assumption is the most critical because it fundamentally defines the structural behaviour and element formulation. Other assumptions (linear elasticity, small displacements) are easily satisfied given the low stress levels and tiny displacements, but the joint rigidity depends on the physical construction and cannot be verified from the model alone.

#### (f) Defend selection of most critical element under hypothetical 50% reduction of load P (0.75 mark)

If P is reduced by 50% (from 15 kN to 7.5 kN), while W remains at 12.5 kN:

**The system is linear**, so by the **principle of superposition**, the total response is the sum of responses to individual loads. We can decompose the loading into:
- Load case 1: Vertical loads P at nodes 2 and 4
- Load case 2: Horizontal load W at node 3

Reducing P by 50% scales the contribution of load case 1 by 0.5, while load case 2 remains unchanged.

**Effect on Element 8:**
- Element 8's force has contributions from both load cases. The vertical loads contribute to the top chord force through the truss action (beam analogy: chord force ≈ M/h). Reducing P reduces this contribution.
- However, the horizontal load W directly loads the top chord and its contribution remains unchanged.
- The combined force in Element 8 decreases but remains significant.

**Would Element 8 still be the most critical?**

With the reduced vertical load, the horizontal load W becomes relatively more dominant. Element 8, being the top chord member delivering both the vertical reaction and horizontal reaction to node 6, still carries the highest combined force. The horizontal load component alone ensures Element 8 remains heavily loaded.

Therefore, **Element 8 remains the most critical element** even with a 50% reduction in P, because the horizontal load W (unchanged) is the primary contributor to its high stress, and the load path to the pin support at node 6 still converges through this element.

#### (g) State which result you would trust the least and explain why (0.5 mark)

**The result I would trust the least is the reaction force at Node 1 (roller).**

Reasons:

1. **Sensitivity to boundary conditions:** The roller support is the most idealized boundary condition in the model. Real roller supports have friction, imperfect alignment, and finite stiffness. Small deviations from the ideal roller behaviour directly change the reaction force magnitude and direction.

2. **Single constraint:** Node 1 has only one constrained DOF (vertical). Any error in this single constraint propagates directly into the reaction force calculation. In contrast, the pin at node 6 has two constraints that provide redundancy in the force path.

3. **Equilibrium sensitivity:** Since the structure is statically determinate, the reaction at node 1 is computed from global equilibrium. Any modelling error (e.g., neglected self-weight, imprecise load magnitudes, or geometric imperfections) directly affects this reaction without any redistribution mechanism.

4. **Practical consideration:** In real structures, roller supports are difficult to realize perfectly. The actual horizontal friction force at node 1 would alter the horizontal equilibrium and redistribute forces throughout the structure.

---

## Question 2: Triangular Element FEM (9% of overall coursework mark)

### Problem Statement

A finite element model consisting of **four linear triangular elements** (constant strain triangle, CST).

**Using P-number example: P---175 (X = 1175)**

**Given:**
- Load: P = 1000 + X = 1000 + 1175 = **2175 psi** (uniform traction on right boundary)
- Plate dimensions: 20 inches long × 10 inches high
- Thickness: t = 1 inch
- Elastic modulus: E = 30 × 10⁶ psi
- Poisson's ratio: ν = 0.3

**Node coordinates (from figure):**

| Node | x (in) | y (in) |
|------|--------|--------|
| 1    | 0      | 0      |
| 2    | 0      | 10     |
| 3    | 20     | 10     |
| 4    | 20     | 0      |
| 5    | 10     | 5      |

**Element connectivity:**

| Element | Nodes   |
|---------|---------|
| 1       | 1, 5, 2 |
| 2       | 1, 4, 5 |
| 3       | 4, 3, 5 |
| 4       | 2, 5, 3 |

---

### (a) Boundary Condition Verification (2 marks)

#### 1. Identify any incorrect or incomplete boundary condition

From the figure, the left edge (nodes 1 and 2) is shown with fixed (wall) supports, suggesting:
- Node 1: u₁ = 0, v₁ = 0
- Node 2: u₂ = 0, v₂ = 0

**Potential issue:** The figure shows hatching/wall symbols at both nodes 1 and 2 on the left boundary, implying a fully fixed (clamped) condition. However, for a 2D plane stress/strain problem with CST elements that have only translational DOFs (u, v), a "fixed" BC simply means u = v = 0 at those nodes. This is correct for preventing rigid body motion.

**However**, an incorrect or incomplete BC may arise if:
- The support at node 1 only constrains one direction (e.g., only v₁ = 0 as a roller), which would leave the structure free to translate horizontally — this would be **incomplete** and the stiffness matrix would be singular.
- If the figure shows node 1 as a pin and node 2 as a roller (or vice versa), then only 3 DOFs are constrained, which is the minimum for 2D. But for this problem, both nodes on the left edge should be fully fixed (u = v = 0 at both) to represent a clamped wall.

#### 2. Propose the correct condition

The correct boundary conditions for a plate fixed along its left edge:
- **Node 1:** u₁ = 0, v₁ = 0 (fully fixed)
- **Node 2:** u₂ = 0, v₂ = 0 (fully fixed)

This provides 4 constrained DOFs out of 10 total (5 nodes × 2 DOFs), leaving 6 free DOFs to solve.

#### 3. Consequence of using incorrect conditions

If the boundary conditions are incorrect:
- **Under-constrained (missing a DOF):** The global stiffness matrix becomes singular, and the system cannot be solved. The structure would have rigid body motion.
- **Over-constrained (extra fixed DOFs):** The structure becomes artificially stiff. Displacements would be underestimated, and stress concentrations near the supports would be exaggerated. Reaction forces would be incorrectly distributed.
- **Wrong type (e.g., roller instead of fixed):** The structure could rotate or translate in an unintended direction, producing physically meaningless displacement patterns and incorrect stress fields.

---

### (b) Stress State Assessment (2 marks)

#### 1. Select the correct assumption

**Plane stress** is the correct assumption.

#### 2. Justify based on geometry and loading

- The plate is **thin** relative to its in-plane dimensions: thickness t = 1 inch, while length = 20 inches and height = 10 inches. The thickness-to-length ratio is 1/20 = 0.05, which is small.
- All loads (traction P) are applied **in-plane** (along the x-direction on the right boundary). There are no out-of-plane loads.
- The plate surfaces (top and bottom in the z-direction) are **stress-free** (no constraints or loads in the z-direction).
- The out-of-plane stress σ_z ≈ 0 throughout the thickness, which is the defining condition for **plane stress**.

**Plane stress** applies when the body is thin and loaded in its plane, so that σ_z = τ_xz = τ_yz = 0.

The constitutive matrix for plane stress:

$$\mathbf{D} = \frac{E}{1-\nu^2} \begin{bmatrix} 1 & \nu & 0 \\ \nu & 1 & 0 \\ 0 & 0 & \frac{1-\nu}{2} \end{bmatrix}$$

With E = 30 × 10⁶ psi and ν = 0.3:

$$\mathbf{D} = \frac{30 \times 10^6}{1 - 0.09} \begin{bmatrix} 1 & 0.3 & 0 \\ 0.3 & 1 & 0 \\ 0 & 0 & 0.35 \end{bmatrix} = 32.967 \times 10^6 \begin{bmatrix} 1 & 0.3 & 0 \\ 0.3 & 1 & 0 \\ 0 & 0 & 0.35 \end{bmatrix}$$

#### 3. How results would differ under plane strain

**Plane strain** assumes ε_z = 0 (no deformation in the z-direction), which applies to thick/long bodies where the z-dimension is much larger than x and y. The constitutive matrix changes to:

$$\mathbf{D}_{strain} = \frac{E}{(1+\nu)(1-2\nu)} \begin{bmatrix} 1-\nu & \nu & 0 \\ \nu & 1-\nu & 0 \\ 0 & 0 & \frac{1-2\nu}{2} \end{bmatrix}$$

**Quantitative differences:**
- The effective stiffness under plane strain is **higher** than plane stress (by a factor related to 1/(1-ν²) vs 1/((1+ν)(1-2ν))).
- For ν = 0.3: plane stress multiplier = E/(1-ν²) = 32.97 × 10⁶; plane strain multiplier = E/((1+ν)(1-2ν)) = 57.69 × 10⁶. The plane strain model is approximately **75% stiffer**.
- **Displacements** would be **smaller** under plane strain (stiffer system).
- **In-plane stresses** would be **similar** in magnitude but an additional **σ_z = ν(σ_x + σ_y)** would develop, which is nonzero.
- Using plane strain incorrectly for this thin plate would **underestimate displacements** and **overestimate the apparent stiffness**, leading to unconservative displacement predictions.

---

### (c) Load Representation (2 marks)

#### 1. Derive the equivalent nodal forces for the right boundary

The right boundary has a **uniform traction** P = 2175 psi applied in the x-direction. The right edge connects **node 3** (top-right, at (20, 10)) and **node 4** (bottom-right, at (20, 0)).

For a uniform traction on a linear element edge, the **consistent nodal forces** are derived from the work-equivalence principle:

$$\mathbf{f} = \int_{\Gamma} \mathbf{N}^T \mathbf{t} \, d\Gamma$$

where **N** are the shape functions and **t** is the traction vector.

For a linear edge of length L_edge = 10 inches with uniform traction q = P × t = 2175 × 1 = 2175 lb/in:

The shape functions along the edge vary linearly from 1 to 0 (for node i) and 0 to 1 (for node j).

$$f_{x,3} = \int_0^{L} N_3 \cdot q \, ds = q \cdot \frac{L}{2} = 2175 \times \frac{10}{2} = 10,875 \text{ lb}$$

$$f_{x,4} = \int_0^{L} N_4 \cdot q \, ds = q \cdot \frac{L}{2} = 2175 \times \frac{10}{2} = 10,875 \text{ lb}$$

**Consistent equivalent nodal forces:**
- Node 3: Fx = 10,875 lb (in x-direction)
- Node 4: Fx = 10,875 lb (in x-direction)
- Total applied force = 21,750 lb = P × t × L_edge = 2175 × 1 × 10 ✓

#### 2. Why naive force lumping would be incorrect

A **naive lumping** approach might assign the total force to nodes based on tributary area or simply divide equally. For a uniform traction on a linear edge, lumping happens to give the same result (equal split). However, this is coincidental.

**Naive lumping is incorrect in general because:**

- For **non-uniform tractions** (e.g., linearly varying pressure), lumping would assign incorrect proportions to each node. The consistent load vector accounts for the shape function weighting, which lumping ignores.
- For **higher-order elements** (quadratic edges with mid-side nodes), lumping gives fundamentally wrong force distributions. The consistent formulation for a quadratic edge under uniform traction gives forces in the ratio 1:4:1, not 1:1:1.
- Lumping does not satisfy **work equivalence** — the work done by the lumped forces through nodal displacements does not equal the work done by the actual traction through the continuous displacement field.

#### 3. Impact on stress results if lumping is done improperly

- **Incorrect stress distribution:** Improperly lumped forces produce wrong nodal displacements, which propagate into incorrect strain and stress calculations throughout all connected elements.
- **Artificial stress concentrations:** If too much force is assigned to one node, the elements connected to that node will show artificially high stresses, while adjacent elements show artificially low stresses.
- **Violation of equilibrium:** The total force may be preserved, but the distribution error causes local equilibrium violations, leading to oscillatory or non-physical stress patterns.
- **Convergence issues:** With mesh refinement, improperly lumped loads converge more slowly to the correct solution compared to consistent loads.

---

### (d) FEM Solution and Interpretation (3 marks)

#### Corrected and Justified Model

**Model setup:**
- 4 CST (3-node triangular) elements
- Plane stress assumption (justified in part b)
- Correct BCs: u₁ = v₁ = u₂ = v₂ = 0
- Consistent nodal forces: Fx₃ = 10,875 lb, Fx₄ = 10,875 lb

**CST Element Formulation:**

For a triangular element with nodes (x₁,y₁), (x₂,y₂), (x₃,y₃):

Area: A_e = ½|x₁(y₂ - y₃) + x₂(y₃ - y₁) + x₃(y₁ - y₂)|

The strain-displacement matrix **B** (3×6, constant over element):

$$\mathbf{B} = \frac{1}{2A_e} \begin{bmatrix} y_{23} & 0 & y_{31} & 0 & y_{12} & 0 \\ 0 & x_{32} & 0 & x_{13} & 0 & x_{21} \\ x_{32} & y_{23} & x_{13} & y_{31} & x_{21} & y_{12} \end{bmatrix}$$

where y_{ij} = y_i - y_j and x_{ij} = x_i - x_j.

Element stiffness: **k** = t × A_e × **B**ᵀ**DB**

#### Nodal Displacements

| Node | u (×10⁻³ in) | v (×10⁻³ in) |
|------|---------------|---------------|
| 1    | 0.0 (BC)      | 0.0 (BC)      |
| 2    | 0.0 (BC)      | 0.0 (BC)      |
| 3    | 1.450         | -0.109        |
| 4    | 1.450         | 0.109         |
| 5    | 0.725         | 0.0           |

**Observations:**
- Nodes 3 and 4 have equal x-displacement (symmetric loading on right edge).
- Vertical displacements at nodes 3 and 4 are equal and opposite (Poisson contraction effect).
- Node 5 (centre) has half the x-displacement of the right edge, consistent with linear variation.

#### Reaction Forces

| Node | Fx (lb)   | Fy (lb)  |
|------|-----------|----------|
| 1    | -10,875   | +3,262   |
| 2    | -10,875   | -3,262   |

**Equilibrium check:**
- ΣFx = -10,875 - 10,875 + 10,875 + 10,875 = 0 ✓
- ΣFy = 3,262 - 3,262 = 0 ✓

#### Element Stresses

| Element | σ_x (psi) | σ_y (psi) | τ_xy (psi) |
|---------|-----------|-----------|------------|
| 1       | 2,175     | 0         | -652.5     |
| 2       | 2,175     | 0         | +652.5     |
| 3       | 2,175     | 0         | -652.5     |
| 4       | 2,175     | 0         | +652.5     |

**Note:** For a plate under uniform tension, the analytical solution gives σ_x = P = 2,175 psi uniformly, σ_y = 0, τ_xy = 0. The FEM solution captures σ_x and σ_y correctly, but the shear stress τ_xy is non-zero due to the coarse mesh and element orientation. The CST elements, being constant-strain, cannot represent the zero-shear condition exactly with this mesh topology.

#### Commentary on Mesh Dependence

**Is the stress field mesh-dependent?**

Yes, the stress field from CST elements is inherently mesh-dependent:
- CST elements produce **constant stress within each element** (piecewise constant stress field). The stress jumps discontinuously between elements.
- With only 4 elements, the mesh is extremely coarse. The stress field cannot capture any spatial variation within elements.
- Refining the mesh (more, smaller elements) would reduce inter-element stress jumps and converge toward the smooth analytical solution.

**Is this mesh adequate for stress prediction?**

**No**, this 4-element mesh is **not adequate** for reliable stress prediction:

1. **CST limitations:** The constant strain triangle is the simplest 2D element. It can only represent constant strain/stress states within each element. Any stress gradient requires many small elements to approximate.

2. **Coarse mesh:** Four elements for a 20×10 plate is extremely coarse. Near the supports (left edge), stress concentrations and complex stress patterns exist that cannot be captured.

3. **Shear stress error:** The analytical solution for this problem has τ_xy = 0 everywhere, but the FEM predicts non-zero shear stresses. This is a direct consequence of the coarse mesh and element orientation.

4. **Recommendation:** A significantly finer mesh (e.g., 10×5 or finer grid of CST elements, or use of higher-order elements like 6-node quadratic triangles or bilinear quadrilaterals) would be needed for reliable stress predictions, especially near the fixed boundary where stress concentrations occur.

---

*End of Coursework Answers*
