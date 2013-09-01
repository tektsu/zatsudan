// 
// Class client implementation
//
// Class client is the client process handler
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

// $Id: client.cc,v 1.1 1999/09/12 12:55:26 ssbaker Exp $ 

#include "zatsudan.h"

//
// Constructors
//
// Parameters: c - Client Socket
//             p - Parent Socket
//
client::client(int c, int p) {

  client_sock = c;		// Save sockets
  parent_sock = p;
  client = new fstream(c);	// Open streams
  parent = new fstream(p);
  rbuf = new char[BUFSIZ];	// Allocate the read buffer

  maxfd = max(c,p);		// Set max fd
  client_cr = parent_cr = 0;	// Clear CR flags
  FD_ZERO(&readfd);		// Clear the fd list
}

//
// Destructor
//
client::~client() {
  delete rbuf;
  delete client;
  delete parent;
}

//
// Run the client handler
//
void client::run(void) {

  // Print the welcome token
  *client <<"YO" EOL;
  client->flush();

  // Run forever (or for a while anyway...)
  char *p, *e;
  ssize_t count;
  unsigned char c;
  while(1) {

    // Watch for input
    FD_SET(client_sock, &readfd);
    FD_SET(parent_sock, &readfd);

    struct timeval timeout;
    timeout.tv_sec=1;
    timeout.tv_usec=0;
    if(select(maxfd+1, &readfd, NULL, NULL, &timeout) < 0)
      continue;

    // Something came in from the client
    if(FD_ISSET(client_sock, &readfd)) {

      if((count = read(client_sock, rbuf, BUFSIZ-1)) <= 0)
	break;
      for(p=rbuf, e=rbuf+count; p<e; p++) {
	c = *p;
	if(isgraph(c) || c==' '|| c=='\t')
	  client_buf+=c;
	else if (c == '\r'  ||  (c == '\n'  &&  !client_cr)) {
	  process_client();
	  client_buf="";
	}
	client_cr = (c == '\r');
      }
    }

    // Something came in from the parent
    if(FD_ISSET(parent_sock, &readfd)) {

      if((count = read(parent_sock, rbuf, BUFSIZ-1)) <= 0)
	break;
      for(p=rbuf, e=rbuf+count; p<e; p++) {
	c = *p;
	if(isgraph(c) || c==' '|| c=='\t')
	  parent_buf+=c;
	else if (c == '\r'  ||  (c == '\n'  &&  !parent_cr)) {
	  process_parent();
	  parent_buf="";
	}
	parent_cr = (c == '\r');
      }
    }

  }

  // Shutdown
  shutdown(parent_sock,2);
  close(parent_sock);
  shutdown(client_sock,2);
  close(client_sock);
  sleep(1);
}

//
// Process a line of input from the client
//
// The line of input is in client_buf
//
void client::process_client(void) {

  // A valid command is always 2 characters (or more)
  if(client_buf.size() < 2) {
    *client <<"IV " <<client_buf <<endl;
    return;
  }

  // Get the command and the argument
  string cmd=client_buf.substr(0, CMDLEN);
  client_buf.replace(0, CMDLEN, "");
  string arg=client_buf;
  string::size_type notwhite = arg.find_first_not_of(" \t\n");
  arg.erase(0, notwhite);

  // If we haven't logged in yet, that's the only valid command
  if(!loggedin && cmd != "LI") {
      *client <<"NL" <<endl;
      return;
  }

  // Handle a login command
  if(cmd == "LI") {
  } 

  // Handle a text command
  else if (cmd == "TX") {
    if(arg.size())
      *parent <<cmd <<arg <<endl;
    return;
  } 

  // Handle a move command
  else if (cmd == "MV") {
  } 

  // Command not valid -- complain!
  else {
    *client <<"IV" <<cmd <<endl;
    return;
  }
}

//
// Process a line of input from the parent
//
// The line of input is in parent_buf
//
void client::process_parent(void) {
  *client <<parent_buf <<endl;	// Just pass it to the client
}

/// Local Variables: ***
/// mode: c++ ***
/// End: ***
