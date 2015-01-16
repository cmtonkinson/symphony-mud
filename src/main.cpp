
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
    fprintf(stderr, "%s: SocketException (error: %s)\n", "top-level", e.getError().c_str());
    return World::EXIT_ERROR;
  } catch (MysqlException e) {
    fprintf(stderr, "%s: MysqlException (message: %s)\n", "top-level", e.getMessage().c_str());
    return World::EXIT_ERROR;
  } catch (RegexException e) {
    fprintf(stderr, "%s: RegexException (message: %s)\n", "top-level", e.getMessage().c_str());
    return World::EXIT_ERROR;
  } catch (std::string s) {
    fprintf(stderr, "%s: std::string (%s)\n", "top-level", s.c_str());
    return World::EXIT_ERROR;
  } catch (std::exception e) {
    fprintf(stderr, "%s: std::exception (what: %s)\n", "top-level", e.what());
    return World::EXIT_ERROR;
  } catch (...) {
    fprintf(stderr, "%s: something failed - good luck!\n", "top-level");
    return World::EXIT_ERROR;
  }

  return World::EXIT_NORMAL;
}
