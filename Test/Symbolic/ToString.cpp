#include <gtest/gtest.h>

#include "Symbolic/Operators.hpp"
#include "Symbolic/Variable.hpp"

TEST(ToString, String) {
    sym::Variable<"acceleration"> v;
    EXPECT_EQ(toString(v), "acceleration");
}

TEST(ToString, Char) {
    sym::Variable<'a'> v;
    EXPECT_EQ(toString(v), "a");
}

namespace sym {
    constexpr auto tuple_first(auto tuple) {
        return std::get<0>(tuple);
    }

    constexpr auto tuple_rest(auto tuple) {
        if constexpr (std::tuple_size_v<decltype(tuple)> == 1) {
            return std::make_tuple();
        }
        return std::apply([](auto /*tuple_first*/, auto... tuple_rest) { return std::make_tuple(tuple_rest...); },
                          tuple);
    }


    template<typename... Bindings, Expression... Args>
    constexpr auto resolveNamedList(std::tuple<Args...> args, Bindings... bindings) -> std::string {
        if constexpr (sizeof...(Args) == 0) {
            return std::string{""};
        }
        if constexpr (sizeof...(Args) == 1) {
            return tuple_first(args).resolve_named(bindings...);
        }

        return tuple_first(args).resolve_named(bindings...) + ", " + resolveNamedList(tuple_rest(args), bindings...);
    }

    template<typename... Bindings>
    constexpr auto resolveNamedList(std::tuple<>, Bindings...) {
        return std::string{""};
    }


    template<Expression E, fixed_string EN, Expression... Args>
    class WithName {
      public:
        // explicit WithName(E exp) : e{exp} {};
        WithName(E exp, Args... a) : e{exp}, args{std::make_tuple(a...)} {};

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const {
            return e.resolve(bindings...);
        }

        template<typename... Bindings>
        constexpr std::string resolve_named(const Bindings &...bindings) const {
            if constexpr (sizeof...(Args) == 0) {
                return toString(*this) + "<" + std::to_string(this->resolve(bindings...)) + ">";
            } else {
                return std::string{EN.data} + "(" + resolveNamedList(args, bindings...) + ")" + "<" +
                       std::to_string(this->resolve(bindings...)) + ">";
            }
        }

        template<Expression E_, fixed_string ID_, Expression... Args_, fixed_string ID>
        friend auto constexpr gradient(const WithName<E_, ID_, Args_...> &x, const Variable<ID> &d);

        template<Expression E_, fixed_string ID_, Expression... Args_>
        constexpr friend auto getChildren(const WithName<E_, ID_, Args_...> &x) -> std::tuple<E_>;

        template<fixed_string ID, Expression E_, Expression... Args_>
        constexpr friend auto toString(const WithName<E_, ID, Args_...> &x) -> std::string;

      private:
        [[no_unique_address]] E e;
        std::tuple<Args...> args;
    };

    template<Expression E_, fixed_string ID_, Expression... Args_, fixed_string ID>
    auto constexpr gradient(const WithName<E_, ID_, Args_...> &x, const Variable<ID> &d) {
        return gradient(x.e, d);
    }

    template<Expression E_, fixed_string ID_, Expression... Args>
    constexpr auto getChildren(const WithName<E_, ID_, Args...> &x) -> std::tuple<E_> {
        return std::make_tuple(x.e);
    }

    template<fixed_string ID>
    struct withName {
        template<Expression E>
        static constexpr auto make(E exp) {
            return WithName<E, ID>{exp};
        }

        template<Expression E, Expression... Args>
        static constexpr auto make_with_args(E exp, Args... args) {
            return WithName<E, ID, Args...>{exp, args...};
        }
    };


    template<Expression... Args>
    constexpr auto toStringList(std::tuple<Args...> args) {
        if constexpr (sizeof...(Args) == 0) {
            return std::string{""};
        }
        if constexpr (sizeof...(Args) == 1) {
            return toString(std::get<0>(args));
        }

        return toString(tuple_first(args)) + ", " + toStringList(tuple_rest(args));
    }


    template<>
    constexpr auto toStringList(std::tuple<>) {
        return std::string{""};
    }

    template<fixed_string ID, Expression E, Expression... Args>
    constexpr auto toString(const WithName<E, ID, Args...> &x) -> std::string {
        if constexpr (sizeof...(Args) == 0) {
            return std::string{ID.data};
        } else {
            return std::string{ID.data} + "(" + toStringList(x.args) + ")";
        }
        return "";
    }
} // namespace sym

auto sum_function(auto a, auto b) {
    return a + b;
}

auto sum_named(auto a, auto b) {
    return sym::withName<"(sum term)">::make_with_args(a + b, a, b);
}

TEST(ToString, Expression) {
    SYM_VARIABLE(a);
    SYM_VARIABLE(b);
    auto sum = a + b;
    EXPECT_EQ(toString(sum), "(a + b)");

    auto product = a * b;
    EXPECT_EQ(toString(product), "(a * b)");

    auto productMinusSum = product - sum;
    EXPECT_EQ(toString(productMinusSum), "((a * b) - (a + b))");

    auto sum_fn = sum_function(a, b);
    EXPECT_EQ(toString(sum_fn), "(a + b)");

    auto sn = sum_named(a, b);
    EXPECT_EQ(toString(sn), "(sum term)(a, b)");

    auto three_times_sn = sym::CompiletimeConstant<int, 3>{} * sn;
    EXPECT_EQ(toString(three_times_sn), "(3 * (sum term)(a, b))");

    auto description = sn.resolve_named(a = 1, b = 2);
    EXPECT_EQ(description, "(sum term)(a<1>, b<2>)<3>");

    auto description_s = sum.resolve_named(a = 1, b = 2);
    EXPECT_EQ(description_s, "(a<1> + b<2>)<3>");

    auto x = sum_named(sum, b);
    auto y = a + x;
    auto description_complex = y.resolve_named(a = 1, b = 2);
    EXPECT_EQ(description_complex, "(a<1> + (sum term)((a<1> + b<2>)<3>, b<2>)<5>)<6>");
}

TEST(ToString, Number) {
    sym::Variable<(std::size_t) 7> v;
    EXPECT_EQ(toString(v), "0000000000000007");

    sym::Variable<(std::size_t) 0xabcdef0123456789> v2;
    EXPECT_EQ(toString(v2), "abcdef0123456789");
}

TEST(ToString, IdentifierName) {
    SYM_VARIABLE(v);
    EXPECT_EQ(toString(v), "v");

    SYM_VARIABLE(longName);
    EXPECT_EQ(toString(longName), "longName");
}

TEST(ToString, AutomaticName) {
    SYM_A_VARIABLE v;
    EXPECT_TRUE(toString(v).ends_with("Test/Symbolic/ToString.cpp:" + std::to_string(__LINE__ - 1)));

    SYM_A_VARIABLE longName;
    EXPECT_TRUE(toString(longName).ends_with("Test/Symbolic/ToString.cpp:" + std::to_string(__LINE__ - 1)));

    EXPECT_NE(toString(v), toString(longName));
    EXPECT_FALSE((std::is_same_v<decltype(v), decltype(longName)>) );
}
