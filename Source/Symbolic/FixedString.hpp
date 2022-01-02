/**
 * @file FixedString.hpp
 * @author ottojo
 * @date 1/2/22
 * Description here TODO
 */

#ifndef SYM_FIXEDSTRING_HPP
#define SYM_FIXEDSTRING_HPP

#include <cstdint>

namespace sym {

    template<typename Char, typename InputIt, typename OutputIt>
    constexpr auto copy_str(InputIt begin, InputIt end, OutputIt out) -> OutputIt {
        while (begin != end) {
            *out++ = static_cast<Char>(*begin++);
        }
        return out;
    }

    template<std::size_t N>
    struct fixed_string {
        constexpr fixed_string(const char (&str)[N]) {
            copy_str<char, const char *, char *>(static_cast<const char *>(str), str + N, data);
        }

        constexpr fixed_string(char c) requires(N == 1) {
            data[0] = c;
        }

        constexpr bool operator==(const fixed_string<N> &rhs) const {
            for (std::size_t i = 0; i < N; i++) {
                if (data[i] != rhs.data[i]) {
                    return false;
                }
            }
            return true;
        }

        template<std::size_t M>
        requires(M != N) constexpr bool operator==(const fixed_string<M> &) const {
            return false;
        }

        char data[N]{};
    };

    // deduction guide for single-char ctor
    fixed_string(char c) -> fixed_string<1>;

} // namespace sym
#endif // SYM_FIXEDSTRING_HPP
