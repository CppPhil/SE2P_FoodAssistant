#include "NutrientTableImpl.h"
#include <mylibs/io.hpp> // pl::printContainer
#include "Utils.h" // fa::utils::makeClassString
#include <functional> // std::plus, std::minus
#include <utility> // std::move
#include <mylibs/container.hpp> // pl::eraseIf

namespace fa {
    using namespace literals;

    NutrientTableImpl::NutrientPool::NutrientPool(std::initializer_list<value_type> initL) noexcept {
        for (auto &&e : initL) {
            container_.emplace(e.getNutrientType(),
                               e); // can not move e as evaluation order of function arguments is undefined
        }
    }

    NutrientTableImpl::const_pointer NutrientTableImpl::NutrientPool::getNutrient(key_type nutrientType) const {
        auto it = container_.find(nutrientType);
        if (it == std::end(container_)) {
            throw NoSuchNutrientException{ "Nutrient could not "
                                           "be found in "
                                           "NutrientTableImpl::NutrientPool::getNutrient" };
        }
        return &it->second;
    }

    OStream &operator<<(OStream &os, NutrientTableImpl::NutrientPool::this_type const &obj) {
        OStringStream oStringStream{ };
        pl::printContainer(obj.container_, oStringStream);
        return os << utils::makeClassString(STR("NutrientTableIMpl::NutrientPool"),
            STR("cont_"), oStringStream.str());
    }

    NutrientTableImpl::NutrientTableImpl() noexcept
        : cont_{ } { }
    
    //! Do not put more than one of each NutrientType in here!
    NutrientTableImpl::NutrientTableImpl(std::initializer_list<std::pair<key_type, NutrientAmount>> initL)
        : cont_{ } {

        for (auto &&e : initL) {
            auto pair = cont_.emplace(nutrientPool.getNutrient(e.first),
                                      e.second);

            if (!pair.second) { // if there already was an element with that key
                throw std::logic_error{ "Error in "
                                        "NutrientTableImpl::NutrientTableImpl"
                                        "(std::initializer_list<std::pair<key_type, NutrientAmount>>): "
                                        "there were duplicate Nutrients in the initializer_list." };
            }
        }
    }

    NutrientTableImpl::this_type operator+(NutrientTableImpl::this_type const &lhs,
                                           NutrientTableImpl::this_type const &rhs) {

        using BinaryOperator = std::plus<>;

        return NutrientTableImpl::insertOrBinaryOpAssign(lhs, rhs, BinaryOperator{ });
    }

    NutrientTableImpl::this_type operator-(NutrientTableImpl::this_type const &lhs,
                                           NutrientTableImpl::this_type const &rhs) {

        auto res = lhs;
        
        auto end = std::end(rhs.cont_);
        for (auto &&e : res.cont_) {
            auto it = rhs.cont_.find(e.first);
            if (it != end) { // it was there
                e.second -= it->second;
            }
        }
        
        pl::eraseIf(res.cont_, [](auto &&e) { // if there is less than 0 of some Nutrient -> erase it
            return e.second < 0.0_micg;
        });

        return res;
    }

    NutrientTableImpl::this_type operator*(NutrientTableImpl::this_type const &table,
                                           Amount::value_type multiplier) {

        return NutrientTableImpl::unaryOpTheAmounts(table, [multiplier] (auto &&amount) {
            return amount * multiplier;
        });
    }

    NutrientTableImpl::this_type operator/(NutrientTableImpl::this_type const &table,
                                           Amount::value_type divisor) {

        return NutrientTableImpl::unaryOpTheAmounts(table, [divisor] (auto &&amount) {
            return amount / divisor;
        });
    }

    NutrientTableImpl::this_type NutrientTableImpl::getMacroNutrients() const {
        return createFilteredTable([](auto &&nutrient) {
            return nutrient.isMacro();
        });
    }

    NutrientTableImpl::this_type NutrientTableImpl::getMicroNutrients() const {
        return createFilteredTable([](auto &&nutrient) {
            return nutrient.isMicro();
        });
    }

    NutrientTableImpl::pair NutrientTableImpl::getNutrient(key_type nutrientType) const {
        auto pointer = nutrientPool.getNutrient(nutrientType);
        auto it = cont_.find(pointer);
        if (it == std::end(cont_)) { // it wasn't there
            throw NoSuchNutrientException{ "Nutrient could "
                                           "not be found in "
                                           "NutrientTableImpl::"
                                           "getNutrient(key_type) const" };
        }

        return *it;
    }

    // TODO: implement this function
    pl::HashSet<NutrientTableImpl::key_type, NutrientTableImpl::const_pointer>
    // ReSharper disable once CppMemberFunctionMayBeStatic
        NutrientTableImpl::getPercentages() const {


		
        throw NotYetImplementedException{ "NutrientTableImpl::getPercentages() const "
                                          "has not yet been implemented" };
										  
    }
    
    OStream &operator<<(OStream &os, NutrientTableImpl::this_type const &obj) {
        static auto constexpr charsToRemoveAtTheEnd = 2;

        OStringStream oStringStream{ }; /* style: [(5, "hi"), (7, "sup"), (9, "lol")] */
        
        oStringStream << '[';

        for (auto &&e : obj.cont_) {
            oStringStream << '('
                << *e.first
                << ", "
                << e.second
                << "), ";
        }

        auto mapString = oStringStream.str();
        mapString.erase(std::end(mapString) - 
                  charsToRemoveAtTheEnd,
                  std::end(mapString)); // erase the superfluous ',' and ' ' characters at the end

        mapString.push_back(']');

        return os << utils::makeClassString(STR("NutrientTableImpl"),
                                            STR("cont_"),
                                            mapString);
    } // END of function operator<<

    bool operator==(NutrientTableImpl::this_type const &lhs,
                           NutrientTableImpl::this_type const &rhs) noexcept {

        return lhs.cont_ == rhs.cont_;
    }

    bool operator!=(NutrientTableImpl::this_type const &lhs,
                           NutrientTableImpl::this_type const &rhs) noexcept {

        return !(lhs == rhs);
    }

	/*
		TODO: Add the correct overdose description, underdose...
		Would be nicer, if this would be taken from a file or the db.
		

		The amount to get an optiumdose was taken from:
		https://www.dge.de/wissenschaft/referenzwerte/
		(19+ years old, male). I assume it's not trivial to implement 
		different dosages for different ages, gender, or weights)
		
		TODO: Some Nutrients are measured in IU or other measurements.
		This has to be implemented correctly.

		TODO: A synonym function that takes a string like thiamin and returns
		vitamin b1.
	*/
    NutrientTableImpl::NutrientPool const NutrientTableImpl::nutrientPool = {
        value_type{ pl::value_type<value_type>{
                      NutrientType::VitaminA, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_milg, // minimumDose
                      990.0_g, // maximumDose
                      1.0_milg, // optimumDose
                      0.0_micg, // storage
                   } 
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::VitaminC, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      110.0_milg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::VitaminK, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      70.0_micg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::VitaminE, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      992.0_g, // maximumDose
                      0.0_milg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::VitaminD, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      20.0_micg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::VitaminB1, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      1.2_milg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::VitaminB2, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      1.4_milg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::VitaminB3, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      15.0_milg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::VitaminB6, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      1.5_milg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::VitaminB9, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      300.0_micg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::VitaminB12, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      3.0_micg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::Calcium, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      1000.0_milg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::Iron, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      10.0_milg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::Selen, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      70.0_micg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::Phosphorus, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      700.0_micg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::Potassium, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      6.0_milg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::Zinc, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      10.0_milg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::Magnesium, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      400.0_milg, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::Water, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      99990.0_g, // maximumDose
                      2500.0_g, // optimumDose - not from dge
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::Protein, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      57.0_g, // optimumDose
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::Fat, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      57.0_g, // optimumDose - magic number
                      0.0_g, // storage
                    }
        },
        value_type{ pl::value_type<value_type>{
                      NutrientType::Carbohydrates, // NutrientType
                      STR("not implemented"), // description
                      STR("not implemented"), // overdoseDescription
                      0.0_g, // minimumDose
                      990.0_g, // maximumDose
                      150.0_g, // optimumDose - magic number
                      0.0_g, // storage
                    }
        },
		value_type{ pl::value_type<value_type>{
			NutrientType::Kcal, // NutrientType
				STR("not implemented"), // description
				STR("not implemented"), // overdoseDescription
				0.0_g, // minimumDose
				900.0_g, // maximumDose
				200.0_g, // optimumDose - magic number
				0.0_g, // storage
			}
		},
    }; // END of nutrientPool initialization

    NutrientTableImpl::NutrientTableImpl(container_type &&container)
        : cont_{ std::move(container) } { }
    
} // END of namespace fa
