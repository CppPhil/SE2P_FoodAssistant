#include "NutrientImpl.h"
#include <mylibs/container.hpp> // pl::StaticArray
#include <utility> // std::move
#include "Utils.h" // fa::utils::makeClassString

namespace fa {    
    fa::String toString(NutrientType nutrientType) {
        // don't miss any enumerators!
        switch (nutrientType) {
            case NutrientType::VitaminA : return STR("VitaminA");
            case NutrientType::VitaminC : return STR("VitaminC");
            case NutrientType::VitaminK : return STR("VitaminK");
            case NutrientType::VitaminE : return STR("VitaminE");
            case NutrientType::VitaminD : return STR("VitaminD");
            case NutrientType::VitaminB1 : return STR("VitaminB1");
            case NutrientType::VitaminB2 : return STR("VitaminB2");
            case NutrientType::VitaminB3 : return STR("VitaminB3");
            case NutrientType::VitaminB6 : return STR("VitaminB6");
            case NutrientType::VitaminB9 : return STR("VitaminB9");
            case NutrientType::VitaminB12 : return STR("VitaminB12");
            case NutrientType::Calcium : return STR("Calcium");
            case NutrientType::Iron : return STR("Iron");
            case NutrientType::Selen : return STR("Selen");
            case NutrientType::Phosphorus : return STR("Phosphorus");
            case NutrientType::Potassium: return STR("Potassium,");
            case NutrientType::Zinc : return STR("Zinc");
            case NutrientType::Magnesium: return STR("Magnesium");
            case NutrientType::Water : return STR("Water");
            case NutrientType::Protein: return STR("Protein");
            case NutrientType::Fat : return STR("Fat");
            case NutrientType::Carbohydrates: return STR("Carbohydrates");
            case NutrientType::Kcal: return STR("Kcal");
            default : throw NoSuchNutrientException{ "invalid nutrient in toString(NutrientType)" };
        }
    }

    NutrientType toNutrientType(String const &string) {
        using StaticArray = pl::StaticArray<String, amountOfNutrientTypes>;
		static StaticArray nutrientStrings{ toString(NutrientType::VitaminA), // keep this array updated
											toString(NutrientType::VitaminC), // and in correct order
											toString(NutrientType::VitaminK),
											toString(NutrientType::VitaminE),
											toString(NutrientType::VitaminD),
											toString(NutrientType::VitaminB1),
											toString(NutrientType::VitaminB2),
											toString(NutrientType::VitaminB3),
											toString(NutrientType::VitaminB6),
											toString(NutrientType::VitaminB9),
											toString(NutrientType::VitaminB12),
											toString(NutrientType::Calcium),
											toString(NutrientType::Iron),
											toString(NutrientType::Selen),
											toString(NutrientType::Phosphorus),
											toString(NutrientType::Potassium),
											toString(NutrientType::Zinc),
											toString(NutrientType::Magnesium),
											toString(NutrientType::Water),
											toString(NutrientType::Protein),
											toString(NutrientType::Fat),
											toString(NutrientType::Carbohydrates),
											toString(NutrientType::Kcal) };

        auto it = pl::find(nutrientStrings, string);
        if (it != std::end(nutrientStrings)) {
            return static_cast<NutrientType>(std::distance(std::begin(nutrientStrings), it));
        }

        throw NoSuchNutrientException{ "Nutrient not found in toNutrientType(String const &)" };
    }

    OStream &operator<<(OStream &os, NutrientType nutrientType) {
        return os << toString(nutrientType);
    }

    OStream &operator<<(OStream &os, NutrientKind nutrientKind) {
        return os << (nutrientKind == NutrientKind::Macro ? (STR("Macro")) : (STR("Micro")));
    }

	NutrientKind toNutrientKind(const NutrientType &type) {
		switch (type) {
		case NutrientType::VitaminA:
		case NutrientType::VitaminC:
		case NutrientType::VitaminK:
		case NutrientType::VitaminE:
		case NutrientType::VitaminD:
		case NutrientType::VitaminB1:
		case NutrientType::VitaminB2:
		case NutrientType::VitaminB3:
		case NutrientType::VitaminB6:
		case NutrientType::VitaminB9:
		case NutrientType::VitaminB12:
		case NutrientType::Calcium:
		case NutrientType::Iron:
		case NutrientType::Selen:
		case NutrientType::Phosphorus:
		case NutrientType::Potassium:
		case NutrientType::Zinc:
		case NutrientType::Magnesium:
			return NutrientKind::Micro;
		case NutrientType::Water:
		case NutrientType::Protein:
		case NutrientType::Fat:
		case NutrientType::Carbohydrates:
			return NutrientKind::Macro;
		case NutrientType::Kcal:
			return NutrientKind::Kcal;
		default:
			throw NoSuchNutrientException("Can't deduce the kind of the nutrient. "
										  "I probably forgot to mention it in the switch statement "
										  "in toNutrientKind(const NutrientType &type)");
		}
	}

    NoSuchNutrientException::NoSuchNutrientException(std::string const &message) noexcept
        : std::logic_error{ message } { }

    NoSuchNutrientException::~NoSuchNutrientException() = default;

    char const *NoSuchNutrientException::what() const noexcept {
        return std::logic_error::what();
    }

    NutrientImpl::NutrientImpl(NutrientType nutrientType,
                               String description,
                               String overdoseDescription,
                               NutrientAmount minimumDose,
                               NutrientAmount maximumDose,
                               NutrientAmount optimumDose,
                               NutrientAmount storage)
        : nutrientType_{ nutrientType },
          name_{ toString(nutrientType) },
          description_{ std::move(description) },
          overdoseDescription_{ std::move(overdoseDescription)},
          minimumDose_{ minimumDose },
          maximumDose_{ maximumDose },
          optimumDose_{ optimumDose },
          storage_{ storage },
          nutrientKind_{ toNutrientKind(nutrientType) } {
        
        if (minimumDose >= maximumDose) {
            throw std::logic_error{ "minimumDose in "
                                    "NutrientImpl::NutrientImpl(NutrientType,"
                                    " String, String, NutrientAmount, "
                                    "NutrientAmount, NutrientAmount, "
                                    "NutrientAmount) was > maximumDose" };
        }

		// I divided it to see a more precise error message.
        if (optimumDose < minimumDose) {
            throw std::logic_error{ "optimumDose in "
                                    "NutrientImpl::NutrientImpl(NutrientType,"
                                    " String, String, NutrientAmount, "
                                    "NutrientAmount, NutrientAmount, "
                                    "NutrientAmount) was < minimumDose"};
        }

		if (optimumDose > maximumDose) {
			throw std::logic_error{ "optimumDose in "
				"NutrientImpl::NutrientImpl(NutrientType,"
				" String, String, NutrientAmount, "
				"NutrientAmount, NutrientAmount, "
				"NutrientAmount) was > minimumDose"};
		}
    }

    NutrientType NutrientImpl::getNutrientType() const noexcept {
        return nutrientType_;
    }

    NutrientImpl::ConstStringSpan NutrientImpl::getName() const noexcept {
        return name_;
    }

    NutrientImpl::ConstStringSpan NutrientImpl::getDescription() const noexcept {
        return description_;
    }

    NutrientImpl::ConstStringSpan NutrientImpl::getOverDoseDescription() const noexcept {
        return overdoseDescription_;
    }

    NutrientImpl::NutrientAmount NutrientImpl::getMinimumDose() const noexcept {
        return minimumDose_;
    }

    NutrientImpl::NutrientAmount NutrientImpl::getMaximumDose() const noexcept {
        return maximumDose_;
    }

    NutrientImpl::NutrientAmount NutrientImpl::getOptimumDose() const noexcept {
        return optimumDose_;
    }

    NutrientImpl::NutrientAmount NutrientImpl::getStorage() const noexcept {
        return storage_;
    }

    bool NutrientImpl::isMicro() const noexcept {
        return nutrientKind_ == NutrientKind::Micro;
    }

    bool NutrientImpl::isMacro() const noexcept {
        return nutrientKind_ == NutrientKind::Macro;
    }

    OStream &operator<<(OStream &os, NutrientImpl::this_type const &obj) {
        return os << utils::makeClassString(STR("NutrientImpl"),
                                            STR("nutrientType_"), obj.nutrientType_,
                                            STR("name_"), obj.name_,
                                            STR("description_"), obj.description_,
                                            STR("overdoseDescription_"), obj.overdoseDescription_,
                                            STR("minimumDose_"), obj.minimumDose_,
                                            STR("maximumDose_"), obj.maximumDose_,
                                            STR("optimumDose_"), obj.optimumDose_,
                                            STR("storage_"), obj.storage_,
                                            STR("nutrientKind_"), obj.nutrientKind_);
    }

} // END of namespace fa
