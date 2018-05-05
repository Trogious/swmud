/*
 * SWTcpSender.h
 *
 *  Created on: Feb 19, 2010
 *      Author: Trog
 */

#ifndef SWTCPSENDER_H_
#define SWTCPSENDER_H_
#include "SWThread.h"
#include <arpa/inet.h>

class SWTcpSender: public SWThread
{
	const SWString host;
	uint16_t port;
	const SWString request;
	bool synchronous;

public:
	SWTcpSender(const SWString &host, uint16_t port, const SWString &request, bool synchronous = false) :
		host(host), port(port), request(request), synchronous(synchronous)
	{
	}
	virtual ~SWTcpSender()
	{
	}
	void run();
	virtual void send();
};

#endif /* SWTCPSENDER_H_ */
