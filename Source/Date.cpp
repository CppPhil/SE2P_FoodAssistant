#include "Date.h"
#include <mylibs/utility.hpp> // pl::to
#include "Utils.h" // fa::utils::makeClassString

namespace fa {
    Date::Date()
        : Date{ defaultMonth, defaultDay, defaultYear } { }

    Date::Date(Month month, unsigned day, unsigned year) {
        stringInit(month, day, year);
    }

    Date::Date(pl::TimeStruct timeStruct)
        : Date{ static_cast<Month>(timeStruct.month),
                static_cast<unsigned>(timeStruct.day),
                static_cast<unsigned>(timeStruct.year) } { }

    Date Date::today() {
        return Date{ pl::getDateTime() };
    }

    OStream &operator<<(OStream &os, Date::this_type const &date) {
        return os << utils::makeClassString(STR("Date"), STR("date_"),
                                            boost::gregorian::to_simple_string(date.date_));
    }

    bool operator==(Date::this_type const &lhs, Date::this_type const &rhs) {
        return lhs.date_ == rhs.date_;
    }

    bool operator<(Date::this_type const &lhs, Date::this_type const &rhs) {
        return lhs.date_ < rhs.date_;
    }

    Date::Month const Date::defaultMonth = Month::January;

    unsigned const Date::defaultDay = 1U;

    unsigned const Date::defaultYear = 1970U;

    void Date::stringInit(Month month, unsigned day, unsigned year) {
        static auto constexpr delim = '-';
        auto dateStr = pl::to<std::string>(year) + delim + 
                       pl::to<std::string>(static_cast<int>(month)) + delim + 
                       pl::to<std::string>(day);

        date_ = boost::gregorian::from_simple_string(dateStr);
    }


} // END of namespace fa
