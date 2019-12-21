#include "Utils.h"
#include <boost/algorithm/string.hpp> // boost::to_lower
#include <cctype> // std::toupper

namespace fa {
    DivideByZeroException::DivideByZeroException(std::string const &message)
        : std::logic_error{ message } { }

    DivideByZeroException::~DivideByZeroException() = default;

    char const *DivideByZeroException::what() const noexcept {
        return std::logic_error::what();
    }

    NotYetImplementedException::NotYetImplementedException(std::string const &message)
        : std::runtime_error{ message } { }

    NotYetImplementedException::~NotYetImplementedException() = default;

    char const *NotYetImplementedException::what() const noexcept {
        return std::runtime_error::what();
    }

    IdOverflowException::IdOverflowException(std::string const &message)
        : std::runtime_error{ message } { }

    IdOverflowException::~IdOverflowException() = default;

    char const *IdOverflowException::what() const noexcept {
        return std::runtime_error::what();
    }

    void wordify(String &str) {
        if (str.empty()) {
            return;
        }
        boost::to_lower(str);
        auto prev = '\0';        
        // always uppercase the first character
        str[0] = static_cast<String::value_type>(std::toupper(static_cast<int>(str[0])));
        for (auto &&e : str) {
            if (prev == ' ') {
                e = static_cast<String::value_type>(std::toupper(static_cast<int>(e)));
            }
            prev = e;
        }
    }

    String wordifyCopy(String str) {
        wordify(str);
        return str;
    }

} // END of namespace fa
