#pragma once
#include <utility>
#include <iostream>
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "AddFood.h"
#include "AddNote.h"
#include "SceneManager.h"
#include <boost/any.hpp>
#include <memory> // std::unique_ptr
#include <vector> // std::vector
#include "DailyPlanImpl.h"
#include "DBConnectionManager.h"

namespace fa {
    namespace gui {
        class FoodPlanPanel final : public juce::Component {
        public:
            explicit FoodPlanPanel(SceneManager &, DBConnectionManager &);
            virtual void resized() override;

			void addEntry(FoodWithAmount);
			void addEntry(NoteImpl);
        private:
            // #######################################################
            // private classes
            // #######################################################
            class TopPanel final : public Component {
            public:
                TopPanel(DailyPlanImpl &);
                virtual void paint(juce::Graphics&) override;
                virtual void resized() override;
            private:
                juce::ProgressBar macroBar_;
                juce::ProgressBar microBar_;
                juce::ProgressBar kcalBar_;
                juce::ProgressBar overallBar_;
                double macroValue_ = 0.0;
                double microValue_ = 0.0;
                double kcalValue_ = 0.0;
                double overallValue_ = 0.0;
                juce::String macroText_{ "Macro: " };
                juce::String microText_{ "Micro: " };
                juce::String kcalText_{ "Kcal: " };
                juce::String overallText_{ "Overall: " };

                juce::ComboBox profile_;
                juce::TextEditor date_;
                juce::TextButton recommendation_;

				DailyPlanImpl &dailyPlan_;
            }; // END of class TopPanel

            // --------------------------------------------------------
            // FoodTable
            // --------------------------------------------------------

            class FoodList final : public juce::ListBox, public juce::ListBoxModel {
            private:
                class Entry;

            public:
                using SmartPointer = std::unique_ptr<Entry>;
                using container_type = std::vector<SmartPointer>;

                explicit FoodList(DailyPlanImpl &);
				void addEntry(FoodWithAmount);
				void addEntry(NoteImpl);

                //virtual void resized() override;

                virtual int getNumRows() override;
                virtual void paintListBoxItem(int, juce::Graphics&, int, int, bool) override; // TO BE CHANGED
                Component *refreshComponentForRow(int, bool, Component *) override;

                virtual void listBoxItemClicked(int, juce::MouseEvent const &) override;
            private:
                // #######################################################
                // FoodList - private classes
                // #######################################################
                class Entry {
                public:
                    virtual void paint(juce::Graphics &, int, int) = 0;
                    virtual void openEditDialog() = 0;
                    virtual ~Entry() = default;
                }; // END of class Entry

                class FoodEntry final : public Entry {
                public:
                    explicit FoodEntry(FoodWithAmount);
                    virtual void paint(juce::Graphics &, int, int) override;
                    virtual void openEditDialog() override;
                private:
					FoodWithAmount food_;
                }; // END of class FoodEntry

                class NoteEntry final : public Entry {
                public:
                    explicit NoteEntry(NoteImpl);
                    virtual void paint(juce::Graphics &, int, int) override;
                    virtual void openEditDialog() override;
                private:
                    NoteImpl note_;
                }; // END of class NoteEntry

				DailyPlanImpl &dailyPlan_;
                container_type entries_;
                juce::PopupMenu popupMenu_;
            }; // END of class FoodList

            // --------------------------------------------------------
            // End FoodTable
            // --------------------------------------------------------

            class MiddlePanel final : public Component {
            public:
                explicit MiddlePanel(DailyPlanImpl &);
                void addEntry(FoodWithAmount);
				void addEntry(NoteImpl);

                virtual void resized() override;
            private:
                FoodList foodList_;
            }; // END of class MiddlePanel

            class BottomPanel : public Component, public juce::Button::Listener {
            public:
                explicit BottomPanel(SceneManager &, DBConnectionManager &);
                virtual void resized() override;
                virtual void buttonClicked(juce::Button *) override;
            private:
                void startDialogWindow();

                juce::ToggleButton noteVisibility_;
                juce::TextButton addNote_;
                juce::TextButton addFood_;
                SceneManager &sceneManager_;
				DBConnectionManager &dbManager_;
                std::unique_ptr<juce::Component> p_;
            }; // END of class BottomPanel

            // #######################################################
            // vars
            // #######################################################
            TopPanel topPanel_;
            MiddlePanel middlePanel_;
            BottomPanel bottomPanel_;

			DailyPlanImpl dailyPlan_;
        }; // END of class FoodPlanPanel
    } // END of namespace gui
} // END of namespace fa
