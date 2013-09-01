// 
// zatsudan main program
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

// $Id: zatsudan.cc,v 1.2 1999/09/12 13:01:10 ssbaker Exp $ 

#include "zatsudan.h"

// Local Prototypes
void deadkid(int);
void killserver(int);

// Globals
extern int errno;
connmap conn;			// Connection Table

int
main(int argc, char **argv) {

  int tcpport = PORT;		// Port to bind to

  setlocale(LC_ALL, "");
  signal(SIGCHLD, &deadkid);
  signal(SIGTERM, &killserver);
  signal(SIGQUIT, &killserver);
  signal(SIGINT, &killserver);
  signal(SIGKILL, &killserver);

  // Read command line options

  // Create the socket
  Socket sock(tcpport);
  if(sock.err()) {
    cout <<__FILE__ <<", Line " <<__LINE__ <<" - " 
	 <<"Cannot create socket: " <<strerror(errno) <<endl;
    exit(-1);
  }
  cout <<"Started server on port " <<tcpport <<endl;

  // Watch for activity
  fd_set  readfd, clientfd;
  memset(&clientfd, 0, sizeof(fd_set));
  while(1) {

    // Build the fd_set
    int maxconn=sock.sock();
    FD_ZERO(&readfd);
    FD_SET(sock.sock(), &readfd);
    for(connmap::const_iterator i=conn.begin(); i!=conn.end(); ++i) {
      FD_SET((*i).first, &readfd);
      if((*i).first > maxconn)
	maxconn=(*i).first;
    }

    // Watch for input
    struct timeval timeout;
    timeout.tv_sec=1;
    timeout.tv_usec=0;
    if(select(maxconn+1, &readfd, NULL, NULL, &timeout) < 0)
      continue;

    // New connection?
    if(FD_ISSET(sock.sock(), &readfd)) {

      // Accept the connection
      SOCKET clsock = sock.accept();
      if(sock.err()) {
	cout <<__FILE__ <<", Line " <<__LINE__ <<" - "
	     <<"accept error: " <<strerror(errno) <<endl;
	continue;
      }

      // Build the connection record
      conninfo newconn(clsock);
      
      // Start the child process to handle this connection
      int isock[2];
      if(socketpair(AF_UNIX, SOCK_STREAM, 0, isock)) {
	cout <<__FILE__ <<", Line " <<__LINE__ <<" - "
	     <<"Error creating internal socket: " <<strerror(errno) <<endl;
      }
      pid_t pid = fork();
      if(pid == 0) {
      
	// Child
	client *c = new client(clsock, isock[1]);
	c->run();
	delete c;
	exit(0);
	
      } else if(pid < 0) {
	shutdown(clsock, 2);
	close(clsock);
	cout <<__FILE__ <<", Line " <<__LINE__ <<" - "
	     <<"Unable to fork: " <<strerror(errno) <<endl;
	continue;
      }

      // Parent
      newconn.m_pid = pid;
      conn[isock[0]] = newconn;
      cout <<"New connection received from " <<conn[isock[0]].m_host <<endl;
      close(clsock);
    }

    // Client Command
    for(connmap::const_iterator i=conn.begin(); i!=conn.end(); ++i) {
      if(FD_ISSET((*i).first, &readfd)) {
	SOCKET s = (*i).first;
	
	// Something came in from the client
	char buf[BUFSIZ], *p, *e;
	ssize_t count;
	if((count = read(s, buf, BUFSIZ-1)) <= 0)
	  break;
	for(p=buf, e=buf+count; p<e; p++) {
	  unsigned char c = *p;
	  if(isgraph(c) || c==' '|| c=='\t')
	    conn[s].m_buf+=c;
	  else if (c == '\r'  ||  (c == '\n'  &&  !conn[s].m_lastcr)) {
	    switchboard(s);
	    conn[s].m_buf="";
	  }
	  conn[s].m_lastcr = (c == '\r');
	}
      }
    } 

  }

  return 0;
}

//
// Wait for all complete processes - This is the handler for SIGCHLD
//
// Parameters: sig  - The signal received
//
void
deadkid(int sig) {

  switch(sig) {
  
  case SIGCHLD:
    pid_t pid;
    int sock;
    while(1) {
      pid = waitpid(-1, NULL, WNOHANG);
      if(pid < 1)
	return;
      for(connmap::const_iterator i=conn.begin(); i!=conn.end(); ++i) {
	if((*i).second.m_pid==pid) {
	  sock = (*i).first;
	  conn.erase((*i).first);
	  break;
	}
      }
      cout <<"Killed pid " <<pid <<" (socket " <<sock <<")" <<endl;
    }
    break;

  default:
    break;
  }
}

//
// Kill the server
//
// Parameters: sig  - The signal received
//
void
killserver(int sig) {

  switch(sig) {
  
  case SIGINT:
  case SIGKILL:
  case SIGTERM:
  case SIGQUIT:
    break;

  default:
    break;
  }
  cout <<"Exiting server (pid " <<getpid() <<") on sig " <<sig <<endl;
  sleep(1);
  exit(0);
}

/// Local Variables: ***
/// mode: c++ ***
/// End: ***
