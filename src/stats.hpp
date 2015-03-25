
#ifndef H_SYMPHONY_STATS
#define H_SYMPHONY_STATS

/*
 * Terminology:
 *
 * gain: A non-negative integer intended primarily as a modifier to some Being attribute upon
 *       leveling up. The magnitude of the gain may be completely deterministic, completely random,
 *       or anything in between, depending on the usage.
 */
class Stats {

  public:

    /*
     * Returns a gain for a stat based on a deterministic logistic function.
     *
     * level - The level of the Being (in the context of a level up, this is the NEW level).
     * base - The minimum gain for the growth function (i.e. the theoretical gain at level 1).
     * target - The maximum sum of all gains for a particular growth function from level 1 to 100.
     *
     * This growth function produces a deterministic gain for a given level based on fixing two
     * values - the initial value of the function (level = 0) and the final accumulated total of
     * values the curve will produce between levels 1 and 100. It's a method of reverse-engineering
     * the gains given a desired stat value at max-level. Base and target, respectively, are the
     * factors used to tune the function to achieve a given result.
     *
     * Example: Suppose a warrior should get a health boost of about 6 hp at level 2, and a Hero
     * warrior should have approximately 4k hp. This particular warrior just reached level 19. The
     * function call would look like `logistic_gain(19, 6, 4000);`
     */
    static unsigned logistic(unsigned level, unsigned base, unsigned target);

    /*
     */
    static int polynomial(int level, int base, int target);

    /*
     * Transforms the input by adding randomness. The randomness is added using a normal
     * distribution and the amount of variability is controlled in part by CONE_FACTOR, and in part
     * by the magnitude of the input value (the larger the input, the more variability in the
     * output).
     */
    static int cone_randomization(int value, int cone_factor = CONE_FACTOR);

    /*
     * CONE_FACTOR is used to control the standard deviation of outputs from the cone randomization
     * function. The larger CONE_FACTOR is, the smaller the standard deviation used, and so the
     * less variablility in the output. 95% of output will lie within (CONE_FACTOR/2)% of the mean.
     */
    static const unsigned short CONE_FACTOR = 20; // 95% of output within 10% of mean

};

#endif // #ifndef H_SYMPHONY_STATS
