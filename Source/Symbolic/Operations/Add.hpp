/**
 * @file Add.hpp
 * @author paul
 * @date 25.08.20
 * Description here TODO
 */
#ifndef SYM_ADD_HPP
#define SYM_ADD_HPP

#include "../Expression.hpp"
#include "../Simplifier/GradientSimplifcation.hpp"
#include "../Variable.hpp"

namespace sym {
    template<Expression Lhs, Expression Rhs>
    class Add {
      public:
        constexpr Add(Lhs lhs, Rhs rhs);

        template<typename... Bindings>
        constexpr auto resolve(const Bindings &...bindings) const;

        template<Expression Lhs_, Expression Rhs_, fixed_string ID>
        friend constexpr auto gradient(const Add<Lhs_, Rhs_> &x, const Variable<ID> &d);

        template<Expression Lhs_, Expression Rhs_>
        friend auto toString(const Add<Lhs_, Rhs_> &x) -> std::string;

        template<Expression Lhs_, Expression Rhs_>
        constexpr friend auto getChildren(const Add<Lhs_, Rhs_> &x) -> std::tuple<Lhs_, Rhs_>;

      private:
        [[no_unique_address]] Lhs lhs;
        [[no_unique_address]] Rhs rhs;
    };

    template<Expression Lhs, Expression Rhs>
    constexpr Add<Lhs, Rhs>::Add(Lhs lhs, Rhs rhs) : lhs{lhs}, rhs{rhs} {
    }

    template<Expression Lhs, Expression Rhs>
    template<typename... Bindings>
    constexpr auto Add<Lhs, Rhs>::resolve(const Bindings &...bindings) const {
        return lhs.resolve(bindings...) + rhs.resolve(bindings...);
    }

    template<Expression Lhs_, Expression Rhs_, fixed_string ID>
    constexpr auto gradient(const Add<Lhs_, Rhs_> &x, const Variable<ID> &d) {
        return _GRADIENT_SIMPLIFY(Add{gradient(x.lhs, d), gradient(x.rhs, d)});
    }

    template<Expression Lhs_, Expression Rhs_>
    auto toString(const Add<Lhs_, Rhs_> &x) -> std::string {
        return "(" + toString(x.lhs) + ")+(" + toString(x.rhs) + ")";
    }

    template<Expression Lhs_, Expression Rhs_>
    constexpr auto getChildren(const Add<Lhs_, Rhs_> &x) -> std::tuple<Lhs_, Rhs_> {
        return std::make_tuple(x.lhs, x.rhs);
    }
} // namespace sym

#include "../Simplifier/CompileTime.hpp"

#endif // SYM_ADD_HPP
