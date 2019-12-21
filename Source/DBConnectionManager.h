#pragma once
#include "DAOMakersImpl.h"

namespace fa {
	class DBConnectionManager final {
	public:
		DBConnectionManager(String host, unsigned port, String dbName);
		const NoteDAO &getNoteDAO() const;
		const FoodDAO &getFoodDAO() const;
		const PersonDAO &getPersonDAO() const;
	private:
		MySQLDAOMaker daoMaker_;
		const PersonDAO personDAO_;
		const FoodDAO foodDAO_;
		const NoteDAO noteDAO_;
	}; // END of class DBConnectionManager
} // END of namespace fa



