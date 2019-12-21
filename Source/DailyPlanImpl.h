#pragma once
#include <mylibs/container.hpp> // pl::Vector
#include <initializer_list> // std::initializer_list
#include "FoodImpl.h" // fa::FoodWithAmount
#include "NoteImpl.h" // fa::NoteImpl
#include "PlanEntry.h" // fa::PlanEntryDynamicDelegator
#include "Date.h" // fa::Date
#include "GlobalHeader.h" // fa::OStream
#include <utility> // std::forward
#include <mylibs/utility.hpp> // pl::forEachArgument
#include <gsl.h> // gsl::not_null

namespace fa {
    namespace daily_plan_detail {
        template <class Type>
        using Container = pl::Vector<Type>;
    } // END of namespace daily_plan_detail

    class DailyPlanImpl final {
    public:
        using this_type = DailyPlanImpl;
        using DailyPlanFood = FoodWithAmount;
        using DailyPlanNote = NoteImpl;
        using Interface = PlanEntryInterface;
        using FoodUniqueOwner = pl::PolymorphicUniqueOwner<Interface, PlanEntryDynamicDelegator>;
        using NoteUniqueOwner = pl::PolymorphicUniqueOwner<Interface, PlanEntryDynamicDelegator>;
        using FoodCont = daily_plan_detail::Container<FoodUniqueOwner>;
        using NoteCont = daily_plan_detail::Container<NoteUniqueOwner>;

        //! put FoodWithAmounts and NoteImpls in here.
        template <class ...Args>
        explicit DailyPlanImpl(Args &&...args) : date_{ Date::today() } {
            pl::forEachArgument([this](auto &&e) {
                                    initCont(std::forward<decltype(e)>(e));
                                }, std::forward<Args>(args)...);
        }

        Date getDate() const;
        void setDate(Date const &);
        void setDate(Date &&);
        NutrientTable getAllNutrients() const;
        NutrientTable getMacroNutrients() const;
        NutrientTable getMicroNutrients() const;
        double getMacroPercentage() const;
        double getMicroPercentage() const;
        double getKcalPercentage() const;
        double getOverallPercentage() const;
        void addEntry(FoodWithAmount const &);
        void addEntry(FoodWithAmount &&);
        void addEntry(NoteImpl const &);
        void addEntry(NoteImpl &&);

        pl::View<FoodCont const> getFoods() const;
        pl::View<FoodCont> getFoods();
        pl::View<NoteCont const> getNotes() const;
        pl::View<NoteCont> getNotes();
        daily_plan_detail::Container<gsl::not_null<Interface const *>> getEntries() const;
        daily_plan_detail::Container<gsl::not_null<Interface *>> getEntries();
        daily_plan_detail::Container<gsl::not_null<DailyPlanFood *>> getFoodsDownCasted();
        daily_plan_detail::Container<gsl::not_null<DailyPlanFood const *>> getFoodsDownCasted() const;
        daily_plan_detail::Container<gsl::not_null<DailyPlanNote *>> getNotesDownCasted();
        daily_plan_detail::Container<gsl::not_null<DailyPlanNote const *>> getNotesDownCasted() const;

        friend OStream &operator<<(OStream &, this_type const &);

    private:
        void initCont(DailyPlanFood const &dpf);

        void initCont(DailyPlanFood &&dpf);

        void initCont(DailyPlanNote const &note);

        void initCont(DailyPlanNote &&note);

        NutrientTable getNutrients(FoodCont::const_iterator b,
                                   FoodCont::const_iterator e,
                                   NutrientTable accu) const;

        FoodCont foodCont_;
        NoteCont noteCont_;
        Date date_;
    }; // END of class DailyPlanImpl
} // END of namespace fa
