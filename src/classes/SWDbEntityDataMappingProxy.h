/*
 * SWDbEntityDataMappingProxy.h
 *
 *  Created on: 26-06-2011
 *      Author: Trog
 */

#ifndef SWDBENTITYDATAMAPPINGPROXY_H_
#define SWDBENTITYDATAMAPPINGPROXY_H_
#include <SWDbArray.h>

#define CREATE_PROXY2(methodNamePart,clazz)		SWDbEntityDataMappingProxy::createProxy(&clazz::set##methodNamePart, const_cast<clazz *>(this))
#define CREATE_PROXY3(methodNamePart,clazz)		SWDbEntityDataMappingProxy::createProxy(&clazz::set##methodNamePart, const_cast<clazz *>(dynamic_cast<const clazz *>(this)))
#define OWNER_PROXY								SWDbEntityDataMappingProxy::createProxy(&SWOwnerEntityId::setOwnerEntityId, const_cast<SWOwnerEntityId *>(dynamic_cast<const SWOwnerEntityId *>(this)))

struct SWDbEntityDataMappingProxy
{
public:
	struct Proxy
	{
		virtual void setValue(const SWString &) = 0;
		virtual ~Proxy() = 0;
	};

	//szablon ogolny
	template<class ValueType, class ProxyOtherSide>
	class CommonProxy: public Proxy
	{
		typedef void (ProxyOtherSide::*Setter)(const ValueType &value);
		Setter setter;
		ProxyOtherSide *proxyOtherSide;

	public:
		void setValue(const SWString &v)
		{
			(proxyOtherSide->*setter)(ValueType::fromString(v));
		}

		CommonProxy(Setter setter, ProxyOtherSide *proxyOtherSide) :
			setter(setter), proxyOtherSide(proxyOtherSide)
		{
		}
	};

	//specjalizacja dla vector<SWInt>
	template<class ProxyOtherSide>
	class CommonProxy<std::vector<SWInt>, ProxyOtherSide> : public Proxy
	{
		typedef void (ProxyOtherSide::*Setter)(const std::vector<SWInt> &value);
		Setter setter;
		ProxyOtherSide *proxyOtherSide;

	public:
		void setValue(const SWString &v)
		{
			(proxyOtherSide->*setter)(SWDbArray::toArray<SWInt, vector>(v));
		}

		CommonProxy(Setter setter, ProxyOtherSide *proxyOtherSide) :
			setter(setter), proxyOtherSide(proxyOtherSide)
		{
		}
	};

	//specjalizacja dla vector<SWInt64>
	template<class ProxyOtherSide>
	class CommonProxy<std::vector<SWInt64>, ProxyOtherSide> : public Proxy
	{
		typedef void (ProxyOtherSide::*Setter)(const std::vector<SWInt64> &value);
		Setter setter;
		ProxyOtherSide *proxyOtherSide;

	public:
		void setValue(const SWString &v)
		{
			(proxyOtherSide->*setter)(SWDbArray::toArray<SWInt64, vector>(v));
		}

		CommonProxy(Setter setter, ProxyOtherSide *proxyOtherSide) :
			setter(setter), proxyOtherSide(proxyOtherSide)
		{
		}
	};

	//specjalizacja dla vector<SWString>
	template<class ProxyOtherSide>
	class CommonProxy<std::vector<SWString>, ProxyOtherSide> : public Proxy
	{
		typedef void (ProxyOtherSide::*Setter)(const std::vector<SWString> &value);
		Setter setter;
		ProxyOtherSide *proxyOtherSide;

	public:
		void setValue(const SWString &v)
		{
			(proxyOtherSide->*setter)(SWDbArray::toArray<SWString, vector>(v));
		}

		CommonProxy(Setter setter, ProxyOtherSide *proxyOtherSide) :
			setter(setter), proxyOtherSide(proxyOtherSide)
		{
		}
	};

	template<class ValueType, class ProxyOtherSide>
	static Proxy *createProxy(void(ProxyOtherSide::*setter)(const ValueType &value), ProxyOtherSide *proxyOtherSide)
	{
		return new CommonProxy<ValueType, ProxyOtherSide> (setter, proxyOtherSide);
	}
};

#endif /* SWDBENTITYDATAMAPPINGPROXY_H_ */
