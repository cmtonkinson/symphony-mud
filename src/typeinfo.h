
#ifndef H_TYPEINFO
#define H_TYPEINFO

#include <typeinfo>

using std::type_info;

class Typeinfo {
  public:
    explicit          Typeinfo(const type_info& ref): _info(ref)  { return; }
    bool              operator < (const Typeinfo& ref) const      { return _info.before(ref.info()); }
    const type_info&  info(void) const                            { return _info; }
  private:
    const type_info& _info;
};

#endif // #ifndef H_EVENT
