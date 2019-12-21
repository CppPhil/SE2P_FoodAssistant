#include "PlanEntry.h"

namespace fa {
    PlanEntryInterface::~PlanEntryInterface() = default;

    OStream &operator<<(OStream &ostr, PlanEntryInterface const &planEntry) {
        return planEntry.printOn(ostr);
    }

} // END of namespace fa
