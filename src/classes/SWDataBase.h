/*
 * SWDataBase.h
 *
 *  Created on: 2009-09-11
 *      Author: Trog
 */

#ifndef SWDATABASE_H_
#define SWDATABASE_H_
#include <list>
#include <map>
#include <pqxx/connection>
#include <pqxx/transaction>
#include <pqxx/result>
#include "SWString.h"
#include "SWQueryResult.h"

using namespace pqxx;

class SWDataBase
{
	SWString connectionString;
	connection *dbConnection;
	work *dbTx /*!< the transaction */;
	result queryResult;
	list<SWQueryResult *> results;

	friend class SWQueryResult;

public:
	SWDataBase(bool = true);
	SWDataBase(const SWString &, bool = true);
	virtual ~SWDataBase();

	bool connect();
	bool isConnected() const;
	void beginTransaction(const SWString & = "customTransaction");
	void commitTransaction();
	bool singleQuery(const SWString &);
	bool query(const SWString &);
	SWQueryResult *getResult(const list<SWString> &columns);

	static SWString &preEncodePassword(SWString &password)
	{
		return password.prepend("encode(digest('").append("','sha512'),'base64')");
	}

	static SWString preEncodePassword(const SWString &password)
	{
		return SWString(password).prepend("encode(digest('").append("','sha512'),'base64')");
	}
};

#endif /* SWDATABASE_H_ */
