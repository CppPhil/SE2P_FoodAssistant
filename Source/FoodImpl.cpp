#include "FoodImpl.h"
#include <mylibs/container.hpp> // pl::equal

namespace fa {
    OStream &operator<<(OStream &ostr,
                        AtomicFoodImpl::this_type const &food) {
        return ostr <<
            utils::makeClassString(STR("AtomicFoodImpl"),
                                   STR("nutrientTable_"),
                                   food.nutrientTable_);
    }

    NutrientTable AtomicFoodImpl::getNutrientTable() const {
        return nutrientTable_;
    }

    Id AtomicFoodImpl::getId() const {
        return id_;
    }

    String AtomicFoodImpl::getText() const {
        return name_;
    }

    bool operator==(AtomicFoodImpl::this_type const &lhs,
                    AtomicFoodImpl::this_type const &rhs) {
        return lhs.nutrientTable_ == rhs.nutrientTable_;
    }

    bool operator!=(AtomicFoodImpl::this_type const &lhs,
                    AtomicFoodImpl::this_type const &rhs) {
        return !(lhs == rhs);
    }

    OStream &operator<<(OStream &os, CompoundFoodImpl::this_type const &food) {
        OStringStream oStringStream{ };
        for (auto &&e : food.cont_) {
            oStringStream << *e;
        }

        return os << 
            utils::makeClassString(STR("CompoundFoodImpl"),
                                   STR("cont_"), oStringStream.str());
    }

    NutrientTable CompoundFoodImpl::getNutrientTable() const {
        return sumOfNutrientTables(std::begin(cont_), std::end(cont_), { });
    }

    Id CompoundFoodImpl::getId() const {
        return id_;
    }

    String CompoundFoodImpl::getText() const {
        return name_;
    }

    bool operator==(CompoundFoodImpl::this_type const &lhs, CompoundFoodImpl::this_type const &rhs) {
        return pl::equal(lhs.cont_, rhs.cont_, [](auto &&arg1, auto &&arg2) {
            return arg1->equals(*arg2);
        });
    }

    bool operator!=(CompoundFoodImpl::this_type const &lhs, CompoundFoodImpl::this_type const &rhs) {
        return !(lhs == rhs);
    }

    OStream &operator<<(OStream &os, FoodWithAmount const &foodWithAmount) {
        return os << utils::makeClassString(STR("FoodWithAmount"), STR("val_"),
                                            *foodWithAmount.val_, STR("count_"), foodWithAmount.count_);
    }

    NutrientTable FoodWithAmount::getNutrientTable() const {
        return val_->getNutrientTable();
    }

    FoodWithAmount::AmountType FoodWithAmount::getCount() const {
        return count_;
    }

    Id FoodWithAmount::getId() const {
        return val_->getId();
    }

    String FoodWithAmount::getText() const {
        return val_->getText();
    }

    bool operator==(FoodWithAmount const &fwc1, FoodWithAmount const &fwc2) {
        return *fwc1.val_ == *fwc2.val_;
    }

    bool operator!=(FoodWithAmount const &fwc1, FoodWithAmount const &fwc2) {
        return !(fwc1 == fwc2);
    }
    
} // END of namespace fa
