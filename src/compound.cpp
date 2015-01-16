
#include "compound.h"

Compound::Compound(void) {
  return;
}

Compound::Compound( std::string shortname,
                    std::string longname,
                    double darkness,
                    double density,
                    double ductility,
                    double durability,
                    double electricalConductivity,
                    double flammability,
                    double flexibility,
                    double fragrance,
                    double hardness,
                    double luster,
                    double magicalness,
                    double maleability,
                    double meltingPoint,
                    double reactivity,
                    double smoothness,
                    double thermalConductivity,
                    unsigned long tags)
                    : _darkness(darkness),
                      _density(density),
                      _ductility(ductility),
                      _durability(durability),
                      _electricalConductivity(electricalConductivity),
                      _flammability(flammability),
                      _flexibility(flexibility),
                      _fragrance(fragrance),
                      _hardness(hardness),
                      _luster(luster),
                      _magicalness(magicalness),
                      _maleability(maleability),
                      _meltingPoint(meltingPoint),
                      _reactivity(reactivity),
                      _smoothness(smoothness),
                      _thermalConductivity(thermalConductivity),
                      _tags(tags) {
  identifiers().shortname(shortname);
  identifiers().longname(longname);
  return;
}

Compound::~Compound(void) {
  return;
}
