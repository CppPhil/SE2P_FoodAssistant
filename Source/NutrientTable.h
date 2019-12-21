#pragma once
#include "GlobalHeader.h" // fa::OStream
#include <utility> // std::move
#include "Nutrient.h" // fa::NutrientType
#include "NutrientTableImpl.h" // fa::NutrientTableImpl

namespace fa {
    template <class Target>
    class NutrientTableStaticDelegator final {
    public:
        using this_type = NutrientTableStaticDelegator;
        using value_type = Target;

        NutrientTableStaticDelegator() noexcept
            : val_{ } { }

        //! Do not put more than one of each NutrientType in here!
        template <class ...Args>
        explicit NutrientTableStaticDelegator(Args &&...args) noexcept 
            : val_{ std::forward<Args>(args)... } { }

        friend this_type operator+(this_type const &lhs, this_type const &rhs) {
            return this_type{ lhs.val_ + rhs.val_ };
        }

        friend this_type operator-(this_type const &lhs, this_type const &rhs) {
            return this_type{ lhs.val_ - rhs.val_ };
        }

        friend this_type operator*(this_type const &nutrientTable, Amount::value_type amt) {
            return this_type{ nutrientTable.val_ * amt };
        }

        friend this_type operator/(this_type const &nutrientTable, Amount::value_type amt) {
            return this_type{ nutrientTable.val_ / amt };
        }

        this_type getMacroNutrients() const {
            return this_type{ val_.getMacroNutrients() };
        }

        this_type getMicroNutrients() const {
            return this_type{ val_.getMicroNutrients() };
        }

        auto getNutrient(NutrientType nutrientType) const -> decltype(auto) {
            return val_.getNutrient(nutrientType);
        }

        auto getPercentages() const -> decltype(auto) {
            return val_.getPercentages();
        }

        friend OStream &operator<<(OStream &os, this_type const &obj) {
            return os << obj.val_;
        }

        friend bool operator==(this_type const &lhs, this_type const &rhs) noexcept {
            return lhs.val_ == rhs.val_;
        }

        friend bool operator!=(this_type const &lhs, this_type const &rhs) noexcept {
            return !(lhs == rhs);
        }

    private:
        value_type val_;
    }; // END of class NutrientTableStaticDelegator

    using NutrientTable = NutrientTableStaticDelegator<NutrientTableImpl>;

} // END of namespace fa
