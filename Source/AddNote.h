#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SceneManager.h"
#include "FoodPlan.h"
#include "NoteImpl.h"

namespace fa {
    namespace gui {

        class AddNotePanel : public juce::Component, public juce::Button::Listener {
        public:
            explicit AddNotePanel(SceneManager &);
            virtual void resized() override;
            virtual void buttonClicked(juce::Button *) override;
        private:
            const int max_char_amount{ 1200 };
            juce::TextEditor textArea_;
            juce::TextButton save_;

            SceneManager &sceneManager_;
        }; // END of class AddNotePanel
    } // END of namespace gui
} // END of namespace fa