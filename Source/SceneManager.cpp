#include "SceneManager.h"
#include <string> // std::string
#include <stdexcept> // std::runtime_error
#include <utility> // std::move

namespace fa {
    namespace gui {
        namespace exceptions {
            class IllegalStateException : public std::runtime_error {
            public:
                explicit IllegalStateException(std::string str = "The object has reached an illegal state!")
                    : std::runtime_error{ std::move(str) } { }

                virtual const char* what() const noexcept override {
                    return std::runtime_error::what();
                }
            }; // END of class IllegalStateException
        } // END of namespace exceptions

        // should there really be two step initlization?
		// Yes, because unfortunately our SE2P_FoodAssistantApplication is also two step initialized.
        SceneManager::SceneManager() noexcept
            : window_{ nullptr }, componentTimeline_{ } { }

        void SceneManager::init(juce::DocumentWindow *window) { // what if window == nullptr?
			if (window != nullptr) {
				window_ = window;
			} else {
				throw std::invalid_argument("The given window is not allowed to be nullptr!");
			}
            
        }

        void SceneManager::toNextScene(SmartPointer &&nextComponent) {
            componentTimeline_.push_back(std::move(nextComponent));       

            // TODO: FIXME: this shit fucks shit up.
			window_->setContentNonOwned(componentTimeline_.back().get(), true);
        }

        void SceneManager::toPreviousScene() {
            componentTimeline_.pop_back();
            window_->setContentNonOwned(componentTimeline_.front().get(), true);
        }

        juce::Component &SceneManager::getPreviousScene() {
            return getPreviousScene(1);
        }

        juce::Component &SceneManager::getPreviousScene(int num) { // TODO: FIXME: doesn't work.
            try {
                return *componentTimeline_.at(componentTimeline_.size() - 1 - num);
            }
            catch (const std::out_of_range &e) {
                std::cerr << "There is no previous scene! " << e.what() << '\n';
                throw; // rethrow
            }
        }

    } // END of namespace gui
} // END of namespace fa
