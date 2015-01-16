
#ifndef H_SYMPHONY_MODIFIER
#define H_SYMPHONY_MODIFIER

class Modifier {
  public:
    // Constructors...
    Modifier(void);
    Modifier(const Modifier& ref);
    Modifier(const unsigned short& attribute, const int& magnitude);
    ~Modifier(void);

    // Public accessor methods...
    void            attribute(const unsigned short& attribute)  { _attribute = attribute; }
    unsigned short  attribute(void) const                       { return _attribute; }
    void            magnitude(const int& magnitude)             { _magnitude = magnitude; }
    int             magnitude(void) const                       { return _magnitude; }

  private:
    unsigned short  _attribute;
    int             _magnitude;
};

#endif // #ifndef H_SYMPHONY_MODIFIER
