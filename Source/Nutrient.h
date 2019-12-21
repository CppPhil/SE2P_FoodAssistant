#pragma once
#include "GlobalHeader.h" // fa::OStream
#include <utility> // std::move
#include "NutrientImpl.h"

namespace fa {    
    template <class Target>
    class NutrientStaticDelegator final {
    public:
        using this_type = NutrientStaticDelegator;
        using value_type = Target;

        template <class ...Args>
        explicit NutrientStaticDelegator(Args &&...args) noexcept 
            : val_{ std::forward<Args>(args)... } { }

        auto getNutrientType() const noexcept -> decltype(auto) {
            return val_.getNutrientType();
        }

        auto getName() const noexcept -> decltype(auto) {
            return val_.getName();
        }

        auto getDescription() const noexcept -> decltype(auto) {
            return val_.getDescription();
        }

        auto getOverdoseDescription() const noexcept -> decltype(auto) {
            return val_.getOverdoseDescription();
        }

        auto getMinimumDose() const noexcept -> decltype(auto) {
            return val_.getMinimumDose();
        }

        auto getMaximumDose() const noexcept -> decltype(auto) {
            return val_.getMaximumDose();
        }

        auto getOptimumDose() const noexcept -> decltype(auto) {
            return val_.getOptimumDose();
        }

        auto getStorage() const noexcept -> decltype(auto) {
            return val_.getStorage();
        }

        auto isMicro() const noexcept -> decltype(auto) {
            return val_.isMicro();
        }

        auto isMacro() const noexcept -> decltype(auto) {
            return val_.isMacro();
        }

        friend OStream &operator<<(OStream &os, this_type const &obj) {
            return os << obj.val_;
        }

    private:
        value_type val_;
    }; // END of class NutrientStaticDelegator

    using Nutrient = NutrientStaticDelegator<NutrientImpl>;

} // END of namespace fa
