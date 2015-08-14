
#ifndef H_SYMPHONY_MATH
#define H_SYMPHONY_MATH

#define ROUND_2_INT(f) ((int)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))
#define ROUND_2_UINT(f) ((unsigned)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))

#define MAX(x,y) (x > y ? x : y)
#define MIN(x,y) (X < y ? x : y)

class Math {

  public:

    /*
     * Basic min/max random int function.
     */
    static unsigned rand(unsigned min, unsigned max);

    /*
     * Standard logistic function of the form y = L / (1 + Ae^-kx) where
     *
     * limit is L in the equation above
     * start is the value y(0)
     * half_max_x is the median value of x
     *
     * Assumptions:
     *  1. x is a member of a finite interval [1, N]
     *  2. half_max_x = floor(N / 2)
     *  3. the function produced is relatively "even" (reflecting across X- and Y-axes)
     */
    static double logistic(double x, double limit, double start, double half_max_x);

    /*
     * Return a normally distributed random variable using the Cartesian Box-Muller transform.
     *
     * Note: The Box-Muller transform yields two normally distributed random numbers, but we only
     * need one per call to Math::random_normal() so we store the extra to save computation every
     * other invocation.
     */
    static double random_normal(double mean = 0.0, double stddev = 1.0);

    /* Return the given value bounded by min and max.
     *
     * If value < min, min will be returned.
     * If value > max, max will be returned.
     * If value is between min and max, value will be returned.
     */
    static int      bound(int value, int min, int max);
    static double   bound(double value, double min, double max);

    /*
     */
    static bool percent_chance(unsigned percent);

  private:

    // State variables for the Box-Muller algorithm in Math::random_normal().
    static bool has_extra_random;
    static double extra_random;
};

#endif // #ifndef H_SYMPHONY_MATH
