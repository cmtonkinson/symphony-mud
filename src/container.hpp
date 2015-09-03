
#ifndef H_SYMPHONY_CONTAINER
#define H_SYMPHONY_CONTAINER

#include <list>
#include <map>
#include <string>
#include <vector>

class Item;

/* abstract base class for item containers */
class Container {
  public:

    static const int ALL_W_KEYWORDS = -2;
    static const int ALL            = -1;
    static const int NONE           =  0;

    Container(void);
    virtual ~Container(void);

    virtual void                remove(Item* item) = 0;
    virtual void                remove(const std::list<Item*>& items) = 0;
    virtual void                purgeItems(void) = 0;
    virtual std::list<Item*>    searchItems(const std::string& q) = 0;

    static std::vector<std::string> parseQuery(const std::string& q, int& multiplier, int& index);
    std::list<Item*>                search(const std::list<Item*>& items, const std::vector<std::string>& keywords, const int& multiplier, const int& index);
    static std::list<Item*>         search(const std::map<int,Item*>& items, const std::vector<std::string>& keywords, const int& multiplier, const int& index);
    static bool                     commonSearch(Item* item, const std::vector<std::string>& keywords);
};

#endif // !H_SYMPHONY_CONTAINER
