#pragma once
#include <vector> // std::vector
#include <memory> // unique_ptr
#include <exception> // exception
#include "../JuceLibraryCode/JuceHeader.h"

/*
This class is used to navigate through different scenes (== ownedComponents) of
the window. Example: FoodPlanPanel -- (user clicks on "add food") -> AddFoodPanel
-- (user adds a new food) -> FoodPlanPanel

I use this class to get back to the FoodPlanPanel, without hardcoding it in the
FoodPlanPanel class. Futhermore we can reference the last components in the
current component, without passing all the pointers through potentionally many
classes.

It's important to note that we don't use "setContentOwned" in the DocumentWindow,
because the next call of this method (to set the next scene) would delete the old
one and we would have a dangling pointer in our timeline.

@TODO: Exceptions & Tests
*/

namespace fa {
    namespace gui {

        class SceneManager {
        public:
            using SmartPointer = std::unique_ptr<juce::Component>;

            SceneManager() noexcept;

            /*
            This method has to be called before any other method (except the constructor) is used.
            Takes a reference (this pointer) to the mainWindow.
            @throw invalid_argument: The given window hasn't a contentComponent set.
            */
            void init(juce::DocumentWindow *);

            /*
            Stores the old scene and switches to the new one. Since juce is working with raw pointers,
            this method also takes a raw pointer, but takes care of its deletion.
            */
            void toNextScene(SmartPointer &&);

            /*
            Deletes the current scene and switches to the previous one.

            @throw out_of_range exception: There isn't a previous scene.
            */
            void toPreviousScene();

            /*
            Returns the previous scene (without switching to it). It will throw an

            @throw out_of_range: There isn't a previous scene.
            */
            juce::Component &getPreviousScene();

            /*
            Returns a previous scene. The param num specifies which scene will be returned. A num value of
            0 returns the current scene, a 1 corresponds to a getPreviousScene() call (parameterless)

            @throw out_of_range: num is less than 0 or greater than scenes.length - 1.
            */
            juce::Component &getPreviousScene(int num);
        private:
            juce::DocumentWindow *window_;
            std::vector<SmartPointer> componentTimeline_; /* "stack" for backtracking. It's necessary to navigate back to
                                                              the last scenes (== ownedComponents of the window)
                                                              I use an unique pointer, because of the usement of setContentNonOwned. 
															  Because of that, I have to take care of the pointer myself and the 
															  unique ownership is adequate for this due to fact that an instance 
															  of this class will live at least as long as every content component.
                                                              */
        }; // END of class SceneManager
    } // END of namespace gui
} // END of namespace fa
