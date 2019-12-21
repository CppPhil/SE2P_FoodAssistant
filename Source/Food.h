#pragma once
#include "GlobalHeader.h"
#include "NutrientTable.h" // fa::NutrientTable
#include <memory> // std::unique_ptr
#include <typeinfo> // typeid, std::bad_cast
#include <mylibs/container.hpp> // pl::is_random_access_iterator
#include "Utils.h" // fa::Id

namespace fa {
    class FoodInterface {
    public:
        using this_type = FoodInterface;
        using SmartPointer = std::unique_ptr<this_type>;

        virtual ~FoodInterface();
        virtual SmartPointer copy_() = 0;
        virtual OStream &printOn(OStream &) const = 0;
        virtual NutrientTable getNutrientTable() const = 0;
        virtual bool equals(FoodInterface const &) const = 0;
        virtual Id getId() const = 0;
        virtual String getText() const = 0;
    }; // END of class FoodInterface

    OStream &operator<<(OStream &, FoodInterface const &);
    bool operator==(FoodInterface const &, FoodInterface const &);
    bool operator!=(FoodInterface const &, FoodInterface const &);

    template <class Target>
    class FoodDynamicDelegator final : public FoodInterface {
    public:
        using this_type = FoodDynamicDelegator;
        using value_type = Target;

        explicit FoodDynamicDelegator(value_type val)
            : val_{ std::move(val) } { }

        virtual ~FoodDynamicDelegator() override = default;

        virtual SmartPointer copy_() override {
            return std::make_unique<this_type>(*this);
        }

        virtual OStream &printOn(OStream &ostr) const override {
            return ostr << val_;
        }

        virtual NutrientTable getNutrientTable() const override {
            return val_.getNutrientTable();
        }

        virtual bool equals(FoodInterface const &other) const override {
            if (typeid(this_type) != typeid(other)) { // if not the same type -> false
                return false;
            }
            try {
                auto &downcasted = dynamic_cast<this_type const &>(other);

                return val_ == downcasted.val_;

            } catch (std::bad_cast const &ex) {
                std::cerr << STR("std::bad_cast caught in "
                                "FoodDynamicDelegator::equals, "
                                "failed to downcast. Error msg: ")
                          << ex.what() << STR('\n');
                throw; // rethrow
            }
        }

        virtual Id getId() const override {
            return val_.getId();
        }

        virtual String getText() const override {
            return val_.getText();
        }

    private:
        value_type val_;
    }; // END of class FoodDynamicDelegator

    // although Prof. Boehm would be proud this may need to be changed if it is too inefficient.
    template <class RandomAccessIterator>
    NutrientTable sumOfNutrientTables(RandomAccessIterator begin, RandomAccessIterator end,
        NutrientTable accu) {
        static_assert(pl::is_random_access_iterator<RandomAccessIterator>::value, "RandomAccessIterator in sumOfNutrientTables function in Food.h was not a random access iterator.");

        if (begin == end) {
            return accu;
        }

        return sumOfNutrientTables(begin + 1, end, // beautiful recursion as NutrientTables are immutable ...
            accu + ((*begin)->getNutrientTable()));
    }

} // END of namespace fa
