
#ifndef SYMPHONY_LP
#define SYMPHONY_LP

/*
 * The LanguageProcessor is a singleton that provides a single
 * point of access and abstraction for the following components:
 *  -compiler
 *  -virtual machine
 *  -file backing
 *  -context layer
 */

class LanguageProcessor {
  public:
    LanguageProcessor(void);
    ~LanguageProcessor(void);
};

#endif // #ifndef SYMPHONY_LP
