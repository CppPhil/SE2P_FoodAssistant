#pragma once
#include "TestDB.h"
#pragma warning(push)
#pragma warning(disable:4100) // unreferenced formal parameter
#pragma warning(disable:4127) // conditional statement is const
#pragma warning(disable:4244) // type conversion: possible data loss
#pragma warning(disable:4245) // signed/unsigned mismatch
#include <Poco/Data/Session.h> // Poco::Data::Session
#pragma warning(pop)
#include "NoteImpl.h"
#include "FoodImpl.h"
#include <utility>
#include "Amount.h"

namespace fa {
    //! Person DAO for MySQL
    class MySQLPersonDAO final {
    public:
        using this_type = MySQLPersonDAO;
        using value_type = testCrap::Person;
        using DBSession = Poco::Data::Session;
        using Statement = Poco::Data::Statement;
        using container_type = std::vector<value_type>;

        explicit MySQLPersonDAO(DBSession &dbSession);

        /* consider using pass by value instead if the type is cheap enough to copy
           the parameter may not be const, Poco has some nasty macro hidden in Poco::Data::Keywords::use
           that will abort compilation if used with const qualified arguments. */
        bool insertPerson(value_type &) const;
        bool deletePerson(value_type &) const;
        container_type findPerson(decltype(value_type::name)) const;
        bool updatePerson(value_type &) const;

    private:
        DBSession &session_;

    }; // END of class MySQLPersonDAO

    //! Food DAO for MySQL
    class MySQLFoodDAO final {
    public:
        using this_type = MySQLFoodDAO;
        using value_type = AtomicFoodImpl;
        using DBSession = Poco::Data::Session;
        using Statement = Poco::Data::Statement;
        using container_type = std::vector<value_type>;

        explicit MySQLFoodDAO(DBSession &dbSession);

		// insert a string of one or more keywords (e.g. "butter salt")
        container_type findFood(String searchItem) const; // TODO: may need to use different types.

        // TODO: this probably needs to be able to do more than just find food.

    private:
        DBSession &dbSession_;
    }; // END of class MySQLFoodDAO

	//! Note DAO for MySQL
	class MySQLNoteDAO final {
	public:
		using this_type = MySQLNoteDAO;
		using value_type = NoteImpl;
		using DBSession = Poco::Data::Session;
		using Statement = Poco::Data::Statement;
		using container_type = std::vector<value_type>;

		explicit MySQLNoteDAO(DBSession &dbSession);

        container_type findNote(fa::String searchTerm); // TODO: this might have to be different
		bool insertNote(value_type, Id) const;
		bool deleteNote(value_type) const;
		bool updateNote(value_type) const;

	private:
		DBSession &session_;
	}; // END of class MySQLNoteDAO

	//! Entry DAO for MySQL
	class MySQLEntryDAO final { // TODO: test this class
	public:
		using this_type = MySQLEntryDAO;
		using value_type_Food = AtomicFoodImpl;
		using value_type_Amount = Amount;
		using value_type_Person = int; // TODO: replace by Person dataType (or by Tagesplan in Project and MySQL)
		using DBSession = Poco::Data::Session;
		using Statement = Poco::Data::Statement;
		//using container_type = std::vector<value_type>;

		explicit MySQLEntryDAO(DBSession &dbSession);

		bool insertEntry(value_type_Food &, value_type_Amount &, value_type_Person &) const;

	private:
		DBSession &session_;
	};

} // END of namespace fa
