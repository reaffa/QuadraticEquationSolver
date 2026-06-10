#define TEST_MODE 0
#include <stdio.h>
#include <stdlib.h>
#include "../include/performoperation.h"
#include "../include/test_suite.h"

/*  TODO:
 *  Maybe rewrite the "c = constant" logic to parse the number and actually compúte the two sides, not just compare the strings, to account for stuff like 0.5 = 1/2
 *  Handle b and c parsing
 *
 *  Rewrite the logic of different pointers in the string to offsets with uint16_t (systems programming clue) and dont forget to rewrite statements like "mainPointer = xQuadTermLoc" to satisfy the new relative position approach instead of the absolute to save memory
 *  I feel like I should rather make a documentation on how to input the equation correctly, so that I don't have to deal with handling edge cases like "0.5 = 0.5000" or "6 + 7 = 13" or "1526/15 - 4249 = 3052/30 - 2124.5" etc. and also "a_(1)x^2 + a_(2)x^2 - b_(1)x + b_(2)x = c_1 + c_2" since I dont have the logic to deal with this yet.
 */

char* Message;
void RunProgram();
char* PrintMessageOrder(uint8_t order);

int main(void) {
#if TEST_MODE
    RunTests();
#else
    RunProgram();
#endif
    return 0;
}

void RunProgram() {
    printf("Enter quadratic equation in the form: \"ax2 +- bx +- c = 0\",\n"
                 "where \"a\", \"b\", \"c\" are real numbers\n"
                 "and enter either + or - between the terms.\n"
                 "Enclose fractions in brackets and type only integers in fractions.\n");
    char* tempInput = calloc(128, sizeof(char));
    fgets(tempInput, 128, stdin);
    char* input = ShrinkRemoveWhiteSpace(tempInput);
    free(tempInput);
    Feedback feedback = PerformOperationNew(input);
    printf("The equation is %s, has ", PrintMessageOrder(feedback.order));
    switch (feedback.numOfSolutions) {
        case 0:
            printf("no solution.\n");
            break;
        case -1:
            printf("infinite number of solutions.\n");
            break;
        case 1:
            printf("one solution: x = %lf.\n", feedback.x1); // TODO: If returns a int, print w/o the decimal point, eventually add support for fractions
            break;
        case 2:
            printf("two solutions:\nx_1 = %lf\nx_2 = %lf\n", feedback.x1, feedback.x2); // TODO: If returns a int, print w/o the decimal point, eventually add support for fractions
            break;
    }
    free(input);
}

char* PrintMessageOrder(uint8_t order) {
    switch (order) {
        case 2: // Quadratic
            Message = "quadratic";
            break;
        case 1: // Linear
            Message = "linear";
            break;
        case 0: // Not a 'Quadratic or Linear equation'/'other'
            Message = "not a quadratic or linear equation";
            break;
        default:
            char Message[] = "Error";
            break;
    }
    return Message;
}



