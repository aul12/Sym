# Sym++

Framework for efficient symbolic calculations during compile time using C++ zero cost abstractions.

## Examples

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

// Gradients/Derivatives can be calculated symbolically during compile time:
auto da_f = sym::gradient(f, a);
auto db_f = sym::gradient(f, b);

// Printing of functions is supported:
std::cout << sym::toString(da_f) << std::endl;

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

# Usage

## Expressions

Sym++ is based on the concept of expressions: an expression is represented as a tree of operations and elementary
tokens. Consider the following piece of code:

```c++
auto c = a + a * b;
```

in this example both `a` and `b` are elementary tokens (for now ignore how they are defined), and `c` is an expression
tree. To be precise it is the addition operations of the elementary token `a` with the expression `a * b` which is once
again an expression tree consisting of the multiplication of the two elementary tokens `a` and `b`.

All expressions, so both operations and basic tokens implement the C++ concept
`sym::Expression`.

## Elementary Tokens

Conceptually there are two different kinds of elementary tokens: variables and constants. In practise constants are
divided into compile time constants, i.e. values that are known at compile time and runtime constants, which are both
for values that are only known when the expression tree is built and for floating point numbers (which can not be
template arguments in pre C++20).

### Variables

Variables are named placeholders in expressions. When you build an expression they have neither a value nor a type. Both
are only determined once you evaluate the expression (see below) and both can change between evaluations. Thus,
variables can be used to evaluate the same expression with different values. The second important role of variables is
during symbolic differentiation (see below).

A variable is uniquely identified by its ID, which is a string that has to be known at compile-time.
Two variables with the same ID are considered to be the same variable.

Helper macros exist to create a variable with the same ID as its identifier, or with some automatically determined unique ID.

Example:

```c++
sym::Variable<0> a; // A variable with ID 0
sym::Variable<'b'> b; // a single character or a string can be used instead of a number to increase readability
sym::Variable<"velocity"> vel; 
SYM_VARIABLE(acc); // Shorthand for sym::Variable<"acc"> acc;
SYM_A_VARIABLE pos; // SYM_A_VARIABLE Expands to a variable with new unique ID
```

### Compile Time Constants

A compile time constant is an elementary token with a type and value that is known at compile time (similarly
to `constexpr` variables):

```c++
sym::CompiletimeConstant<int, 17> seventeen;
```

### Runtime Constants

A runtime constant is similar to a compile time constant but the value is not fixed at compile time:

```c++
sym::RuntimeConstant<int> seventeen{17};
```

## Operations

Operations are expressions that combine one or multiple expressions to form another expression. They are represented as
C++ class templates that are parametrized with its children, for example the addition operation is defined as:

```c++
template<sym::Expression Lhs, sym::Expression Rhs>
class Add {
        ...
};
```

### Operators

To simplify the definition of expressions the corresponding operators for the declared operations are overwritten:

```c++
sym::Variable<'a'> a;
sym::Variable<'b'> b;

Add<decltype(a), decltype(b)> sum{a, b}; // Not very readable
Add sum{a, b}; // Better readable thanks to class template argument deduction
auto sum = a + b; // Even better readable, looks like normal C++
```

### Functions

In addition to the basic operations most of the relevant mathematical functions are supported as well:

```c++
sym::Variable<'a'> a;
auto sqrt_a = std::sqrt(a);
auto pow_a_a = std::pow(a, a);
```

## Evaluating expressions

Now that expressions can be constructed they can be evaluated. To do this all variables need to be bound to a value,
this defines both their type and value. The type is determined using normal C++ broadcasting and conversion rules. The
bind a variable you can assign a value to a variable, this does not directly influence the variable but return
a `Binding` object which can then be used to resolve an expression:

```c++
sym::Variable<'a'> a;
sym::Variable<'b'> b;

auto sum = a + b;

auto aBinding = a=17; // A binding can be explicitly specified
sum.resolve(aBinding, b=15); // or directly given to resolve

auto bindings = std::make_tuple(a=3, b=17); // Bindings can be collected in tuples which can be used like normal bindings in resolve (and mixed with normal bindings)
sum.resolve(bindings);
```

the compilation will fail if an expression is resolved without providing all necessary variables.

## Symbolic differentiation

The representation of expressions as compile time expression trees allows for easy implementation of symbolic
differentiation using the forward mode of automatic differentiation. For this a function
`gradient` exists which takes an expression as first argument and a variable as second argument and calculates the
derivative of the expression with respect to the variable:

```c++
sym::Variable<'a'> a;
sym::Variable<'b'> b;

auto prod = a * b;

auto da_prod = sym::gradient(prod, a); // d/da (a*b) = b
auto db_prod = sym::gradient(prod, b); // d/db (a*b) = a
```

## Simplification

Especially when using the symbolic differentiation the resulting expression trees can be very large and can be
containing many parts which are 0. To remove these parts the tree can be simplified:

```c++
sym::Variable<'a'> a;
sym::Variable<'b'> b;

auto prod = a * b;

auto da_prod = sym::gradient(prod, a); // The actual expression would be 1 * b + 0 * a
auto da_prod_simplified = sym::simpifier::simplify(da_prod); // The simplified expression is now only b
```

As simplification is done during compile time no overhead will incur on runtime, thus it is advisable to simplify the
expression most of the time. One important drawback of the simplification is the fact that compile times increase
massively. Thus there is an CMake-Option `SIMPLIFY_GRADIENTS` which defines if the gradients are directly simplified during calculation
or if exlicit simplification is required. The default value is `ON`.

## Vectors

A vector is a sorted collection of expressions:

```c++
sym::Variable<'a'> a;
sym::Variable<'b'> b;

sym::Vector vec{a, b, a+b, a*b};
```

### Resolving vectors

Like all other expressions vectors can be evaluate using `.resolve(...)` with all required variables. As the types of
the individual components are not required to be the same the resulting type is a tuple consisting of the individual
resolved components with their respective type:

```c++
sym::Variable<'a'> a;
sym::Variable<'b'> b;

sym::Vector vec{a, b};
auto res = vec.resolve(a=1, b=0.5); // res is a tuple<int, double> containing {1, 0.5}
```

To simplify the interopability with other code there are two special resolve like functions for vectors, `.resolveAs<T>`
and `.resolveTo`. The first variant constructs a `T` with the resolved values as constructor arguments, the second
variant fills the first argument with the values returned as constructor arguments:

```c++
sym::Variable<'a'> a;
sym::Variable<'b'> b;
sym::Vector vec{a, b};

auto stdVec = vec.resolveAs<std::vector<double>>(a=1, b=0.5);

std::vector<double> stdVec;
auto stdVec = vec.resolveTo(stdVec, a=1, b=0.5);
```

For vectors which consist of other vectors the CTor arguments for `resolveTo` and `resolveAs` are the column major
flattened values.

### Binding vectors

Similar to the two special resolve variants there is also a special bind function to bind a vector of variables to any
container with `[]`-access:

```c++
sym::Variable<'a'> a;
sym::Variable<'b'> b;
sym::Vector vec{a, b};

std::vector<double> vals{1, 0.5};
vec.resolve(sym::bindVectorFromContainer(vec, vals));
```

### Symbolic differentiation with vectors

The gradient of vectors can be calculated identitcal to all other gradients, the resulting expression is a vector:

```c++
sym::Variable<'a'> a;
sym::Variable<'b'> b;
sym::Vector vec{a, b};

sym::gradient(vec, a);
```

instead of calculating the derivative with respect to a single variable the gradient can also be calculated with respect
to a vector:

```c++
sym::Variable<'a'> a;
sym::Variable<'b'> b;
sym::Vector vec{a, b};

sym::gradient(a, vec);
```

the resulting type is a vector, or in case the first argument is a vector, a vector of vectors.

## Extending Sym++

### Expression Tree Traversal

### Custom Operations
