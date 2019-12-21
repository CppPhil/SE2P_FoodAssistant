#include "DBConnectionManager.h"

namespace fa {
	DBConnectionManager::DBConnectionManager(String host, unsigned port, String dbName)
		: daoMaker_{ MySQLDAOMaker::valueOf(host, port, dbName) },
		personDAO_{ daoMaker_.createPersonDAO() },
		foodDAO_{ daoMaker_.createFoodDAO() }, 
		noteDAO_{ daoMaker_.createNoteDAO() }
		{
		
	}

	const NoteDAO &DBConnectionManager::getNoteDAO() const {
		return noteDAO_;
	}	
	
	const FoodDAO &DBConnectionManager::getFoodDAO() const {
		return foodDAO_;
	}	
	
	const PersonDAO &DBConnectionManager::getPersonDAO() const {
		return personDAO_;
	}
}