#pragma once
#include <utility> // std::forward, std::move
#include "GlobalHeader.h" // fa::OStream
#include "FoodImpl.h" // fa::FoodWithAmount
#include "NoteImpl.h" // fa::NoteImpl
#include "DailyPlanImpl.h" // fa::DailyPlanImpl

namespace fa {
    template <class Target>
    class DailyPlanStaticDelegator {
    public:
        using this_type = DailyPlanStaticDelegator;
        using value_type = Target;

        friend class DailyPlanImpl;

        template <class ...Args>
        explicit DailyPlanStaticDelegator(Args &&...args) noexcept
            : val_{ std::forward<Args>(args)... } { }

        auto getDate() const -> decltype(auto) {
            return val_.getDate();
        }

        auto setDate(Date const &date) -> decltype(auto) {
            return val_.setDate(date);
        }

        auto setDate(Date &&date) -> decltype(auto) {
            return val_.setDate(std::move(date));
        }

        auto getAllNutrients() const -> decltype(auto) {
            return val_.getAllNutrients();
        }

        auto getMacroNutrients() const -> decltype(auto) {
            return val_.getMacroNutrients();
        }

        auto getMicroNutrients() const -> decltype(auto) {
            return val_.getMicroNutrients();
        }

        auto getMacroPercentage() const -> decltype(auto) {
            return val_.getMacroPercentage();
        }

        auto getMicroPercentage() const -> decltype(auto) {
            return val_.getMicroPercentage();
        }

        auto getKcalPercentage() const -> decltype(auto) {
            return val_.getKcalPercentage();
        }

        auto getOverallPercentage() const -> decltype(auto) {
            return val_.getOverallPercentage();
        }
                
        void addEntry(FoodWithAmount const &foodWithAmount) {
            val_.addEntry(foodWithAmount);
        }

        void addEntry(FoodWithAmount &&foodWithAmount) {
            val_.addEntry(std::move(foodWithAmount));
        }

        void addEntry(NoteImpl const &note) {
            val_.addEntry(note);
        }

        void addEntry(NoteImpl &&note) {
            val_.addEntry(note);
        }

        auto getFoods() const -> decltype(auto) {
            return val_.getFoods();
        }

        auto getFoods() -> decltype(auto) {
            return val_.getFoods();
        }

        auto getNotes() const -> decltype(auto) {
            return val_.getNotes();
        }

        auto getNotes() -> decltype(auto) {
            return val_.getNotes();
        }

        auto getEntries() const -> decltype(auto) {
            return val_.getEntries();
        }

        auto getEntries() -> decltype(auto) {
            return val_.getEntries();
        }

        auto getFoodsDownCasted() -> decltype(auto) {
            return val_.getFoodsDownCasted();
        }

        auto getFoodsDownCasted() const -> decltype(auto) {
            return val_.getFoodsDownCasted();
        }

        auto getNotesDownCasted() -> decltype(auto) {
            return val_.getNotesDownCasted();
        }

        auto getNotesDownCasted() const -> decltype(auto) {
            return val_.getNotesDownCasted();
        }

        friend OStream &operator<<(OStream &ost, this_type const &printMe) {
            return ost << printMe.val_;
        }

    private:
        value_type val_;
    }; // END of class DailyPlanStaticDelegator

    using DailyPlan = DailyPlanStaticDelegator<DailyPlanImpl>;

} // END of namespace fa
