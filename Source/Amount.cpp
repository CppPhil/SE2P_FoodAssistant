#include "Amount.h"
#include "Utils.h" // fa::DivideByZeroException

namespace fa {
    Amount::Amount() noexcept
        : value_{ } { }

    Amount::Amount(value_type grams) noexcept
        : value_{ grams } { }

    Amount::value_type Amount::getInternalRepresentation() const noexcept {
        return value_;
    }

    // TODO: test this function.
    String Amount::valueAsString() const {
        static auto constexpr one = 1.0L;
        static auto constexpr factorDivisor = 1000.0L;

        static auto const spaceTxt = STR(" ");
        static auto const kgTxt = STR("kg");
        static auto const gTxt = STR("g");
        static auto const milgTxt = STR("milg");
        static auto const micgTxt = STR("micg");

        auto val = getInternalRepresentation();

        OStringStream oStringStream{ };
        oStringStream << std::fixed << std::setprecision(static_cast<std::streamsize>(6));

        auto pTxt = &gTxt;

        if (val > one * factorDivisor) {
            // kg
            val /= factorDivisor;
            pTxt = &kgTxt;
        } else if (val < one / factorDivisor / factorDivisor) {
            // micg
            val *= factorDivisor;
            val *= factorDivisor;
            pTxt = &micgTxt;
        } else if (val < one / factorDivisor) {
            // milg
            val *= factorDivisor;
            pTxt = &milgTxt;
        } else {
            // g
            val *= one;
            pTxt = &gTxt;
        }

        oStringStream << val;

        if (!oStringStream) {
            throw std::runtime_error{ 
                std::string{ "error in oStringStream in " }
                + __func__
                + std::string{ " in file " }
                + __FILE__ };
        }
        
        auto text = oStringStream.str() + spaceTxt + *pTxt;
        return text;
    }
    
    Amount::this_type &Amount::operator+=(this_type other) noexcept {
        *this = *this + other;
        return *this;
    }

    Amount::this_type &Amount::operator-=(this_type other) noexcept {
        *this = *this - other;
        return *this;
    }

    Amount::this_type &Amount::operator*=(value_type factor) noexcept {
        *this = *this * factor;
        return *this;
    }

    Amount::this_type &Amount::operator/=(value_type divisor) {
        *this = *this / divisor;
        return *this;
    }

    bool operator==(Amount lhs, Amount rhs) noexcept {
        return utils::floating_point::almostEqual( // doesn't support long double
            static_cast<double>(lhs.value_),
            static_cast<double>(rhs.value_));
    }

    bool operator<(Amount lhs, Amount rhs) noexcept {
        return lhs.value_ < rhs.value_;
    }

    Amount operator+(Amount lhs, Amount rhs) noexcept {
        return Amount{ lhs.value_ + rhs.value_ };
    }

    Amount operator-(Amount lhs, Amount rhs) noexcept {
        return Amount{ lhs.value_ - rhs.value_ };
    }

    Amount operator*(Amount amt, Amount::value_type factor) noexcept {
        return Amount{ amt.value_ * factor };
    }

    Amount operator/(Amount amt, Amount::value_type divisor) {
        if (divisor == 0.0L) {
            throw DivideByZeroException{ "attempted to divide by zero"
                                         " in Amount operator/(Amount amt, Amount::value_type divisor)"
                                         " in file Amount.cpp" };
        }
        return Amount{ amt.value_ / divisor };
    }

    OStream &operator<<(OStream &ostr, Amount amt) noexcept {
        return ostr << amt.valueAsString();
    }

    namespace literals {
        Amount operator""_kg(Amount::value_type val) noexcept { // kilograms
            static auto constexpr multiplier = 1'000.0L;
            return Amount{ val * multiplier };
        }

        Amount operator""_g(Amount::value_type val) noexcept { // grams
            return Amount{ val };
        }

        Amount operator""_milg(Amount::value_type val) noexcept { // milligrams
            static auto constexpr divisor = 1'000.0L;
            return Amount{ val / divisor };
        }

        Amount operator""_micg(Amount::value_type val) noexcept { // micrograms
            static auto constexpr divisor = 1'000'000.0L;
            return Amount{ val / divisor };
        }
    } // END of namespace literals
} // END of namespace fa
