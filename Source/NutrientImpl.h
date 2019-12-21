#pragma once
#include "GlobalHeader.h"
#include <stdexcept> // std::logic_error
#include <span.h> // gsl::span
#include "Amount.h" // fa::Amount

namespace fa {
    //! Enum for the different kinds of nutrients
	enum class NutrientType { // TODO: have this contain all the different kinds of nutrients
		VitaminA,             // WARNING: keep amountOfNutrientTypes synchronized with this enum
		VitaminC,
		VitaminK,
		VitaminE,
		VitaminD,
		VitaminB1,
		VitaminB2,
		VitaminB3,
		VitaminB6,
		VitaminB9,
		VitaminB12,
		Calcium,
		Iron,
		Selen,
		Phosphorus,
		Potassium,
		Zinc,
		Magnesium,
		Water,
		Protein,
		Fat,
		Carbohydrates,
		Kcal
    };

    static auto constexpr
        amountOfNutrientTypes{ static_cast<size_type>(23U) }; // make sure this corresponds with the enum

    //! returns a textual representation of a NutrientType enumerator
    String toString(NutrientType nutrientType); 

    //! returns the corresponding NutrientType enumerator for the given string
    NutrientType toNutrientType(String const &string);

    //! prints a NutrientType to an OStream
    OStream &operator<<(OStream &os, NutrientType nutrientType);

    enum class NutrientKind {
        Micro,
        Macro,
		Kcal
    };

    OStream &operator<<(OStream &os, NutrientKind nutrientKind);

    class NoSuchNutrientException : public std::logic_error {
    public:
        using this_type = NoSuchNutrientException;
        
        explicit NoSuchNutrientException(std::string const &message) noexcept;

        virtual ~NoSuchNutrientException() override;

        virtual char const *what() const noexcept override;
    }; // END of class NoSuchNutrientException

    class NutrientImpl final {
    public:
        using this_type = NutrientImpl;
        using NutrientAmount = Amount;
        using ConstStringSpan = gsl::span<const Char>;

        NutrientImpl(NutrientType nutrientType,
                     String description,
                     String overdoseDescription,
                     NutrientAmount minimumDose,
                     NutrientAmount maximumDose,
                     NutrientAmount optimumDose,
                     NutrientAmount storage);
		// I have kicked the NutrientKind out of the parameters, because otherwise the client
		// would be able to declare VitaminA as a macro nutrient.

        this_type &operator=(this_type const &) = delete;
        this_type &operator=(this_type &&) = delete;

        NutrientImpl(this_type const &) = default;
        NutrientImpl(this_type &&) = default;

        NutrientType getNutrientType() const noexcept;
        ConstStringSpan getName() const noexcept;
        ConstStringSpan getDescription() const noexcept;
        ConstStringSpan getOverDoseDescription() const noexcept;
        NutrientAmount getMinimumDose() const noexcept;
        NutrientAmount getMaximumDose() const noexcept;
        NutrientAmount getOptimumDose() const noexcept;
        NutrientAmount getStorage() const noexcept;
        bool isMicro() const noexcept;
        bool isMacro() const noexcept;
        bool isKcal() const noexcept;

        friend OStream &operator<<(OStream &os, this_type const &obj);

    private:
        NutrientType nutrientType_;
        String const name_; // initialized by calling toString on nutrientType_
        String const description_;
        String const overdoseDescription_;
        NutrientAmount const minimumDose_;
        NutrientAmount const maximumDose_;
        NutrientAmount const optimumDose_;
        NutrientAmount const storage_;
        NutrientKind nutrientKind_;
    }; // END of class NutrientImpl

	NutrientKind toNutrientKind(const NutrientType &type);

	
} // END of namespace fa
