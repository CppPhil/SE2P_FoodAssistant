#pragma once
#include "GlobalHeader.h" // fa::String, fa::OStream
#include <boost/operators.hpp> // boost::totally_ordered
#include "Utils.h" // fa::Id

namespace fa {
    class NoteImpl final : private boost::totally_ordered<NoteImpl> {
    public:
        using this_type = NoteImpl;
        using value_type = String;

        NoteImpl();

        explicit NoteImpl(value_type);

		value_type getText() const;
		void setText(value_type const &text);
        void setText(value_type &&text);
		Id getId() const;

        friend OStream &operator<<(OStream &, this_type const &);

        friend bool operator==(this_type const &, this_type const &);
        friend bool operator<(this_type const &, this_type const &);
    private:
        value_type text_;
        Id const id_;
    }; // END of class NoteImpl
} // END of namespace fa
