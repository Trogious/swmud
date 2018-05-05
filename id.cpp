/***************************************************************************
* Star Wars Reality 1.0                                                    *
* copyright (c) 1997, 1998 by Sean Cooper                                  *
* Starwars and Starwars Names copyright (c) Lucasfilm Ltd.                 *
* SMAUG 1.0 (C) 1994, 1995, 1996 by Derek Snider                           *
* SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,                    *
* Scryn, Rennard, Swordbearer, Gorog, Grishnakh and Tricops                *
* Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
* Chastain, Michael Quan, and Mitchell Tse.                                *
* Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
* Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
* ------------------------------------------------------------------------ *
*                     ____________   __     ______                         *
*   Aldegard    Jot  (   ___   ___| /  \   |  __  )   Thanos      Trog     *
*        ______________\  \|   |  /  ^   \ |     <_______________          *
*        \________________/|___|/___/"\___\|__|\________________/          *
*                  \   \/ \/   //  \   |  __  )(  ___/`                    *
*                    \       //  ^   \ |     <__)  \                       *
*                      \_!_//___/"\___\|__|\______/TM                      *
* (c) 2001, 2002            M       U        D                Ver 1.1      *
* ------------------------------------------------------------------------ *
*			 IDENT protocol userid lookup module		   *
****************************************************************************/


#if !defined( WIN32 )

/*	Donated to Realms of Despair by Tricops, author unknown.	    */
/*	Updated to work with IPv6 by Ganis								*/

#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/telnet.h>
#include <errno.h>
#include <netdb.h>
#include "mud.h"

extern int   maxdesc;
//extern int   write   args( ( int fd, char *buf, int nbyte ) );
//extern int   read    args( ( int fd, char *buf, int nbyte ) );
//extern int   close   args( ( int fd ) );
void  nonblock     args( ( int s ) );
void  start_auth   args( ( struct descriptor_data *d ) );
void  read_auth    args( ( struct descriptor_data *d ) );
void  send_auth    args( ( struct descriptor_data *d ) );

/*
 * Initiate an IDENT commention
 */
void  start_auth( struct descriptor_data *d )
{
	union {
		struct	sockaddr_in sock;
		struct	sockaddr_in6 sock6;
	} sock;
	struct	servent			* serv;
	int						err;
	socklen_t				tlen;

	if (d->ipv6)
		d->auth_fd = socket( AF_INET6, SOCK_STREAM, 0);
	else
		d->auth_fd = socket( AF_INET, SOCK_STREAM, 0 );
	err = errno;

	if ( d->auth_fd < 0 && err == EAGAIN )
		bug( "Can't allocate fd for authorization check", 0 );
	nonblock( d->auth_fd );

	tlen = d->ipv6 ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);

	getpeername( d->descriptor, ( struct sockaddr * )&sock, &tlen );
	serv = getservbyname( "ident", "tcp" );
	if ( !serv )
	{
		if (d->ipv6) sock.sock6.sin6_port = htons(113);
		else sock.sock.sin_port = htons( 113 );
	}else{
		if (d->ipv6) sock.sock6.sin6_port = serv->s_port;
		else sock.sock.sin_port = serv->s_port;
	}
	sock.sock.sin_family = d->ipv6 ? AF_INET6 : AF_INET;

	if ( connect( d->auth_fd, ( struct sockaddr *)&sock,
		d->ipv6 ? sizeof(struct sockaddr_in6) :
				sizeof(struct sockaddr_in)) == -1
		&& errno != EINPROGRESS )
	{
	/* Identd Denied */
	/*     bug( "Unable to verify userid", 9 ); */
		log_string( "Jeśli to wyskakuje co chwila, to wołać Ganisa" );
		close( d->auth_fd );
		d->auth_fd = -1;
		d->auth_state = 0;
		++d->atimes;
		return;
	}

	d->auth_state |= ( FLAG_WRAUTH|FLAG_AUTH ); /* Successful, but not sent */
	if ( d->auth_fd > maxdesc ) maxdesc = d->auth_fd;
		return;
}

/* send_auth */

void  send_auth( struct descriptor_data *d )
{
	union {
		struct	sockaddr_in sock;
		struct	sockaddr_in6 sock6;
	} us, them;
    char		authbuf[32];
    socklen_t	ulen, tlen;
	ssize_t		z;

    tlen = ulen = sizeof( us );

    if ( getsockname( d->descriptor, ( struct sockaddr *)&us, &ulen )
    ||   getpeername( d->descriptor, ( struct sockaddr *)&them, &tlen ) )
    {
	bug( "auth getsockname error", 0 );
	goto authsenderr;
    }

    /* compose request */
	if (d->ipv6)
		sprintf( authbuf, "%u , %u\r\n",
			(unsigned int)ntohs(them.sock6.sin6_port),
			(unsigned int)ntohs(us.sock6.sin6_port));
	else
		sprintf( authbuf, "%u , %u\r\n",
			(unsigned int)ntohs(them.sock.sin_port),
			(unsigned int)ntohs(us.sock.sin_port));

    z = write( d->auth_fd, authbuf, strlen( authbuf ) );

    if ( z != (int)strlen( authbuf ) )
    {
	if (d->atimes >= 19)
	{
/*	  sprintf( log_buf, "auth request, broken pipe [%d/%d]", z, errno );
	  log_string(log_buf); */
	  perror("send_auth");
	}
	authsenderr:
	close( d->auth_fd );
	if ( d->auth_fd == maxdesc ) maxdesc--;
	d->auth_fd = -1;
	d->auth_state &= ~FLAG_AUTH;    /* Failure/Continue */
	d->auth_inc = 0;
	if ( d->atimes < 20 ) d->atimes++;
    }
    d->auth_state &= ~FLAG_WRAUTH;  /* Successfully sent request */
    return;
}

/* read_auth */
void  read_auth( struct descriptor_data *d )
{
    char     *s, *t;
    int      len;                    /* length read */
    char     ruser[20], system[8];   /* remote userid */
    u_short  remp = 0, locp = 0;     /* remote port, local port */
   *system = *ruser = '\0';

   /*
    * Can't allow any other reads from client fd while waiting on the
    * authfd to return a full valid string.  Use the client's input buffer
    * to buffer the authd reply.  May take more than one read.
    */

    if ( ( len = read( d->auth_fd, d->abuf + d->auth_inc,
     			sizeof( d->abuf ) - 1 - d->auth_inc ) ) >= 0 )
    {
	d->auth_inc += len;
	d->abuf[d->auth_inc] = '\0';
    }

    if ( ( len > 0 ) && ( d->auth_inc != ( sizeof( d->abuf ) - 1 ) )
    &&   (sscanf( d->abuf, "%hd , %hd : USERID : %*[^:]: %10s",
          &remp, &locp, ruser ) == 3 ) )
    {
	s = rindex( d->abuf, ':');
	*s++ = '\0';
	for ( t = ( rindex( d->abuf, ':' ) + 1 ); *t; t++ )
	    if ( !isspace(*t) )
		break;
	strncpy( system, t, sizeof( system ) );

	for ( t = ruser; *s && ( t < ruser + sizeof( ruser ) ); s++ )
	    if ( !isspace( *s ) && *s != ':' )
		*t++ = *s;
	*t = '\0';

	sprintf( log_buf, "auth reply ok, incoming user: %s@%s",
	    ruser, d->host );
        log_string_plus( log_buf, LOG_COMM, LEVEL_GOD );
    }
    else if ( len != 0 )
    {
	if (!index( d->abuf, '\n' ) && !index( d->abuf, '\r' ) )
	    return;
	sprintf( log_buf, "bad auth reply from %s: %s", d->host, d->abuf );
	log_string_plus( log_buf, LOG_COMM, LEVEL_GOD );
	ruser[0] = '\0';
    }
    close( d->auth_fd );
    if ( d->auth_fd == maxdesc )
	--maxdesc;
    d->auth_inc = 0;
    *d->abuf = '\0';
    d->auth_fd = -1;
    d->auth_state = 0;

    if (ruser[0] == '\0' )
	strcpy(ruser, "UNKNOWN" );

    STRDUP( d->user, ruser );
    if( d->character && d->character->pcdata )
    STRDUP( d->character->pcdata->user, ruser );

    return;
}

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif
void nonblock( int s )
{
  if ( fcntl( s, F_SETFL, FNDELAY ) == -1 )
  {
    perror( "Noblock" );
    exit(2);
  }
}

/*
int workaround( struct descriptor_data *d )
{
   BAN_DATA *tmp;
   char *nicnamehost;

   sprintf( log_buf, "%s!%s@%s", d->name, d->user, d->host );
   nicknamehost = str_dup( log_buf );
   for ( tmp = ncsa_list; tmp; tmp = tmp->next )
   {
     if ( !match( tmp->user, nicknamehost ) )
     {
       FREE( nicknamehost );
       write_to_descriptor( d->descriptor, "NCSA telnet patch loaded.\n" );
       return 1;
     }
   }
   FREE(nicknamehost);
   return 0;
}
*/

#endif	// win32
