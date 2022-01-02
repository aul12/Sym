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

    constexpr char nibble_to_char(uint8_t n) {
        n &= 0b1111;
        switch (n) {
            case 0:
                return '0';
            case 1:
                return '1';
            case 2:
                return '2';
            case 3:
                return '3';
            case 4:
                return '4';
            case 5:
                return '5';
            case 6:
                return '6';
            case 7:
                return '7';
            case 8:
                return '8';
            case 9:
                return '9';
            case 0xa:
                return 'a';
            case 0xb:
                return 'b';
            case 0xc:
                return 'c';
            case 0xd:
                return 'd';
            case 0xe:
                return 'e';
            case 0xf:
                return 'f';
            default:
                return 'x';
        }
    }

    template<std::size_t N>
    struct fixed_string {
        static constexpr std::size_t _N = N;
        constexpr fixed_string(const char (&str)[N]) {
            copy_str<char, const char *, char *>(static_cast<const char *>(str), str + N, data);
        }

        constexpr fixed_string(char c) requires(N == 2) {
            data[0] = c;
            data[1] = 0;
        }

        constexpr fixed_string(std::size_t s) requires(N == sizeof(std::size_t) * 2 + 1) {
            for (std::size_t i = 0; i < N - 1; i++) {
                data[N - i - 2] = nibble_to_char(s >> (4 * i));
            }
            data[N - 1] = 0;
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

    // deduction guide for single-char ctor: use single-char + null-byte
    fixed_string(char c) -> fixed_string<2>;

    // deduction guide for std::size_t ctor: format number as hex + null-byte
    fixed_string(std::size_t s) -> fixed_string<sizeof(std::size_t) * 2 + 1>;

} // namespace sym
#endif // SYM_FIXEDSTRING_HPP
