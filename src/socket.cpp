
#include "socket.h"

/********************************************************* EXCEPTION *********************************************************/
SocketException::SocketException(const std::string& method, const int& n, bool gai_error) {
  char buffer[MAX_BUFFER];
  if (gai_error) {
    sprintf(buffer, "Socket exception in %s: %s\n", method.c_str(), gai_strerror(n));
  } else {
    sprintf(buffer, "Socket exception in %s: %s\n", method.c_str(), strerror(n));
  }
  setError(buffer);
  return;
}

SocketException::~SocketException(void) {
  return;
}

/********************************************************* SOCKET *********************************************************/
char Socket::option_buffer[MAX_BUFFER];

Socket::Socket(unsigned int sock_fd, int domain, int type, int protocol) {
  inputInBuffer(false);
  /*
   * If a valid file descriptor was passed into the constructor,
   * then we know that the socket (described by sockfd) already
   * exists and is open, and we're just filling in the blanks of
   * the Socket object.  If a valid file descriptor isn't given,
   * we assume that we're supposed to create a new socket and go
   * from there.
   */
  if (sock_fd) {
    setFd(sock_fd);
    getpeername();
  } else {
    SOCK_SYSCALL_TRY(sock_fd = ::socket(domain, type, protocol), "Socket::Socket() -> ::socket()")
    setFd(sock_fd);
  }
  return;
}

Socket::~Socket(void) {
  // Disable all further traffic on, and then close, the socket.
  ::shutdown(getFd(), SHUT_RDWR);
  ::close(getFd());
  return;
}

std::string Socket::fetchBuffer(void) {
  std::string foo = _buffer;
  _buffer.clear();
  inputInBuffer(false);
  return foo;
}

unsigned int Socket::private_accept(unsigned int seconds, unsigned int microseconds) {
  unsigned int sock_fd = 0;
  sockaddr_in address;
  socklen_t address_length = 0;
  timeval tv;
  fd_set fd_read;
  fd_set fd_exc;

  // Initialize.
  tv.tv_sec = seconds;
  tv.tv_usec = microseconds;
  FD_ZERO(&fd_read);
  FD_ZERO(&fd_exc);
  FD_SET(getFd(), &fd_read);
  FD_SET(getFd(), &fd_exc);

  // We use select() to check for new connections, so that the thread doesn't block if there aren't any.
  SOCK_SYSCALL_TRY(::select(getFd()+1, &fd_read, NULL, &fd_exc, &tv), "Socket::private_accept() -> ::select()")

  if (FD_ISSET(getFd(), &fd_read)) {
    // accept() will return the file descriptor of the next new connection.
    SOCK_SYSCALL_TRY(sock_fd = ::accept(getFd(), (sockaddr*)&address, &address_length), "Socket::private_accept() -> ::accept()")
    return sock_fd;
  }

  return 0;
}

void Socket::getpeername(void) {
  socklen_t address_length = sizeof(getAddress());
  // getpeername() sets up the scokaddr_in structure for the newly connected socket.
  SOCK_SYSCALL_TRY(::getpeername(getFd(), (sockaddr*)getAddressPtr(), &address_length), "Socket::getpeername() -> ::getpeername()")
  // inet_ntoa() fetches the decimal-and-dot IP address of the socket.
  setIP(::inet_ntoa(getAddress().sin_addr));
  return;
}

void Socket::setOpt(const int& optname, int optval) {
  SOCK_SYSCALL_TRY(::setsockopt(getFd(), SOL_SOCKET, optname, (void*)&optval, sizeof(optval)), "Socket::setOpt() -> ::setsockopt()")
  return;
}

void Socket::bind(unsigned int port, unsigned int family, unsigned int address) {
  // Check to ensure that a valid port was specified.
  if (port < 1025 || port > 65535) {
    fprintf(stderr, "Port %d is out of range.  Defaulting to port %d.\n", port, DEFAULT_PORT);
    port = DEFAULT_PORT;
  }

  // Set up our sockaddr_in structure.
  memset(getAddressPtr(), 0, sizeof(getAddress())); /* Zero out structure */
  getAddressPtr()->sin_family      = family;          /* Internet address family */
  getAddressPtr()->sin_addr.s_addr = htonl(address);  /* Incoming interface */
  getAddressPtr()->sin_port        = htons(port);     /* Local port */

  /*
   * If successful, the call to bind() will, creatively enough, bind the socket
   * to the given port and return a non-negative integer.
   */
  SOCK_SYSCALL_TRY(::bind(getFd(), (sockaddr*)getAddressPtr(), sizeof(getAddress())), "Socket::bind() -> ::bind()")
  setPort(port);
  return;
}

void Socket::listen(unsigned int backlog) {
  // listen() will direct a socket (which is already bound to a specific port) to recieve incoming connection attempts.
  SOCK_SYSCALL_TRY(::listen(getFd(), backlog), "Socket::listen() -> ::listen()")
  return;
}

void Socket::startup(unsigned int port) {
  setOpt(SO_REUSEADDR, 1);
  bind(port);
  listen();
  return;
}

std::vector<Socket*> Socket::accept(unsigned int seconds, unsigned int microseconds) {
  unsigned int sock_fd = 0;
  std::vector<Socket*> new_sockets;

  /*
   * Loop until all incoming connections have been processed and stored in a vector.
   * We do this because it makes the interface cleaner.  The developer can call
   * Socket::accept() and recieve a vector of all new Sockets, and not have to
   * fool around with a select()/accept() loop as in Socket::private_accept().
   */
  while ((sock_fd = private_accept(seconds, microseconds)) > 0) {
    new_sockets.push_back(new Socket(sock_fd));
  }

  return new_sockets;
}

void Socket::send(const std::string& message) {
  SOCK_SYSCALL_TRY(::send(getFd(), message.c_str(), message.length(), 0), "Socket::send() -> ::send()")
  return;
}

std::string Socket::receive(void) {
  char buffer[MAX_BUFFER];
  timeval tv;
  fd_set fd_read;
  fd_set fd_exc;

  /*
   * Initialize.
   */
  memset(buffer, 0, MAX_BUFFER);
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fd_read);
  FD_ZERO(&fd_exc);
  FD_SET(getFd(), &fd_read);
  FD_SET(getFd(), &fd_exc);

  // We use select() to check for new data, so that the thread doesn't block if there isn't any.
  SOCK_SYSCALL_TRY(::select(getFd()+1, &fd_read, NULL, &fd_exc, &tv), "Socket::receive() -> ::select()")

  if (FD_ISSET(getFd(), &fd_read)) {
    SOCK_SYSCALL_TRY(::recv(getFd(), buffer, MAX_BUFFER, 0), "Socket::receive() -> recv()")
    return buffer;
  } else {
    return std::string();
  }
}

bool Socket::hasInput(void) {
  std::string foo;
  std::string bar;

  if (inputInBuffer()) {
    return true;
  }

  foo = receive();

  if (!foo.length()) {
    inputInBuffer(false);
  } else {
    /*
     * Trim all non-printable (according to the ASCII chart) characters
     * off of the begining of foo. [Spaces are considered trash here]
     */
    while (foo.length() && (foo[0] < 33 || foo[0] > 126)) {
      foo.erase(foo.begin());
    }
    /*
     * Copy all characters from foo into bar until either 1) we hit the end
     * of foo, or 2) we hit a non-printable character in foo.
     */
    for (std::string::iterator it = foo.begin(); it != foo.end(); ++it) {
      if (*it < 32 || *it > 126) {
        break;
      }
      bar.append(1, *it);
    }
    /*
     * Trim all whitespace off the end of bar.
     */
    while (bar.length() && isspace(bar[bar.length()-1])) {
      bar.erase(bar.end()-1);
    }
    /*
     * If there is data in bar, we know that it's a trimmed sequence of
     * printable ASCII characters.
     */
    if (bar.length()) {
      setBuffer(bar);
      inputInBuffer(true);
    } else {
      inputInBuffer(false);
    }
  }

  return inputInBuffer();
}

std::string Socket::getInput(void) {
  if (inputInBuffer()) {
    return fetchBuffer();
  }
  return std::string();
}

Socket* Socket::connect(const std::string& host, const std::string& port) {
  struct addrinfo hints;
  struct addrinfo* result;
  int fd = 0;
  int gai_errno = 0;

  // Initialize our hints for getaddrinfo()...
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  // Get our server info...
  SOCK_SYSCALL_TRY(gai_errno = ::getaddrinfo(host.c_str(), port.c_str(), &hints, &result), "Socket::connect() -> ::getaddrinfo()")

  // Create a new socket with the info from getaddrinfo()...
  SOCK_SYSCALL_TRY(fd = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol), "Socket::connect() -> ::socket()")

  // Connect the socket to the server from getaddrinfo()...
  SOCK_SYSCALL_TRY(::connect(fd, result->ai_addr, result->ai_addrlen), "Socket::connect() -> ::connect()")

  // Clean up everything that getaddrinfo() allocated...
  freeaddrinfo(result);

  // Return a pretty new connected Socket pointer...
  return new Socket(fd);
}
