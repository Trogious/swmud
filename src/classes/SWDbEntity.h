/*
 * SWDbEntity.h
 *
 *  Created on: 2010-07-10
 *      Author: Trog
 */

#ifndef SWDBENTITY_H_
#define SWDBENTITY_H_
#include <SWDataBase.h>
#include <SWDbEntityDataMapping.h>

/*!
 * Encja, czyli cos co mozna zapisac w bazie danych i z niej odczytac.
 * Wszystkie klasy, ktore sa czytane z bazy danych powinny dziedziczyc
 * po SWDbEntity.
 */
class SWDbEntity
{
private:
	//	SWString table; /*!< nazwa tabeli tej encji w bazie danych */
	SWInt id; /*!< id encji w bazie danych */
	bool modified; /*!< indykator modyfikacji encji, true jesli dane obiektu sa rozne od danych w bazie */

protected:
	bool isModified() const
	{
		return modified;
	}

	void setModified()
	{
		if (!modified)
		{
			modified = true;
		}
	}

	SWString getWhereId() const;
	SWString getUpdateSet() const;
	SWString &addUpdateSuffix(SWString &updateQueryInfix) const;
	SWInt getLastId(SWDataBase *db) const;
	virtual SWDbEntityDataMapping getDataMapping() const = 0;
	virtual SWString getDeleteQuery() const;
	virtual SWString getInsertQuery() const;
	virtual SWString getUpdateQuery() const;
	virtual bool loadRelated(SWDataBase *db)
	{
		return true;
	}

	template<class T>
	static SWString getInsertQueries(const T& container);
	template<class T>
	static SWString getUpdateQueries(const T& container);
	template<class T>
	static SWString getInsertQueriesPtr(const T& container);
	template<class T>
	static SWString getUpdateQueriesPtr(const T& container);

public:
	SWDbEntity(const SWInt &id = 0) :
		id(id), modified(false)
	{
	}

	virtual ~SWDbEntity()
	{
	}

	const SWInt &getId() const
	{
		return id;
	}

	virtual const SWString getTableName() const = 0;/*!< nazwa tabeli tej encji w bazie danych */

	void setId(const SWInt &id)
	{
		this->id = id;
		if (!modified)
		{
			modified = true;
		}
	}

	bool persist();
	bool purge();
	bool load(const SWString &query, SWDataBase *db = 0);
};

template<class T>
SWString SWDbEntity::getInsertQueries(const T& container)
{
	SWString queries;
	for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
	{
		queries.append(it->getInsertQuery()).append(";\n");
	}

	return queries;
}

template<class T>
SWString SWDbEntity::getUpdateQueries(const T& container)
{
	SWString queries;
	for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
	{
		queries.append(it->getUpdateQuery()).append(";\n");
	}

	return queries;
}

template<class T>
SWString SWDbEntity::getInsertQueriesPtr(const T& container)
{
	SWString queries;
	for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
	{
		queries.append((*it)->getInsertQuery()).append(";\n");
	}

	return queries;
}

template<class T>
SWString SWDbEntity::getUpdateQueriesPtr(const T& container)
{
	SWString queries;
	for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
	{
		queries.append((*it)->getUpdateQuery()).append(";\n");
	}

	return queries;
}

#endif /* SWDBENTITY_H_ */
