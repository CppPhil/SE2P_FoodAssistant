#include "../JuceLibraryCode/JuceHeader.h"
#include "FoodPlan.h"
#include <memory> // std::unique_ptr, std::make_unique
#include <Poco/Data/DataException.h> // Poco::Data::ConnectionFailedException
#include <iostream> // std::cerr
#include "GlobalHeader.h"
#include <mylibs/utility.hpp> // HANDLE_EXCEPTIONS
#include <cstdlib> // std::exit, EXIT_FAILURE
#ifdef _WIN32
#   include <WinUser.h> // ::MessageBoxA
#endif
#include "TestBoost.h"
#include "TestDB.h"
#include "DBConnectionManager.h"
#include "Amount.h"
#include "NutrientImpl.h"
#include "NutrientTableImpl.h"

namespace win_terminate {
    inline void msgBox(std::string const &title, std::string const &text) noexcept {
#ifdef _WIN32
        ::MessageBoxA(nullptr, text.data(), title.data(), MB_OK | MB_ICONERROR);
#else
        std::cerr << "win_terminate::msgBox is not implemented for this platform.\n";
#endif
    }
}

class SE2P_FoodAssistantApplication : public juce::JUCEApplication {
public:
	SE2P_FoodAssistantApplication() 
		: dbManager_(STR("141.22.29.117"), 3306U, STR("db_se2")) { }

    virtual juce::String const getApplicationName() override {
        return ProjectInfo::projectName;
    }

    virtual juce::String const getApplicationVersion() override {
        return ProjectInfo::versionString;
    }

    virtual bool moreThanOneInstanceAllowed() override {
        return true;
    }

#pragma warning(push)
#pragma warning(disable:4100) // unreferenced formal parameter
    virtual void initialise(juce::String const &commandLine) override {
#pragma warning(pop)
        // This method is where you should put your application's initialisation code...

        auto terminateMsg{ STR("terminating program\n") };
#ifdef TERMINATE
        static_assert(false, "TERMINATE was already defined in Main.cpp");
#endif
#define TERMINATE   std::cerr << terminateMsg; \
                    win_terminate::msgBox("FoodAssistant: "\
                                          "UNHANDLED EXCEPTIONS",\
                                          "There were unhandled exceptions.\n"\
                                          "The application will now terminate.\n"\
                                          "Check the console for details."); \
                    std::exit(EXIT_FAILURE)

        try {
            // TODO: this shit doesn't work.
			// auto ptr = std::make_unique<fa::gui::FoodPlanPanel>(sceneManager);

            //mainWindow_ = std::make_unique<MainWindow>(getApplicationName(), ptr.get());

            //sceneManager.init(mainWindow_.get());            

            // sceneManager.toNextScene(std::move(ptr));
            //ptr = nullptr;

           // mainWindow_->centreWithSize(mainWindow_->getWidth(), mainWindow_->getHeight());
			//auto maker = fa::MySQLDAOMaker::valueOf("DESKTOP-J06MH3F", 3306, "MySQL57");
			//auto NutrientTable


			
			//DBG(foodDao.findFood(STR("Apple")).front);


            mainWindow_ = std::make_unique<MainWindow>(getApplicationName());
            sceneManager_.init(mainWindow_.get());

			auto ptr = std::make_unique<fa::gui::FoodPlanPanel>(sceneManager_, dbManager_);
			sceneManager_.toNextScene(std::move(ptr));
            mainWindow_->centreWithSize(mainWindow_->getWidth(), mainWindow_->getHeight());

			/*
			fa::DBConnectionManager dbManager(dbHost, dbPort, dbName);
			auto foodDao = dbManager.getFoodDAO();
			
			// works with butter salt; thanks meinhardt
			std::string thisIsJustAStringForNow{ STR("butter salt") }; // TODO: use a real food type here

			auto resFoods = foodDao.findFood(thisIsJustAStringForNow); // TODO: returns an empty container for now; make it work somehow.

			std::cout << STR("resFoods: \n");
			for (auto &&e : resFoods) {
				std::cout << (*e).getText() << '\n';
			}

			//pl::printContainer(resFoods);
			*/
        } catch (Poco::Data::ConnectionFailedException const &ex) { // handle exceptions
            std::cerr << STR("Poco::Data::ConnectionFailedException caught: ")
                << ex.what() << STR("\n");
            TERMINATE;
        } catch (...) {
            try {
                throw;
            } HANDLE_EXCEPTIONS(\0);
            TERMINATE;
        }
    } // END of member function initialise

    void shutdown() override {
        // Add your application's shutdown code here..
        mainWindow_ = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }
#pragma warning(push)
#pragma warning(disable:4100) // unereferenced formal parameter
    void anotherInstanceStarted(juce::String const &commandLine) override {
#pragma warning(pop)
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    /*
    This class implements the desktop window that contains an instance of
    our MainContentComponent class.
    */
    class MainWindow : public juce::DocumentWindow {
    public:
        explicit MainWindow(juce::String const &name)
            : DocumentWindow{ name, juce::Colours::lightgrey, allButtons } {//,
              //sceneManager_{ mngr }, 
			//pPanel_{ std::make_unique<fa::gui::FoodPlanPanel>(sceneManager_) } {

            //setContentComponent(pFoodPanel_, true, true);
            //addAndMakeVisible(pFoodPanel_);
            setUsingNativeTitleBar(true);
            //setContentNonOwned(pPanel_.get(), true);

			//sceneManager_.toNextScene(std::move(pPanel_)); // this doesn't work
           // pPanel_ = nullptr;
            
            centreWithSize(getWidth(), getHeight());
            Component::setVisible(true);
            setResizable(true, true);

            // stuff begin
            testCrap::boostTestFunc();
            testCrap::testDB();
            // stuff end
        }

        virtual void closeButtonPressed() override {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
        class uses a lot of them, so by overriding you might break its functionality.
        It's best to do all your work in your content component instead, but if
        you really have to override any DocumentWindow methods, make sure your
        subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    }; // END of class MainWindow

private:
    std::unique_ptr<MainWindow> mainWindow_;
    fa::gui::SceneManager sceneManager_;
	/*
	auto dbHost{ STR("141.22.29.117") }; // put correct IP address here
	auto dbPort{ 3306U }; // put correct Port here
	auto dbName{ STR("db_se2") }; // put correct database to use here
	*/
	fa::DBConnectionManager dbManager_;
}; // END of class SE2P_FoodAssistantApplication

   //==============================================================================
   // This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(SE2P_FoodAssistantApplication)
