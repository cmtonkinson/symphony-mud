
#ifndef H_SYMPHONY_ITEM_SET
#define H_SYMPHONY_ITEM_SET

#include <set>
#include <string>

class Item;
class Modifier;

class ItemSet {
  public:
    ItemSet(void)  { return; }
    ~ItemSet(void) { return; }

    std::set<Item*>&             items(void)         { return _items; }
    const std::set<Item*>&       items(void) const   { return _items; }
    std::set<Modifier*>&         modifiers(void)         { return _modifiers; }
    const std::set<Modifier*>&   modifiers(void) const   { return _modifiers; }

    std::string   serializeItems(void) const;
    void          unserializeItems(std::string ser);
    std::string   serializeModifiers(void) const;
    void          unserializeModifiers(std::string ser);

  private:
    std::set<Item*>      _items;
    std::set<Modifier*>  _modifiers;
};

#endif // H_SYMPHONY_ITEM_SET
