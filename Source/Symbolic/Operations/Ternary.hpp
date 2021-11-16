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
        constexpr auto resolve(Bindings... bindings) const {
            return cond.resolve(bindings...) ? trueVal.resolve(bindings...) : falseVal.resolve(bindings...);
        }

        template<Expression Cond_, Expression TrueVal_, Expression FalseVal_, std::size_t ID>
        friend constexpr auto gradient(const Ternary<Cond_, TrueVal_, FalseVal_> &x, const Variable<ID> &d);

        template<Expression Cond_, Expression TrueVal_, Expression FalseVal_>
        friend auto toString(const Ternary<Cond_, TrueVal_, FalseVal_> &x) -> std::string;

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
} // namespace sym

#endif // SYM_TERNARY_HPP
