#pragma once
#include <mylibs/detail/unicode.hpp> // GLUE
#include <cstddef> // std::size_t
#include "RandomNumberGenerator.h" // utils::rng

#ifdef STR
    static_assert(false, "STR was already defined in GlobalHeader.h");
#endif

#ifdef STR_PREFIX
    static_assert(false, "STR_PREFIX was already defined in GlobalHeader.h");
#endif

namespace fa {
    using Char = char;
    using String = std::basic_string<Char>;
    using IFStream = std::basic_ifstream<Char>;
    using OFStream = std::basic_ofstream<Char>;
    using FStream = std::basic_fstream<Char>;
    using FileBuf = std::basic_filebuf<Char>;
    using IOs = std::basic_ios<Char>;
    using IStream = std::basic_istream<Char>;
    using IOStream = std::basic_iostream<Char>;
    using IStringStream = std::basic_istringstream<Char>;
    using OStringStream = std::basic_ostringstream<Char>;
    using StringBuf = std::basic_stringbuf<Char>;
    using StreamBuf = std::basic_streambuf<Char>;
    using Regex = std::basic_regex<Char>;
    using size_type = std::size_t;
    using OStream = std::basic_ostream<Char>;
} // END of namespace fa

#define STR_PREFIX(stringLiteral)   stringLiteral /* GLUE(L, stringLiteral) */

#define STR(stringLiteral)  fa::String{ STR_PREFIX(stringLiteral) }
    