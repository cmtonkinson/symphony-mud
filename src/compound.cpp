
#include "compound.h"

// Attribute bound definitions...
float Compound::darknessMin = 0.0;
float Compound::darknessMax = 1.0;
float Compound::densityMin = 0.0;
float Compound::densityMax = 100.0;
float Compound::ductilityMin = 0.0;
float Compound::ductilityMax = 1.0;
float Compound::durabilityMin = 0.0;
float Compound::durabilityMax = 1.0;
float Compound::electricalConductivityMin = 0.0;
float Compound::electricalConductivityMax = 100.0;
float Compound::flammabilityMin = 0.0;
float Compound::flammabilityMax = 1.0;
float Compound::flexibilityMin = 0.0;
float Compound::flexibilityMax = 1.0;
float Compound::fragranceMin = 0.0;
float Compound::fragranceMax = 1.0;
float Compound::hardnessMin = 0.0;
float Compound::hardnessMax = 15.0;
float Compound::lusterMin = 0.0;
float Compound::lusterMax = 1.0;
float Compound::magicalnessMin = 0.0;
float Compound::magicalnessMax = 1.0;
float Compound::maleabilityMin = 0.0;
float Compound::maleabilityMax = 1.0;
float Compound::meltingPointMin = 0.0;
float Compound::meltingPointMax = 100000000.0; // (100,000,000 == one hundred million)
float Compound::reactivityMin = 0.0;
float Compound::reactivityMax = 1.0;
float Compound::smoothnessMin = 0.0;
float Compound::smoothnessMax = 1.0;
float Compound::thermalConductivityMin = 0.0;
float Compound::thermalConductivityMax = 1.0;

Compound::Compound(void) {
  return;
}

Compound::Compound(const std::string& shortname,
                    const std::string& longname,
                    const float& darkness,
                    const float& density,
                    const float& ductility,
                    const float& durability,
                    const float& electricalConductivity,
                    const float& flammability,
                    const float& flexibility,
                    const float& fragrance,
                    const float& hardness,
                    const float& luster,
                    const float& magicalness,
                    const float& maleability,
                    const float& meltingPoint,
                    const float& reactivity,
                    const float& smoothness,
                    const float& thermalConductivity,
                    const unsigned long& tags)
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
