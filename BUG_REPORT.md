# Sootlib Code Review - Bug Summary

**Date:** March 11, 2026 (Third Pass)  
**Repository:** BYUignite/sootlib  
**Branch:** master  
**Review Type:** Comprehensive source code audit (third pass — full verification)

---

## Summary

A complete re-audit of every source file in sootlib's `src/` directory verified the 12 bugs from the second-pass report, expanded one (Bug #5 now covers 4 integer-division instances), and uncovered 2 additional critical bugs, for a total of **14 confirmed bugs**. All bugs have been verified against the actual source code.

| Severity | Count |
|----------|-------|
| Critical | 11    |
| Moderate | 3     |
| **Total** | **14** |

---

## Bug #1: Static Temperature-Dependent Variable in PAH Nucleation

**File:** `src/nucleationModels/nucleationModel_PAH.cc`  
**Line:** 65  
**Classification:** Critical - Produces incorrect results when temperature changes

**Issue:** The `preFac` variable is declared `static`, meaning it is initialized only once on the first function call and retains that value for all subsequent calls. However, it depends on `state.T`, which varies. After the first call, the temperature dependency is frozen.

**Current Code:**
```cpp
static double preFac = 4.0*sqrt(M_PI*kb*state.T)*
                       pow(6./(M_PI*rhoSoot), twothird);
```

**Expected Code:**
```cpp
double preFac = 4.0*sqrt(M_PI*kb*state.T)*
                pow(6./(M_PI*rhoSoot), twothird);
```

---

## Bug #2: Incorrect Surface Area Formula in Moss-Brookes Growth Model

**File:** `src/growthModels/growthModel_MB.cc`  
**Line:** 63  
**Classification:** Critical - Produces incorrect numerical results

**Issue:** The surface area concentration formula uses `pow(M_PI, 1.0/3.0)` where it should use `M_PI`, resulting in a value off by a factor of pi^(2/3). The commented-out original formula and the equivalent formulas in `growthModel_LL.cc` and `growthModel_FAIR.cc` all yield pi^(1/3) in the fully expanded expression, but the current active code yields pi^(-1/3).

**Current Code:**
```cpp
Am2m3 = pow(M_PI, 1.0/3.0) * pow(6 / (M_PI * rhoSoot) * M1 / M0, 2.0/3.0) * M0;
```

**Expected Code (consistent with LL and FAIR):**
```cpp
Am2m3 = M_PI * pow(6 / (M_PI * rhoSoot) * M1 / M0, 2.0/3.0) * M0;
```

---

## Bug #3: Wrong mechType and Positive Arrhenius Exponent in FAIR Oxidation

**File:** `src/oxidationModels/oxidationModel_FAIR.cc`  
**Lines:** 17, 36  
**Classification:** Critical - Multiple errors

**Issue (a):** The constructor sets `mechType = oxidationMech::LL` instead of `oxidationMech::FAIR`, causing the model to misidentify itself.

**Current Code:**
```cpp
mechType = oxidationMech::LL;
```

**Expected Code:**
```cpp
mechType = oxidationMech::FAIR;
```

**Issue (b):** The Arrhenius exponential has a positive exponent. All Arrhenius rate expressions use `exp(-Ea/RT)` with a negative sign. The positive exponent causes the rate to increase without bound at high temperatures.

**Current Code:**
```cpp
return 1.78E4 * sqrt(state.T) * exp(39E3 / (state.T*1.987)) *
       state.getGasSpC(gasSp::O2);
```

**Expected Code:**
```cpp
return 1.78E4 * sqrt(state.T) * exp(-39E3 / (state.T*1.987)) *
       state.getGasSpC(gasSp::O2);
```

---

## Bug #4: Variable Scope Error in Tar Cracking Rate

**File:** `src/tarModels/tarModel_AJ_RED.cc`  
**Line:** 46  
**Classification:** Critical - Variable always zero

**Issue:** Inside `getCrackingTarRate()`, the variable `Cl` is initialized to `0.0` on line 45. The subsequent `if` statement declares a new `double Cl` inside its scope, which shadows the outer `Cl`. After the `if` block ends, the inner `Cl` is destroyed and the outer `Cl` remains `0.0` regardless of `N0`. All surrogate tar mole fraction calculations use `Cl=0.0`.

**Current Code:**
```cpp
double Cl = 0.0;
if (N0 > 0) double Cl   = log10 (N0);
```

**Expected Code:**
```cpp
double Cl = 0.0;
if (N0 > 0) Cl = log10(N0);
```

---

## Bug #5: Integer Division in Tar Cracking Rates (4 instances)

**File:** `src/tarModels/tarModel_AJ_RED.cc`  
**Lines:** 57, 59, 63, 64  
**Classification:** Critical - Multiple reaction terms always zero

**Issue:** Four expressions use C++ integer division, all evaluating to `0`:

| Line | Expression | Integer result | Correct value |
|------|------------|----------------|---------------|
| 57   | `1/6`      | 0              | 0.1667        |
| 59   | `1/3`      | 0              | 0.3333        |
| 63   | `50/128`   | 0              | 0.3906        |
| 64   | `14/92`    | 0              | 0.1522        |

This zeroes the `xphe`, `xtol`, `r3`, and `r4` terms, corrupting `xben` and the overall cracking rate. Note: `31.1/94` on line 61 is fine (31.1 is a double literal, so float division occurs).

**Current Code:**
```cpp
xphe = 1/6*tanh(...);
xtol = 1/3*tanh(...);
double r3 = 50/128*k3*xnapth*...;
double r4 = 14/92*k4*xtol*...;
```

**Expected Code:**
```cpp
xphe = (1.0/6.0)*tanh(...);
xtol = (1.0/3.0)*tanh(...);
double r3 = (50.0/128.0)*k3*xnapth*...;
double r4 = (14.0/92.0)*k4*xtol*...;
```

---

## Bug #6: Incorrect Aromatic Ring Diameter Calculation in Tar Deposition

**File:** `src/tarModels/tarModel_AJ_RED.cc`  
**Line:** 72  
**Classification:** Critical - Value off by ~5 orders of magnitude

**Issue:** The expression `sqrt(1.395E-10 * 3)` computes the square root of the product, yielding ~2.05E-5 m. The correct expression (matching `nucleationModel_AJ_RED.cc` line 40) is `1.395E-10 * sqrt(3)`, yielding ~2.42E-10 m. The current code produces a result approximately 84,000 times too large.

**Current Code:**
```cpp
double d_a  = sqrt(1.395E-10 * 3);
```

**Expected Code:**
```cpp
double d_a  = 1.395E-10 * sqrt(3.0);
```

---

## Bug #7: Assignment Instead of Accumulation in MOMIC Continuum Coagulation

**File:** `src/sootModel_MOMIC.cc`  
**Line:** ~530 (inside `MOMICCoagulationRates`)  
**Classification:** Critical - Produces incorrect coagulation rates

**Issue:** Inside the continuum coagulation loop for `r > 0`, the code uses `Rates_C[r] =` (assignment) instead of `Rates_C[r] +=` (accumulation). This means only the last iteration of the `k` loop contributes to `Rates_C[r]`, discarding all previous terms. The free-molecular loop correctly uses `+=`.

**Current Code:**
```cpp
for (size_t k=1; k<=r-1; k++) {
    kk = k*3;
    rk = (r-k)*3;
    Rates_C[r] = binomial_coefficient(r,k) * (
                       Mp6[kk+1]*Mp6[rk+3] +
                 ...
                       );
}
```

**Expected Code:**
```cpp
for (size_t k=1; k<=r-1; k++) {
    kk = k*3;
    rk = (r-k)*3;
    Rates_C[r] += binomial_coefficient(r,k) * (
                       Mp6[kk+1]*Mp6[rk+3] +
                 ...
                       );
}
```

---

## Bug #8: Out-of-Bounds Array Access in MOMIC for 8 Moments

**File:** `src/sootModel_MOMIC.cc`  
**Classification:** Critical - Undefined behavior when Nmom=8

**Issue:** The `np` and `nq` vectors each contain 8 elements (indices 0-7). They are accessed as `np[Nmom]` and `nq[Nmom]` in `set_fractional_moments_Mp6_Mq6()`. The constructor allows `Nmom` up to 8 (from `nsoot_ > 8` check). When `Nmom=8`, `np[8]` and `nq[8]` are out-of-bounds accesses.

**Fix:** Add a 9th element to `np` and `nq` to cover the `Nmom=8` case, or change the constructor constraint to `nsoot_ > 7`.

---

## Bug #9: Uninitialized Variables in Sectional Model

**File:** `src/sootModel_SECT.cc`  
**Functions:** `get_M0_sectional`, `get_M1_sectional`  
**Classification:** Critical - Undefined behavior

**Issue:** The local variables `M0` and `M1` are declared without initialization, then used with the `+=` operator. In C++, uninitialized local variables have indeterminate values, producing garbage results.

**Current Code:**
```cpp
double sootModel_SECT::get_M0_sectional(const state &state) {
    double M0;
    for (size_t k=0; k<nsoot; k++)
        M0 += state.sootVar[k];
    return M0;
}
```

**Expected Code:**
```cpp
double sootModel_SECT::get_M0_sectional(const state &state) {
    double M0 = 0.0;
    for (size_t k=0; k<nsoot; k++)
        M0 += state.sootVar[k];
    return M0;
}
```

(Same fix for `get_M1_sectional`.)

---

## Bug #10: Unreachable Error Check in State Initialization

**File:** `src/state.cc`  
**Lines:** 66-74  
**Classification:** Moderate - Dead code / inconsistent logic

**Issue:** The code first clamps negative soot variables to 0.0, then subsequently checks for negative values and throws an error. The error check is unreachable for elements 0 through `nsoot-1` since they were already clamped. Same pattern exists for tar variables on lines 76-82.

**Current Code:**
```cpp
for(int i=0; i<nsoot; i++)
    if(sootVar_[i] < 0)
        sootVar_[i] = 0.0;

nsoot = nsoot_;
for (double s : sootVar_)
    if (s < 0)
        throw domain_error("Unphysical state value input: negative soot moment(s)");
```

---

## Bug #11: Iterate-by-Value Prevents Gas Species Clamping

**File:** `src/state.cc`  
**Lines:** 86-89  
**Classification:** Moderate - Negative mass fractions not actually clamped

**Issue:** The loop `for (double y : yGas_)` iterates by value (copy), so `y = 0` modifies the local copy only and does not update the vector element. Negative gas species mass fractions pass through uncorrected.

**Current Code:**
```cpp
for (double y : yGas_) {
    if (y < 0) {
        y = 0;
    }
    ...
}
```

**Expected Code:**
```cpp
for (double &y : yGas_) {
    if (y < 0) {
        y = 0;
    }
    ...
}
```

---

## Bug #12: Unprotected Logarithm in MOMIC Difference Table

**File:** `src/sootModel_MOMIC.cc`  
**Line:** ~629 (inside `set_diffTable`)  
**Classification:** Moderate - Potential NaN propagation

**Issue:** The function takes `log10(M[k])` without checking if `M[k] > 0`. The protective function `downselectIfNeeded` is commented out in `setSourceTerms`, so negative or zero moments can reach this code path and produce NaN values.

**Current Code:**
```cpp
for (int k=0; k<Nmom; k++)
    diffTable[k][0] = log10(M[k]);
```

---

## Bug #13: Double Negation — Oxidation Adds Soot Mass in LOGN Model (NEW)

**File:** `src/sootModel_LOGN.cc`  
**Lines:** 260, 300-302  
**Classification:** Critical - Oxidation increases soot mass instead of decreasing it

**Issue:** The oxidation terms `X0`, `X1`, `X2` are computed with a negative sign (line 260: `X1 = -Koxi*...`), making them already negative. However, the source term assembly on lines 300-302 uses `- X1`, which negates the already-negative value, producing a **positive** contribution. This means oxidation **adds** soot mass instead of removing it.

The MONO model (line 262: `+ X1`) and QMOM model (line 183: `+ oxiSrcM[i]`) both correctly add the already-negative oxidation term.

**Current Code:**
```cpp
X1 = -Koxi*M_PI*pow(6.0/(M_PI*rhoSoot), twothird)*M46;    // X1 is negative
...
sources.sootSources[0] = (N0 + G0 + Cnd0 - X0 + C0);      // -(-val) = +val → WRONG
sources.sootSources[1] = (N1 + G1 + Cnd1 - X1 + C1);
sources.sootSources[2] = (N2 + G2 + Cnd2 - X2 + C2);
```

**Expected Code:**
```cpp
sources.sootSources[0] = (N0 + G0 + Cnd0 + X0 + C0);
sources.sootSources[1] = (N1 + G1 + Cnd1 + X1 + C1);
sources.sootSources[2] = (N2 + G2 + Cnd2 + X2 + C2);
```

---

## Bug #14: Wrong Unit Conversion in NSC-Neoh Oxidation (NEW)

**File:** `src/oxidationModels/oxidationModel_NSC_NEOH.cc`  
**Line:** 53  
**Classification:** Critical - O2 oxidation rate off by factor of ~185

**Issue:** The NSC rate constants (`kA = 20`, `kB = 4.46E-3`, `kT = 1.51E5`, `kz = 21.3`) match the original Nagle & Strickland-Constable (1962) values in CGS units, so `NSC_rate` has units of g/(cm²·s). To convert to kg/(m²·s), the correct factor is ×10 (i.e., ×10⁻³ kg/g × 10⁴ cm²/m²). The code multiplies by `rhoSoot` (1850 kg/m³) instead, which is dimensionally wrong and overpredicts the O₂ oxidation component by a factor of ~185.

The OH term (line 55: `1290. * 0.13 * pOH_atm / sqrt(state.T)`) is correct — the constant 1290 already produces kg/(m²·s).

**Current Code:**
```cpp
double rSootO2 = NSC_rate * rhoSoot;                        // kg/m2*s
```

**Expected Code:**
```cpp
double rSootO2 = NSC_rate * 10.0;                           // convert g/(cm2*s) to kg/(m2*s)
```

---

## Previously Reported Items - Status

The following items from the March 6 report have been resolved or were false positives:

| # | Description | Status |
|---|-------------|--------|
| Old #1 | FUCHS d2 uses m1 | False positive -- code correctly uses m2 |
| Old #2 | HM d2 uses m1 | False positive -- code correctly uses m2 |
| Old #3 | LOGN harmonic mean condition | Fixed -- now uses `!= 0.0` |
| Old #5 | PAH duplicate DIMER.mDimer | Fixed -- only one assignment remains |
| Old #6 | state.h Doxygen `///>` | Fixed -- now uses `///<` |

---

## Files Reviewed (No Additional Bugs Found)

The following source files were audited and found to be free of bugs:

- `src/growthModels/growthModel_LL.cc` — Surface area formula correct, Arrhenius sign correct
- `src/growthModels/growthModel_FAIR.cc` — Surface area formula correct, Arrhenius sign correct
- `src/growthModels/growthModel_HACA.cc` — HACA mechanism correctly implemented
- `src/growthModels/growthModel_LIN.cc` — Linear growth model correct
- `src/oxidationModels/oxidationModel_LL.cc` — Arrhenius sign correct, MW factor included
- `src/oxidationModels/oxidationModel_LEE_NEOH.cc` — Both O₂ and OH terms correct
- `src/oxidationModels/oxidationModel_HACA.cc` — HACA oxidation correct, alpha calculation correct
- `src/oxidationModels/oxidationModel_AJ_RED.cc` — Arrhenius sign correct, mechType correct
- `src/nucleationModels/nucleationModel_LIN.cc` — Correct implementation
- `src/nucleationModels/nucleationModel_LL.cc` — Correct implementation
- `src/nucleationModels/nucleationModel_AJ_RED.cc` — `d_a` formula correct (reference for Bug #6)
- `src/nucleationModels/nucleationModel_FAIR.cc` — Correct implementation
- `src/nucleationModels/nucleationModel_MB.cc` — Correct implementation
- `src/coagulationModels/coagulationModel_FM.cc` — Free-molecular kernel correct
- `src/coagulationModels/coagulationModel_CONTINUUM.cc` — Continuum kernel correct
- `src/coagulationModels/coagulationModel_FUCHS.cc` — Fuchs interpolation correct
- `src/coagulationModels/coagulationModel_HM.cc` — Harmonic mean correct
- `src/sootModel_MONO.cc` — Sign convention correct (`+ X1` where X1 is negative)
- `src/sootModel_QMOM.cc` — Sign convention correct (`+ oxiSrcM[i]` where oxiSrcM is negative)
- `src/sootModel.cc` — Base class correct
- `src/sootDefs.h` — Constants and enumerations correct
- `src/state.h` — `getGasSpC` (kmol/m³) and `getGasSpP` (Pa) correct

---

**Report Prepared:** March 11, 2026 (Third Pass)
