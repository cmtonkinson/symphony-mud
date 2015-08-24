
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

    static constexpr unsigned MISSED    = 1 << 1;
    static constexpr unsigned AVOIDED   = 1 << 2;
    static constexpr unsigned EVADED    = 1 << 3;
    static constexpr unsigned BLOCKED   = 1 << 4;
    static constexpr unsigned HIT       = 1 << 5;
    static constexpr unsigned COUNTERED = 1 << 6;

    Strike(void);
    Strike(Being* attacker, Being* defender, bool primary);
    Strike(const Strike& ref);
    ~Strike(void);

    Strike& operator = (const Strike& ref);

    void        attacker(Being* attacker)                 { _attacker = attacker; }
    Being*      attacker(void) const                      { return _attacker; }
    void        defender(Being* defender)                 { _defender = defender; }
    Being*      defender(void) const                      { return _defender; }
    void        weapon(Item* weapon);
    ItemWeapon* weapon(void) const                        { return _weapon; }
    void        primary(bool primary)                     { _primary = primary; }
    bool        primary(void) const                       { return _primary; }
    void        unarmed(bool unarmed)                     { _unarmed = unarmed; }
    bool        unarmed(void) const                       { return _unarmed; }
    void        status(unsigned status)                   { _status = status; }
    unsigned    status(void) const                        { return _status; }
    void        setStatus(unsigned flag)                  { _status &= flag; }
    void        sequenceNumber(unsigned sequence_number)  { _sequence_number = sequence_number; }
    unsigned    sequenceNumber(void) const                { return _sequence_number; }
    void        base(int base_)                           { _base = base_; }
    int         base(void) const                          { return _base; }
    void        adjustment(int adjustment_)               { _adjustment = adjustment_; }
    int         adjustment(void) const                    { return _adjustment; }
    void        defense(int defense_)                     { _defense = defense_; }
    int         defense(void) const                       { return _defense; }

    bool        willMiss(void);
    unsigned    getDamage(void);
    void        calculateBase(void);
    void        calculateAdjustments(void);
    void        calculateDefense(void);

    bool        strike(void);
    bool        avoid(void);
    bool        reactTo(const Strike* antecedent);
    unsigned    timeUntilNext(void);

  private:
    void        _init(void);

    // Important objects.
    Being*      _attacker;
    Being*      _defender;
    ItemWeapon* _weapon;
    // Metadata.
    bool        _primary;
    bool        _unarmed;
    unsigned    _status;
    unsigned    _sequence_number;
    // Calculations.
    int         _base;
    int         _adjustment;
    int         _defense;
};

#endif // !H_SYMPHONY_STRIKE
