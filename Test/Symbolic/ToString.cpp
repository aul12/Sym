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
    template<Expression E, fixed_string EN>
    class WithName {
      public:
        explicit WithName(E exp) : e{exp} {};

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const {
            return e.resolve(bindings...);
        }

        template<typename... Bindings>
        constexpr auto resolve_named(const Bindings &...bindings) const {
            return std::make_tuple("" + toString(*this) + "<" + std::to_string(e.resolve(bindings...)) + ">",
                                   e.resolve(bindings...));
        }

        template<Expression E_, fixed_string ID_, fixed_string ID>
        friend auto constexpr gradient(const WithName<E_, ID_> &x, const Variable<ID> &d);

        template<Expression E_, fixed_string ID_>
        constexpr friend auto getChildren(const WithName<E_, ID_> &x) -> std::tuple<E_>;

      private:
        [[no_unique_address]] E e;
    };

    template<Expression E_, fixed_string ID_, fixed_string ID>
    auto constexpr gradient(const WithName<E_, ID_> &x, const Variable<ID> &d) {
        return gradient(x.e, d);
    }

    template<Expression E_, fixed_string ID_>
    constexpr auto getChildren(const WithName<E_, ID_> &x) -> std::tuple<E_> {
        return std::make_tuple(x.e);
    }

    template<fixed_string ID>
    struct withName {
        template<Expression E>
        static constexpr auto make(E exp) {
            return WithName<E, ID>{exp};
        }
    };

    template<fixed_string ID, Expression E>
    auto toString(const WithName<E, ID> & /*x*/) -> std::string {
        return std::string{ID.data};
    }
} // namespace sym

auto sum_function(auto a, auto b) {
    return a + b;
}

auto sum_named(auto a, auto b) {
    return sym::withName<"(sum term)">::make(a + b);
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
    EXPECT_EQ(toString(sn), "(sum term)");

    auto three_times_sn = 3 * sn;
    EXPECT_EQ(toString(three_times_sn), "(3 * (sum term))");

    auto [description, value] = sn.resolve_named(a = 1, b = 2);
    EXPECT_EQ(description, "(sum term)<3>");

    auto [description_s, value_s] = sum.resolve_named(a = 1, b = 2);
    EXPECT_EQ(description_s, "(a<1> + b<2>)<3>");

    auto [description_complex, value_comples] = (a + sum_named(sum, b)).resolve_named(a = 1, b = 2);
    EXPECT_EQ(description_complex, "(a<1> + b<2>)<3>");
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
