/*
 * Created by Risa on 07.02.2026.
 */

#include "../include/performoperation.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#define EPSILON 0.000001
// #include "main.c" // TESTING - REMOVE

Feedback PerformOperationNew(char* input) {
    // initialization out of way:
    Feedback feedback = {0};
    feedback.status = 2;
    assert(input != NULL);
    TermParseFeedback termParseFeedback = {0};
    feedback.status = 0;
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    uint8_t equalSignCount = 0;
    char* mainPointer = input;
    (char)*mainPointer;
    char* equalSignLoc = input;
    mainPointer = input;
    bool* tempFlag = malloc(sizeof(bool));
    if (tempFlag == NULL) {
        feedback.status = 2;
        return feedback;
    }
    *tempFlag = 1;
    while (*mainPointer != '\0') { // lets actually test whether there is only 1 equal sign
        equalSignCount += *++mainPointer == '=';
        if (equalSignCount == 1 && *tempFlag) {
            equalSignLoc = mainPointer;
            *tempFlag = 0;
        }
    }
    free(tempFlag);
    if (equalSignCount != 1) {
        feedback.status = 1;
        return feedback;
    }
    // now we test if there is any x on the left (OR just use strchr() ofc, but I wanted to make it my own)
    mainPointer = input;
    while (*mainPointer != 'x' && *mainPointer != '=') { // move till you find x or equal sign
        mainPointer++;
    }
    if (*mainPointer == '=') {
        mainPointer++;
        while (*mainPointer != '\0') {
            mainPointer++;
        }
    }
    if (*mainPointer == '\0') {
        // if pointer on end, means no "x" in the equation, so either no or infinite solutions
        feedback.order = 0;
        char* endPointer = mainPointer - 1;
        // left side
        termParseFeedback = TermParsing(mainPointer, input, equalSignLoc);
        if (termParseFeedback.status == 1) {
            feedback.status = 1;
            return feedback;
        }
        // right side
        TermParseFeedback termParseFeedback2 = {0};
        termParseFeedback2 = TermParsing(equalSignLoc + 1, equalSignLoc + 1, endPointer + 1);
        if (termParseFeedback2.status == 1) {
            feedback.status = 1;
            return feedback;
        }
        if (fabs(termParseFeedback2.coefficient - termParseFeedback.coefficient) < 0.00001) { // identity
            feedback.numOfSolutions = -1;
        }else { // contradiction
            feedback.numOfSolutions = 0;
        }
        return feedback;
    }
    // now we arrived at x, we check if it is quadratic or not (for now we assume that the user typed it in correct format, that means quadratic term first, then linear, then constant, if they're present
    char* xFirstLoc = mainPointer;
    // we check if zero is on the right side first though

    mainPointer = xFirstLoc + 1;
    bool capetFormat = 0;
    if (*mainPointer == '^') {
        mainPointer++;
        capetFormat = 1;
    }
    if (*mainPointer == '2') {
        char* xTermLoc = xFirstLoc;
        char* twoInX2Loc = mainPointer; // we mark the location of the 2
        switch (*(twoInX2Loc + 1)) { // if the character after 2 is anything but +, - or = we return to avoid bugs
            case '+':
            case '-':
            case '=':
                break;
            default:
                feedback.status = 1;
                return feedback;
        }
        mainPointer = input; // move to beginning
        feedback.order = 2;
        // a parsing
        // TODO: Thought - the TermParsing() return value could be reduced to double only instead of the struct by removing the status variable and implementing it's 1 value as a 0 value of the double, since the status only changes to 0 if there is a issue in the input (this works for "a" in this case but might be harder to implement for "b" and "c" coefficients if we want the function to be generalized and work at all times)
        termParseFeedback = TermParsing(mainPointer, input, xTermLoc);
        if (termParseFeedback.status == 1) { // because if we return from the TermParsing function, there is an error in the input, so we need to return here too, since doesnt make sense to continue
            feedback.status = 1;
            return feedback;
        }
        a = termParseFeedback.coefficient;


        // now the b or c
        mainPointer = twoInX2Loc + 1;
        while (*mainPointer != 'x' && *mainPointer != '=') { // iterate till "x" or "=" (to find if we have linear term or no (and in case of no, we stop at = sign)
            mainPointer++;
        }
        if (*mainPointer == '=') { // we at "=", so no linear ("bx") term
            if (*(twoInX2Loc + 1) != '=') { // this is so that we know if there is a c term before the "=" sign or no (if value at mainPointer is "=", then quadratic term is alone). But because we have negated equality comparasion, this executes when quadratic term is not alone)
                xTermLoc = twoInX2Loc; // we need to push the xTermLoc to the location of "twoInX2Loc" so that c Parsing works properly and behaves as if the "2" in "ax^2" is the "x" in "bx"
                goto cParsing;
            } // else - the quadratic term is alone, so we skip to discriminant calculation
        }
        if (*mainPointer == 'x') {
            xTermLoc = mainPointer;
            mainPointer = twoInX2Loc + 1;
            // b parsing
            termParseFeedback = TermParsing(mainPointer, twoInX2Loc + 1, xTermLoc);
            if (termParseFeedback.status == 1) {
                feedback.status = 1;
                return feedback;
            }
            b = termParseFeedback.coefficient;
            // c parsing
            cParsing:
                mainPointer = xTermLoc + 1;
                while (PointerCheck(mainPointer)) {
                    mainPointer++;
                }
                if (*mainPointer == '=') {
                    c = 0;
                }else {
                    char* cLastDigitPointer = mainPointer; // this is probably redundant and we can just replace with equalSignLoc
                    while (*cLastDigitPointer != '=') { // so that we know where the last digit before "=" is
                        cLastDigitPointer++;
                    }
                    mainPointer = xTermLoc + 1;
                    termParseFeedback = TermParsing(mainPointer, xTermLoc + 1, cLastDigitPointer);
                    if (termParseFeedback.status == 1) {
                        feedback.status = 1;
                        return feedback;
                    }
                    c = termParseFeedback.coefficient;
                }
        }






        // 1. Calculate Discriminant: D = b^2 - 4ac
        double discriminant = (b * b) - (4 * a * c);

        // 2. Determine Number of Solutions based on D
        if (fabs(discriminant) <= EPSILON) {
            feedback.numOfSolutions = 1;
            if (a == 0) {
                feedback.numOfSolutions = -1;
                feedback.order = 0;
            }else {
                feedback.x1 = - b / (2 * a);
            }
        }else if (discriminant > 0) {
            feedback.numOfSolutions = 2;
            feedback.x1 = (- b + sqrt(discriminant)) / (2 * a);
            feedback.x2 = (- b - sqrt(discriminant)) / (2 * a);
        } else {
            feedback.numOfSolutions = 0; // Negative discriminant = no real solutions, but 2 complex solutions
            // TODO: WHEN YOU IMPLEMENT COMPLEX SOLUTIONS, CHANGE THE numOfSolutions variable to value "-2"
        }
    }


    // Special Case: Linear Equation (if a is 0, but b is not)
    // Example: 0x^2 + 2x + 4 = 0  ->  2x = -4 or just "2x + 4 = 0"
    if (a == 0 && b != 0) {
        feedback.order = 1; // Linear
        feedback.numOfSolutions = 1;
    }


    feedback.a = a;    //
    feedback.b = b;    //  FOR TESTING ONLY
    feedback.c = c;    //

    return feedback;
}

TermParseFeedback TermParsing(char* mainPointer, char* startPointer, char* endPointer) { // pretty sure startPointer when evaluating the coefficients is always at the sign, endPointer does not point at the last digit, it is the character after last digit of the coefficient we parse, might have kept the xInTermPointer name, since technically in the constant term the x is to the power of 0, therefore it equals one and in the constant term case it would be at the place of the equal sign :))
    mainPointer = startPointer;
    TermParseFeedback feedback = {0};
    feedback.status = 0;
    char sign = '+';
    if (*startPointer == '-') {
        sign = '-';
        mainPointer++;
    }
    if (*startPointer == '+') { // in case there is a "+"
        mainPointer++;
    }
    // check if we are dealing with integer or not, we already moved the pointer to the first digit
    while (*mainPointer != '.' && *mainPointer != ',' && *mainPointer != '/' && mainPointer < endPointer) {
        mainPointer++;
    }
    if (*mainPointer == '.' || *mainPointer == ',') { // we found a decimal point
        char* decimalLoc = mainPointer;
        mainPointer++;
        while (*mainPointer != '.' && *mainPointer != ',' && *mainPointer != '/' && mainPointer < endPointer) { // "=" for "c" term, which precedes "="
            mainPointer++;
        }
        if (*mainPointer == '.' || *mainPointer == ',') { // we return an error for 2 decimals
            feedback.status = 1;
            return feedback;
        }
        int digit = 0;
        // we start with the left side of it
        mainPointer = decimalLoc - 1;
        for (int i = 0; mainPointer >= startPointer && *mainPointer != '-' && *mainPointer != '+'; i++) { // we move till we find the end or the sign
            digit = *mainPointer - '0';
            feedback.coefficient += digit * PerformOperationMyPow(10, i);
            mainPointer--;
        }
        // we continue with right side
        digit = 0;
        mainPointer = decimalLoc + 1;
        for (int i = -1; mainPointer < endPointer; i--) { // we move till we find x
            digit = *mainPointer - '0';
            feedback.coefficient += digit * pow(10, i);
            mainPointer++;
        }
        if (sign == '-') {
            feedback.coefficient *= -1;
        }
    } else if (*mainPointer == '/') {// we found a fraction (so far support for only a simple one, might redo with a function for executing fractions with recursion if the fractions are nested)
        int numerator = 0;
        int denominator = 0; // for now
        int digit = 0;
        char* SlashPointer = mainPointer;
        // we start converting the left side (numerator)
        // first we check it is not a double, because so far no support for doubles
        while (mainPointer > startPointer) {
            if (*mainPointer == '.'  || *mainPointer == ',') {
                feedback.status = 1;
                return feedback;
            }
            mainPointer--;
        }
        // then we convert
        mainPointer = SlashPointer - 1;
        for (int i = 0; *mainPointer != '(' && *mainPointer != '-' && *mainPointer != '+' && mainPointer >= startPointer; i++) { // checking for + and - in case someone forgets to put the bracket
            digit = *mainPointer - '0';
            numerator += digit * PerformOperationMyPow(10, i);
            mainPointer--;
        }
        if (*mainPointer == '-') {
            if (sign == '+') {
                sign = '-';
            }
        }
        // then the denominator
        mainPointer = SlashPointer + 1;
        // first we check it is not a double
        while (mainPointer < endPointer) {
            if (*mainPointer == '.'  || *mainPointer == ',') {
                feedback.status = 1;
                return feedback;
            }
            mainPointer++;
        }
        // now check the sign just in case
        mainPointer = SlashPointer + 1;
        if (*mainPointer == '-') {
            if (sign == '-') { // if the of both the numerator and denominator is -, we make the sign of a + (since we already checked the sign of in the beginning
                sign = '+';
            }else {
                sign = '-';
            }
        }
        // we convert the denominator
        mainPointer = endPointer - 1; // cant use xQuadTermLoc-- (returns the value of xQuadTermLoc and after that decrements, and therefore the mainPointer is on the x itself and not the character before the x) or --xQuadTermLoc (modifies the xQuadTermLoc variable)
        if (*mainPointer == ')') { // if someone forgot the bracket we would be fine but if they have done it properly we move so we can start the loop below
            mainPointer--;
        }
        for (int i = 0;*mainPointer != '/' && *mainPointer != '-' && *mainPointer != '+'; i++) { // checking for either the slash or the sign if someone put it there
            digit = *mainPointer - '0';
            denominator += digit * PerformOperationMyPow(10, i);
            mainPointer--;
        }
        if (denominator == 0) { // if the denominator is zero, we return
            feedback.status = 1;
            return feedback;
        }
        feedback.coefficient = (double)numerator / (double)denominator;

        if (sign == '-') { // don't change, the sign in the denominator was handled above, so this executes only if the numerator had a sign
            feedback.coefficient *= -1;
        }
    } else { // we found either int or nothing (1)
        int digit = 0;
        mainPointer = endPointer - 1;
        int i = 0;
        for (i = 0; *mainPointer != '-' && *mainPointer != '+' && mainPointer >= startPointer; i++) {
            digit = *mainPointer - '0';
            feedback.coefficient += digit * PerformOperationMyPow(10, i);
            mainPointer--;
        }
        if (i == 0) { // if the i is 0, then the pointer didn't move from the spot one before the x, therefore the x has a coeficient of 1
            feedback.coefficient = 1;
        }
        if (sign == '-') {
            feedback.coefficient *= -1;
        }
    }
    return feedback;
}

int PerformOperationMyPow(int ten, int i) {
    // USE ONLY IF i >= 0
    if (i < 0) {
        return 0;
    }
    int result = 1;
    for (int j = 0; j < i; j++) {
        result *= ten;
    }
    return result;
}

bool PointerCheck(char* pointer) {
    assert(pointer != NULL);
    switch (*pointer) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '=':
            return false;
        default:
            return true;
    }
}

char* Shrink(char* input){
    assert(input != NULL);
    char* start = input;
    // 1. Skip leading spaces
    // check for *start != '\0' to prevent crashing on empty strings
    while (*start == ' ' && *start != '\0') {
        start++;
    }
    // 2. Calculate the length of the REMAINING string (up to newline or null)
    char* end = start;
    int len = 0;
    while (*end != '\n' && *end != '\0' && *end != ' ') {
        end++;
        len++;
    }
    // 3. Allocate memory
    char* result = malloc(len + 1); // +1 for the null terminator
    if (result == NULL) {
        return NULL;
    }
    // 4. Copy from 'start' (the trimmed position), not 'input'
    for (int i = 0; i < len; i++) {
        if (*start == ' ') {
            continue;
        }
        result[i] = start[i];
    }

    result[len] = '\0'; // Ensure it is null-terminated
    return result;
}

char* ShrinkRemoveWhiteSpace(char* input) {
    if (input == NULL) {
        return NULL;
    }
    char* pointer = input;
    uint8_t len = 0;
    while (*pointer != '\0') {
        if (!(*pointer == ' ' || *pointer == '\n')) {
            len++;
        }
        pointer++;
    }
    char* result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    pointer = input;
    uint8_t i = 0;
    while (i < len) {
        if (!(*pointer == ' ' || *pointer == '\n')) {
            result[i] = *pointer;
            i++;
        }
        pointer++;
    }
    result[len] = '\0';
    return result;
}
/*

Feedback PerformOperationOld(char* input) {
    /*   So far we assume:
     *   - the equation is in the proper format, we are going to make a mechanism to check for it later if feasible
     *      - equation sign
     *      - zero on the right side
     *      - "ax^2 +- bx +- c = 0"
     *      - if no x, check if the right side contains anything other than "0", "," or "." in one term, if so, it is interpreted as non-zero constant
     *
     *
    char* pointer = input; // start of string
    Feedback feedback;
    // initialization out of way:
    feedback.numOfSolutions = 0;
    feedback.x1 = 0;
    feedback.x2 = 0;
    feedback.order = 0;
    while (*pointer != 'x' || *pointer != '\0') { // move till you find x or end
        pointer++;
    }
    if (*pointer == '\0') { // if pointer on end, means no "x" in the equation, so either no or infinite solutions
        feedback.order = 0;
        pointer = input; // go to the start
        if (*pointer == '0') { // if you found zero
            // doing it this way to spice things up and check whether someone typed 007 instead of 7 for example, the do while loop is removable, just keep the pointer increment
            do {
                pointer++; // go one after zero
            }while (PointerCheck(pointer) && *pointer != '\0'); // go till you find number other than zero
            // Does not work after striming the string from white spaces:
            if (*pointer == ' ') { // if blank space at pointer at space, that means zero all the way (though no way to handle letter inside the letter string, might just ingnore it to be honest, since the input is flawed)
                feedback.numOfSolutions = -1; // zero on the right side, therefore infinite number of solutions (similarly to how "f(x)" of constant function is always a constant, at any point "x")
            }else {
                feedback.numOfSolutions = 0; // non-zero on the right side, therefore no solutions
            }
        }

    }
    char* XQTermLoc = pointer; // x Quadratic Term Location
    pointer++; // we at the character after x
    if (*pointer == '^') { // if the character is caret, go to what is after the caret
        pointer++;
    }
    if (*pointer == '2') {
        char* twoInX2Loc = pointer;
        feedback.order = 2;
        char aSign = '+';
        if (*input == '-') {
            aSign = '-';
        }
        // 12354x
        // need to rework this, since no support for negative "a", a = 1 and also for double
        char QTermConstLen = input - XQTermLoc; // We find the length by substracting location of x and the first character of the input, which corresponds to first digit of the constant in quadratic term
        if (aSign == '-') {
            QTermConstLen--; // We need to decrement the length of the constant, if the first character is a "-"
        }
        pointer = XQTermLoc - 1;
        // integer conversion, will add double later
        int a = 0; // the "a"
        int digit = 0;
        for (int i = 0; i < QTermConstLen; i++) {
            digit = *pointer - '0';
            a += digit * pow(10, i);
            pointer--;
        }
        if (aSign == '-') {
            a *= -1;
        }
        // find b
        // pointer = twoInX2Loc;
        // pointer++;
        // while (*pointer != 'x' && *pointer != '\0') {
        //     pointer++;
        // }




    }
    else if (*pointer == ' ' || *pointer == '1') {
        feedback.order = 1;
    }



    return feedback;
}
*/