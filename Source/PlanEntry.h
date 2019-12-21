#pragma once
#include <memory> // std::unique_ptr
#include "GlobalHeader.h" // fa::OStream
#include <utility> // std::move
#include <type_traits> // std::is_same
#include "NoteImpl.h" // fa::NoteImpl

namespace fa {
    class DailyPlanImpl;
} // END of namespace fa

namespace fa {
    class PlanEntryInterface {
    public:
        using this_type = PlanEntryInterface;
        using SmartPointer = std::unique_ptr<this_type>;

        virtual ~PlanEntryInterface();
        virtual SmartPointer copy_() = 0;
        virtual OStream &printOn(OStream &) const = 0;
        virtual bool isNote() const = 0;
        virtual bool isFood() const = 0;
        virtual bool equals(PlanEntryInterface const &) const = 0;
        virtual String getText() const = 0;
    }; // END of class PlanEntryInterface

    OStream &operator<<(OStream &, PlanEntryInterface const &);

    template <class Target>
    class PlanEntryDynamicDelegator : public PlanEntryInterface {
    public:
        friend class DailyPlanImpl;

        using this_type = PlanEntryDynamicDelegator;
        using value_type = Target;

        explicit PlanEntryDynamicDelegator(value_type value) noexcept
            : val_{ std::move(value) } { }

        virtual ~PlanEntryDynamicDelegator() = default;

        virtual SmartPointer copy_() override {
            return std::make_unique<this_type>(*this);
        }

        virtual OStream &printOn(OStream &ostr) const override {
            return ostr << val_;
        }

        virtual bool isNote() const override {
            return std::is_same<value_type, NoteImpl>::value;
        }

        virtual bool isFood() const override {
            return !isNote();
        }

        virtual bool equals(PlanEntryInterface const &other) const override {
            auto ptr = &other;
            auto downcasted = dynamic_cast<this_type const *>(ptr);
            if (downcasted == nullptr) {
                return false;
            }

            return val_ == downcasted->val_;
        }

        virtual String getText() const override {
            return val_.getText();
        }

    private:
        value_type val_;
    }; // END of class PlanEntryDynamicDelegator
    
} // END of namespace fa
