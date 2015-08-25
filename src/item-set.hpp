
#ifndef H_SYMPHONY_ITEM_SET
#define H_SYMPHONY_ITEM_SET

#include <list>
#include <string>

class Being;
class Item;
class Modifier;

class ItemSet {
  public:
    ItemSet(void)  { return; }
    ~ItemSet(void) { return; }

    std::list<Item*>&             items(void)         { return _items; }
    const std::list<Item*>&       items(void) const   { return _items; }
    std::list<Modifier>&          modifiers(void)         { return _modifiers; }
    const std::list<Modifier>&    modifiers(void) const   { return _modifiers; }

    std::string   serializeItems(void) const;
    void          unserializeItems(std::string ser);
    std::string   serializeModifiers(void) const;
    void          unserializeModifiers(std::string ser);

    void  add(Modifier mod);
    void  remove(Modifier mod);

    unsigned          size(void) const;
    unsigned          completion(Being* being) const;
    bool              isComplete(Being* being) const;
    std::list<Item*>  equipped(Being* being) const;
    std::list<Item*>  unequipped(Being* being) const;
    bool              contains(Item* item) const;
    bool              willCompleteSet(Being* being, Item* item) const;
    bool              willBreakSet(Being* being, Item* item) const;

  private:
    std::list<Item*>      _items;
    std::list<Modifier>   _modifiers;
};

#endif // H_SYMPHONY_ITEM_SET
