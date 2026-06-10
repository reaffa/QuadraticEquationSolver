# Quadratic Equation Solver

The project is currently in pre-alpha stage.
Experimental parser for processing and solving equations provided as text input.

The current implementation focuses primarily on quadratic term parsing and establishing architecture for future support of more complex expressions.

## Running modes
To run, clone/download the repository and run `main.c`.

The project currently supports two execution modes:
1. Normal mode - default (`TEST_MODE 0`)
2. Test mode - runs predefined test cases (`TEST_MODE 1`)

To switch modes, set the value of `TEST_MODE` to either 0 or 1 on the first line of `main.c` file.

## What I learned
- Pointer manipulation and memory access in C
- String parsing and manual processing of text input
- Automated testing of multiple input scenarios
- Struct layout considerations and memory efficiency
- Designing extensible parsing logic

## Planned improvements
- Support linear and constant terms
- Clean up the `PerformOperationNew()` function
- Replace absolute pointer positions with relative offsets to reduce memory usage
- Add parser documentation and supported syntax specification

## Known limitations
- Parser currently supports only quadratic term, identity and contradiction processing
- Input format is intentionally constrained
- Expressions are not yet fully normalized
