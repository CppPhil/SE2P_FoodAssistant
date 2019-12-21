#include "Food.h"

namespace fa {
    FoodInterface::~FoodInterface() = default;

    OStream &operator<<(OStream &ostr, FoodInterface const &food) {
        return food.printOn(ostr);
    }

    bool operator==(FoodInterface const &food1, FoodInterface const &food2) {
        return food1.equals(food2);
    }

    bool operator!=(FoodInterface const &food1, FoodInterface const &food2) {
        return !(food1 == food2);
    }

} // END of namespace fa
