/*
 * $Id: socket.h 382 2010-06-02 15:53:46Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef H_SOCKET
#define H_SOCKET

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <vector>

#define MAX_BUFFER    8192
#define DEFAULT_PORT  2000

// Macro to make it a bit easier to handle failed system calls...
#define SOCK_SYSCALL_TRY(call,error)              \
  while ((call) < 0) {                          \
    switch (errno) {                            \
      case EINTR:                                 \
        continue;                                 \
      default:                                    \
        throw SocketException((error), errno);  \
    }                                             \
  }                                               \

/********************************************************* EXCEPTION *********************************************************/
class SocketException {
  public:
    // Constructors...
    SocketException(const std::string& method, const int& n, bool gai_error = false);
    ~SocketException(void);

    // Public accessor methods...
    void                  setError(const std::string& error)          { _error = error; }
    std::string           getError(void) const                        { return _error; }

  private:
    std::string _error;
};

/********************************************************* SOCKET *********************************************************/
class Socket {
  public:
    // Constructors...
    Socket(unsigned int sock_fd = 0, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    ~Socket(void);

    // Public accessor methods...
    int                   getFd(void) const                           { return _fd; }
    void                  setFd(const int& fd)                        { _fd = fd; }
    sockaddr_in           getAddress(void) const                      { return _address; }
    sockaddr_in*          getAddressPtr(void)                         { return &_address; }
    void                  setAddress(const sockaddr_in& address)      { _address = address; }
    unsigned short        getPort(void) const                         { return _port; }
    void                  setPort(const unsigned short& port)         { _port = port; }
    std::string           getIP(void) const                           { return _ip; }
    void                  setIP(const std::string& ip)                { _ip = ip; }

    // General public methods...
    void                  setOpt(const int& name, int optval);
    void                  bind(unsigned int port = DEFAULT_PORT, unsigned int family = AF_INET, unsigned int address = INADDR_ANY);
    void                  listen(unsigned int backlog = 128);
    void                  startup(unsigned int port = DEFAULT_PORT);
    std::vector<Socket*>  accept(unsigned int seconds = 0, unsigned int microseconds = 0);
    void                  send(const std::string& message);
    bool                  hasInput(void);
    std::string           getInput(void);
    std::string           receive(void);

    // Static whatnot...
    static char           option_buffer[MAX_BUFFER];
    static Socket*        connect(const std::string& host, const std::string& port);

  private:
    int                   _fd;
    sockaddr_in           _address;
    unsigned short        _port;
    std::string           _ip;
    bool                  _inputInBuffer;
    std::string           _buffer;

    // Private accessor methods...
    void                  inputInBuffer(const bool& inputInBuffer)    { _inputInBuffer = inputInBuffer; }
    bool                  inputInBuffer(void) const                   { return _inputInBuffer; }
    void                  setBuffer(const std::string& buffer)        { _buffer = buffer; }

    // General private methods...
    std::string           fetchBuffer(void);
    unsigned int          private_accept(unsigned int seconds, unsigned int microseconds);
    void                  getpeername(void);
};



#endif // #ifndef H_SOCKET

