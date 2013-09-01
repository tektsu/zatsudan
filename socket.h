// 
// Socket class interface
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

// $Id: socket.h,v 1.2 1999/09/12 12:59:48 ssbaker Exp $ 

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

// Possible error types
enum SocketErr {
  SE_NONE = 0,			// No error
  SE_PORT,			// No port specified
  SE_SOCK,			// Error creating socket
  SE_BIND,			// Error binding to port
  SE_LIST,			// Error in listen()
  SE_CLOS,			// Error closing socket
  SE_ACPT			// Error Accepting Connection
};

typedef int SOCKET;

//
// Socket Class
//
class Socket {
  
  SocketErr m_err;		// Error type
  int m_port;			// Port to bind to
  SOCKET m_fd;			// File descriptor for socket

public:
  Socket(int);
  Socket();
  SOCKET accept(void);
  int port(void) { return m_port; }
  SOCKET sock(void) { return m_fd; }
  int close(void) { int r=::close(m_fd); m_err=r?SE_CLOS:SE_NONE; return r; }
  SocketErr err(void) { return m_err; }
};

#endif __SOCKET_H__

/// Local Variables: ***
/// mode: c++ ***
/// End: ***

