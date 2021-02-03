#ifndef SORBET_HASHING_H
#define SORBET_HASHING_H

#include "core/Names.h"
#include "xxhash.h"

namespace sorbet::core {
static constexpr unsigned int HASH_MULT = 65599; // sdbm
static constexpr unsigned int HASH_MULT2 = 31;   // for names

inline unsigned int mix(unsigned int acc, unsigned int nw) {
    return nw + (acc << 6) + (acc << 16) - acc; // HASH_MULT in faster version
}

inline unsigned int _hash(std::string_view utf8) {
    // TODO: replace with http://www.sanmayce.com/Fastest_Hash/, see https://www.strchr.com/hash_functions
    // and https://github.com/rurban/smhasher
    auto end = utf8.end();
    unsigned int res = 0;
    auto it = utf8.begin();
#ifndef HAS_SANITIZER
#pragma clang loop unroll(enable)
#endif
    for (; it != end; it++) {
        res = mix(res, *it - '!'); // "!" is the first printable letter in ASCII.
        // This will help Latin1 but may harm utf8 multibyte
    }
    return res * HASH_MULT2 + static_cast<u4>(NameKind::UTF8);
}
} // namespace sorbet::core
#endif // SORBET_HASHING_H
