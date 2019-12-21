#include "AddFood.h"
#include "Global.h"
#include <utility> // std::move
#include "GlobalHeader.h" // utils::rng
#include "SceneManager.h"
#include "AddFoodDialog.h"
#include <mylibs/utility.hpp> // pl::to
#include <memory> // std::make_unique

namespace fa {
    namespace gui {
        AddFoodPanel::AddFoodPanel(SceneManager &sceneManager, DBConnectionManager &dbManager)
            : topPanel_{ middlePanel_, dbManager }, bottomPanel_{ sceneManager, middlePanel_ } {

            addAndMakeVisible(topPanel_);
            addAndMakeVisible(middlePanel_);
            addAndMakeVisible(bottomPanel_);

            setSize(glo::winWidth, glo::winHeight);
        }

        void AddFoodPanel::resized() {
            auto const middleSize = 0.9f;
            auto const topSize = (glo::totalSize - middleSize) * 0.5f;
            auto const bottomSize = glo::totalSize - middleSize - topSize;

            topPanel_.setBoundsRelative(0.0f, 0.0f, glo::totalSize, topSize);
            middlePanel_.setBoundsRelative(0.0f, topSize, glo::totalSize, middleSize);
            bottomPanel_.setBoundsRelative(0.0f, topSize + middleSize, glo::totalSize, bottomSize);
        }

        AddFoodPanel::TopPanel::TopPanel(MiddlePanel &context, DBConnectionManager &dbManager)
            : context_{ context }, dbManager_{ dbManager } {

            addAndMakeVisible(searchLabel_);
            addAndMakeVisible(searchEditor_);
            addAndMakeVisible(sortButton_);

            searchLabel_.setText("Search:",
                                 juce::NotificationType::dontSendNotification);
            sortButton_.setButtonText("Advanced sorting");
            searchEditor_.addListener(this);
        }

        void AddFoodPanel::TopPanel::resized() {
            auto const labelX = glo::margin;
            auto const labelY = glo::margin; // unused variable
            auto const labelW = 0.05f, editW = 0.12f, sortW = 0.16f;
            auto const topElemH = glo::totalSize - glo::margin * 2.0f;

            searchLabel_.setBoundsRelative(labelX, glo::margin, labelW, topElemH);
            searchEditor_.setBoundsRelative(labelX + labelW + glo::margin, glo::margin, editW, topElemH);
            sortButton_.setBoundsRelative(1.0f - glo::margin - sortW, glo::margin, sortW, topElemH);
        }

        void AddFoodPanel::TopPanel::textEditorReturnKeyPressed(juce::TextEditor &editor) {
            static auto constexpr lowerBound = 3; // unused variable
            static auto constexpr upperBound = 102; // unused variable

            if (&editor == &searchEditor_) {
                auto foodResultContainer = dbManager_.getFoodDAO().findFood(editor.getText().toStdString());
                context_.clear();

                for (auto &&e : foodResultContainer) {
                    context_.addFoodResult(
                                           std::make_unique<FoodDynamicDelegator<AtomicFoodImpl>>(std::move(e)));
                }

                //auto resultAmount = utils::rng.getRandomNumber(lowerBound, upperBound);

                /*
                for (auto i = 0; i < 3; i++) { // TO BE CHANGED
                    context_.addFoodResult(std::make_unique<DummyFood>(editor.getText(),
                                                                       80, 1, 1, 14)); // TO BE CHANGED
                }
                */
            }
        }

        AddFoodPanel::FoodTable::FoodTable() {
            setModel(this);
            setRowHeight(50); // to be changed
        }

        namespace detail { // some private helper
            int center(int outerW, int innerW) {
                return (outerW - innerW) / 2;
            }
        } // END of namespace detail

        int AddFoodPanel::FoodTable::getNumRows() {
            return static_cast<int>(entries_.size());
        }

        void AddFoodPanel::FoodTable::paintRowBackground(juce::Graphics &, int, int, int, bool) {
            // TODO: to be implemented
        }

        void AddFoodPanel::FoodTable::paintCell(juce::Graphics &g, int row, int column,
                                                int width, int height, bool /*rowIsSelected*/) {
            //auto txt = entries_.at(row)->getText();//entries_.at(row)->columnToValue(column);
            const auto &food = *entries_.at(row);
            auto txt = toString(food, food.getNutrientTable(), column);

            auto font = g.getCurrentFont(); // to be optimized?
            auto txtW = font.getStringWidth(txt);
            auto txtX = column == 1 ? width * glo::margin
                            : detail::center(width, txtW); // The name of a food should be centered.
            // if you have some time, you can do it in a beautiful way.
            g.drawText(txt, txtX, 0, // converting float to int, possible loss of data
                       width, height,
                       juce::Justification::centredLeft);
        }

        void AddFoodPanel::FoodTable::paintListBoxItem(int rowNumber,
                                                       juce::Graphics &g,
                                                       int /*width*/, int /*height*/,
                                                       bool rowIsSelected) {

            static auto constexpr color = 0XFF096AB1;

            if (rowIsSelected) {
                g.fillAll(juce::Colour{ color });
                selectedRow = rowNumber;
            }
        }

        void AddFoodPanel::FoodTable::clear() {
            entries_.clear();
        }

        void AddFoodPanel::FoodTable::addFoodResult(SmartPointer &&food) {
            entries_.push_back(std::move(food));
            updateContent();
        }

        const FoodInterface *AddFoodPanel::FoodTable::getSelection() {
            if (selectedRow >= entries_.size()) {
                return nullptr;
            }

            return entries_.at(selectedRow).get();
        }

        // TODO: The mapping of columns to foodInterface content should be in one place.
        String AddFoodPanel::FoodTable::toString(const value_type &food,
                                                 const NutrientTable &table, int column) const {
            switch (column) {
                case 1 : return food.getText();
                case 2 : return pl::to<String>(table.getNutrient(fa::NutrientType::Kcal).second);
                case 3 : return pl::to<String>(table.getNutrient(fa::NutrientType::Protein).second);
                case 4 : return pl::to<String>(table.getNutrient(fa::NutrientType::Fat).second);
                case 5 : return pl::to<String>(table.getNutrient(fa::NutrientType::Carbohydrates).second);
                default :
                    throw NotYetImplementedException{ std::string{ "Some column value in "
                            "toString(const value_type &food, int column)"
                            "is not supported. Column: " }
                        + pl::to<std::string>(column)
                    };
            }
        }

        void AddFoodPanel::FoodTable::listBoxItemClicked(int /*row*/, const juce::MouseEvent &/*event*/) {
            // TODO: to be implemented
        }

        AddFoodPanel::MiddlePanel::MiddlePanel() {
            addAndMakeVisible(foodTable_);
            foodTable_.getHeader().addColumn("Name", 1, 80); // TODO: make constants out of magic numbers
            foodTable_.getHeader().addColumn("kcal", 2, 80);
            foodTable_.getHeader().addColumn("Protein", 3, 140);
            foodTable_.getHeader().addColumn("Fat", 4, 70);
            foodTable_.getHeader().addColumn("Carbs", 5, 120);
            foodTable_.getHeader().setStretchToFitActive(true);
            foodTable_.setAutoSizeMenuOptionShown(true);
        }

        void AddFoodPanel::MiddlePanel::resized() {
            foodTable_.setBoundsRelative(glo::margin, 0, 1.0f - glo::margin * 2.0f, glo::totalSize);
        }

        void AddFoodPanel::MiddlePanel::clear() {
            foodTable_.clear();
        }

        void AddFoodPanel::MiddlePanel::addFoodResult(SmartPointer &&food) {
            foodTable_.addFoodResult(std::move(food));
        }

        const FoodInterface *AddFoodPanel::MiddlePanel::getSelection() {
            return foodTable_.getSelection();
        }

        AddFoodPanel::BottomPanel::BottomPanel(SceneManager &sceneManager, MiddlePanel &context)
            : sceneManager_{ sceneManager }, context_{ context } {

            addAndMakeVisible(cancel_);
            addAndMakeVisible(create_);
            addAndMakeVisible(add_);

            cancel_.setButtonText("Cancel");
            create_.setButtonText("Create");
            add_.setButtonText("Add");

            cancel_.addListener(this);
            create_.addListener(this);
            add_.addListener(this);
        }

        void AddFoodPanel::BottomPanel::resized() {
            auto const bottomButtonW = 0.10f;
            auto const bottomButtonH = glo::totalSize - glo::margin * 2.0f;
            auto const firstBottomX = 1.0f - glo::margin - bottomButtonW;

            add_.setBoundsRelative(firstBottomX - (glo::margin + bottomButtonW) * 0, glo::margin, bottomButtonW, bottomButtonH);
            create_.setBoundsRelative(firstBottomX - (glo::margin + bottomButtonW) * 1, glo::margin, bottomButtonW, bottomButtonH);
            cancel_.setBoundsRelative(firstBottomX - (glo::margin + bottomButtonW) * 2, glo::margin, bottomButtonW, bottomButtonH);
        }

        void AddFoodPanel::BottomPanel::buttonClicked(juce::Button *clickedButton) {
            if (clickedButton == &cancel_) {
                sceneManager_.toPreviousScene();

            } else if (clickedButton == &create_) { } else if (clickedButton == &add_) {
                if (context_.getSelection() != nullptr) {
                    startDialogWindow();
                }
            }
        }

        void AddFoodPanel::BottomPanel::startDialogWindow() { // TODO: FIXME: doesn't work
            juce::DialogWindow::LaunchOptions launchOptions{ };
            launchOptions.dialogTitle = "Choose the amount";
            launchOptions.escapeKeyTriggersCloseButton = true;
            launchOptions.resizable = true;
            launchOptions.componentToCentreAround = &sceneManager_.getPreviousScene(0);
            p_ = std::make_unique<AddFoodDialogPanel>(sceneManager_,
                                                      *context_.getSelection()); // something needs to be selected. Otherwise -> error
            launchOptions.content.setNonOwned(p_.get());

            launchOptions.launchAsync();
        }
    } // END of namespace gui
} // END of namespace fa
