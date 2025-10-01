# Symbolic Differentiator

A C++ program that performs symbolic differentiation of mathematical expressions.

## Features

*   Parses algebraic mathematical expressions into an Abstract Syntax Tree (AST).
*   Differentiates expressions with respect to a given variable.
*   Simplifies the resulting derivative expression.
*   Supports basic arithmetic operations (+, -, *, /) and variables.

## Building

To build the project, you will need a C++ compiler that supports C++17. Then, simply run `make`:

```bash
make
```

This will create an executable named `sdi` in the root directory.

## Usage

To run the program, execute the `sdi` executable:

```bash
./sdi
```

The program will prompt you to enter a mathematical expression and the variable to differentiate with respect to.

### Example

```
Enter an expression: 2*x^2+5
Differentiate with respect to: x
Parsed expression AST: ((2 * x^2) + 5)
Derivative: 2*2*x
```

## Testing
I'm lazy.

## License

This project is licensed under the GPLv3 license.
