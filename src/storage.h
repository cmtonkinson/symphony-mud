
#ifndef _H_SYMPHONY_STORAGE
#define _H_SYMPHONY_STORAGE

/*
 * This Storage API is a horrifying smoke-and-mirror-powered light show beamed through a veritable
 * fun-house-of-horrors of paradigms, techniques, and metaprogramming. You will laugh, you will cry,
 * and you will have nightmares. The roiling concoction of C-style metaprogramming macros and 80's
 * style FILE pointer-based IO is crudly and crassly tethered to C++ templates and lambda functions
 * in a fashion that just begs for a comparison to some 3rd world despot in a brown baret with a
 * Kalishnikov rifle slung across his back holding an iPhone watching House of Cards while his
 * guards relieve themselves in buckets in the room next door.
 *
 * In short, this is a momentous example of how lives are shattered as worlds collide.
 *
 * It's a great case study in what not to do, and how not to do it.
 */

#include <cstdio>
#include <functional>

class Exit;
class PlacementNpc;
class PlacementItem;
class Npc;
class Room;
class SocialCommand;
class Board;
class Note;

typedef std::function<void()> voidFunc;

// STORE_OUT is used internally to define the body of any Storage::out(...) methods for basic types
// which can be trivially fprintf()'d to the file. More custom or more complex types require
// manually defined method bodies.
#define STORE_OUT(SPECIFIER) {                    \
  char format[32];                                \
  sprintf(format, "%s %%%s\n", key, SPECIFIER);   \
  fprintf(fp, format, value);                     \
  return;                                         \
}                                                 \

// STORE_IN is used internally and is the inverse of STORE_OUT in that it is used to define the
// bodies of the various Storage::in(...) template methods. Similarly to STORE_OUT, it can only be
// used for trivially fscanf()able types.
#define STORE_IN(TYPE, SPECIFIER) {               \
  TYPE x;                                         \
  fscanf(fp, SPECIFIER, &x);                      \
  (*item.*method)(x);                           \
  return;                                         \
}                                                 \

// BEGIN and END are to be used in the various Storage::dump() methods to signal the boundaries of
// an items serialization. All item keys should appear between matching BEGIN/END markers, only
// one set of BEGIN/END markers is permissable per item, and all nested items must be placed
// within the parents markers.
#define BEGIN(BOUNDARY) fprintf(fp, "%s\n", BOUNDARY);
#define END(BOUNDARY)   fprintf(fp, "/%s\n", BOUNDARY);

// STORE_KEY is meant to be used "publicly" in the various Storage::load() methods to perform the
// key lookup and proxy to the appropriate template in(...) variant given a method pointer.
#define STORE_CASE(KEY, METHOD)                   \
  if (strcmp(input, KEY) == 0) {                  \
    in(fp, loading, METHOD);                      \
    return;                                       \
  }                                               \

#define STORE_CASE_WITH_TYPE(KEY, METHOD, TYPE)   \
  if (strcmp(input, KEY) == 0) {                  \
    in(fp, (TYPE*)loading, METHOD);               \
    return;                                       \
  }                                               \

#define STORE_CASE_WITH_CODE(KEY, TYPE, SPECIFIER, CODE)  \
  if (strcmp(input, KEY) == 0) {                          \
    TYPE val;                                             \
    fscanf(fp, SPECIFIER, &val);                          \
    CODE                                                  \
    return;                                               \
  }                                                       \

#define STORE_CASE_STRING(KEY, CODE)                      \
  if (strcmp(input, KEY) == 0) {                          \
    std::string str = read_string(fp);                    \
    CODE                                                  \
    return;                                               \
  }                                                       \

// STORE_DESCEND is anotehr "public" macro for easily defining nested items. This is used inside
// of the lambda and should be placed after all STORE_CASE calls. CODE is an arbitrary code fragment
// which associates the nested item with the parent in whatever way is needed.
#define STORE_DESCEND_NEW(KEY, CLASS, CODE)       \
  if (strcmp(input, KEY) == 0) {                  \
    fseek(fp, -strlen(KEY), SEEK_CUR);            \
    CLASS* instance = new CLASS();                \
    if (load(fp, instance)) {                     \
      { CODE }                                    \
    } else {                                      \
      delete instance;                            \
    }                                             \
  }                                               \

#define STORE_DESCEND(KEY)                        \
  if (strcmp(input, KEY) == 0) {                  \
    fseek(fp, -strlen(KEY), SEEK_CUR);            \
    load_base(fp, loading);                       \
  }                                               \

class Storage {

  public:

    static const unsigned LOAD_DONE = 0;
    static const unsigned LOAD_NULL = 1;
    static const unsigned LOAD_NEW  = 2;

    static void dump(FILE* fp, Zone* zone);
    static bool load(FILE* fp, Zone* loading);

    static void dump(FILE* fp, Room* room);
    static bool load(FILE* fp, Room* loading);

    static void dump(FILE* fp, Exit* exit);
    static bool load(FILE* fp, Exit* loading);

    static void dump(FILE* fp, Placement* rule);
    static bool load(FILE* fp, Placement* loading);

    static void dump(FILE* fp, Item* item, const char* suffix = nullptr);
    static bool load(FILE* fp, Item* loading);

    static void dump(FILE* fp, Npc* npc);
    static bool load(FILE* fp, Npc* loading);

    static void dump(FILE* fp, Avatar* avatar);
    static bool load(FILE* fp, Avatar* loading);

    static void dump(FILE* fp, SocialCommand* social);
    static bool load(FILE* fp, SocialCommand* loading);

    static void dump(FILE* fp, Board* board);
    static bool load(FILE* fp, Board* loading);

    static void dump(FILE* fp, Note* note);
    static bool load(FILE* fp, Note* loading);

    // dump() and load() can be overloaded ad nauseum, as long as no method signatures contains a
    // type which is a subclass of another (e.g. Being and Npc or Avatar). In that case, the
    // rules of implicit parametric polymorphism invoke the most-specific method for a given
    // signature match regardless of explicit casting, etc. In those cases, a different method
    // name is required to distinguish base class calls from subclass calls.
    //
    // This is primarily an issue when there is sufficient state in a base class with more than one
    // descendent class so as to desire DRYing out the common storage logic (again as in Being,
    // Npc, and Avatar).
    //
    // For this purpose the names dump_base() and load_base() are arbitrarily used.

    static void dump_base(FILE* fp, Being* being);
    static bool load_base(FILE* fp, Being* loading);

    // Centralized file pathing.
    static std::string avatar_glob_pattern(void);
    static std::string filename(Avatar* avatar);

    static std::string zone_glob_pattern(void);
    static std::string filename(Zone* zone);

    static std::string social_glob_pattern(void);
    static std::string filename(SocialCommand* social = nullptr);

    static std::string disabled_command_glob_pattern(void);

    static std::string deadpool_directory(void);

    static std::vector<std::string> glob(std::string pattern);
    static bool                     file_exists(std::string path);

  private:

    static unsigned     load_inner(FILE* fp, void* loading, char* input, const char* boundary, voidFunc lambda);
    static std::string  read_string(FILE* fp);
    static std::string  peek(FILE* fp);

    // Native types.
    static void out(FILE* fp, const char* key, short          value)  STORE_OUT("hd")
    static void out(FILE* fp, const char* key, int            value)  STORE_OUT("d")
    static void out(FILE* fp, const char* key, long           value)  STORE_OUT("ld")
    static void out(FILE* fp, const char* key, unsigned short value)  STORE_OUT("hu")
    static void out(FILE* fp, const char* key, unsigned int   value)  STORE_OUT("u")
    static void out(FILE* fp, const char* key, unsigned long  value)  STORE_OUT("lu")
    static void out(FILE* fp, const char* key, float          value)  STORE_OUT("f")
    static void out(FILE* fp, const char* key, double         value)  STORE_OUT("f")
    static void out(FILE* fp, const char* key, char           value)  STORE_OUT("c")
    static void out(FILE* fp, const char* key, bool           value);
    static void out(FILE* fp, const char* key, const char*    value);
    // Complex/custom types.
    static void out(FILE* fp, const char* key, std::string  value) { out(fp, key, value.c_str()); }
    static void out(FILE* fp, const char* key, Enum         value) { out(fp, key, value.number()); }

    // Hybrid typing saver.
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(short))           STORE_IN(short, "%hd")
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(int))             STORE_IN(int, "%d")
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(long))            STORE_IN(long, "%ld")
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(unsigned short))  STORE_IN(unsigned short, "%hu")
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(unsigned int))    STORE_IN(unsigned int, "%u")
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(unsigned long))   STORE_IN(unsigned long, "%lu")
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(float))           STORE_IN(float, "%f")
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(double))          STORE_IN(double, "%f")
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(char))            STORE_IN(char, " %c") // that space is important
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(bool));
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(const char*));
    template <class ItemType> static void in(FILE* fp, ItemType* item, void (ItemType::*method)(std::string));

};

#endif // #ifndef _H_SYMPHONY_STORAGE
