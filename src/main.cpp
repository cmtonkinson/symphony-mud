
#include <stdio.h>
#include "os.hpp"
#include "world.hpp"

int main(int argc, char* argv[], char* envp[]) {

  /////////////////////////////////////////////////////////////////////////////
  // Option hefaults
  /////////////////////////////////////////////////////////////////////////////
  os::console_log_level = os::LOG_INFO;
  bool reboot           = false;
  int socket            = 0;

  /////////////////////////////////////////////////////////////////////////////
  // Process command line options
  /////////////////////////////////////////////////////////////////////////////
  int opt = 0;

  while ((opt = getopt(argc, argv, "s:12345678")) != -1) {
    // Switch on returned value
    switch (opt) {
      // Console output verbosity. If multiple given, last one wins
      case '1': os::console_log_level = os::LOG_SILLY;   break;
      case '2': os::console_log_level = os::LOG_DEBUG;   break;
      case '3': os::console_log_level = os::LOG_VERBOSE; break;
      case '4': os::console_log_level = os::LOG_INFO;    break;
      case '5': os::console_log_level = os::LOG_WARN;    break;
      case '6': os::console_log_level = os::LOG_ERROR;   break;
      case '7': os::console_log_level = os::LOG_FATAL;   break;
      case '8': os::console_log_level = os::LOG_SILENT;  break;
      // If rebooting, what's the server socket fd?
      case 's':
        reboot = true;
        socket = estring(optarg);
        break;
      // Unrecognized
      case '?':
        fprintf(stderr, "invalid option given: %c\n", optopt);
        return os::EXIT_BAD_OPTION;
      // wtf
      default:
        fprintf(stderr, "unhandled character from getopt: %c\n", opt);
        return os::EXIT_BAD_OPTION;
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // Sanity checks
  /////////////////////////////////////////////////////////////////////////////

  // Can't reboot without a socket above stderr
  if (reboot && socket < 3) {
    fprintf(stderr, "nonsensical reboot socket: %d\n", socket);
    return os::EXIT_BAD_OPTION;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Start the World
  /////////////////////////////////////////////////////////////////////////////
  srand(time(NULL));

  try {
    if (reboot) {
      World::Instance().rebooting(true);
      World::Instance().recover(socket);
    } else {
      World::Instance().startup();
    }
  } catch (SocketException e) {
    fprintf(stderr, "main.cpp: SocketException (error: %s)\n", e.getError().c_str());
    return os::EXIT_ERROR;
  } catch (RegexException e) {
    fprintf(stderr, "main.cpp: RegexException (message: %s)\n", e.getMessage().c_str());
    return os::EXIT_ERROR;
  } catch (std::string s) {
    fprintf(stderr, "main.cpp: std::string (%s)\n", s.c_str());
    return os::EXIT_ERROR;
  } catch (std::exception e) {
    fprintf(stderr, "main.cpp: std::exception (what: %s)\n", e.what());
    return os::EXIT_ERROR;
  } catch (...) {
    fprintf(stderr, "main.cpp: something failed - good luck!\n");
    return os::EXIT_ERROR;
  }

  return os::EXIT_NORMAL;
}
