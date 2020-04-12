/*
 * SWDbEntityDataMapping.h
 *
 *  Created on: 01-01-2011
 *      Author: Trog
 */

#ifndef SWDBENTITYDATAMAPPING_H_
#define SWDBENTITYDATAMAPPING_H_
#include <list>
#include <SWContainers.h>
#include <SWString.h>
#include <SWInt.h>
#include <SWInt64.h>
#include <SWTimeStamp.h>
#include <SWDbArray.h>
#include <SWDataBase.h>
#include <SWDbEntityDataMappingProxy.h>

class SWDbEntityDataMapping
{
public:
	class MappingElement
	{
		friend class SWDbEntityDataMapping;

		const SWString first; //column name
		const SWString *second; //value for that column/row
		SWDbEntityDataMappingProxy::Proxy *proxy;

	public:
		MappingElement(const SWString &column, const SWString *value = 0, SWDbEntityDataMappingProxy::Proxy *proxy = 0) :
			first(column), second(value), proxy(proxy)
		{
		}
	};

private:
	list<MappingElement> mapping;

	struct DataMappingPredicateSecond
	{
		const SWString *operator()(MappingElement &mapping) const
		{
			return mapping.second;
		}
	};

	struct DataMappingPredicateProxy
	{
		const SWDbEntityDataMappingProxy::Proxy *operator()(MappingElement &mapping) const
		{
			return mapping.proxy;
		}
	};

	void addMapping(const SWString &column, const SWString * const value, SWDbEntityDataMappingProxy::Proxy *proxy = 0)
	{
		mapping.push_back(MappingElement(column, value, proxy));
	}

	void addUnQuotedMapping(const SWString &column, const SWString &value, SWDbEntityDataMappingProxy::Proxy *proxy = 0)
	{
		addMapping(column, new SWString(value), proxy); //tworzymy tu, usuwamy w destruktorze
	}

public:
	~SWDbEntityDataMapping()
	{
		SWContainers::freeSparseContents(mapping, DataMappingPredicateSecond());
		SWContainers::freeSparseContents(mapping, DataMappingPredicateProxy());
	}

	void addMapping(const SWString &column, const SWString &value, SWDbEntityDataMappingProxy::Proxy *proxy = 0);

	void addMapping(const SWString &column, SWDbEntityDataMappingProxy::Proxy *proxy = 0)
	{
		addMapping(column, 0, proxy);
	}

	void addMapping(const SWString &column, const SWInt &value, SWDbEntityDataMappingProxy::Proxy *proxy = 0)
	{
		addUnQuotedMapping(column, value.toString(), proxy);
	}

	void addMapping(const SWString &column, const SWInt64 &value, SWDbEntityDataMappingProxy::Proxy *proxy = 0)
	{
		addUnQuotedMapping(column, value.toString(), proxy);
	}
	/*
	 void addMapping(const SWString &column, const SWTimeStamp &value)
	 {
	 addMapping(column, static_cast<const SWString> (value));
	 }
	 */
	void addMapping(const SWString &column, const vector<SWInt> &value, SWDbEntityDataMappingProxy::Proxy *proxy = 0)
	{
		addMapping(column, SWDbArray::toQueryValue(value), proxy);
	}

	void addMapping(const SWString &column, const vector<SWInt64> &value, SWDbEntityDataMappingProxy::Proxy *proxy = 0)
	{
		addMapping(column, SWDbArray::toQueryValue(value), proxy);
	}

	void addMapping(const SWString &column, const vector<SWString> &value, SWDbEntityDataMappingProxy::Proxy *proxy = 0)
	{
		addMapping(column, SWDbArray::toQueryValue(value, "\""), proxy);
	}

	void addPasswordMapping(const SWString &column, const SWString &value)
	{
		addUnQuotedMapping(column, SWDataBase::preEncodePassword(value));
	}

	const list<SWString> getNonNullColumns() const;

	const list<SWString> getAllColumns() const;

	SWString getInsertQuery(const SWString &tableName) const;
	SWString getUpdateQuery(const SWString &tableName, const SWInt &id) const;

	void setValues(SWQueryResult::SWQueryResultMap &resultMap);
};

#endif /* SWDBENTITYDATAMAPPING_H_ */
