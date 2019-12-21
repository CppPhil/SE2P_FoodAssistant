#pragma once
#include "GlobalHeader.h"
#include "Nutrient.h" // fa::NutrientType
#include <mylibs/container.hpp> // pl::HashMap
#include <utility> // std::pair
#include <initializer_list> // std::initializer_list
#include <iterator> // std::inserter

namespace fa {
    //! a table of nutrients
    class NutrientTableImpl final {
    public:
        using this_type = NutrientTableImpl;
        using key_type = NutrientType;
        using value_type = Nutrient;
        using const_pointer = value_type const *;
        using NutrientAmount = Amount;
        using container_type = pl::HashMap<const_pointer, NutrientAmount>;
        using pair = container_type::value_type;

        // contains all the nutrients; to be used as a static member of NutrientTableImpl
        class NutrientPool final {
        public:
            using this_type = NutrientPool;
            using container_type = pl::HashMap<key_type, value_type>;

            NutrientPool(std::initializer_list<value_type>) noexcept;

            const_pointer getNutrient(key_type nutrientType) const;

            friend OStream &operator<<(OStream &os, this_type const &obj);

        private:
            container_type container_;
        }; // END of class NutrientPool

        NutrientTableImpl() noexcept;

        /* construct from a list of NutrientType enumerators and the respective amount of that nutrient,
           Do not put more than one of each NutrientType in here! */
        NutrientTableImpl(std::initializer_list<std::pair<key_type, NutrientAmount>>);

        this_type &operator=(this_type const &) = delete;
        this_type &operator=(this_type &&) = delete;

        NutrientTableImpl(this_type const &) = default;
        NutrientTableImpl(this_type &&) = default;

        //! adds two NutrientTableImpls; returns the new NutrientTableImpl
        friend this_type operator+(this_type const &, this_type const &);
        friend this_type operator-(this_type const &, this_type const &);
        //! multiplies the amounts of a NutrientTableImpl and returns the resulting NutrientTableImpl
        friend this_type operator*(this_type const &, Amount::value_type);
        friend this_type operator/(this_type const &, Amount::value_type);

        //! creates a new NutrientTableImpl consisting just of the Macro nutrients
        this_type getMacroNutrients() const; // creates an empty container_type, copy_ifs the
        this_type getMicroNutrients() const; // macro/micro stuff creates a new this_type by
                                             // move constructing it from that container_type just
                                             // created using that private constructor down below

        //! returns a pair of a pointer to the nutrient requested and the corresponding amount of this table
        pair getNutrient(key_type nutrientType) const;

        //! does some unknown thing in mysterious ways
        pl::HashSet<key_type, const_pointer> getPercentages() const; // TODO: Implement this; I have no idea how to. I reckon the return type is incorrect too.

        //! prints a NutrientTableImpl to an OStream
        friend OStream &operator<<(OStream &os, this_type const &obj);

        friend bool operator==(this_type const &lhs, this_type const &rhs) noexcept;
        friend bool operator!=(this_type const &lhs, this_type const &rhs) noexcept;

    private:
        //! creates a new NutrientTableImpl that only contains the nutrients that satisfy the UnaryPredicate
        template <class UnaryPredicate>
        this_type createFilteredTable(UnaryPredicate &&filter) const {
            container_type newContainer{ };

            pl::copy_if(cont_, std::inserter(newContainer, std::begin(newContainer)), [&filter](auto &&pair) {
                return filter(*(pair.first));
            });

            return this_type{ std::move(newContainer) };
        }
        
        /*! creates a new NutrientTableImpl that contains all the nutrients of lhs and rhs;
            if a nutrient from rhs exists in lhs the 
            Amount used for that nutrient is 
            the result of the BinaryOp 
            applied to the Amount in lhs and rhs for the nutrient in question.*/
        template <class BinaryOp>
        static this_type insertOrBinaryOpAssign(this_type const &lhs, this_type const &rhs, BinaryOp &&binaryOp) {
            auto retVal = lhs;

            for (auto &&e : rhs.cont_) {
                auto it = retVal.cont_.find(e.first);
                if (it == std::end(retVal.cont_)) { // not found
                    retVal.cont_.insert(e);
                } else { // found
                    it->second = binaryOp(it->second, e.second);
                }
            }

            return retVal;
        }

        //! creates a new NutrientTableImpl that is a copy of table having had UnaryOp applied to its amounts.
        template <class UnaryOp> 
        static this_type unaryOpTheAmounts(this_type const &table, UnaryOp &&unaryOp) {
            auto newContainer = table;

            for (auto &&e : newContainer.cont_) {
                e.second = unaryOp(e.second);
            }

            return newContainer;
        }

        //! the NutrientPool that contains all nutrients.
        static NutrientPool const nutrientPool;

        explicit NutrientTableImpl(container_type &&); // construct from an rvalue HashMap

        container_type cont_;
    }; // END of class NutrientTableImpl
} // END of namespace fa
