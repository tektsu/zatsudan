// 
// Class conninfo implementation
//
// Class conninfo is a record of a specific connection to the server
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

// $Id: conninfo.cc,v 1.2 1999/09/12 12:58:15 ssbaker Exp $ 

#include "conninfo.h"

//
// Constructors
//
conninfo::conninfo() {
  init();
}

conninfo::conninfo(SOCKET s) {

  sockaddr addr;
  size_t addrlen = sizeof(sockaddr);
  int opt = 1;

  // Initialize the members
  init();
  m_sock = s;

  // Get the local and remote names
  if(getpeername(s, (sockaddr *)&m_remote, &addrlen)) {
    shutdown(s, 2);
    close(s);
    return;
  }
  addrlen = sizeof(sockaddr);
  if(getsockname(s, (sockaddr *)&m_local, &addrlen)) {
    shutdown(s, 2);
    close(s);
    return;
  }

  // Fill in the host name
  sockaddr_in *locip, *remip;
  locip = &m_local;
  remip = &m_remote;
  if(memcmp(&(locip->sin_addr), &(remip->sin_addr), sizeof(u_long))== 0)
    m_host = "localhost";
  else
    m_host = "unknown host";

  setsockopt(s, SOL_SOCKET, SO_KEEPALIVE,(char *)&opt, sizeof(int));
}  

//
// Initialization
//
// Initilialize the conninfo record
//
void
conninfo::init(void) {
  m_sock = -1;
  m_room=0;
  m_lastcr=0;
  m_loggedin=0;
  m_pid=-1;
  m_cntime=time(NULL);
  memset(&m_local, 0, sizeof(sockaddr_in));
  memset(&m_remote, 0, sizeof(sockaddr_in));
}

//
// Make a copy of an instance of the class
//
// Called by copy constructor ad operator=
//
void
conninfo::copy(const conninfo& ref) {
  m_room = ref.m_room;
  m_loggedin = ref.m_loggedin;
  m_cntime = ref.m_cntime;
  m_host = ref.m_host;
  m_user = ref.m_user;
  m_local=ref.m_local;
  m_remote=ref.m_remote;
  m_pid=ref.m_pid;
}
