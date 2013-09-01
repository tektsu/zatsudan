// 
// zatsudan switchboard routines
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

// $Id: switchboard.cc,v 1.1 1999/09/12 12:55:26 ssbaker Exp $ 

#include "zatsudan.h"

extern connmap conn;		// Connection Table

//
// Handle one line of inout from the client
//
// Parameters: s - Client Socket
//
void switchboard(SOCKET s) {

  // Get the command and the argument
  string cmd=conn[s].m_buf.substr(0, CMDLEN);
  conn[s].m_buf.replace(0, CMDLEN, "");
  string arg=conn[s].m_buf;
  string::size_type notwhite = arg.find_first_not_of(" \t\n");
  arg.erase(0, notwhite);

  cout <<"cmd: " <<cmd <<endl
       <<"arg: " <<arg <<endl;
}
