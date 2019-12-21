#pragma once
#include "GlobalHeader.h" // fa::OStream
#include "NutrientTable.h" // fa::NutrientTable
#include "Food.h" // fa::FoodDynamicDelegator, fa::FoodInterface
#include <utility> // std::forward
#include <brigand/brigand.hpp> // brigand::set, brigand::contains
#include <mylibs/utility.hpp> // pl::remove_cv_ref_t
#include "Utils.h" // generateId

namespace fa {
    class AtomicFoodImpl final {
    public:
        using this_type = AtomicFoodImpl;

        template <class ...Args>
        explicit AtomicFoodImpl(String name, Args &&...args) noexcept
            : nutrientTable_{ std::forward<Args>(args)... },
              id_{ generateId<this_type>() },
              name_{ std::move(name) } { }

        friend OStream &operator<<(OStream &, this_type const &);
        NutrientTable getNutrientTable() const;
        Id getId() const;
        
        //! returns the name of this AtomicFoodImpl
        String getText() const;

        friend bool operator==(this_type const &, this_type const &);
        friend bool operator!=(this_type const &, this_type const &);

    private:
        NutrientTable nutrientTable_;
        Id id_;
        String name_;
    }; // END of class AtomicFoodImpl

    class CompoundFoodImpl final {
    public:
        using this_type = CompoundFoodImpl;
        using UniqueOwner = pl::PolymorphicUniqueOwner<FoodInterface, FoodDynamicDelegator>;
        using container_type = std::vector<UniqueOwner>;

        template <class ...Args>
        explicit CompoundFoodImpl(Args &&...args)
            : cont_{ UniqueOwner{ args }...},
              id_{ generateId<this_type>() } { }

        friend OStream &operator<<(OStream &, this_type const &);
        NutrientTable getNutrientTable() const;
        Id getId() const;

        //! returns the name of this CompoundFoodImpl
        String getText() const;

        friend bool operator==(this_type const &, this_type const &);
        friend bool operator!=(this_type const &, this_type const &);

    private:
        container_type cont_;
        Id id_;
        String name_;
    }; // END of class CompoundFoodImpl

    class FoodWithAmount final {
    public:
        using this_type = FoodWithAmount;
        using UniqueOwner = pl::PolymorphicUniqueOwner<FoodInterface, FoodDynamicDelegator>;
        using AmountType = Amount;

        template <class Food>
        FoodWithAmount(Food &&food, AmountType count)
            : val_{ std::forward<Food>(food) }, count_{ count } {
            
            using TypeSet = brigand::set<AtomicFoodImpl, CompoundFoodImpl, UniqueOwner>;
            static_assert(brigand::contains<TypeSet, pl::remove_cv_ref_t<Food>>::value,
                          "Food in FoodWithAmount::FoodWithAmount(Food &&, AmountType) was neither "
                          "AtomicFoodImpl nor CompoundFoodImpl"
                          " nor pl::PolymorphicUniqueOwner<FoodInterface, FoodDynamicDelegator>");            
        }

        friend OStream &operator<<(OStream &, this_type const &);
        NutrientTable getNutrientTable() const;
        AmountType getCount() const;
        Id getId() const;

        //! returns the name of the Food that this FoodWithAmount has.
        String getText() const;

        friend bool operator==(this_type const &, this_type const &);
        friend bool operator!=(this_type const &, this_type const &);

    private:
        UniqueOwner val_;
        AmountType count_;
    }; // END of class FoodWithAmount

} // END of namespace fa
