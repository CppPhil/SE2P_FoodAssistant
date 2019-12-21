#pragma once
#include "DAOs.h"
#pragma warning(push)
#pragma warning(disable:4100) // unreferenced formal parameter
#pragma warning(disable:4127) // conditional statement is const
#pragma warning(disable:4244) // type conversion: possible data loss
#pragma warning(disable:4245) // signed/unsigned mismatch
#include <Poco/Data/Session.h> // Poco::Data::Session
#pragma warning(pop)

namespace fa {
    namespace db = Poco::Data;

    //! creates DAOs for MySQL
    class MySQLDAOMaker final {
    public:
        using this_type = MySQLDAOMaker;

        struct LoginInfo final {
            using value_type = String;
            value_type userName;
            value_type password;
        }; // END of struct LoginInfo

        /*! factory method is necessary because db::MySQL::Connector::registerConnector() has to be called
            before creating an instance of this class, because it contains a session, which
            is not default-constructible */
        static this_type valueOf(String host, unsigned port, String dbName);

        PersonDAO createPersonDAO();
        FoodDAO createFoodDAO();
		NoteDAO createNoteDAO();
    private:
        MySQLDAOMaker(String host, unsigned port, String dbName);

        static LoginInfo getLoginInfo();

        LoginInfo loginInfo_;        
        String const &dbUserName_;
        String const &dbPassWord_;
        String const dbHost_;
        String const dbPort_;
        String const dbName_;
        String const dbTitle_;
        String const dbConnectString_;
        db::Session session_;
    }; // END of class MySQLDAOMaker
} // END of namespace fa
