![alt text](https://github.com/colinhect/nominal/raw/master/Nominal.png "Nominal")
=======

Build status: [![Build status](https://travis-ci.org/colinhect/nominal.png)](https://travis-ci.org/colinhect/nominal)

Nominal is a small, dynamic, general-purpose programming language.  It attempts to strike a balance between the simple extensibility of the Lisp family and the pragmatic expressiveness of Python.

# Language Tour

### Define variables and perform arithmetic
```
x := 5
y := 10
z := (x + y) * 10
print: z
```
`150`

### Define and invoke functions
```
add := [ a b | a + b ]
sum := add: 3 4
print: sum
```
`7`

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

# Getting Started
The core Nominal language itself is simple and does not have built-in syntax for control structures. Everything is expressed in terms of functions, closures, and associative arrays. Common control structures such as `if` are just like any other function.

## Definition vs. Assignment
The `:=` defines a new variable in the current scope. The `=` operator assigns a new value to a previously defined variable in the current scope or any scope above.

Define a new variable called `x` and assign value `3` to it:
```
x := 3
```

Assign a new value to `x`:
```
x = x + 1
```

## Types
Each variable can either be assigned a value or a reference to an object.

### Value Types
Value types are copied.

* `Nil` - The void type
  - `x := nil`
* `Number` - A numeric value which can behave as a signed integer or a floating point number (e.g. `7` or `3.14`)
  - `x := 7`
* `Boolean` - A boolean value which is `true` or `false`.
  - `x := true`

### Reference Types
Reference types are reference counted and garbage collected.

* `String` - A string of unicode characters
  - `x := "A string"`
* `Map` - An associative array of one-to-one mapping of values to values (can also behave as a list)
  - `x := { 1, 2, 7, 9 }`
  - `x := { 5 -> "something", "what" -> 3 }`
* `Function` - A sequence of code which can be called with arguments (can have a captured scope to behave as a closure)

## Arithmetic
Arithmetic operators work like in any p

## Defining Functions
Functions are defined within `[` and `]` with optional input variables demarcated before `|`. Input variables are separated by spaces.

Function that takes no arguments and returns the number 7:
```
f := [ 7 ]
```

Function that takes an argument and returns the argument plus one:
```
f := [ x | x + 1 ]
```

Function that takes three arguments and returns their sum:
```
f := [ x y z | x + y + z ]
```

## Invoking Functions
Functions are invoked by postfixing `:` after a function. Input variables passed to the function appear after `:` and are separated by spaces.

Invoke a function with no arguments and assign the returned value to a new defined variable `x`:
```
x := f:
```

Invoke a function with three numeric arguments and assign the returned value to a new defined variable `x`:
```
x := f: 1 2 3
```

## Control Structures

## Strings

## Maps

## Classes

## Overriding Operators

## Modules
