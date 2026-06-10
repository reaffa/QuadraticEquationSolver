/*
 * Created by Risa on 07.02.2026.
 *
 * TODO: Line 30, Tasks in main.c
 *
 */

#include "../include/performoperation.h"
#include <math.h>
#include <stdlib.h>
// #include "main.c" // TESTING - REMOVE

Feedback PerformOperationNew(char* input) {
    char* mainPointer = input;
    (char)*mainPointer;
    // initialization out of way:
    Feedback feedback = {0};
    feedback.status = 0;
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    uint8_t equalSignCount = 0;
    char* equalSignLoc = input;
    mainPointer = input;
    while (*mainPointer != '\0') { // lets actually test whether there is only 1 equal sign
        equalSignCount += *mainPointer++ == '=';
        if (equalSignCount == 1) {
            equalSignLoc = mainPointer - 1;
        }
    }
    if (equalSignCount != 1) {
        feedback.status = 1;
        return feedback;
    }
    // now we test if constant (preferably zero) is on the right
    // so
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
        mainPointer = input; // go to the start
        while (PointerCheck(mainPointer)){
            mainPointer++;
        };
        if (*mainPointer == '=') { // if you found =, means no non-zero digit was encountered
            while (PointerCheck(endPointer)) { // start checking from the left
                endPointer--;
            }
            if (*endPointer == '=') { // you found =, that means no non-zero digit was encountered
                feedback.numOfSolutions = -1;
            }else { // if non-zero digit, no solution
                feedback.numOfSolutions = 0;
            }
        }else { // if non-zero digit, we check if the numbers on both side are the same, for now I do this the lazy way of just comparing both sides as strings even if edge cases like 0.50000 = 0.5 or 0.5 = 1/2 appear
            char rightSide[64] = {0};
            mainPointer = input; // again doing it imperatively to really learn the way to do things in memory
            for (int i = 0; *mainPointer != '='; i++, mainPointer++) {
                rightSide[i] = *mainPointer;
            }
            char leftSide[64] = {0};
            mainPointer = equalSignLoc;
            for (int i = 0; *mainPointer != '\0'; i++, mainPointer++) {
                leftSide[i] = *mainPointer;
            }
            char* tempPointer1 = rightSide;
            char* tempPointer2 = leftSide;
            bool mismatch = false;
            for (int i = 0; i < 64; i++, tempPointer1++, tempPointer2++) {
                if (*tempPointer1 != *tempPointer2) {
                    mismatch = true;
                    break;
                }
            }
            if (mismatch == true) {
                feedback.numOfSolutions = 0;
            }else {
                feedback.numOfSolutions = -1;
            }
        }
        /*
         * Possible extension to this part with checking whether the numbers are equal, that would mean infinite solutions too,
         */
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
        char* xQuadTermLoc = xFirstLoc;
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
        char aSign = '+';
        if (*input == '-') {
            aSign = '-';
            mainPointer++;
        }
        if (*input == '+') { // in case there is a +
            mainPointer++;
        }
        // check if we are dealing with integer or not, we already moved the pointer to the first digit
        while (*mainPointer != 'x' && *mainPointer != '.' && *mainPointer != ',' && *mainPointer != '/') {
            mainPointer++;
        }
        if (*mainPointer == '.' || *mainPointer == ',') { // we found a decimal point
            char* decimalALoc = mainPointer;
            mainPointer++;
            while (*mainPointer != 'x' || *mainPointer != '.' || *mainPointer != ',') {
                mainPointer++;
            }
            if (*mainPointer == '.' || *mainPointer == ',') { // we return an error for 2 decimals
                feedback.status = 1;
                return feedback;
            }
            int digit = 0;
            // we start with the left side of it
            mainPointer = decimalALoc - 1;
            for (int i = 0; mainPointer <= input && *mainPointer != '-' && *mainPointer != '+'; i++) { // we move till we find the end or the sign
                digit = *mainPointer - '0';
                a += digit * PerformOperationMyPow(10, i);
                mainPointer--;
            }
            // we continue with right side
            mainPointer = decimalALoc + 1;
            for (int i = -1; *mainPointer != 'x'; i--) { // we move till we find x
                digit = *mainPointer - '0';
                a += digit * pow(10, i);
                mainPointer++;
            }
            if (aSign == '-') {
                a *= -1;
            }
        } else if (*mainPointer == '/') {// we found a fraction (so far support for only a simple one, might redo with a function for executing fractions with recursion if the fractions are nested
            int numerator = 0;
            int denominator = 0; // for now
            int digit = 0;
            char* SlashPointer = mainPointer;
            // we start converting the left side (numerator)
            // first we check it is not a double
            while (mainPointer <= input) {
                if (*mainPointer == '.'  || *mainPointer == ',') {
                    feedback.status = 1;
                    return feedback;
                }
                mainPointer--;
            }
            // then we convert
            mainPointer = SlashPointer;
            for (int i = 0;*mainPointer != '(' && *mainPointer != '-' && *mainPointer != '+' && mainPointer <= input; i++) { // checking for + and - in case someone forgets to put the bracket
                digit = *mainPointer - '0';
                numerator += digit * PerformOperationMyPow(10, i);
                mainPointer--;
            }
            // then the denominator
            mainPointer = SlashPointer++;
            // first we check it is not a double
            while (*mainPointer != 'x') {
                if (*mainPointer == '.'  || *mainPointer == ',') {
                    feedback.status = 1;
                    return feedback;
                }
                mainPointer++;
            }
            // now check the sign just in case
            mainPointer = SlashPointer++;
            if (*mainPointer == '-') {
                if (aSign == '-') { // if the of both the numerator and denominator is -, we make the sign of a + (since we already checked the sign of in the beginning
                    aSign = '+';
                }
            }
            // we convert the denominator
            mainPointer = xQuadTermLoc - 1; // cant use xQuadTermLoc-- (returns the value of xQuadTermLoc and after that decrements, and therefore the mainPointer is on the x itself and not the character before the x) or --xQuadTermLoc (modifies the xQuadTermLoc variable)
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
            a = (double)numerator / (double)denominator;

            if (aSign == '-') { // don't change, the sign in the denominator was handled above, so this executes only if the numerator had a sign
                a *= -1;
            }
        } else { // we found either int or nothing (1)
            int digit = 0;
            mainPointer = xQuadTermLoc - 1;
            int i = 0;
            for (i = 0; *mainPointer != '-' && *mainPointer != '+' && mainPointer >= input; i++) {
                digit = *mainPointer - '0';
                a += digit * PerformOperationMyPow(10, i);
                mainPointer--;
            }
            if (i == 0) { // if the i is 0, then the pointer didn't move from the spot one before the x, therefore the x has a coeficient of 1
                a = 1;
            }
            if (aSign == '-') {
                a *= -1;
            }
        }
        // now the b
        mainPointer = twoInX2Loc + 1;
        if (*mainPointer == '=') {

        }
        char bSign = '+';
        if (*mainPointer == '-') {
            bSign = '-';
            mainPointer++;
        }
        if (*mainPointer == '+') { // in case there is a +
            mainPointer++;
        }
    }
    /* REMOVE (JUST FOR TEST): */

    // 1. Calculate Discriminant: D = b^2 - 4ac
    double discriminant = (b * b) - (4 * a * c);

    // 2. Determine Number of Solutions based on D
    if (discriminant > 0) {
        feedback.numOfSolutions = 2;
    } else if (discriminant == 0) {
        feedback.numOfSolutions = 1;
    } else {
        feedback.numOfSolutions = 0; // Negative discriminant = no real solutions
    }

    // Special Case: Linear Equation (if a is 0, but b is not)
    // Example: 0x^2 + 2x + 4 = 0  ->  2x = -4
    if (a == 0 && b != 0) {
        feedback.order = 1; // Linear
        feedback.numOfSolutions = 1;
    }


    feedback.a = a;
    // feedback.b = b;      FOR TESTING ONLY
    // feedback.c = c;

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
    if (pointer == NULL) {
        return NULL;
    }
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
    if (input == NULL) {
        return NULL;
    }
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