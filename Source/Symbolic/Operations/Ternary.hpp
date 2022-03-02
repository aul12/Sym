/**
 * @file Ternary.hpp
 * @author paul
 * @date 16.11.21
 * Description here TODO
 */
#ifndef SYM_TERNARY_HPP
#define SYM_TERNARY_HPP

#include "../Expression.hpp"
#include "../Simplifier/GradientSimplifcation.hpp"
#include "../Variable.hpp"

namespace sym {
    template<Expression Cond, Expression TrueVal, Expression FalseVal>
    class Ternary {
      public:
        constexpr Ternary(Cond cond, TrueVal trueVal, FalseVal falseVal) :
            cond{cond},
            trueVal{trueVal},
            falseVal{falseVal} {
        }

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const {
            return cond.resolve(bindings...) ? trueVal.resolve(bindings...) : falseVal.resolve(bindings...);
        }

        template<Expression Cond_, Expression TrueVal_, Expression FalseVal_, fixed_string ID>
        friend constexpr auto gradient(const Ternary<Cond_, TrueVal_, FalseVal_> &x, const Variable<ID> &d);

        template<Expression Cond_, Expression TrueVal_, Expression FalseVal_>
        friend auto toString(const Ternary<Cond_, TrueVal_, FalseVal_> &x) -> std::string;

        template<Expression Cond_, Expression Lhs_, Expression Rhs_>
        constexpr friend auto getChildren(const Ternary<Cond_, Lhs_, Rhs_> &x) -> std::tuple<Cond_, Lhs_, Rhs_>;

      private:
        [[no_unique_address]] Cond cond;
        [[no_unique_address]] TrueVal trueVal;
        [[no_unique_address]] FalseVal falseVal;
    };

    template<Expression Cond_, Expression TrueVal_, Expression FalseVal_, fixed_string ID>
    constexpr auto gradient(const Ternary<Cond_, TrueVal_, FalseVal_> &x, const Variable<ID> &d) {
        return _GRADIENT_SIMPLIFY(Ternary{x.cond, gradient(x.trueVal, d), gradient(x.falseVal, d)});
    }

    template<Expression Cond_, Expression TrueVal_, Expression FalseVal_>
    auto toString(const Ternary<Cond_, TrueVal_, FalseVal_> &x) -> std::string {
        return "((" + toString(x.cond) + ") ? (" + toString(x.trueVal) + ") : (" + toString(x.falseVal) + "))";
    }
    template<Expression Cond_, Expression Lhs_, Expression Rhs_>
    constexpr auto getChildren(const Ternary<Cond_, Lhs_, Rhs_> &x) -> std::tuple<Cond_, Lhs_, Rhs_> {
        return std::make_tuple(x.cond, x.trueVal, x.falseVal);
    }
} // namespace sym

#include "../Simplifier/CompileTime.hpp"

#endif // SYM_TERNARY_HPP
