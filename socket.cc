// 
// Socket class implementation
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

// $Id: socket.cc,v 1.2 1999/09/12 12:59:48 ssbaker Exp $ 

#include "socket.h"

// Constructors
Socket::Socket(int port) {

  m_port = port;
  m_err = SE_NONE;
 
  // Create the socket
  m_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(m_fd < 0) {
    m_err = SE_SOCK;
    return;
  }
  int opt = 1;
  setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int));

  // Bind to the port
  sockaddr_in addr;
  memset(&addr, 0, sizeof(sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons((u_short)m_port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(m_fd, (sockaddr *)&addr, sizeof(sockaddr_in)) < 0) {
    m_err = SE_BIND;
    return;
  }

  // Begin listening
  if(listen(m_fd, SOMAXCONN)) {
    m_err = SE_LIST;
    return;
  }
}

Socket::Socket() {
  m_err = SE_PORT;
}

SOCKET Socket::accept(void) {

  sockaddr_in addr;
  size_t addrlen;
    
  // Accept the connection
  m_err = SE_NONE;
  memset(&addr, 0, sizeof(addr));
  addrlen = sizeof(addr);
  SOCKET newsock = ::accept(m_fd, (sockaddr *)&addr, &addrlen);
  if(newsock < 0) {
    m_err = SE_ACPT;
    return -1;
  }
  return newsock;
}

/// Local Variables: ***
/// mode: c++ ***
/// End: ***
