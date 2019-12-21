#include "DummyFood.h"
#include <utility> // std::move
#include <mylibs/utility.hpp> // pl::to
#include <stdexcept> // std::invalid_argument

namespace fa {
    namespace gui {

        DummyFood::DummyFood(juce::String name, int kcal, int protein, int fat, int carbs)
            : name_{ std::move(name) }, kcal_{ kcal }, protein_{ protein },
              fat_{ fat }, carbs_{ carbs } { }

        juce::String DummyFood::getName() const {
            return name_;
        }

        int DummyFood::getKcal() const {
            return kcal_;
        }

        int DummyFood::getProtein() const {
            return protein_;
        }

        int DummyFood::getFat() const {
            return fat_;
        }

        int DummyFood::getCarbs() const {
            return carbs_;
        }

        juce::String DummyFood::columnToValue(int column) const {
            switch (column) { // columns start with 1
                case 1 :
                    return getName();
                case 2 :
                    return juce::String{ getKcal() };
                case 3 :
                    return juce::String{ getProtein() };
                case 4 :
                    return juce::String{ getFat() };
                case 5 :
                    return juce::String{ getCarbs() };
                default :
                    throw std::invalid_argument{ std::string{ "Can't convert column to value - column: " } + 
                                                 pl::to<std::string>(column) };
            }
        }
    } // END of namespace gui
} // END of namespace fa
