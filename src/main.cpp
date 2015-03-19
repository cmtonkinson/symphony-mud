
#include <stdio.h>
#include "display.h"
#include "world.h"

int main(int argc, char* argv[], char* envp[]) {
  std::string arguments;
  std::string descriptor;

  try {
    srand(time(NULL));

    for (int i = 0; i < argc; ++i) {
      arguments.append(argv[i]).append(" ");
    }

    if (Regex::match("-copyover\\s+-fd=(\\d+)", arguments, descriptor)) {
        World::Instance().copyover(true);
        World::Instance().recover(estring(descriptor));
    } else {
      World::Instance().startup();
    }

  } catch (SocketException e) {
    fprintf(stderr, "main.cpp: SocketException (error: %s)\n", e.getError().c_str());
    return World::EXIT_ERROR;
  } catch (RegexException e) {
    fprintf(stderr, "main.cpp: RegexException (message: %s)\n", e.getMessage().c_str());
    return World::EXIT_ERROR;
  } catch (std::string s) {
    fprintf(stderr, "main.cpp: std::string (%s)\n", s.c_str());
    return World::EXIT_ERROR;
  } catch (std::exception e) {
    fprintf(stderr, "main.cpp: std::exception (what: %s)\n", e.what());
    return World::EXIT_ERROR;
  } catch (...) {
    fprintf(stderr, "main.cpp: something failed - good luck!\n");
    return World::EXIT_ERROR;
  }

  return World::EXIT_NORMAL;
}
