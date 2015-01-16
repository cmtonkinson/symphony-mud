
#ifndef H_SYMPHONY_CONTAINER
#define H_SYMPHONY_CONTAINER

#include <list>
#include <map>
#include <string>
#include <vector>

#define CONTAINER_ALL_W_KEYWORDS  -2
#define CONTAINER_ALL             -1
#define CONTAINER_NONE             0

class Object;

/* abstract base class for object containers */
class Container {
  public:
    Container(void);
    virtual ~Container(void);

    virtual void                remove(Object* object) = 0;
    virtual void                remove(const std::list<Object*>& objects) = 0;
    virtual void                purgeObjects(void) = 0;
    virtual std::list<Object*>  searchObjects(const std::string& q) = 0;

    static std::vector<std::string> parseQuery(const std::string& q, int& multiplier, int& index);
    std::list<Object*>              search(const std::list<Object*>& objects, const std::vector<std::string>& keywords, const int& multiplier, const int& index);
    static std::list<Object*>       search(const std::map<int,Object*>& objects, const std::vector<std::string>& keywords, const int& multiplier, const int& index);
    static bool                     commonSearch(Object* object, const std::vector<std::string>& keywords);
};

#endif // !H_SYMPHONY_CONTAINER
