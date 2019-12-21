#pragma once
#include <utility> // std::move
#include "DAOMakersImpl.h" // MySQLDAOMaker

namespace fa {
    //! delegates to a DAOMaker like MySQLDAOMaker
    template <class DAODatabase>
    class DAODatabaseDelegator final {
    public:
        using this_type = DAODatabaseDelegator;
        using value_type = DAODatabase;

        template <class ...Args>
        explicit DAODatabaseDelegator(Args &&...args)
            : val_{ std::forward<Args>(args)... } { }

        auto createPersonDAO() -> decltype(auto) {
            return val_.createPersonDAO();
        }

        auto createFoodDAO() -> decltype(auto) {
            return val_.createFoodDAO();
        }

		auto createNoteDAO() -> decltype(auto) {
			return val_.createNoteDAO();
		}

        /* more functions to make different DAOs here... */

    private:
        value_type val_;
    };

    using DAOMaker = DAODatabaseDelegator<MySQLDAOMaker>;
} // END of namespace fa
