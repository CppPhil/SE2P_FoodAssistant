#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector> // std::vector
#include <memory> // std::unique_ptr
#include "DBConnectionManager.h"
#include "AddFood.h"
#include "Amount.h"

namespace fa {
    namespace gui {
        class SceneManager;

        class AddFoodPanel : public juce::Component {
        public:
            explicit AddFoodPanel(SceneManager &, DBConnectionManager &);

            virtual void resized() override;
        private:
            // #######################################################
            // private classes
            // #######################################################

            class FoodTable : public juce::TableListBox, public juce::TableListBoxModel { // TO BE CHANGED
            public:
				using value_type = FoodInterface;
                using SmartPointer = std::unique_ptr<value_type>;
                using container_type = std::vector<SmartPointer>;

                FoodTable();
                virtual int getNumRows() override;
                virtual void paintRowBackground(juce::Graphics &, int, int, int, bool) override;
                virtual void paintCell(juce::Graphics &, int, int, int, int, bool) override;
                virtual void paintListBoxItem(int, juce::Graphics &, int, int, bool) override;
                virtual void listBoxItemClicked(int, const juce::MouseEvent &) override;

				void clear();
                void addFoodResult(SmartPointer &&food);
                const FoodInterface *getSelection();
            private:
				String toString(const value_type &, const NutrientTable &, int column) const;
				static const auto column_amount = 5;

                container_type entries_;
                int selectedRow;
            };

            class MiddlePanel : public Component {
            public:
                using value_type = FoodTable;
                using SmartPointer = value_type::SmartPointer;

                MiddlePanel();
                virtual void resized() override;

				void clear();
                void addFoodResult(SmartPointer &&food);
                const FoodInterface *getSelection();
            private:
                value_type foodTable_;
            };

            class BottomPanel : public Component, public juce::Button::Listener {
            public:
                BottomPanel(SceneManager &, MiddlePanel &);
                virtual void resized() override;
                virtual void buttonClicked(juce::Button *) override; // TO BE CHANGED
            private:
                void startDialogWindow(); // TO BE CHANGED

                juce::TextButton cancel_;
                juce::TextButton create_;
                juce::TextButton add_;
                SceneManager &sceneManager_;
                MiddlePanel &context_;
                std::unique_ptr<juce::Component> p_;
            };

            class TopPanel final : public Component, public juce::TextEditor::Listener { // TO BE CHANGED
            public:
                explicit TopPanel(MiddlePanel &, DBConnectionManager &);
                virtual void resized() override;
                virtual void textEditorReturnKeyPressed(juce::TextEditor &) override; // TO BE CHANGED
            private:
                juce::Label searchLabel_;
                juce::TextEditor searchEditor_;
                juce::TextButton sortButton_;

                MiddlePanel &context_;
				DBConnectionManager &dbManager_;
				//MySQLFoodDAO foodDao_;
            };

            TopPanel topPanel_;
            MiddlePanel middlePanel_;
            BottomPanel bottomPanel_;
        }; // END of class AddFoodPanel
    } // END of namespace gui
} // END of namespace fa
