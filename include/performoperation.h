//
// Created by Risa on 07.02.2026.
//

#ifndef PERFORMOPERATION_H
#define PERFORMOPERATION_H
#include <stdbool.h>
#include <stdint.h>
typedef struct {
    double x1;
    double x2;
    double a;
    double b;
    double c;
    int8_t numOfSolutions; // instances: 0 - no results, -1 - infinite number of results, 1 - one result, 2 - two results
    uint8_t order; // Possible values: 2 - Quadratic, 1 - Linear, 0 - Not an equation / other
    uint8_t status; // for returning, possible values: '0' - OK, '1' error in the input
}Feedback;

typedef struct {
    double coefficient;
    uint8_t status; // values - "0" proceed after function ends, "1" return immidietly from the function above (identical with status of Feedback struct values)
} TermParseFeedback;

Feedback PerformOperationNew(char* input);
int PerformOperationMyPow(int ten, int i);
Feedback PerformOperationOld(char* input);
char* Shrink(char* input);
char* ShrinkRemoveWhiteSpace(char* input);
TermParseFeedback TermParsing(char* mainPointer, char* startPointer, char* xInTermPointer);

bool PointerCheck(char* pointer);

#endif


