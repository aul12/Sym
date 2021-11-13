# Sym++

Framework for efficient symbolic calculations using C++ zero cost abstractions

## Demo
### Symbolic differentiation
```c++
// Initialize two variables which can be used to form expressions,
// at this point they have neither a type nor a value
sym::Variable<'a'> a;
sym::Variable<'b'> b;

// Build an expression from the variables
auto f = a*a + b - a * b;

// with .resolve the actual value can be calculated when all values are given
std::cout << f.resolve(a=1, b=2) << std::endl;

// Gradients/Derivatives can be calculated symbolically:
auto da_f = sym::gradient(f, a);
auto db_f = sym::gradient(f, b);

// Printing of functions is supported (but not very pretty):
std::cout << da_f << std::endl;

// The derivative is once again an expression which can be evaluated
std::cout << db_f.resolve(a=3, b=2) << std::endl;

// or the derivative can be calculated once more:
auto dadb_f = gradient(db_f, a);
```
### Vector Handling and Interaction with other Libraries
```c++
sym::Variable<'a'> a;

// A vector is a combination of multiple expressions
sym::Vector vec{a, a + a, a * a};

// .resolve yields a tuple, this makes it possible that the different
// subexpressions have different type
auto result = vec.resolve(a=42);
// std::get<0>(result) -> 42
// std::get<1>(result) -> 42+42
// std::get<2>(result) -> 42*42

// .resolveAs allows to write the result into any object that can
// be constructed from a parameter pack consisting of the individual results
// such as STL containers or vectors of your favourite linear algebra software.
auto stlVec = vec.resolveAs<std::vector<int>>(a=17);
// res[0] -> 17
// res[1] -> 17+17
// res[2] -> 17*17
```
