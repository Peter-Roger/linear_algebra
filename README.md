## Implementation notes

This library was implemented using [Expression Templates][1]. Normally, temporary
objects are created to store the intermediary results of computed terms while
evaluation of the expression is progressing.

For example:

```c++
Matrix<int> res = a * b * c;
```

First **a * b** is computed and the result is stored in a temporary. Then the
temporary is multiplied by **c** to give the final result.

Expression templates render temporaries unnecessary by evaluating the entire
expression as a single term in one go. This is achieved by using generics to
create [expression-tree][2] like structures during compile time which are then
evaluated as one monolithic term. This can result in huge savings on memory and
time, especially if we are dealing with objects of a significant size. 

## Compile:
```
make
```

## Include library

```c++
#include "linear_algebra.hpp"
```

## Creating matrices

```c++
Matrix<int> m0; // 0x0 Matrix
Matrix<float> m1(5,7); // 5x7 matrix of floats, all initialized to 0
Matrix<double> m2 = {{2.5, 5.2},{4.1, 7.3}}; // 2x2 matrix of doubles
m0 = m2; // m0 is now a 2x2 matrix of ints {{2, 5},{4, 7}}
```

## Accessing/Setting elements
```c++
Matrix<int> m = {{13, 2, 8}, {1, 4, 21}, {7, 16, 8}};
int x = m(0, 0); // 13
int y = m(1, 2); // 21
m.set(2, 2, 55);
int z = m(2, 2); // 55
```

## Multiplying matrices

```c++
Matrix<int> res = a * b * c * d;
Matrix<int> res2 = mult(a, b);
Matrix<int> res3 = mult(a, mult(b, c)) * d;
```

## Transposing matrices
```c++
Matrix<int> m = trans(a);
```

## Printing

```c++
Matrix<int> m = {{5,2, 3}, {0, 9, 1}, {7, 6, 8}};
std::cout << m << std::endl;
```
```
5 2 3
0 9 1
7 6 8
```

[1]: https://en.wikipedia.org/wiki/Expression_templates
[2]: https://en.wikipedia.org/wiki/Binary_expression_tree