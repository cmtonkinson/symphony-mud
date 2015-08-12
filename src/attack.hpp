
#ifndef H_SYMPHONY_ATTACK
#define H_SYMPHONY_ATTACK

class Being;
class ItemWeapon;


class Attack {
  public:
    Attack(Being* attacker, Being* defender);
    ~Attack(void);

    void        attacker(Being* attacker_)          { _attacker = attacker_; }
    Being*      attacker(void) const                { return _attacker; }
    void        defender(Being* defender_)          { _defender = defender_; }
    Being*      defender(void) const                { return _defender; }
    void        base(unsigned base_)                { _base = base_; }
    unsigned    base(void) const                    { return _base; }
    void        adjustment(unsigned adjustment_)    { _adjustment = adjustment_; }
    unsigned    adjustment(void) const              { return _adjustment; }
    void        offhand(bool offhand_)              { _offhand = offhand_; }
    bool        offhand(void) const                 { return _offhand; }
    void        unarmed(bool unarmed_)              { _unarmed = unarmed_; }
    bool        unarmed(void) const                 { return _unarmed; }

    bool        hit(void);

    unsigned    getDamage(void);
    void        calculateBase(void);
    void        calculateAdjustments(void);

    unsigned    timeUntilNext(void);

  private:
    void    _init(void);

    Being* _attacker;
    Being* _defender;

    unsigned _base;
    unsigned _adjustment;

    bool _offhand;
    bool _unarmed;
    ItemWeapon* _weapon;
};

#endif // !H_SYMPHONY_ATTACK
