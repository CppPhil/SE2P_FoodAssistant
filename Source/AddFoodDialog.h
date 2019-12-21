#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SceneManager.h"
#include "FoodPlan.h"
#include "DummyFood.h"
#include "Global.h"
#include "Food.h"

namespace fa {
    namespace gui {

        class AddFoodDialogPanel final
            : public juce::Component,
              public juce::TextEditor::Listener,
              public juce::Button::Listener {

        public:
			using value_type = FoodInterface;

            AddFoodDialogPanel(SceneManager &, const value_type &); // TO BE CHANGED
            virtual void resized() override;
            virtual void textEditorReturnKeyPressed(juce::TextEditor &) override;
            virtual void buttonClicked(juce::Button *) override;
        protected:
            void addToFoodPlan(); // TO BE CHANGED
		private:
            juce::Label amountLabel_;
            juce::Label kcalMacroLabel_;
            juce::TextEditor amountEditor_;
            juce::TextButton add_;

            SceneManager &sceneManager_;
			const value_type &food_;
        }; // END of class AddFoodDialogPanel
    } // END of namespace gui
} // END of namespace fa