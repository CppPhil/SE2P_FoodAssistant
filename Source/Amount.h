#pragma once
#include "GlobalHeader.h" // fa::OStream
#include <boost/operators.hpp> // boost::totally_ordered

namespace fa {
    namespace detail {
        using AmountValueType = long double;
    } // END of namespace detail
    
    class Amount;
    
    namespace literals {
        Amount operator""_kg(detail::AmountValueType val) noexcept; // kilograms
        Amount operator""_g(detail::AmountValueType val) noexcept; // grams
        Amount operator""_milg(detail::AmountValueType val) noexcept; // milligrams
        Amount operator""_micg(detail::AmountValueType val) noexcept; // micrograms
    } // END of namespace literals

    //! this class is used to represent weights
    class Amount final : private boost::totally_ordered<Amount> {
    public:
        using this_type = Amount;
        using value_type = detail::AmountValueType;
        Amount() noexcept;

        //! Constructs an Amount from a long double, that must represent a value in grams. Only to be used by the database
        explicit Amount(value_type) noexcept;
        //! Returns the amount in grams. Only to be used by the database
        value_type getInternalRepresentation() const noexcept;

        String valueAsString() const;

        this_type &operator+=(this_type) noexcept;
        this_type &operator-=(this_type) noexcept;
        this_type &operator*=(value_type) noexcept;
        this_type &operator/=(value_type);

        friend bool operator==(Amount, Amount) noexcept;
        friend bool operator<(Amount, Amount) noexcept;

        friend Amount operator+(Amount, Amount) noexcept;
        friend Amount operator-(Amount, Amount) noexcept;
        friend Amount operator*(Amount, value_type) noexcept;
        friend Amount operator/(Amount, value_type);

        friend OStream &operator<<(OStream &, Amount) noexcept;

        friend Amount literals::operator""_kg(value_type val) noexcept; // kilograms
        friend Amount literals::operator""_g(value_type val) noexcept; // grams
        friend Amount literals::operator""_milg(value_type val) noexcept; // milligrams
        friend Amount literals::operator""_micg(value_type val) noexcept; // micrograms

    private:
        value_type value_;
    }; // END of class Amount   
} // END of namespace fa
