#pragma once
#include "GlobalHeader.h" // fa::OStream
#include <boost/date_time/gregorian/gregorian.hpp> // boost::gregorian::date
#include <string> // std::string
#include <boost/operators.hpp> // boost::totally_ordered
#include <mylibs/date_time.hpp> // pl::getDateTime

namespace fa {
    class Date final : private boost::totally_ordered<Date> { // see boost gregorian calendar, perhaps
    public:
        using this_type = Date;
        using value_type = boost::gregorian::date;

        enum class Month {
            January = 1,
            February,
            March,
            April,
            May,
            June,
            July,
            August,
            September,
            October,
            November,
            December,
        }; // END of enum class Month

        Date();

        Date(Month month, unsigned day, unsigned year);

        explicit Date(pl::TimeStruct timeStruct);

        static this_type today();

        friend OStream &operator<<(OStream &, this_type const &);

        friend bool operator==(this_type const &, this_type const &);
        friend bool operator<(this_type const &, this_type const &);
    private:
        static Month const defaultMonth;
        static unsigned const defaultDay;
        static unsigned const defaultYear;

        void stringInit(Month month, unsigned day, unsigned year);

        value_type date_;
    }; // END of class Date
} // END of namespace fa
