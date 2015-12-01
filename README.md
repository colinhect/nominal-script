Nominal
=======

Build status: [![Build status](https://travis-ci.org/colinhect/nominal.png)](https://travis-ci.org/colinhect/nominal)

Nominal is a small, dynamic, general-purpose programming language.  It attempts to strike a balance between the simple extensibility of the Lisp family and the pragmatic expressiveness of Python.

## Example Code
```
-- Define a function that adds two values and returns the result
add := [ a b | a + b ]

-- Call the function to add 2 and 3 and assign the result to variable 'x'
x := add: 2 3

-- Define a function that computes the nth fibonacci number
fibonacci := [ n |
  if: (n < 2) [
    n
  ] [
    fibonacci: (n - 1) + fibonacci: (n - 2)
  ]
]

-- Compute the 5th fibonacci number and assign the value to the variable 'y'
y := fibonacci: 5
```
