#include "AddNote.h"
#include "Global.h"
#include <typeinfo> // std::bad_cast
#include "GlobalHeader.h"

namespace fa {
    namespace gui {

        AddNotePanel::AddNotePanel(SceneManager &sceneManager)
            : sceneManager_{ sceneManager } {

            addAndMakeVisible(textArea_);
            addAndMakeVisible(save_);

            save_.setButtonText("Save");
            textArea_.setMultiLine(true);
            textArea_.setReturnKeyStartsNewLine(true);
            textArea_.setEscapeAndReturnKeysConsumed(false);
            textArea_.setTextToShowWhenEmpty("Let me be your note ;)", juce::Colours::grey);
            textArea_.setInputRestrictions(max_char_amount);

            save_.addListener(this);

            setSize(400, 250);
        }

        void AddNotePanel::resized() {
            const auto buttonW{ 0.2f };
            const auto buttonH{ 0.1f };
            const auto buttonX{ glo::totalSize - glo::margin - buttonW };
            const auto buttonY{ glo::totalSize - glo::margin - buttonH };

            const auto textAreaX{ glo::margin };
            const auto textAreaY{ glo::margin };
            const auto textAreaW{ glo::totalSize - glo::margin * 2 };
            const auto textAreaH{ buttonY - glo::margin * 2 };

            textArea_.setBoundsRelative(textAreaX, textAreaY, textAreaW, textAreaH);
            save_.setBoundsRelative(buttonX, buttonY, buttonW, buttonH);
        }

        void AddNotePanel::buttonClicked(juce::Button *button) try {
            if (button == &save_) { // TODO: FIXME: doesn't work.
                auto &foodPlan = dynamic_cast<FoodPlanPanel &>(sceneManager_.getPreviousScene(0));
                foodPlan.addEntry(NoteImpl(textArea_.getText().toStdString()));

                getTopLevelComponent()->exitModalState(0);
            }
        } catch (std::bad_cast const &ex) {
            std::cerr << STR("std::bad_cast caught in void AddNotePanel::buttonClicked(juce::Button *button) :")
                      << ex.what() << STR('\n');
            throw; // rethrow
        }
    } // END of namespace gui
} // END of namespace fa