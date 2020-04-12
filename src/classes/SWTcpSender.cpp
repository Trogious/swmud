/*
 * SWTcpSender.cpp
 *
 *  Created on: Feb 19, 2010
 *      Author: Trog
 */

#include "SWTcpSender.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdlib>

void SWTcpSender::run()
{
	/* ten kod wysyla dowolny tekst (request) po TCP (IPv4) */

	int sock = socket(PF_INET, SOCK_STREAM, getprotobyname("TCP")->p_proto);
	if (sock >= 0)
	{
		struct sockaddr_in sin;
		bzero(&sin, sizeof(struct sockaddr_in));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		inet_pton(AF_INET, host.c_str(), &sin.sin_addr.s_addr);
		int conn = connect(sock, (struct sockaddr *) &sin, sizeof(sin));
		if (!conn)
		{
			int written = write(sock, request.c_str(), request.length());
			if (written < 1)
			{
				if (written < 0)
				{
					//perror("write error");
				}
				//reload request not sent
			}
			close(sock);

			//request sent
		}
		else
		{
			//perror("connect error");
		}
	}
	else
	{
		//perror("socket error");
	}

	//request sent
}

void SWTcpSender::send()
{
	if (synchronous)
	{
		run();
	}
	else
	{
		addToPool(this);
		start();
	}
}
