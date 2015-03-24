
#ifndef H_SYMPHONY_FLAGBANK
#define H_SYMPHONY_FLAGBANK

#include <bitset>
#include <stdint.h>

#define A   (1<<0)
#define B   (1<<1)
#define C   (1<<2)
#define D   (1<<3)
#define E   (1<<4)
#define F   (1<<5)
#define G   (1<<6)
#define H   (1<<7)

#define I   (1<<8)
#define J   (1<<9)
#define K   (1<<10)
#define L   (1<<11)
#define M   (1<<12)
#define N   (1<<13)
#define O   (1<<14)
#define P   (1<<15)

#define Q   (1<<16)
#define R   (1<<17)
#define S   (1<<18)
#define T   (1<<19)
#define U   (1<<20)
#define V   (1<<21)
#define W   (1<<22)
#define X   (1<<23)

#define Y   (1<<24)
#define Z   (1<<25)
#define AA  (1<<26)
#define BB  (1<<27)
#define CC  (1<<28)
#define DD  (1<<29)
#define EE  (1<<30)
#define FF  (1<<31)

typedef unsigned long FlagInt;
typedef std::bitset<sizeof(FlagInt)*8> FlagSet;

class FlagTable;

class FlagBank {
  public:
    // constructors...
    FlagBank(FlagInt bank = 0);
    FlagBank(const FlagBank& ref);
    virtual ~FlagBank(void);

    FlagBank&   operator = (const FlagBank& ref)              { _bank = ref.bank(); return *this; }

    // general methods...
    const FlagSet&  bank(void) const                          { return _bank; }
    void            set(void)                                 { _bank.set(); }
    void            set(const FlagInt& position)              { _bank.set(position); }
    void            set(const FlagInt& position, bool value)  { _bank.set(position, value); }
    void            clear(void)                               { _bank.reset(); }
    void            clear(const FlagInt& position)            { _bank.reset(position); }
    bool            toggle(const FlagInt& position)           { _bank.flip(position); return test(position); }
    bool            test(const FlagInt& position) const       { return _bank.test(position); }
    bool            any(void) const                           { return _bank.any(); }
    bool            none(void) const                          { return _bank.none(); }
    void            value(const FlagInt& value)               { _bank = FlagSet(value); }
    FlagInt         value(void) const                         { return _bank.to_ulong(); }
    std::string     list(const FlagTable& table, bool decorative = false) const;

  private:
    FlagSet         _bank;
};

#endif // H_SYMPHONY_FLAGBANK
