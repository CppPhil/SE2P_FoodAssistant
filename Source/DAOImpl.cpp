#include "DAOImpl.h"
#include <mylibs/container.hpp> // pl::replaceSubstring
#include <utility> // std::move

namespace fa {
    namespace db = Poco::Data;
    namespace dbkw = db::Keywords;

    MySQLPersonDAO::MySQLPersonDAO(DBSession &dbSession)
        : session_{ dbSession } { }

    bool MySQLPersonDAO::insertPerson(value_type &person) const {
        // check argument in parameter

        Statement insert{ session_ };
        insert << STR("INSERT INTO Person VALUES(?, ?, ?)") ,
            dbkw::use(person.name) ,
            dbkw::use(person.address) ,
            dbkw::use(person.age);

        auto retVal{ insert.execute() };
        return retVal != 0; // it's 0 if no rows were affected
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool MySQLPersonDAO::deletePerson(value_type &) const {
        // TODO: implement this
        throw std::exception{ "MySQLPersonDAO::deletePerson in file DAOImpl.cpp is not implemented" };
    }

    MySQLPersonDAO::container_type MySQLPersonDAO::findPerson(decltype(value_type::name) primaryKey) const {
        container_type cont{ };
        value_type person{ };
        auto query{ STR("SELECT Name, Address, Age FROM Person WHERE Name=") };
        query += STR("'");
        query += primaryKey;
        query += STR("'");

        Statement select{ session_ };
        select << query ,
            dbkw::into(person.name) ,
            dbkw::into(person.address) ,
            dbkw::into(person.age) ,
            dbkw::range(0, 1); // iterate over result set one row at a time

        while (!select.done()) {
            select.execute();
            cont.push_back(person);
        }
        return cont;
    } // END of function MySQLPersonDAO::container_type MySQLPersonDAO::findPerson(decltype(value_type::name) primaryKey)

    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool MySQLPersonDAO::updatePerson(value_type &) const {
        // TODO: implement this
        throw std::runtime_error{ "bool MySQLPersonDAO::updatePerson(value_type const &) const "
            "in file DAOImpl.cpp is not yet implemented" };
    }

    MySQLFoodDAO::MySQLFoodDAO(DBSession &dbSession)
        : dbSession_{ dbSession } { }

    MySQLFoodDAO::container_type MySQLFoodDAO::findFood(String searchItem) const {
        using Integer = int;
        using Real = float;

        String Shrt_Desc{ };
        Integer NDB_No{ };
        Integer Energ_Kcal{ };
        Integer Calcium{ }; // Why are they Integer instead of Real?
        Integer Magnesium{ };
        Integer Phosphorus{ };
        Integer Potassium{ };
        Real Water{ };
        Real Protein{ };
        Real Lipid_Tot{ };
        Real Carbohydrt{ };
        Real Iron{ };
        Real Zinc{ };
        Real Selenium{ };
        Real Vit_A_RAE{ };
        Real Vit_B1{ };
        Real Vit_B2{ };
        Real Vit_B3{ };
        Real Vit_B6{ };
        Real Vit_B12{ };
        Real Vit_C{ };
        Real Vit_D_mcg{ };
        Real Vit_E{ };
        Real Vit_K{ };

        // create query
        auto generalInfo = STR("Shrt_Desc, NDB_No, Water, Energ_Kcal");
        auto macro = STR("Protein, Lipid_Tot, Carbohydrt");
        auto micro = STR("Vit_A_RAE, Vit_B1, Vit_B2, Vit_B3, Vit_B6, "
            "Vit_B12, Vit_C, Vit_D_mcg, Vit_E, Vit_K, Calcium, "
            "Iron, Magnesium, Phosphorus, Potassium, Zinc, Selenium");

        auto substr = STR(R"(%%" AND Shrt_Desc LIKE "%%)");
        pl::replaceSubstring(searchItem, STR(" "), substr);

        auto query = STR("SELECT ") + generalInfo + STR(",") +
                                      macro + STR(",") + micro +
                                      STR(R"( FROM ABBREV WHERE Shrt_Desc LIKE "%%)") +
                                      searchItem + STR(R"(%%")");
        pl::replaceSubstring(query, STR(R"( AND Shrt_Desc LIKE "%%%%")"), STR(""));
        // end create query

        /*std::cout << query << "\n";*/

        Statement select{ dbSession_ };
        select << query ,
            // generalInfo
            dbkw::into(Shrt_Desc) ,
            dbkw::into(NDB_No) ,
            dbkw::into(Water) ,
            dbkw::into(Energ_Kcal) ,

            // macro
            dbkw::into(Protein) ,
            dbkw::into(Lipid_Tot) ,
            dbkw::into(Carbohydrt) ,

            // micro
            dbkw::into(Vit_A_RAE) ,
            dbkw::into(Vit_B1) ,
            dbkw::into(Vit_B2) ,
            dbkw::into(Vit_B3) ,
            dbkw::into(Vit_B6) ,
            dbkw::into(Vit_B12) ,
            dbkw::into(Vit_C) ,
            dbkw::into(Vit_D_mcg) ,
            dbkw::into(Vit_E) ,
            dbkw::into(Vit_K) ,
            dbkw::into(Calcium) ,
            dbkw::into(Iron) ,
            dbkw::into(Magnesium) ,
            dbkw::into(Phosphorus) ,
            dbkw::into(Potassium) ,
            dbkw::into(Zinc) ,
            dbkw::into(Selenium) ,

            dbkw::range(0, 1); // iterate over result set one row at a time

        container_type container;


        //auto &dbgOstream = std::cout;
        while (!select.done()) {
            select.execute();

            if (!Shrt_Desc.empty()) {
                Amount kcal{ static_cast<long double>(Energ_Kcal) };
                Amount water{ Water };
                Amount protein{ Protein };
                Amount fat{ Lipid_Tot };
                Amount carbs{ Carbohydrt };

                Amount vita{ Vit_A_RAE };
                Amount vitc{ Vit_C };
                Amount vitk{ Vit_K };
                Amount vite{ Vit_E };
                Amount vitd{ Vit_D_mcg };
                Amount vitb1{ Vit_B1 };
                Amount vitb2{ Vit_B2 };
                Amount vitb3{ Vit_B3 };
                Amount vitb6{ Vit_B6 };
                // Amount vitb9{ Vit_B9 }; ??? missing
                Amount vitb12{ Vit_B12 };

                // TODO: Add calcium and his friends
                AtomicFoodImpl currentFood{
                    Shrt_Desc,
                    std::make_pair(NutrientType::Kcal, kcal),
                    std::make_pair(NutrientType::Water, water),
                    std::make_pair(NutrientType::Protein, protein),
                    std::make_pair(NutrientType::Fat, fat),
                    std::make_pair(NutrientType::Carbohydrates, carbs),
                    std::make_pair(NutrientType::VitaminA, vita),
                    std::make_pair(NutrientType::VitaminC, vitc),
                    std::make_pair(NutrientType::VitaminK, vitk),
                    std::make_pair(NutrientType::VitaminE, vite),
                    std::make_pair(NutrientType::VitaminD, vitd),
                    std::make_pair(NutrientType::VitaminB1, vitb1),
                    std::make_pair(NutrientType::VitaminB2, vitb2),
                    std::make_pair(NutrientType::VitaminB3, vitb3),
                    std::make_pair(NutrientType::VitaminB6, vitb6),
                    std::make_pair(NutrientType::VitaminB12, vitb12),
                }; // END AtomicFoodImpl initialization

                container.push_back(std::move(currentFood));
            } // END if (!Shrt_Desc.empty())
        } // END while(!select.done())

        //dbgOstream << std::flush;

        return container;
    } // END of function MySQLFoodDAO::findFood

    //###########################################################################################################
    //		MySQLNoteDAO

    MySQLNoteDAO::MySQLNoteDAO(DBSession &dbSession)
        : session_{ dbSession } { }

    MySQLNoteDAO::container_type MySQLNoteDAO::findNote(String searchTerm) { // TODO: implement this
        throw NotYetImplementedException{ "MySQLNoteDAO::findNote has not yet been implemented" };
    }

    bool MySQLNoteDAO::insertNote(value_type note, Id dailyPlanId) const {
        auto nullStr = STR("NULL");
        auto noteTxt = note.getText();

        Statement insert{ session_ };
        insert << STR("INSERT INTO Notiz VALUES(?,?,?)") ,
            dbkw::use(nullStr) ,
            dbkw::use(noteTxt) ,
            dbkw::use(dailyPlanId);

        auto retVal{ insert.execute() };
        return retVal != 0;
    }


    bool MySQLNoteDAO::deleteNote(value_type note) const {
        Statement deleteQuery{ session_ };
        auto query{ STR("DELETE FROM Notiz WHERE nzId=") };
        query += STR("'");
        query += pl::to<fa::String>(note.getId());
        query += STR("'");

        deleteQuery << query;

        auto retVal = deleteQuery.execute();
        return retVal != 0;
    }

    bool MySQLNoteDAO::updateNote(value_type note) const {

        Statement update{ session_ };
        auto query{ STR("UPDATE Notiz SET beschreibung = ") };
        query += note.getText();

        update << query;

        auto retVal = update.execute();
        return retVal != 0;

    }

    //###########################################################################################################
    //	MySQLEntryDAO

    MySQLEntryDAO::MySQLEntryDAO(DBSession &dbSession)
        : session_{ dbSession } { }

    bool MySQLEntryDAO::insertEntry(value_type_Food &/*food*/, value_type_Amount &/*amount*/,
                                    value_type_Person &person) const {
        Statement insert{ session_ };
        auto protoAmount = 1;

        auto defaultStr = STR("DEFAULT");
        auto selectStr = STR("(select NDB_No from ABBREV where Shrt_Desc = 'BUTTER,WITH SALT')");

        insert << STR("INSERT INTO Eintrag VALUES(?,?,?,?)") ,
            dbkw::use(defaultStr) ,
            dbkw::use(selectStr) , //TODO: replace String by food.getID
            dbkw::use(protoAmount) , //TODO: get amount
            dbkw::use(person); //TODO: get ID

        auto retVal{ insert.execute() };
        return retVal != 0;
    }
} // END of namespace fa
