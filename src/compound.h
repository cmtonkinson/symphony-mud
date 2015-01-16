
#ifndef H_SYMPHONY_COMPOUND
#define H_SYMPHONY_COMPOUND

#include <set>
#include "flagTable.h"
#include "identifiers.h"

/* Unless noted otherwise, each attribute is expected to be a unitless real number
 * between 0 and 1 inclusive which is set relative to the other compounds being
 * defined in the compounds table.  Such relative values are largely arbitrary.
 *
 * Usually these arbitrary scales are grouped, so that on the whole, for example,
 * clothes will be lighter than metals, and within the cloth category, each cloth
 * is ranked with respect to the others.
 *
 * Certain easily calculable qualitative physical attributes, however, lend
 * themselves too temptingly to "real-world" physical mathematics, so I couldn't
 * resist using actual units and (approximated) real-world values.  An example of
 * such an attribute would be melting point (since thermal calculations in-game
 * will be carried out with proper units in Kelvin.
 */

class Compound {
  public:
    // Constructors...
    Compound(void);
    Compound(const std::string& shortname,
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
              const unsigned long& tags
           );
    ~Compound(void);

    // Attribute bounds...
    static float darknessMin;
    static float darknessMax;
    static float densityMin;
    static float densityMax;
    static float ductilityMin;
    static float ductilityMax;
    static float durabilityMin;
    static float durabilityMax;
    static float electricalConductivityMin;
    static float electricalConductivityMax;
    static float flammabilityMin;
    static float flammabilityMax;
    static float flexibilityMin;
    static float flexibilityMax;
    static float fragranceMin;
    static float fragranceMax;
    static float hardnessMin;
    static float hardnessMax;
    static float lusterMin;
    static float lusterMax;
    static float magicalnessMin;
    static float magicalnessMax;
    static float maleabilityMin;
    static float maleabilityMax;
    static float meltingPointMin;
    static float meltingPointMax;
    static float reactivityMin;
    static float reactivityMax;
    static float smoothnessMin;
    static float smoothnessMax;
    static float thermalConductivityMin;
    static float thermalConductivityMax;

    // Public accessor methods...
    Identifiers&        identifiers(void)               { return _identifiers; }
    const Identifiers&  identifiers(void) const         { return _identifiers; }
    void        darkness(const float& darkness)         { _darkness = darkness < darknessMax ? (darkness > darknessMin ? darkness : darknessMin) : darknessMax; }
    float       darkness(void) const                    { return _darkness; }
    void        density(const float& density)           { _density = density < densityMax ? (density > densityMin ? density : densityMin) : densityMax; }
    float       density(void) const                     { return _density; }
    void        ductility(const float& ductility)       { _ductility = ductility < ductilityMax ? (ductility > ductilityMin ? ductility : ductilityMin) : ductilityMax; }
    float       ductility(void) const                   { return _ductility; }
    void        durability(const float& durability)     { _durability = durability < durabilityMax ? (durability > durabilityMin ? durability : durabilityMin) : durabilityMax; }
    float       durability(void) const                  { return _durability; }
    void        electricalConductivity(const float& electricalConductivity) { _electricalConductivity = electricalConductivity < electricalConductivityMax ? (electricalConductivity > electricalConductivityMin ? electricalConductivity : electricalConductivityMin) : electricalConductivityMax; }
    float       electricalConductivity(void) const      { return _electricalConductivity; }
    void        flammability(const float& flammability) { _flammability = flammability < flammabilityMax ? (flammability > flammabilityMin ? flammability : flammabilityMin) : flammabilityMax; }
    float       flammability(void) const                { return _flammability; }
    void        flexibility(const float& flexibility)   { _flexibility = flexibility < flexibilityMax ? (flexibility > flexibilityMin ? flexibility : flexibilityMin) : flexibilityMax; }
    float       flexibility(void) const                 { return _flexibility; }
    void        fragrance(const float& fragrance)       { _fragrance = fragrance < fragranceMax ? (fragrance > fragranceMin ? fragrance : fragranceMin) : fragranceMax; }
    float       fragrance(void) const                   { return _fragrance; }
    void        hardness(const float& hardness)         { _hardness = hardness < hardnessMax ? (hardness > hardnessMin ? hardness : hardnessMin) : hardnessMax; }
    float       hardness(void) const                    { return _hardness; }
    void        luster(const float& luster)             { _luster = luster < lusterMax ? (luster > lusterMin ? luster : lusterMin) : lusterMax; }
    float       luster(void) const                      { return _luster; }
    void        magicalness(const float& magicalness)   { _magicalness = magicalness < magicalnessMax ? (magicalness > magicalnessMin ? magicalness : magicalnessMin) : magicalnessMax; }
    float       magicalness(void) const                 { return _magicalness; }
    void        maleability(const float& maleability)   { _maleability = maleability < maleabilityMax ? (maleability > maleabilityMin ? maleability : maleabilityMin) : maleabilityMax; }
    float       maleability(void) const                 { return _maleability; }
    void        meltingPoint(const float& meltingPoint) { _meltingPoint = meltingPoint < meltingPointMax ? (meltingPoint > meltingPointMin ? meltingPoint : meltingPointMin) : meltingPointMax; }
    float       meltingPoint(void) const                { return _meltingPoint; }
    void        reactivity(const float& reactivity)     { _reactivity = reactivity < reactivityMax ? (reactivity > reactivityMin ? reactivity : reactivityMin) : reactivityMax; }
    float       reactivity(void) const                  { return _reactivity; }
    void        smoothness(const float& smoothness)     { _smoothness = smoothness < smoothnessMax ? (smoothness > smoothnessMin ? smoothness : smoothnessMin) : smoothnessMax; }
    float       smoothness(void) const                  { return _smoothness; }
    void        thermalConductivity(const float& thermalConductivity) { _thermalConductivity = thermalConductivity < thermalConductivityMax ? (thermalConductivity > thermalConductivityMin ? thermalConductivity : thermalConductivityMin) : thermalConductivityMax; }
    float       thermalConductivity(void) const         { return _thermalConductivity; }
    FlagBank&       tags(void)                          { return _tags; }
    const FlagBank& tags(void) const                    { return _tags; }

  private:
    Identifiers   _identifiers;
    float         _darkness;
    float         _density; // specific gravity, with water being 1.0
    float         _ductility;
    float         _durability;
    float         _electricalConductivity; // *10^7; units assumed S/m (that is, Siemens/meter)
    float         _flammability;
    float         _flexibility;
    float         _fragrance;
    float         _hardness; // mohs scale for metals, stones, and the like - arbitrary values otherwise
    float         _luster;
    float         _magicalness; // can I think up a better word, here?
    float         _maleability;
    float         _meltingPoint; // units are K (that is, Kelvins)
    float         _reactivity; // chemical
    float         _smoothness;
    float         _thermalConductivity; // units are in W/(Km) (that is, Watts over Kelvins*meters)
    FlagBank      _tags;
};



#endif // #ifndef H_SYMPHONY_COMPOUND
