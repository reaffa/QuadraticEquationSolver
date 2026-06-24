//
// Created by Risa on 07.02.2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "performoperation.h"
#define EPSILON 0.000001

// Helper to make the main test function cleaner
void RunIndividualTest(char* input, uint8_t expectedOrder, double expectedA, double expectedB, double expectedC, double expectedX1, double expectedX2, int expectedSolutions, int expectedStatus) {
    printf("Testing: %-25s ... ", input); // %-25s adds padding for alignment
    input = ShrinkRemoveWhiteSpace(input);
    Feedback result = PerformOperationNew(input);

    // 1. Check Status (0 = OK, 1 = Error)
    if (result.status != expectedStatus) {
        printf("FAILED -> Status: %d (Expected %d)\n", result.status, expectedStatus);
        return;
    }

    // 2. Check Order (2, 1, 0)
    if (result.order != expectedOrder) {
        printf("FAILED -> Order: %d (Expected %d)\n", result.order, expectedOrder);
        return;
    }

    // 3. Check Parsed Variables
    if (fabs(result.a - expectedA) > EPSILON) {
        printf("FAILED -> a: %f (Expected %f)\n", result.a, expectedA);
        return;
    }
    if (fabs(result.b - expectedB) > EPSILON) {
        printf("FAILED -> b: %f (Expected %f)\n", result.b, expectedB);
        return;
    }
    if (fabs(result.c - expectedC) > EPSILON) {
        printf("FAILED -> c: %f (Expected %f)\n", result.c, expectedC);
        return;
    }


    // 4. Check Number of Solutions (The Solver Logic)
    // -1 can represent "Infinite" in your design
    if (result.numOfSolutions != expectedSolutions) {
        printf("FAILED -> Solutions: %d (Expected %d)\n", result.numOfSolutions, expectedSolutions);
        return;
    }

    if (result.numOfSolutions == 2) {
        // Allow roots to match in either order (x1=A, x2=B OR x1=B, x2=A)
        bool match1 = (fabs(result.x1 - expectedX1) < EPSILON && fabs(result.x2 - expectedX2) < EPSILON);
        bool match2 = (fabs(result.x1 - expectedX2) < EPSILON && fabs(result.x2 - expectedX1) < EPSILON);
        if (!match1 && !match2) {
            printf("FAILED -> Roots: %f, %f (Expected %f, %f)\n", result.x1, result.x2, expectedX1, expectedX2);
            return;
        }
    }else if (result.numOfSolutions == 1) {
        if (fabs(result.x1 - expectedX1) > EPSILON) {
            printf("FAILED -> x1: %f (Expected %f)\n", result.x1, expectedX1);
            return;
        }
        if (fabs(result.x2 - 0.0) > EPSILON) {
            printf("FAILED -> x2: %f (Expected 0.0 for D=0)\n", result.x2);
            return;
        }
    }else if (result.numOfSolutions == -2) {
        // complex solutions, logic later
    }else { // 0 or -1 solutions
        if (fabs(result.x1 - 0.0) > EPSILON || fabs(result.x2 - 0.0) > EPSILON) {
            printf("FAILED -> Default Roots not 0.0 (Got %f, %f)\n", result.x1, result.x2);
            return;
        }
    }

    free(input);

    printf("PASS\n");
}

void RunTests() {
    printf("=== Running Expanded Test Suite ===\n\n");
    // FORMAT: input, expectedOrder, expectedA, expectedB, expectedC, expectedX1, expectedX2, expectedSolutions, expectedStatus

    // --- SECTION 1: Parsing 'a' (Standard Quadratic Cases) ---
    RunIndividualTest("0x^2 = 0",    0,  0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("2 x^2 = 0",   2,  2.0, 0.0, 0.0,  0.0, 0.0,  1, 0);
    RunIndividualTest("- 4x^2 = 0",  2, -4.0, 0.0, 0.0,  0.0, 0.0,  1, 0);
    RunIndividualTest("x^ 2 =0 ",    2,  1.0, 0.0, 0.0,  0.0, 0.0,  1, 0);
    RunIndividualTest("- x ^2 = 0",  2, -1.0, 0.0, 0.0,  0.0, 0.0,  1, 0);

    // --- SECTION 2: The "No Solution" / "Infinite" Logic ---
    RunIndividualTest("0=0",     0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("5=0",     0, 0.0, 0.0, 0.0,  0.0, 0.0,  0, 0);
    RunIndividualTest("0=5",     0, 0.0, 0.0, 0.0,  0.0, 0.0,  0, 0);
    RunIndividualTest("5=5",     0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("5 =5 ",   0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("5 = 5 ",  0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("5 = 5",   0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("5=  5 ",  0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);

    // --- SECTION 3: Standard Integers ---
    printf("\n--- Standard Integers ---\n");
    RunIndividualTest("x^2+2x+1=0",   2,  1.0,  2.0,  1.0,  -1.0,  0.0, 1, 0); // (x+1)^2 = 0
    RunIndividualTest("x^2-4x+4=0",   2,  1.0, -4.0,  4.0,   2.0,  0.0, 1, 0); // (x-2)^2 = 0
    RunIndividualTest("2x^2-5x+2=0",  2,  2.0, -5.0,  2.0,   2.0,  0.5, 2, 0); // D = 9
    RunIndividualTest("x^2+x+1=0",    2,  1.0,  1.0,  1.0,   0.0,  0.0, 0, 0); // No real solutions
    RunIndividualTest("-3x^2+6x+9=0", 2, -3.0,  6.0,  9.0,   3.0, -1.0, 2, 0);

    // --- SECTION 4: Neglected Terms (Implicit Zeros) ---
    printf("\n--- Neglected Terms ---\n");
    RunIndividualTest("x^2-9=0",      2,  1.0,  0.0, -9.0,   3.0, -3.0, 2, 0); // Missing bx
    RunIndividualTest("3x^2+6x=0",    2,  3.0,  6.0,  0.0,   0.0, -2.0, 2, 0); // Missing c
    RunIndividualTest("x^2=0",        2,  1.0,  0.0,  0.0,   0.0,  0.0, 1, 0); // Missing both
    RunIndividualTest("-x^2+4=0",     2, -1.0,  0.0,  4.0,   2.0, -2.0, 2, 0); // Negative a, missing bx

    // --- SECTION 5: Explicit Zeros ---
    printf("\n--- Explicit Zeros ---\n");
    RunIndividualTest("x^2+0x-9=0",   2,  1.0,  0.0, -9.0,   3.0, -3.0, 2, 0);
    RunIndividualTest("x^2-5x+0=0",   2,  1.0, -5.0,  0.0,   5.0,  0.0, 2, 0);
    RunIndividualTest("x^2-0x+0=0",   2,  1.0,  0.0,  0.0,   0.0,  0.0, 1, 0);
    RunIndividualTest("x^2+0x=0",     2,  1.0,  0.0,  0.0,   0.0,  0.0, 1, 0); // Explicit b zero, implicit c zero

    // --- SECTION 6: Decimals ---
    printf("\n--- Decimals ---\n");
    RunIndividualTest("0.5x^2-2.5x+2=0", 2,  0.5, -2.5,  2.0,    4.0,       1.0, 2, 0);
    RunIndividualTest("1.2x^2+3.4x=0",   2,  1.2,  3.4,  0.0,    0.0, -2.833333, 2, 0); // Missing c
    RunIndividualTest("x^2-0.25=0",      2,  1.0,  0.0, -0.25,   0.5,      -0.5, 2, 0); // Missing bx

    // --- SECTION 7: Fractions ---
    printf("\n--- Fractions ---\n");
    RunIndividualTest("(1/2)x^2-x-4=0",         2,  0.5, -1.0, -4.0,    4.0, -2.0, 2, 0);
    RunIndividualTest("x^2+(3/4)x+1/8=0",       2,  1.0, 0.75, 0.125, -0.25, -0.5, 2, 0);
    RunIndividualTest("(1/4)x^2-(1/2)x+1/4=0",  2, 0.25, -0.5, 0.25,    1.0,  0.0, 1, 0); // D = 0
    RunIndividualTest("x^2+(1/2)x=0",           2,  1.0,  0.5,  0.0,    0.0, -0.5, 2, 0); // Fraction in b, missing c
    RunIndividualTest("(1/4)x^2-(1/2)x-1/4=0",  2, 0.25, -0.5,-0.25, 2.414214, -0.414214, 2, 0); // D > 0

    printf("\n\n\n\n\n=== Running Brutal Combinatorial Test Suite ===\n\n");

    // =========================================================================
    // CATEGORY 1: IMPLICIT COEFFICIENTS AND SIGNS
    // =========================================================================
    printf("--- Implicit Coefficients & Signs ---\n");
    RunIndividualTest("x^2+x+0=0",     2,  1.0,  1.0,  0.0,   0.0, -1.0, 2, 0);
    RunIndividualTest("-x^2-x+0=0",    2, -1.0, -1.0,  0.0,   0.0, -1.0, 2, 0);
    RunIndividualTest("+x^2+x+1=0",    2,  1.0,  1.0,  1.0,   0.0,  0.0, 0, 0);
    RunIndividualTest("-x^2+x-1=0",    2, -1.0,  1.0, -1.0,   0.0,  0.0, 0, 0);

    // =========================================================================
    // CATEGORY 2: FLAT FRACTIONS (WITHOUT BRACKETS)
    // =========================================================================
    printf("\n--- Flat Fractions (No Brackets) ---\n");
    RunIndividualTest("1/2x^2-1/2x-1/2=0", 2,   0.5, -0.5, -0.5, 1.618034, -0.618034, 2, 0);
    RunIndividualTest("-1/4x^2+1/2x-1/4=0",2, -0.25,  0.5,-0.25,      1.0,       0.0, 1, 0); // D = 0
    RunIndividualTest("3/2x^2-3=0",        2,   1.5,  0.0, -3.0, 1.414214, -1.414214, 2, 0);
    RunIndividualTest("x^2+5/2x=0",        2,   1.0,  2.5,  0.0,      0.0,      -2.5, 2, 0);

    // =========================================================================
    // CATEGORY 3: BRACKETED FRACTIONS WITH EXTERNAL SIGNS
    // =========================================================================
    printf("\n--- Bracketed Fractions (External Signs) ---\n");
    RunIndividualTest("(1/2)x^2+(1/2)x+1/8=0",  2,   0.5,  0.5,  0.125, -0.5, 0.0, 1, 0);
    RunIndividualTest("-(1/2)x^2-(1/2)x-1/8=0", 2,  -0.5, -0.5, -0.125, -0.5, 0.0, 1, 0);
    RunIndividualTest("-(1/4)x^2+(1/2)x-1/4=0", 2, -0.25,  0.5,  -0.25,  1.0, 0.0, 1, 0);

    // =========================================================================
    // CATEGORY 4: HOSTILE FRACTION SIGN PLACEMENT
    // =========================================================================
    printf("\n--- Hostile Fraction Sign Placement ---\n");
    RunIndividualTest("(-1/2)x^2+2x-2=0",     2, -0.5,  2.0, -2.0, 2.0, 0.0, 1, 0);
    RunIndividualTest("(1/-2)x^2+2x-2=0",     2, -0.5,  2.0, -2.0, 2.0, 0.0, 1, 0);
    RunIndividualTest("-(1/-2)x^2-2x+2=0",    2,  0.5, -2.0,  2.0, 2.0, 0.0, 1, 0);

    // =========================================================================
    // CATEGORY 5: DECIMALS
    // =========================================================================
    printf("\n--- Decimals ---\n");
    RunIndividualTest("0.25x^2-1.0x+1.0=0",   2,  0.25, -1.0,   1.0,       2.0,       0.0, 1, 0);
    RunIndividualTest("-0.5x^2+0.5x+0.75=0",  2, -0.5,   0.5,  0.75, -0.822876,  1.822876, 2, 0);
    RunIndividualTest("1.5x^2-0.75=0",        2,  1.5,   0.0, -0.75,  0.707107, -0.707107, 2, 0);

    // =========================================================================
    // CATEGORY 6: NEGLECTED AND EXPLICIT ZERO TERMS
    // =========================================================================
    printf("\n--- Neglected & Explicit Zeros ---\n");
    RunIndividualTest("x^2+0x+0=0",    2, 1.0, 0.0,  0.0,  0.0,  0.0, 1, 0);
    RunIndividualTest("x^2-0.0x-0.0=0",2, 1.0, 0.0,  0.0,  0.0,  0.0, 1, 0);
    RunIndividualTest("x^2=0",         2, 1.0, 0.0,  0.0,  0.0,  0.0, 1, 0);
    RunIndividualTest("x^2-4=0",       2, 1.0, 0.0, -4.0,  2.0, -2.0, 2, 0);
    RunIndividualTest("2x^2+3x=0",     2, 2.0, 3.0,  0.0,  0.0, -1.5, 2, 0);

    // =========================================================================
    // CATEGORY 7: NON-EQUATIONS (REGRESSION MATRIX)
    // =========================================================================
    printf("\n--- Non-Equation Sanity Matrix ---\n");
    RunIndividualTest("0=0",       0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("12.5=12.5", 0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("3/4=3/4",   0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("1/2=0.5",   0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("0.5=1/2",   0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("5=0",       0, 0.0, 0.0, 0.0,  0.0, 0.0,  0, 0);
    RunIndividualTest("0=7.2",     0, 0.0, 0.0, 0.0,  0.0, 0.0,  0, 0);
    RunIndividualTest("6/18=1/3",  0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);

    // Int <-> Decimal
    RunIndividualTest("2=2.0",     0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("5.0=5",     0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("0=0.0",     0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);

    // Decimal <-> Fraction
    RunIndividualTest("0.5=1/2",   0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("3/4=0.75",  0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("0.2=1/5",   0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);

    // Int <-> Fraction
    RunIndividualTest("3=6/2",     0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("10/2=5",    0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("4=4/1",     0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);

    // Special Requirements: Fraction <-> Int (Numerator is 0)
    RunIndividualTest("0/5=0",     0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);
    RunIndividualTest("0=0/10",    0, 0.0, 0.0, 0.0,  0.0, 0.0, -1, 0);

    // Contradiction checks just to be safe (Numerator 0 vs non-zero)
    RunIndividualTest("0/5=5",     0, 0.0, 0.0, 0.0,  0.0, 0.0,  0, 0);
    RunIndividualTest("0=1/0",     0, 0.0, 0.0, 0.0,  0.0, 0.0,  0, 1);
    // RunIndividualTest("x^2+2x+1=0", 'Q', 1.0, 1, 0); // Det = 0 (1 solution)
    // RunIndividualTest("x^2+1=0",    'Q', 1.0, 0, 0); // Det < 0 (No real solutions)
    // RunIndividualTest("2x+4=0",     'L', 0.0, 1, 0); // Linear Equation


    printf("\n=== All Active Tests Finished ===\n");
}