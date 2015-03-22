
#ifndef H_SYMPHONY_COMPOUND
#define H_SYMPHONY_COMPOUND

#include <set>
#include "flag-table.h"
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

    static constexpr double DARKNESS_MIN                = 0.0;
    static constexpr double DARKNESS_MAX                = 1.0;
    static constexpr double DENSITY_MIN                 = 0.0;
    static constexpr double DENSITY_MAX                 = 100.0;
    static constexpr double DUCTILITY_MIN               = 0.0;
    static constexpr double DUCTILITY_MAX               = 1.0;
    static constexpr double DURABILITY_MIN              = 0.0;
    static constexpr double DURABILITY_MAX              = 1.0;
    static constexpr double ELECTRICAL_CONDUCTIVITY_MIN = 0.0;
    static constexpr double ELECTRICAL_CONDUCTIVITY_MAX = 100.0;
    static constexpr double FLAMMABILITY_MIN            = 0.0;
    static constexpr double FLAMMABILITY_MAX            = 1.0;
    static constexpr double FLEXIBILITY_MIN             = 0.0;
    static constexpr double FLEXIBILITY_MAX             = 1.0;
    static constexpr double FRAGRANCE_MIN               = 0.0;
    static constexpr double FRAGRANCE_MAX               = 1.0;
    static constexpr double HARDNESS_MIN                = 0.0;
    static constexpr double HARDNESS_MAX                = 15.0;
    static constexpr double LUSTER_MIN                  = 0.0;
    static constexpr double LUSTER_MAX                  = 1.0;
    static constexpr double MAGICALNESS_MIN             = 0.0;
    static constexpr double MAGICALNESS_MAX             = 1.0;
    static constexpr double MALEABILITY_MIN             = 0.0;
    static constexpr double MALEABILITY_MAX             = 1.0;
    static constexpr double MELTING_POINT_MIN           = 0.0;
    static constexpr double MELTING_POINT_MAX           = 100000000.0; // (100,000,000 == one hundred million)
    static constexpr double REACTIVITY_MIN              = 0.0;
    static constexpr double REACTIVITY_MAX              = 1.0;
    static constexpr double SMOOTHNESS_MIN              = 0.0;
    static constexpr double SMOOTHNESS_MAX              = 1.0;
    static constexpr double THERMAL_CONDUCTIVITY_MIN    = 0.0;
    static constexpr double THERMAL_CONDUCTIVITY_MAX    = 1.0;

    // Constructors...
    Compound(void);
    Compound( std::string shortname,
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
              unsigned long tags
    );
    ~Compound(void);

    // Public accessor methods...
    Identifiers&        identifiers(void)                             { return _identifiers; }
    const Identifiers&  identifiers(void) const                       { return _identifiers; }
    void       darkness(double darkness)                              { _darkness = darkness < DARKNESS_MAX ? (darkness > DARKNESS_MIN ? darkness : DARKNESS_MIN) : DARKNESS_MAX; }
    double     darkness(void) const                                   { return _darkness; }
    void       density(double density)                                { _density = density < DENSITY_MAX ? (density > DENSITY_MIN ? density : DENSITY_MIN) : DENSITY_MAX; }
    double     density(void) const                                    { return _density; }
    void       ductility(double ductility)                            { _ductility = ductility < DUCTILITY_MAX ? (ductility > DUCTILITY_MIN ? ductility : DUCTILITY_MIN) : DUCTILITY_MAX; }
    double     ductility(void) const                                  { return _ductility; }
    void       durability(double durability)                          { _durability = durability < DURABILITY_MAX ? (durability > DURABILITY_MIN ? durability : DURABILITY_MIN) : DURABILITY_MAX; }
    double     durability(void) const                                 { return _durability; }
    void       electricalConductivity(double electricalConductivity)  { _electricalConductivity = electricalConductivity < ELECTRICAL_CONDUCTIVITY_MAX ? (electricalConductivity > ELECTRICAL_CONDUCTIVITY_MIN ? electricalConductivity : ELECTRICAL_CONDUCTIVITY_MIN) : ELECTRICAL_CONDUCTIVITY_MAX; }
    double     electricalConductivity(void) const                     { return _electricalConductivity; }
    void       flammability(double flammability)                      { _flammability = flammability < FLAMMABILITY_MAX ? (flammability > FLAMMABILITY_MIN ? flammability : FLAMMABILITY_MIN) : FLAMMABILITY_MAX; }
    double     flammability(void) const                               { return _flammability; }
    void       flexibility(double flexibility)                        { _flexibility = flexibility < FLEXIBILITY_MAX ? (flexibility > FLEXIBILITY_MIN ? flexibility : FLEXIBILITY_MIN) : FLEXIBILITY_MAX; }
    double     flexibility(void) const                                { return _flexibility; }
    void       fragrance(double fragrance)                            { _fragrance = fragrance < FRAGRANCE_MAX ? (fragrance > FRAGRANCE_MIN ? fragrance : FRAGRANCE_MIN) : FRAGRANCE_MAX; }
    double     fragrance(void) const                                  { return _fragrance; }
    void       hardness(double hardness)                              { _hardness = hardness < HARDNESS_MAX ? (hardness > HARDNESS_MIN ? hardness : HARDNESS_MIN) : HARDNESS_MAX; }
    double     hardness(void) const                                   { return _hardness; }
    void       luster(double luster)                                  { _luster = luster < LUSTER_MAX ? (luster > LUSTER_MIN ? luster : LUSTER_MIN) : LUSTER_MAX; }
    double     luster(void) const                                     { return _luster; }
    void       magicalness(double magicalness)                        { _magicalness = magicalness < MAGICALNESS_MAX ? (magicalness > MAGICALNESS_MIN ? magicalness : MAGICALNESS_MIN) : MAGICALNESS_MAX; }
    double     magicalness(void) const                                { return _magicalness; }
    void       maleability(double maleability)                        { _maleability = maleability < MALEABILITY_MAX ? (maleability > MALEABILITY_MIN ? maleability : MALEABILITY_MIN) : MALEABILITY_MAX; }
    double     maleability(void) const                                { return _maleability; }
    void       meltingPoint(double meltingPoint)                      { _meltingPoint = meltingPoint < MELTING_POINT_MAX ? (meltingPoint > MELTING_POINT_MIN ? meltingPoint : MELTING_POINT_MIN) : MELTING_POINT_MAX; }
    double     meltingPoint(void) const                               { return _meltingPoint; }
    void       reactivity(double reactivity)                          { _reactivity = reactivity < REACTIVITY_MAX ? (reactivity > REACTIVITY_MIN ? reactivity : REACTIVITY_MIN) : REACTIVITY_MAX; }
    double     reactivity(void) const                                 { return _reactivity; }
    void       smoothness(double smoothness)                          { _smoothness = smoothness < SMOOTHNESS_MAX ? (smoothness > SMOOTHNESS_MIN ? smoothness : SMOOTHNESS_MIN) : SMOOTHNESS_MAX; }
    double     smoothness(void) const                                 { return _smoothness; }
    void       thermalConductivity(double thermalConductivity)        { _thermalConductivity = thermalConductivity < THERMAL_CONDUCTIVITY_MAX ? (thermalConductivity > THERMAL_CONDUCTIVITY_MIN ? thermalConductivity : THERMAL_CONDUCTIVITY_MIN) : THERMAL_CONDUCTIVITY_MAX; }
    double     thermalConductivity(void) const                        { return _thermalConductivity; }
    FlagBank&       tags(void)                                        { return _tags; }
    const FlagBank& tags(void) const                                  { return _tags; }

  private:
    Identifiers   _identifiers;
    double         _darkness;
    double         _density;                // specific gravity, with water being 1.0
    double         _ductility;
    double         _durability;
    double         _electricalConductivity; // *10^7; units assumed S/m (that is, Siemens/meter)
    double         _flammability;
    double         _flexibility;
    double         _fragrance;
    double         _hardness;               // mohs scale for metals, stones, and the like - arbitrary values otherwise
    double         _luster;
    double         _magicalness;            // can I think up a better word, here?
    double         _maleability;
    double         _meltingPoint;           // units are K (that is, Kelvins)
    double         _reactivity;             // chemical
    double         _smoothness;
    double         _thermalConductivity;    // units are in W/(Km) (that is, Watts over Kelvins*meters)
    FlagBank      _tags;
};

#endif // #ifndef H_SYMPHONY_COMPOUND
