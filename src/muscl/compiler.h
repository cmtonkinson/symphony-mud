
#ifndef SYMPHONY_COMPILER
#define SYMPHONY_COMPILER

/*
 * I'm not even pretending that this class is OOP-y. This basically
 * just provides a wrapper object around yyparse() so that the other
 * language processing components can continue on with their decoupled
 * lives, believing my well-crafted delusion that the rest of their
 * family adheres to proper paradigm.
 *
 * To that end, it would be unwise to attempt maintaining more than
 * one of these at a time; I haven't found reason to worry about
 * trying to coerce flex & bison into producing re-entrant code.
 */
class Compiler {
  public:
    Compiler(void);
    ~Compiler(void);
};

#endif // #ifndef SYMPHONY_COMPILER
