#include <string> // std::string
#include "DAOMakersImpl.h"
#pragma warning(push)
#pragma warning(disable:4100) // unreferenced formal parameter
#pragma warning(disable:4127) // conditional statement is const
#pragma warning(disable:4244) // type conversion: possible data loss
#pragma warning(disable:4245) // signed/unsigned mismatch
#pragma warning(disable:4251) // "Poco::Data::MySQL::Connector::KEY": class "std::basic_string<char,std::char_traits<char>,std::allocator<char>>" erfordert eine DLL - Schnittstelle, die von Clients von class "Poco::Data::MySQL::Connector"
#include <Poco/Data/MySQL/Connector.h> // Poco::Data::MySQL::Connector
#pragma warning(pop)
#include "DAOImpl.h" // fa::MySQLPersonDAO, fa::MySQLFoodDAO
#include <iostream> // std::cerr
#include <mylibs/detail/custom_casts.hpp> //pl::to
#include <utility> // std::move

namespace fa {
    namespace dbkw = db::Keywords;

    MySQLDAOMaker::this_type MySQLDAOMaker::valueOf(String host, unsigned port, String dbName) try {
        static auto created{ false };
        if (!created) {
            // register MySQL connector
            db::MySQL::Connector::registerConnector();
        }
        MySQLDAOMaker instance{ std::move(host), std::move(port), std::move(dbName) };
        created = true;
        return instance;
    } catch (Poco::Data::ConnectionFailedException const &exp) {
        std::cerr << STR("Poco::Data::ConnectionFailedException caught in MySQLDAOMaker::valueOf\n"
                         "in DAOMakersImpl.cpp:\n")
                     << exp.what() << STR("\nAre you connected to the VPN?\n");
        throw; // rethrow
    }

    PersonDAO MySQLDAOMaker::createPersonDAO() {
        pl::value_type<PersonDAO> personImpl{ session_ };
        return PersonDAO{ personImpl };
    }

    FoodDAO MySQLDAOMaker::createFoodDAO() {
        pl::value_type<FoodDAO> foodImpl{ session_ };
        return FoodDAO{ foodImpl };
    }

	NoteDAO MySQLDAOMaker::createNoteDAO() {
        pl::value_type<NoteDAO> noteImpl{ session_ };
		return NoteDAO{ noteImpl };
	}
    
    MySQLDAOMaker::MySQLDAOMaker(String host, unsigned port, String dbName)
        : loginInfo_{ getLoginInfo() }, dbUserName_{ loginInfo_.userName },
          dbPassWord_{ loginInfo_.password }, dbHost_{ std::move(host) },
          dbPort_{ pl::to<String>(port) }, dbName_{ std::move(dbName) },
          dbTitle_{ STR("MySQL") },
          dbConnectString_{ STR("host=")
              + dbHost_ + STR(";")
                + STR("port=") + dbPort_ + STR(";") + STR("db=")
                + dbName_
                + STR(";") + STR("user=") + dbUserName_
                + STR(";") + STR("password=") + dbPassWord_ +
                STR(";") +
                STR("compress=true") + STR(";") +
                STR("auto-reconnect=true") },
          session_{ dbTitle_, dbConnectString_ } { }

    MySQLDAOMaker::LoginInfo MySQLDAOMaker::getLoginInfo() {      
        static auto const userName = STR("genericUsername");
        static auto const password = STR("genericPassword");

        return { userName, password };
    } // END of getLoginInfo
} // END of namespace fa
