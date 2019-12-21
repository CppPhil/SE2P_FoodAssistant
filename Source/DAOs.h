#pragma once
#include "DAOImpl.h" // fa::MySQLPersonDAO
#include "GlobalHeader.h" // fa::String
#include <utility> // std::move
#include <mylibs/detail/meta_functions.hpp> // pl::value_type

namespace fa {
    //! delegates to a concrete PersonDAO such as MySQLPersonDAO
    template <class TargetPersonDAO>
    class PersonDAODelegator final {
    public:
        using this_type = PersonDAODelegator;
        using value_type = TargetPersonDAO;
        using dao_type = pl::value_type<value_type>;

        template <class ...Args>
        explicit PersonDAODelegator(Args &&...args)
            : val_{ std::forward<Args>(args)... } { }

        auto insertPerson(dao_type &person) const -> decltype(auto) {
            return val_.insertPerson(person);
        }

        auto deletePerson(dao_type &person) const -> decltype(auto) {
            return val_.deletePerson(person);
        }

        auto findPerson(String &name) const -> decltype(auto) {
            return val_.findPerson(name);
        }

        auto updatePerson(dao_type &person) const -> decltype(auto) {
            return val_.updatePerson(person);
        }

    private:
        value_type val_;
    }; // END of class PersonDAODelegator

    using PersonDAO = PersonDAODelegator<MySQLPersonDAO>;

    template <class TargetFoodDAO>
    class FoodDAODelegator final {
    public:
        using this_type = FoodDAODelegator;
        using value_type = TargetFoodDAO;
        using dao_type = pl::value_type<value_type>;

        template <class ...Args>
        explicit FoodDAODelegator(Args &&...args)
            : val_{ std::forward<Args>(args)... } { }

        auto findFood(String food) const -> decltype(auto) {
            return val_.findFood(std::move(food));
        }

    private:
        value_type val_;
    }; // END of class FoodDAODelegator

	using FoodDAO = FoodDAODelegator<MySQLFoodDAO>;    

	template <class TargetNoteDAO>
	class NoteDAODelegator final {
		public:
			using this_type = NoteDAODelegator;
			using value_type = TargetNoteDAO;
			using dao_type = pl::value_type<value_type>;

            template <class ...Args>
			explicit NoteDAODelegator(Args &&...args)
				: val_{ std::forward<Args>(args)... } {
			}

			auto findNote(dao_type note) const -> decltype(auto) {
				return val_.findNote(std::move(note));
			}

			auto insertNote(dao_type note) const -> decltype(auto){
				return val_.insertNote(std::move(note));
			}

			auto deleteNote(dao_type note) const -> decltype(auto) {
				return val_.deletetNote(std::move(note));
			}

			auto updateNote(dao_type note) const -> decltype(auto) {
				return val_.updateNote(std::move(note));
			}

	private:
		value_type val_;
	}; // END of class NoteDAODelegator

	using NoteDAO = NoteDAODelegator<MySQLNoteDAO>;

} // END of namespace fa
