#include "FoodPlan.h"
#include <utility> // std::move
#include <mylibs/utility.hpp> // pl::to

namespace fa {
    namespace gui {
        FoodPlanPanel::FoodPlanPanel(SceneManager &sceneManager, DBConnectionManager &dbManager)
			: topPanel_{ dailyPlan_ }, 
            middlePanel_{ dailyPlan_ }, bottomPanel_{ sceneManager, dbManager } {

            addAndMakeVisible(topPanel_);
            addAndMakeVisible(middlePanel_);
            addAndMakeVisible(bottomPanel_);

            setSize(glo::winWidth, glo::winHeight); // calling resized

            // some dummies to have something to select, delete... to play.
            static auto constexpr dummiesToMake = 5;
            for (auto i = 0; i < dummiesToMake; ++i) {
                //addFood("Dummy", i);
            }
        }

		void FoodPlanPanel::resized() {
            auto const middleSize = 0.8f;
            auto const topSize = (glo::totalSize - middleSize) * 0.75f;
            auto const bottomSize = glo::totalSize - middleSize - topSize;

            topPanel_.setBoundsRelative(0.0f, 0.0f, glo::totalSize, topSize);
            middlePanel_.setBoundsRelative(0.0f, topSize, glo::totalSize, middleSize);
            bottomPanel_.setBoundsRelative(0.0f, topSize + middleSize, glo::totalSize, bottomSize);
        }

		void FoodPlanPanel::addEntry(FoodWithAmount food) { // why does this copy in the parameter???
			dailyPlan_.addEntry(food);
			middlePanel_.addEntry(food);
		}

		void FoodPlanPanel::addEntry(NoteImpl noteImpl) { // why does this copy in the parameter???
			dailyPlan_.addEntry(noteImpl);
			middlePanel_.addEntry(noteImpl);
		}

        // ########################################################
        // TopPanel
        // ########################################################
        FoodPlanPanel::TopPanel::TopPanel(DailyPlanImpl &dailyPlan)
            : macroBar_{ macroValue_ }, microBar_{ microValue_ },
            kcalBar_{ kcalValue_ }, overallBar_{ overallValue_ },
			dailyPlan_{ dailyPlan } {

            macroBar_.setTextToDisplay(macroText_ + juce::String{ macroValue_ });
            microBar_.setTextToDisplay(microText_ + juce::String{ microValue_ });
            kcalBar_.setTextToDisplay(kcalText_ + juce::String{ kcalValue_ });
            overallBar_.setTextToDisplay(overallText_ + juce::String{ overallValue_ });

            addAndMakeVisible(macroBar_);
            addAndMakeVisible(microBar_);
            addAndMakeVisible(kcalBar_);
            addAndMakeVisible(overallBar_);

            addAndMakeVisible(profile_);
            addAndMakeVisible(date_);
            addAndMakeVisible(recommendation_);

            profile_.setText("Profile");
            date_.setText("Date");
            recommendation_.setButtonText("Recommendation");
        }

        void FoodPlanPanel::TopPanel::resized() {
            auto const barAmount = 4;

            auto const barW = 0.6f;
            auto const barY = glo::margin;
            auto const barH = (glo::totalSize - barY - glo::margin * barAmount) / barAmount;
            auto const barX = (glo::totalSize - barW) / 2;

            macroBar_.setBoundsRelative(barX, barY + (glo::margin + barH) * 0, barW, barH);
            microBar_.setBoundsRelative(barX, barY + (glo::margin + barH) * 1, barW, barH);
            kcalBar_.setBoundsRelative(barX, barY + (glo::margin + barH) * 2, barW, barH);
            overallBar_.setBoundsRelative(barX, barY + (glo::margin + barH) * 3, barW, barH);

            auto const buttonAmount = 3;

            auto const buttonX = barX + barW + glo::margin;
            auto const buttonW = 1.0f - buttonX;
            auto const buttonH = barH * 1.1f;
            auto const buttonMargin = (glo::totalSize - glo::margin - buttonH * 3) / 2.0f;

            profile_.setBoundsRelative(buttonX, barY + (buttonH + buttonMargin) * 0, buttonW - glo::margin, buttonH);
            date_.setBoundsRelative(buttonX, barY + (buttonH + buttonMargin) * 1, buttonW - glo::margin, buttonH);
            recommendation_.setBoundsRelative(buttonX, barY + (buttonH + buttonMargin) * 2, buttonW - glo::margin, buttonH);
        }

		void FoodPlanPanel::TopPanel::paint(juce::Graphics &g) {
            static auto constexpr color = 0XFF001F36;

            g.fillAll(juce::Colour{ color });
        }

        // ########################################################
        // MiddlePanel
        // ########################################################
        FoodPlanPanel::MiddlePanel::MiddlePanel(DailyPlanImpl &dailyPlan) 
			: foodList_{ dailyPlan } {
            
			addAndMakeVisible(foodList_);
        }

		void FoodPlanPanel::MiddlePanel::addEntry(FoodWithAmount food) {
			foodList_.addEntry(food);
		}

		void FoodPlanPanel::MiddlePanel::addEntry(NoteImpl note) {
			foodList_.addEntry(note);
		}

		void FoodPlanPanel::MiddlePanel::resized() {
            foodList_.setBoundsRelative(0.0f, 0.0f, glo::totalSize, glo::totalSize);
        }

        // --------------------------------------------------------
        // FoodTable
        // --------------------------------------------------------
        FoodPlanPanel::FoodList::FoodList(DailyPlanImpl &dailyPlan) 
			: dailyPlan_{ dailyPlan } {

            setModel(this);

            popupMenu_.addItem(1, "delete");
            popupMenu_.addItem(2, "edit");
            popupMenu_.addItem(3, "copy");

            setRowHeight(50);
            setMultipleSelectionEnabled(true);
        }

        //void FoodPlanPanel::FoodList::resized() {
        //	
        //}

        void FoodPlanPanel::FoodList::addEntry(FoodWithAmount food) {
			entries_.push_back(std::make_unique<FoodEntry>(food));
            updateContent();
        }

		void FoodPlanPanel::FoodList::addEntry(NoteImpl note) {
			entries_.push_back(std::make_unique<NoteEntry>(note));
			updateContent();
		}	

        int FoodPlanPanel::FoodList::getNumRows() {
            return static_cast<int>(entries_.size()); // important: sets the amount of rows in the table
        }

        namespace detail_again { // TODO: refactor this somehow; this has already been seen somewhere else.
            int center(int outerW, int innerW) {
                return (outerW - innerW) / 2;
            }
        }

        void FoodPlanPanel::FoodList::paintListBoxItem(int rowNumber, juce::Graphics &g,
                                                       int width, int height, bool rowIsSelected) {

            static auto constexpr color = 0XFF096AB1;

            if (rowIsSelected) {
                g.fillAll(juce::Colour{ color });
            }

            entries_.at(rowNumber)->paint(g, width, height);
            g.drawLine(0.0F,
                       static_cast<float>(height),
                       static_cast<float>(width),
                       static_cast<float>(height));
        }

        juce::Component *FoodPlanPanel::FoodList::refreshComponentForRow(int, bool, juce::Component *) {
            return nullptr;
        }

        void FoodPlanPanel::FoodList::listBoxItemClicked(int row, juce::MouseEvent const &event) {
            if (event.mods.isRightButtonDown()) {
                auto const result = popupMenu_.show();

                switch (result) {
                    case 0 : break;
                    case 1 : // delete
                        entries_.erase(std::cbegin(entries_) + row);
                        /*
                            http://en.cppreference.com/w/cpp/container/vector/erase
                            should be doing the same as
                            https://www.juce.com/doc/classOwnedArray#ae96b18f21808cd5a62fc35bff85e2df6
                        */

                        // i don't know if this should be falling through, but it is. 
						// nope, it shouldn't. My bad xD
						break;
                    case 2 : break;
                    default : break;
                }
                updateContent();
            }
        } // END of function void FoodPlanPanel::FoodList::listBoxItemClicked(int row, juce::MouseEvent const &event)

        // #######################################################
        // FoodList - private classes
        // #######################################################
        FoodPlanPanel::FoodList::FoodEntry::FoodEntry(FoodWithAmount food)
			: food_{ std::move(food) } { }

		void FoodPlanPanel::FoodList::FoodEntry::paint(juce::Graphics &g, int width, int height) {
			auto nutrientTable = food_.getNutrientTable();
			auto kcalNutrient = nutrientTable.getNutrient(NutrientType::Kcal);

			auto name = food_.getText();//food_.getNutrientTable().getNutrient();
			auto kcal = pl::to<String>(kcalNutrient.second); //juce::String{ kcal_ } +" kcal";
			auto amount = "0"; // fa::toStringWithMess(food_.getCount());//juce::String{ amount_ } +" g";

            auto font = g.getCurrentFont(); // to be optimized?
            auto nameW = font.getStringWidth(name);
            auto kcalW = font.getStringWidth(kcal);
            auto amountW = font.getStringWidth(amount);
            auto partW = static_cast<float>(width) / 3.0f;

            g.drawText(name,
                       static_cast<int>(partW * 0.0F +
                                        detail_again::center(static_cast<int>(partW), static_cast<int>(nameW))),
                       0, static_cast<int>(partW), height, juce::Justification::centredLeft, true);
            g.drawText(kcal, 
                       static_cast<int>(partW * 1.0F + 
                                        detail_again::center(static_cast<int>(partW), static_cast<int>(kcalW))),
                       0, static_cast<int>(partW), height, juce::Justification::centredLeft, true);
            g.drawText(amount,
                       static_cast<int>(partW * 2.0F +
                                        detail_again::center(static_cast<int>(partW), static_cast<int>(amountW))),
                       0, static_cast<int>(partW), height, juce::Justification::centredLeft, true);

            g.drawLine(partW * 1.0F, 0.0F, partW * 1.0F, static_cast<float>(height));
            g.drawLine(partW * 2.0F, 0.0F, partW * 2.0F, static_cast<float>(height));
        } // END of function void FoodPlanPanel::FoodList::FoodEntry::paint(juce::Graphics &g, int width, int height)

        void FoodPlanPanel::FoodList::FoodEntry::openEditDialog() { }

        FoodPlanPanel::FoodList::NoteEntry::NoteEntry(NoteImpl note)
            : note_{ note } {}

		void FoodPlanPanel::FoodList::NoteEntry::paint(juce::Graphics &g, int width, int height) {
            auto const text = note_.getText();
            auto const x = glo::margin * width;
            auto const y = 0;
            auto const w = width - x * 2;

            g.drawText(text, static_cast<int>(x), y, static_cast<int>(w),
                       height, juce::Justification::centredLeft, true);
        }

        void FoodPlanPanel::FoodList::NoteEntry::openEditDialog() { }

        // ########################################################
        // BottomPanel
        // ########################################################
        FoodPlanPanel::BottomPanel::BottomPanel(SceneManager &sceneManager, 
												DBConnectionManager &dbManager) 
			: sceneManager_{ sceneManager }, dbManager_{ dbManager } {

            addAndMakeVisible(noteVisibility_);
            addAndMakeVisible(addNote_);
            addAndMakeVisible(addFood_);

            noteVisibility_.setButtonText("Notes visible");
            addNote_.setButtonText("Add note");
            addFood_.setButtonText("Add food");
            addNote_.addListener(this);
            addFood_.addListener(this);
        }

		void FoodPlanPanel::BottomPanel::resized() {
            auto const bottomMargin = glo::margin;
            auto const bottomButtonW = 0.15f;
            auto const bottomButtonH = glo::totalSize - bottomMargin * 2.0f;

            noteVisibility_.setBoundsRelative(glo::margin * 2.0f, bottomMargin, bottomButtonW, bottomButtonH - bottomMargin);
            noteVisibility_.changeWidthToFitText();

            addFood_.setBoundsRelative(1.0f - (bottomMargin + bottomButtonW) * 1.0f, bottomMargin, bottomButtonW, bottomButtonH);
            addNote_.setBoundsRelative(1.0f - (bottomMargin + bottomButtonW) * 2.0f, bottomMargin, bottomButtonW, bottomButtonH);
        }

        void FoodPlanPanel::BottomPanel::buttonClicked(juce::Button *clickedButton) {
            if (clickedButton == &addFood_) {
                sceneManager_.toNextScene(std::make_unique<AddFoodPanel>(sceneManager_, dbManager_));
            } else if (clickedButton == &addNote_) {
                startDialogWindow();
            }
        }

        void FoodPlanPanel::BottomPanel::startDialogWindow() {
            juce::DialogWindow::LaunchOptions launchOptions{ };
            launchOptions.dialogTitle = "Add note";
            launchOptions.escapeKeyTriggersCloseButton = true;
            launchOptions.resizable = true;
            p_ = std::make_unique<AddNotePanel>(sceneManager_);

            launchOptions.content.setNonOwned(p_.get());
            launchOptions.launchAsync();
        }

    } // END of namespace gui
} // END of namespace fa
