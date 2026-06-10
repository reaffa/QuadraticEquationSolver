//
// Created by Risa on 07.02.2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "performoperation.h" // <--- Include the header!

// Helper to make the main test function cleaner
void RunIndividualTest(char* input, uint8_t expectedOrder, double expectedA, int expectedSolutions, int expectedStatus) {
    printf("Testing: %-15s ... ", input); // %-15s adds padding for alignment
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

    // 3. Check 'a' (Only if it's a Quadratic)
    if (result.order == 2) {
        if (fabs(result.a - expectedA) > 0.001) {
             printf("FAILED -> a: %f (Expected %f)\n", result.a, expectedA);
             return;
        }
    }

    // 4. Check Number of Solutions (The Solver Logic)
    // -1 can represent "Infinite" in your design
    if (result.numOfSolutions != expectedSolutions) {
        printf("FAILED -> Solutions: %d (Expected %d)\n", result.numOfSolutions, expectedSolutions);
        return;
    }

    printf("PASS\n");
}

void RunTests() {
    printf("=== Running Expanded Test Suite ===\n\n");


    // --- SECTION 1: Parsing 'a' (Standard Quadratic Cases) ---

    RunIndividualTest("2 x^2 = 0",  2, 2.0,  1, 0);
    RunIndividualTest("- 4x^2 = 0", 2, -4.0, 1, 0);
    RunIndividualTest("x^ 2 =0 ",   2, 1.0,  1, 0);
    RunIndividualTest("- x ^2 = 0",  2, -1.0, 1, 0);


    // --- SECTION 2: The "No Solution" / "Infinite" Logic ---

    // Case: 0 = 0 (Identity) -> Infinite Solutions
    // We expect numOfSolutions to be -1
    RunIndividualTest("0=0", 0, 0.0, -1, 0);

    // Case: 5 = 0 (Contradiction) -> No Solution
    // We expect numOfSolutions to be 0
    RunIndividualTest("5=0", 0, 0.0, 0, 0);

    // Case: 0 = 5 (Contradiction) -> No Solution
    RunIndividualTest("0=5", 0, 0.0, 0, 0);

    // Case: 5 = 5
    RunIndividualTest("5=5", 0, 0.0, -1, 0);
    RunIndividualTest("5 =5 ", 0, 0.0, -1, 0);
    RunIndividualTest("5 = 5 ", 0, 0.0, -1, 0);
    RunIndividualTest("5 = 5", 0, 0.0, -1, 0);
    RunIndividualTest("5=  5 ", 0, 0.0, -1, 0);
    /* --- SECTION 3: Future Tests (Uncomment when you implement B and C) ---

    */
    // RunIndividualTest("x^2+2x+1=0", 'Q', 1.0, 1, 0); // Det = 0 (1 solution)
    // RunIndividualTest("x^2+1=0",    'Q', 1.0, 0, 0); // Det < 0 (No real solutions)
    // RunIndividualTest("2x+4=0",     'L', 0.0, 1, 0); // Linear Equation


    printf("\n=== All Active Tests Finished ===\n");
}