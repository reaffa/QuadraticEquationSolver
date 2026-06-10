//
// Created by Risa on 10.06.2026.
//

#ifndef TEST_SUITE_H
#define TEST_SUITE_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "performoperation.h"

void RunIndividualTest(char* input, uint8_t expectedOrder, double expectedA, int expectedSolutions, int expectedStatus);
void RunTests();

#endif //TEST_SUITE_H
