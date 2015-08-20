
#ifndef H_SYMPHONY_ATTACK_DAMAGE
#define H_SYMPHONY_ATTACK_DAMAGE

class Being;
class ItemWeapon;


class StrikeDamage {
  public:

    static constexpr double FIRST_ATTACK_DELAY = 2.0;
    static constexpr double CRIT_MULTIPLIER    = 0.4;

    StrikeDamage(Being* attacker, Being* defender);
    ~StrikeDamage(void);

    void        attacker(Being* attacker_)          { _attacker = attacker_; }
    Being*      attacker(void) const                { return _attacker; }
    void        defender(Being* defender_)          { _defender = defender_; }
    Being*      defender(void) const                { return _defender; }
    void        base(unsigned base_)                { _base = base_; }
    unsigned    base(void) const                    { return _base; }
    void        adjustment(unsigned adjustment_)    { _adjustment = adjustment_; }
    unsigned    adjustment(void) const              { return _adjustment; }
    void        defense(unsigned defense_)          { _defense = defense_; }
    unsigned    defense(void) const                 { return _defense; }
    void        offhand(bool offhand_)              { _offhand = offhand_; }
    bool        offhand(void) const                 { return _offhand; }
    void        unarmed(bool unarmed_)              { _unarmed = unarmed_; }
    bool        unarmed(void) const                 { return _unarmed; }
    ItemWeapon* weapon(void)                        { return _weapon; }

    void        init(void);

    bool        hit(void);

    unsigned    getDamage(void);
    void        calculateBase(void);
    void        calculateAdjustments(void);
    void        calculateDefense(void);

    unsigned    timeUntilNext(void);

  private:

    Being*      _attacker;
    Being*      _defender;
    unsigned    _base;
    unsigned    _adjustment;
    unsigned    _defense;
    bool        _offhand;
    bool        _unarmed;
    ItemWeapon* _weapon;
};

#endif // !H_SYMPHONY_ATTACK_DAMAGE
