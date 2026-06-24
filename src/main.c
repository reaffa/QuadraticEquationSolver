#define TEST_MODE 1
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/performoperation.h"
#include "../include/test_suite.h"

/*  TODO:
     *  Rework the "sign" variable in "performoperation.c" file functions to be a boolean, so that we can inverse it more easily, of course there has to be rewritten the other logic in the parser
     *  Handle zeros before everything (because fractions mess up the fractions like 0 / 1, we need to replace the terms that evaluate to 0 before anything so that parsing of the terms works (the fraction cases and also 0x^2 = 0 (though this specific case I fixed already))
     *  Also check that = doesnt border anything except numbers or "x", (again another nitpick optimization so that parsing works as it should
     *  The "order" value of "Feedback" struct needs to be reworked, because for equation in the format 0x^2 = 0, it spits out order "2", but the equation is technically a identity (therefore order "0"), not a quadratic
     *  Function that organizes a non-standard (not "ax^2 + bx + c = 0, contradiction or identity) input, future improvements and implications of that:
     *   - need to remove some blocks of code that become redundant
     *   - handles wierd stuff like multiple signs before the terms or terms on both sides (for example x^2 ++--- 2 = 92) by assigning either + or - by the mathematical rules to each of the final terms in the snadard form so that function, that performs the final calculation (most likely PerformOperationNew()) works as it should
     *   - handles multiple occurrences of the same term and adds them together
     *   - distributing (as an inverse of factoring)
     *
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
                 "where \"a\", \"b\", \"c\" are rational numbers\n"
                 "and enter either + or - between the terms.\n"
                 "Enclose fractions in brackets and type only integers in fractions.\n> ");
    char* tempInput = calloc(128, sizeof(char));
    if (tempInput == NULL) {
        printf("Memory allocation error.\n");
        return;
    }
    fgets(tempInput, 128, stdin);
    char* input = ShrinkRemoveWhiteSpace(tempInput);
    free(tempInput);
    Feedback feedback = PerformOperationNew(input);
    if (feedback.status == 1) {
        printf("Invalid input");
    }else if (feedback.status == 2) {
        printf("NULL pointer.");
    }else {
        printf("The equation is %s, has ", PrintMessageOrder(feedback.order));
        switch (feedback.numOfSolutions) {
            case 0:
                printf("no solution.\n");
                break;
            case -1:
                printf("infinite number of solutions.\n");
                break;
            case -2:
                printf("no real solutions, but 2 complex solutions");
            case 1:
                printf("one solution: x = %lf.\n", feedback.x1); // TODO: If returns a int, print w/o the decimal point, eventually add support for fractions
                break;
            case 2:
                printf("two solutions:\nx_1 = %lf\nx_2 = %lf\n", feedback.x1, feedback.x2); // TODO: If returns a int, print w/o the decimal point, eventually add support for fractions
                break;
            default:
                printf("an error has occured.\n");
        }
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



