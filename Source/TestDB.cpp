#include "TestDB.h"
#include "DAOMakers.h"
#include <mylibs/container.hpp> // pl::printContainer
#include "FoodImpl.h"
#include <vector>
#include <iostream>

namespace testCrap {
    std::ostream &operator<<(std::ostream &os, Person const &p) {
        return os << p.name << STR(" ") << p.address << STR(" ") << p.age;
    }
    
    int giveOne() {
        return 1;
    }

    void testDB() {
        auto dbHost{ STR("141.22.29.117") }; // put correct IP address here
        auto dbPort{ 3306U }; // put correct Port here
        auto dbName{ STR("db_se2") }; // put correct database to use here

        fa::DAOMaker daoMaker{ fa::DAOMaker::value_type::valueOf(dbHost, dbPort, dbName) };

        // person stuff is not in Database: "db_se2".

        /*auto personDAO{ daoMaker.createPersonDAO() };
        using person_type = pl::remove_cv_ref_t<decltype(personDAO)>::dao_type;

        auto personName{ STR("Peter") };
        person_type person{ personName, STR("tolle Strasse"), 55 };
        personDAO.insertPerson(person);

        auto matches{ personDAO.findPerson(personName) };
        std::cout << STR("matches: ") << STR("\n");
        pl::printContainer(matches);*/

        // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

        // create concrete food dao
        auto foodDAO = daoMaker.createFoodDAO();
        
        //using food_type = pl::remove_cv_ref_t<decltype(foodDAO)>::dao_type;
        // works with butter salt; thanks meinhardt
        auto thisIsJustAStringForNow = STR("butter salt");
        
        auto resFoods = foodDAO.findFood(thisIsJustAStringForNow); // TODO: returns an empty container for now; make it work somehow.

        std::cout << STR("resFoods: \n");
		std::cout << resFoods.at(0);
        //pl::printContainer(resFoods);

    } // END of function void testDB()
} // END of namespace testCrap 
