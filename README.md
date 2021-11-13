# Symbolic++

Framework for efficient symbolic calculations using C++ zero cost abstractions

Fancy stuff:
```c++
Variable<'a'> a;
Variable<'b'> b;

auto f = a*a + b - a * b;
std::cout << f.resolve(a=1, b=2) << std::endl;

auto da_f = gradient(f, a);
auto db_f = gradient(f, b);
std::cout << da_f << std::endl;
std::cout << db_f.resolve(a=3, b=2) << std::endl;
```
