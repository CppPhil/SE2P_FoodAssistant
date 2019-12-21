#include "AddFoodDialog.h"
#include "GlobalHeader.h"
#include <typeinfo> // std::bad_cast

namespace fa {
    namespace gui {
        AddFoodDialogPanel::AddFoodDialogPanel(SceneManager &sceneManager, const value_type &food)
            : sceneManager_{ sceneManager }, food_{ food } {

            addAndMakeVisible(amountLabel_);
            addAndMakeVisible(kcalMacroLabel_);
            addAndMakeVisible(amountEditor_);
            addAndMakeVisible(add_);

            amountLabel_.setText("Amount?", juce::NotificationType::dontSendNotification);
            kcalMacroLabel_.setText("kcal: 0 | Protein: 0 | Fat: 0 | Carbs: 0", juce::NotificationType::dontSendNotification);
            amountEditor_.setTextToShowWhenEmpty("0 g", juce::Colours::grey);
            add_.setButtonText("Add");

            /*
            void TextEditor::setInputFilter(InputFilter *newFilter, bool takeOwnership)		
            Sets an input filter that should be applied to this editor.

            The filter can be nullptr, to remove any existing filters. 
			If takeOwnership is true, then the filter will be owned and deleted by the editor when no longer needed.
            */
            amountEditor_.setInputFilter(new juce::TextEditor::LengthAndCharacterRestriction{ 6, "0123456789" },
                                         true);
            amountEditor_.addListener(this);
            add_.addListener(this);

            setSize(300, 90);
        }

        void AddFoodDialogPanel::resized() {
            auto const middleW = 0.9f;
            auto const edgeW = (glo::totalSize - middleW) / 2;

            auto const elemH = (glo::totalSize - glo::margin * 4) / 3;
            auto const buttonW = 0.25f;
            auto const editorW = (middleW - glo::margin) * 0.7f;
            auto const amountW = middleW - editorW;

            auto const amountX = edgeW;

            amountLabel_.setBoundsRelative(amountX, glo::margin, amountW, elemH);
            amountEditor_.setBoundsRelative(amountX + amountW + glo::margin, glo::margin, editorW, elemH);

            kcalMacroLabel_.setBoundsRelative(edgeW, glo::margin * 2 + elemH, middleW, elemH);

            add_.setBoundsRelative(glo::totalSize - glo::margin - buttonW, glo::margin * 3 + elemH * 2, buttonW, elemH);
        }

        void AddFoodDialogPanel::textEditorReturnKeyPressed(juce::TextEditor &editor) {
            if (&editor == &amountEditor_) {
                addToFoodPlan();
            }
        }
		/* 
			Removed the const try around the function, because otherwise I am not able to
			exitModalState and for some reason it's effectless to use it after this call.
		*/
        void AddFoodDialogPanel::addToFoodPlan() try { 
            auto &foodPlan = dynamic_cast<FoodPlanPanel &>(sceneManager_.getPreviousScene()); // unreferenced local variable
            //foodPlan.addEntry(food_->getName(), amountEditor_.getText().getIntValue());
			//foodPlan.addEntry(FoodWithAmount(food_, amountEditor_.getText().getIntValue()));

			/*
				The next to lines have to stay in this order. Otherwise it would crash after
				the call of getTopLevelComponent.
			*/
			getTopLevelComponent()->exitModalState(0);
            sceneManager_.toPreviousScene();
        } catch (std::bad_cast const &ex) {
            std::cerr << STR("bad cast in AddFoodDialogPanel::addToFoodPlan: ") << ex.what() << STR('\n');
            throw; // rethrow
		}

        void AddFoodDialogPanel::buttonClicked(juce::Button *button) {
            if (button == &add_) {
                addToFoodPlan();
            }
        }
    } // END of namespace gui
} // END of namespace fa
