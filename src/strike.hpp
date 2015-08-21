
#ifndef H_SYMPHONY_STRIKE
#define H_SYMPHONY_STRIKE

class Being;
class ItemWeapon;


class Strike {
  public:
    static const unsigned   SEQUENCE_LIMIT     = 5;
    static constexpr double CRIT_MULTIPLIER    = 0.4;
    static constexpr double FIRST_STRIKE_DELAY = 2.0;
    static constexpr double OFFHAND_PENALTY    = 0.75;

    Strike(void);
    Strike(Being* attacker, Being* defender, bool primary);
    Strike(const Strike& ref);
    ~Strike(void);

    void        attacker(Being* attacker)   { _attacker = attacker; }
    Being*      attacker(void) const        { return _attacker; }
    void        defender(Being* defender)   { _defender = defender; }
    Being*      defender(void) const        { return _defender; }
    void        primary(bool primary)       { _primary = primary; }
    bool        primary(void) const         { return _primary; }
    void        unarmed(bool unarmed)       { _unarmed = unarmed; }
    bool        unarmed(void) const         { return _unarmed; }
    void        weapon(Item* weapon);
    ItemWeapon* weapon(void) const          { return _weapon; }
    void        base(int base_)             { _base = base_; }
    int         base(void) const            { return _base; }
    void        adjustment(int adjustment_) { _adjustment = adjustment_; }
    int         adjustment(void) const      { return _adjustment; }
    void        defense(int defense_)       { _defense = defense_; }
    int         defense(void) const         { return _defense; }
    void        nesting(unsigned nesting)   { _nesting = nesting; }
    unsigned    nesting(void) const         { return _nesting; }

    bool        willMiss(void);
    unsigned    getDamage(void);
    void        calculateBase(void);
    void        calculateAdjustments(void);
    void        calculateDefense(void);

    bool        strike(void);
    bool        evade(void);

    unsigned    timeUntilNext(void);

  private:
    // Important objects.
    Being*      _attacker;
    Being*      _defender;
    ItemWeapon* _weapon;
    // Metadata.
    bool        _primary;
    bool        _unarmed;
    // Calculations.
    int         _base;
    int         _adjustment;
    int         _defense;
    // Display.
    unsigned    _nesting;
};

#endif // !H_SYMPHONY_STRIKE
