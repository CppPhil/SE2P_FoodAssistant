#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

namespace fa {
    namespace gui {
        class DummyFood final {
        public:
            DummyFood(juce::String name, int kcal, int protein, int fat, int carbs);
            juce::String getName() const;
            int getKcal() const;
            int getProtein() const;
            int getFat() const;
            int getCarbs() const;

            juce::String columnToValue(int) const;
        private:
            juce::String const name_;
            int const kcal_;
            int const protein_;
            int const fat_;
            int const carbs_;
        }; // END of class DummyFood
    } // END of namespace gui
} // END of namespace fa