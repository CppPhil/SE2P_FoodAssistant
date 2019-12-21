#include "NoteImpl.h" // fa::NoteImpl
#include <utility> // std::move
#include "Utils.h" // fa::utils::makeClassString

namespace fa {
    NoteImpl::NoteImpl() 
        : NoteImpl{ STR("") } { }
    
    NoteImpl::NoteImpl(value_type text) 
        : text_{ std::move(text) },
          id_{ generateId<this_type>() } { }

    OStream &operator<<(OStream &os, NoteImpl::this_type const &note) {
        return os << utils::makeClassString(STR("NoteImpl"), STR("text_"), note.text_);
    }

    bool operator==(NoteImpl::this_type const &lhs, NoteImpl::this_type const &rhs) {
        return lhs.text_ == rhs.text_;
    }

    bool operator<(NoteImpl::this_type const &lhs, NoteImpl::this_type const &rhs) {
        return lhs.text_ < rhs.text_;
    }

	Id NoteImpl::getId() const {
		return id_;
	}

	NoteImpl::value_type NoteImpl::getText() const {
		return text_;
	}

	void NoteImpl::setText(value_type const &text) {
		text_ = text;
	}

    void NoteImpl::setText(value_type &&text) {
        text_ = std::move(text);
    }
} // END of namespace fa
