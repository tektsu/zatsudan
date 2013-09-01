// 
// Class conninfo definition
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

// $Id: conninfo.h,v 1.2 1999/09/12 12:58:15 ssbaker Exp $ 

#ifndef __CONNINFO_H__
#define __CONNINFO_H__

#include <stdlib.h>
#include <time.h>
#include <string>

#include "socket.h"

class conninfo {
  SOCKET       m_sock;		// Sock used to create this instance

public:
  string       m_host,		// Remote Host Name
               m_user,		// Remote User Name
               m_buf;		// Input buffer
  int          m_room,		// Assigned Room
               m_lastcr,	// Last CR flag
               m_loggedin;	// Logged in flag (true if client logged in)
  pid_t        m_pid;		// Child pid
  time_t       m_cntime;	// Time connected
  sockaddr_in  m_local,		// Local and remote connections
               m_remote;	

  // Constructors and Destructors are defined in conninfo.cc
  conninfo();
  conninfo(SOCKET);
  conninfo(const conninfo& ref) { copy(ref); }
  conninfo& operator= (const conninfo& ref) { copy(ref); }

  // Comparision operators - use the time of connection as the basis
  bool operator< (const conninfo& r) const {
    return m_cntime < r.m_cntime;
  }
  bool operator== (const conninfo& r) const {
    return m_cntime == r.m_cntime;
  }

private:
  void init(void);
  void copy(const conninfo&);
};

#endif //__CONNINFO_H__

/// Local Variables: ***
/// mode: c++ ***
/// End: ***
