#pragma once
#include "GlobalHeader.h" // fa::String
#include <stdexcept> // std::logic_error, std::runtime_error
#include <string> // std::string
#include <gtest/gtest.h> // ::testing::internal::FloatingPoint
#include <mylibs/utility.hpp> // pl::Uinteger
#include <limits> // std::numeric_limits
#include <type_traits> // std::is_same

namespace fa {
    namespace utils {
        namespace detail {
            template <size_type>
            struct AddDelimiter final {
                static void func(OStringStream &ostr) {
                    ostr << ", ";
                }
            };

            template <>
            struct AddDelimiter<static_cast<size_type>(0U)> final {
                static void func(OStringStream &) { }
            };

            template <class ...Args>
            void makeClassStringPairHelper(OStringStream &ostr) {
                ostr << ')';
            }

            template <class StringType, class ValueType, class ...Args>
            void makeClassStringPairHelper(OStringStream &ostr,
                                           StringType const &str,
                                           ValueType const &val,
                                           Args &&...args) {
                
                ostr << str << ": " << val;
                AddDelimiter<sizeof...(Args)>::func(ostr);
                makeClassStringPairHelper(ostr, std::forward<Args>(args)...);
            }

        } // END of namespace detail

        /*! generates strings for the operator<< functions that print all defined classes
        in the following format: ClassName(member1name: member1value, ...)
        for instance: Nutrient(name: Vitamin A, ...)
        this function is to be called by operator<< functions so that they can print their argument
        the arguments shall be the class' name (className parameter)
        followed by (logical, not actual) two element tuples
        of each members identifier (as an fa::String)
        and that member's value. Note that the value must be printable.
        */
        template <class ...Args> /* recursive templates are slow to compile
                                 and increase executable size significantly; possible replace this with much
                                 more clever trickery at some point */
        String makeClassString(String const &className, Args &&...args) {
            OStringStream ostringstream{ };
            ostringstream << className << '(';
            detail::makeClassStringPairHelper(ostringstream, std::forward<Args>(args)...);
            return ostringstream.str();
        }

        namespace floating_point {
            template <class Type>
            bool almostEqual(Type first, Type second) { // doesn't support long double
                using TypeSet = brigand::set<double, float>;
                static_assert(brigand::contains<TypeSet, Type>::value, "Type in almostEqual was neither double nor float");
                using TypeName = ::testing::internal::FloatingPoint<decltype(first)>;
                TypeName firstVal{ first };
                TypeName secondVal{ second };
                return firstVal.AlmostEquals(secondVal);
            }
        } // END of namespace floating_point

    } // END of namespace utils

    class DivideByZeroException : public std::logic_error {
    public:
        using this_type = DivideByZeroException;

        explicit DivideByZeroException(std::string const &message);

        virtual ~DivideByZeroException() override;

        virtual char const *what() const noexcept override;
    }; // END of class DivideByZeroException

    class NotYetImplementedException : public std::runtime_error {
    public:
        using this_type = NotYetImplementedException;

        explicit NotYetImplementedException(std::string const &message);

        virtual ~NotYetImplementedException() override;

        virtual char const *what() const noexcept override;
    }; // END of class NotYetImplementedException

    class IdOverflowException : public std::runtime_error {
    public:
        using this_type = IdOverflowException;

        explicit IdOverflowException(std::string const &message);

        virtual ~IdOverflowException() override;

        virtual char const *what() const noexcept override;
    }; // END of class IdOverflowException

    namespace detail {
        static auto constexpr idByteSize = 8;
    } // END of namespace detail

    using Id = pl::Uinteger<detail::idByteSize>;

    namespace detail {
        inline Id generateFoodIdImpl() {
            static Id currentId{ };
            if (currentId == std::numeric_limits<Id>::max()) {
                throw IdOverflowException{ "Id overflow in generateFoodIdImpl, too many Ids generated for this type" };
            }
            return currentId++;
        }
    } // END of namespace detail

    class AtomicFoodImpl;
    class CompoundFoodImpl;

    template <class TypeToGenerateIdFor>
    Id generateId() {
        if (std::is_same<TypeToGenerateIdFor, AtomicFoodImpl>::value || std::is_same<TypeToGenerateIdFor, CompoundFoodImpl>::value) {
            return detail::generateFoodIdImpl();
        }

        static Id currentId{ };

        if (currentId == std::numeric_limits<Id>::max()) {
            throw IdOverflowException{ "Id overflow in generateId, too many Ids generated for this type" };
        }

        return currentId++;
    }

    //! turns all characters into lowercase except for the first character of a word, a word is any amount of characters following a space
    void wordify(String &);
    String wordifyCopy(String);

} // END of namespace fa
