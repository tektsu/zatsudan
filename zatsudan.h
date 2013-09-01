// 
// zatsudan header
//
// Copyright (c) 1999 by Steve Baker (steve@tandika.com)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

// $Id: zatsudan.h,v 1.2 1999/09/12 13:01:10 ssbaker Exp $ 

#ifndef __PRP_H__
#define __PRP_H__

#include <assert.h>
#include <ctype.h>
#include <error.h>
#include <fstream>
#include <list>
#include <locale.h>
#include <map.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "conninfo.h"

#define PORT 9999		// Default port

#define EOL "\015\012"		// Internet end-of-line

#define BUFSIZ 1024		// Default buffer size
#define CMDLEN 2		// Length of the command token

// Connection Records
typedef map<pid_t, conninfo, less<pid_t> > connmap;

// Prototypes

// clienthandler.cc
void manageclient(int,int);

// switchboard.cc
void switchboard(SOCKET);

// client class interface
class client {

private:
  int client_cr, parent_cr;	// Client and parent CR flags
  int client_sock, parent_sock;	// Client and parent sockets
  fstream *client, *parent;	// Client and parent streams
  string client_buf, parent_buf; // Client and parent input buffers
  int loggedin;			// Logged in flag
  int maxfd;			// Max fd to used in select()
  fd_set readfd;		// fds to watch with select()
  char *rbuf;			// Read Buffer

public:
  client(int, int);
  ~client();
  void run(void);

private:
  void process_client(void);
  void process_parent(void);
};

#endif //__PRP_H__

/// Local Variables: ***
/// mode: c++ ***
/// End: ***




