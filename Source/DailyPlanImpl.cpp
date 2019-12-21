#include "DailyPlanImpl.h"
#include <utility> // std::move
#include <mylibs/container.hpp> // CBEGIN_CEND
#include "Utils.h" // utils::makeClassString

namespace fa {
    Date DailyPlanImpl::getDate() const {
        return date_;
    }

    void DailyPlanImpl::setDate(Date const &date) {
        date_ = date;
    }

    void DailyPlanImpl::setDate(Date &&date) {
        date_ = std::move(date);
    }

    NutrientTable DailyPlanImpl::getNutrients(FoodCont::const_iterator b, FoodCont::const_iterator e, NutrientTable accu) const {
        if (b == e) {
            return accu;
        }

        auto delegatorPtr = dynamic_cast<PlanEntryDynamicDelegator<DailyPlanFood> const *>(b->get());
        if (delegatorPtr == nullptr) {
            throw std::logic_error{ "delegatorPtr in getNutrients in DailyPlanImpl.cpp was nullptr." };
        }

        return getNutrients(b + 1, e, accu + delegatorPtr->val_.getNutrientTable());
    }

    NutrientTable DailyPlanImpl::getAllNutrients() const {
        return getNutrients(CBEGIN_CEND(foodCont_), { });
    }

    NutrientTable DailyPlanImpl::getMacroNutrients() const {
        return getAllNutrients().getMacroNutrients();
    }

    NutrientTable DailyPlanImpl::getMicroNutrients() const {
        return getAllNutrients().getMicroNutrients();
    }

    double DailyPlanImpl::getMacroPercentage() const { // TODO: implement this
        throw NotYetImplementedException{ std::string{ __func__ } + std::string{ " not yet implemented, no idea how to" } };
    }

    double DailyPlanImpl::getMicroPercentage() const { // TODO: implement this
        throw NotYetImplementedException{ std::string{ __func__ } + std::string{ " not yet implemented, no idea how to" } };
    }

    double DailyPlanImpl::getKcalPercentage() const { // TODO: implement this
        throw NotYetImplementedException{ std::string{ __func__ } + std::string{ " not yet implemented, no idea how to" } };
    }

    double DailyPlanImpl::getOverallPercentage() const { // TODO: implement this
        throw NotYetImplementedException{ std::string{ __func__ } + std::string{ " not yet implemented, no idea how to" } };
    }

    void DailyPlanImpl::addEntry(FoodWithAmount const &foodWithAmount) {
        foodCont_.emplace_back(foodWithAmount);
    }

    void DailyPlanImpl::addEntry(FoodWithAmount &&foodWithAmount) {
        foodCont_.emplace_back(std::move(foodWithAmount));
    }

    void DailyPlanImpl::addEntry(NoteImpl const &noteImpl) {
        noteCont_.emplace_back(noteImpl);
    }

    void DailyPlanImpl::addEntry(NoteImpl &&noteImpl) {
        noteCont_.emplace_back(std::move(noteImpl));
    }

    pl::View<DailyPlanImpl::FoodCont const> DailyPlanImpl::getFoods() const {
        return pl::makeView(foodCont_);
    }

    pl::View<DailyPlanImpl::FoodCont> DailyPlanImpl::getFoods() {
        return pl::makeView(foodCont_);
    }

    pl::View<DailyPlanImpl::NoteCont const> DailyPlanImpl::getNotes() const {
        return pl::makeView(noteCont_);
    }

    pl::View<DailyPlanImpl::NoteCont> DailyPlanImpl::getNotes() {
        return pl::makeView(noteCont_);
    }

    daily_plan_detail::Container<gsl::not_null<DailyPlanImpl::Interface const *>> DailyPlanImpl::getEntries() const {
        daily_plan_detail::Container<gsl::not_null<Interface const *>> ret{ };
        for (auto const &e : foodCont_) {
            ret.push_back(e.get());
        }
        for (auto const &e : noteCont_) {
            ret.push_back(e.get());
        }
        return ret;
    }

    daily_plan_detail::Container<gsl::not_null<DailyPlanImpl::Interface *>> DailyPlanImpl::getEntries() {
        daily_plan_detail::Container<gsl::not_null<Interface *>> ret{ };
        for (auto &e : foodCont_) {
            ret.push_back(e.get());
        }
        for (auto &e : noteCont_) {
            ret.push_back(e.get());
        }
        return ret;
    }

    // TODO: test this.
    daily_plan_detail::Container<gsl::not_null<DailyPlanImpl::DailyPlanFood *>> DailyPlanImpl::getFoodsDownCasted() {
        daily_plan_detail::Container<gsl::not_null<DailyPlanFood *>> ret{ };
        for (auto &e : foodCont_) {
            auto p = dynamic_cast<PlanEntryDynamicDelegator<DailyPlanFood> *>(e.get());
            if (p == nullptr) {
                throw std::logic_error{ "downcast failed in DailyPlanImpl::getFoodsDownCasted()" };
            }
            ret.push_back(&p->val_);
        }
        return ret;
    }

    // TODO: test this.
    daily_plan_detail::Container<gsl::not_null<DailyPlanImpl::DailyPlanFood const *>> DailyPlanImpl::getFoodsDownCasted() const {
        daily_plan_detail::Container<gsl::not_null<DailyPlanFood const *>> ret{ };
        for (auto const &e : foodCont_) {
            auto p = dynamic_cast<PlanEntryDynamicDelegator<DailyPlanFood> const *>(e.get());
            if (p == nullptr) {
                throw std::logic_error{ "downcast failed in DailyPlanImpl::getFoodsDownCasted() const" };
            }
            ret.push_back(&p->val_);
        }
        return ret;
    }

    // TODO: test this.
    daily_plan_detail::Container<gsl::not_null<DailyPlanImpl::DailyPlanNote *>> DailyPlanImpl::getNotesDownCasted() {
        daily_plan_detail::Container<gsl::not_null<DailyPlanNote *>> ret{ };

        for (auto &e : noteCont_) {
            auto p = dynamic_cast<PlanEntryDynamicDelegator<DailyPlanNote> *>(e.get());
            if (p == nullptr) {
                throw std::logic_error{ "downcast failed in DailyPlanImpl::getNotesDownCasted" };
            }
            ret.push_back(&p->val_);
        }
        return ret;
    }

    // TODO: test this.
    daily_plan_detail::Container<gsl::not_null<DailyPlanImpl::DailyPlanNote const *>> DailyPlanImpl::getNotesDownCasted() const {
        daily_plan_detail::Container<gsl::not_null<DailyPlanNote const *>> ret{ };

        for (auto const &e : noteCont_) {
            auto p = dynamic_cast<PlanEntryDynamicDelegator<DailyPlanNote> const *>(e.get());
            if (p == nullptr) {
                throw std::logic_error{ "downcast failed in DailyPlanImpl::getNotesDownCasted const" };
            }
            ret.push_back(&p->val_);
        }
        return ret;
    }

    namespace {
        template <class Container>
        auto makeDailyPlanContString(Container &&cont) {
            OStringStream myOstringstream{ };
            myOstringstream << '[';
            for (auto &&e : cont) {
                myOstringstream << *e << ', ';
            }
            auto str = myOstringstream.str();
            static auto constexpr charsToRemoveAtTheEnd = 2;
            str.erase(std::end(str) -
                      charsToRemoveAtTheEnd,
                      std::end(str)); // remove extra , and ' ' at the end
            str += ']';
            return str;
        }
    } // END of anonymous namespace

    OStream &operator<<(OStream &os, DailyPlanImpl const &dailyPlanImpl) {
        return os << utils::makeClassString(STR("DailyPlanImpl"),
                                                                STR("foodCont_"), makeDailyPlanContString(dailyPlanImpl.foodCont_),
                                                                STR("noteCont_"), makeDailyPlanContString(dailyPlanImpl.noteCont_),
                                                                STR("date_"), dailyPlanImpl.date_);
    }

    void DailyPlanImpl::initCont(DailyPlanFood const &dpf) {
        foodCont_.emplace_back(dpf);
    }

    void DailyPlanImpl::initCont(DailyPlanFood &&dpf) {
        foodCont_.emplace_back(std::move(dpf));
    }

    void DailyPlanImpl::initCont(DailyPlanNote const &note) {
        noteCont_.emplace_back(note);
    }

    void DailyPlanImpl::initCont(DailyPlanNote &&note) {
        noteCont_.emplace_back(std::move(note));
    }

} // END of namespace fa
