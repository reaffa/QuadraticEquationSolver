# Changelog

All notable changes to this project will be documented in this file.

## [v1.0.1] - 2026-06-24

### Added
- NULL pointer checks for safety
- Feedback.status value 2 for memory allocation failure reporting

### Changed
- Discriminant zero case is now handled by epsilon comparison instead of exact equality (epsilon defined at the top of `performoperation.c`)

## [v1.0.0] - 2026-06-22

### Added
- Linear term parsing (bx)
- Constant term parsing (c)
- Parser now actually solves the equation

### Changed
- Identity and contradiction detection is now handled by evaluation instead of string comparison
- Term parsing is now a separate function

### Fixed
- Fraction and decimal parsing

## [2026-06-15] - Pre-release: quadratic term parser

### Added
- Parser for the quadratic (ax^2) term
- Test mode (TEST_MODE) with predefined test scenarios
- Basic project structure (CMake, README)

### Known limitations
- Parser supported only the quadratic term, identity, and contradiction processing
- Input format was intentionally constrained