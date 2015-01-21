
#ifndef _H_SYMPHONY_STORAGE
#define _H_SYMPHONY_STORAGE

/*
 * This Storage API is a horrifying smoke and mirror powered light show beamed through a veritable
 * fun house of horrors of paradigms, techniques, and metaprogramming. You will laugh, you will cry,
 * and you will have nightmares. The roiling concoction of C-style metaprogramming macros and 80's
 * style FILE pointer based IO is crudly and crassly shoehorned together with C++ templates and
 * lambda functions in a fashion that just begs for a comparison to some 3rd world despot in a brown
 * baret with a Kalishnikov rifle slung across his back holding an iPhone watching House of Cards
 * while his guards relieve themselves in buckets in the room next door.
 *
 * In short, this is a momentous example of how worlds collide, and lives are shattered as a result.
 *
 * It's a great case study in what not to do, and how not to write code.
 *
 * What it DOES do well, though, is provide for an on-disk format which is largely human-editable,
 * not horrifically space-inefficient, and somewhat robust with regards to skipping unknown keys.
 */

#include <cstdio>
#include <functional>
#include "exit.h"

typedef std::function<void()> voidFunc;

#define BEGIN(BOUNDARY) fprintf(fp, "%s\n", BOUNDARY);
#define END(BOUNDARY)   fprintf(fp, "/%s\n", BOUNDARY);

#define STORE_OUT(SPECIFIER) {                    \
  char format[32];                                \
  sprintf(format, "%s %%%s\n", key, SPECIFIER);   \
  fprintf(fp, format, value);                     \
  return;                                         \
}                                                 \

#define STORE_IN(TYPE, SPECIFIER) {               \
  TYPE x;                                         \
  fscanf(fp, SPECIFIER, &x);                      \
  (*object.*method)(x);                           \
  return;                                         \
}                                                 \

#define STORE_CASE(KEY, METHOD)                   \
  if (strcmp(input, KEY) == 0) {                  \
    in(fp, loading, METHOD);                      \
    return;                                       \
  }                                               \

class Storage {

  public:

    static const unsigned LOAD_DONE = 0;
    static const unsigned LOAD_NULL = 1;
    static const unsigned LOAD_NEW  = 2;

    static void dump(FILE* fp, Exit* exit);
    static bool load(FILE* fp, Exit* exit);

  private:

    static unsigned load_inner(FILE* fp, void* loading, char* input, const char* boundary, voidFunc lambda);

    // Native types.
    static void out(FILE* fp, const char* key, short          value) STORE_OUT("hd")
    static void out(FILE* fp, const char* key, int            value) STORE_OUT("d")
    static void out(FILE* fp, const char* key, long           value) STORE_OUT("ld")
    static void out(FILE* fp, const char* key, unsigned short value) STORE_OUT("hu")
    static void out(FILE* fp, const char* key, unsigned int   value) STORE_OUT("d")
    static void out(FILE* fp, const char* key, unsigned long  value) STORE_OUT("lu")
    static void out(FILE* fp, const char* key, float          value) STORE_OUT("f")
    static void out(FILE* fp, const char* key, double         value) STORE_OUT("f")
    static void out(FILE* fp, const char* key, char           value) STORE_OUT("c")
    static void out(FILE* fp, const char* key, const char*    value) STORE_OUT("s")
    // Complex/custom types.
    static void out(FILE* fp, const char* key, std::string  value) { out(fp, key, value.c_str()); }
    static void out(FILE* fp, const char* key, Enum         value) { out(fp, key, value.number()); }

    // Hybrid typing saver.
    template <class ObjectType> static void in(FILE* fp, ObjectType* object, void (ObjectType::*method)(short))          STORE_IN(short, "%hd")
    template <class ObjectType> static void in(FILE* fp, ObjectType* object, void (ObjectType::*method)(int))            STORE_IN(int, "%d")
    template <class ObjectType> static void in(FILE* fp, ObjectType* object, void (ObjectType::*method)(long))           STORE_IN(long, "%ld")
    template <class ObjectType> static void in(FILE* fp, ObjectType* object, void (ObjectType::*method)(unsigned short)) STORE_IN(unsigned short, "%hu")
    template <class ObjectType> static void in(FILE* fp, ObjectType* object, void (ObjectType::*method)(unsigned int))   STORE_IN(unsigned int, "%u")
    template <class ObjectType> static void in(FILE* fp, ObjectType* object, void (ObjectType::*method)(unsigned long))  STORE_IN(unsigned long, "%lu")
    template <class ObjectType> static void in(FILE* fp, ObjectType* object, void (ObjectType::*method)(float))          STORE_IN(float, "%f")
    template <class ObjectType> static void in(FILE* fp, ObjectType* object, void (ObjectType::*method)(double))         STORE_IN(double, "%f")

};

#endif // #ifndef _H_SYMPHONY_STORAGE
