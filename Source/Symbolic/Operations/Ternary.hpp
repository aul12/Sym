/**
 * @file Ternary.hpp
 * @author paul
 * @date 16.11.21
 * Description here TODO
 */
#ifndef SYM_TERNARY_HPP
#define SYM_TERNARY_HPP

#include "../Expression.hpp"
#include "../Variable.hpp"

namespace sym {
    template<Expression Cond, Expression TrueVal, Expression FalseVal>
    class Ternary {
      public:
        Ternary(Cond cond, TrueVal trueVal, FalseVal falseVal) : cond{cond}, trueVal{trueVal}, falseVal{falseVal} {
        }

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const {
            return cond.resolve(bindings...) ? trueVal.resolve(bindings...) : falseVal.resolve(bindings...);
        }

        template<Expression Cond_, Expression TrueVal_, Expression FalseVal_, std::size_t ID>
        friend constexpr auto gradient(const Ternary<Cond_, TrueVal_, FalseVal_> &x, const Variable<ID> &d);

        template<Expression Cond_, Expression TrueVal_, Expression FalseVal_>
        friend auto toString(const Ternary<Cond_, TrueVal_, FalseVal_> &x) -> std::string;

        template<Expression Cond_, Expression Lhs_, Expression Rhs_>
        constexpr friend auto getChildren(const Ternary<Cond_, Lhs_, Rhs_> &x) -> std::tuple<Cond_, Lhs_, Rhs_>;
      private:
        Cond cond;
        TrueVal trueVal;
        FalseVal falseVal;
    };

    template<Expression Cond_, Expression TrueVal_, Expression FalseVal_, std::size_t ID>
    constexpr auto gradient(const Ternary<Cond_, TrueVal_, FalseVal_> &x, const Variable<ID> &d) {
        return Ternary{x.cond, gradient(x.trueVal, d), gradient(x.falseVal, d)};
    }

    template<Expression Cond_, Expression TrueVal_, Expression FalseVal_>
    auto toString(const Ternary<Cond_, TrueVal_, FalseVal_> &x) -> std::string {
        return "(" + sym::toString(x.cond) + ") ? (" + sym::toString(x.trueVal) + ") : (" + sym::toString(x.falseVal) +
               ")";
    }
    template<typename Cond_, typename Lhs_, typename Rhs_>
    constexpr auto getChildren(const Ternary<Cond_, Lhs_, Rhs_> &x) -> std::tuple<Cond_, Lhs_, Rhs_> {
        return std::make_tuple(x.cond, x.trueVal, x.falseVal);
    }
} // namespace sym

#endif // SYM_TERNARY_HPP
