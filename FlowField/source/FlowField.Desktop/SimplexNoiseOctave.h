#pragma once
class SimplexNoiseOctave
{
public:

#pragma region Construction/Copy/Assignment
    SimplexNoiseOctave(int seed);
    ~SimplexNoiseOctave();
    SimplexNoiseOctave(const SimplexNoiseOctave& rhs) = delete;
    SimplexNoiseOctave& operator=(const SimplexNoiseOctave& rhs) = delete;
    SimplexNoiseOctave(SimplexNoiseOctave&& rhs) = delete;
    SimplexNoiseOctave& operator=(SimplexNoiseOctave&& rhs) = delete;
#pragma endregion

    /**
     * Computes 2D simplex noise based on the given coordinates
     * @Param xin: The simplex x-coordinate
     * @Param yin: The simplex y-coordinate
     * @Return: The simplex noise value computed at the given coordinate
     */
    double Noise(double xin, double yin);

    /**
     * Computes 3D simplex noise based on the given coordinates
     * @Param xin: The simplex x-coordinate
     * @Param yin: The simplex y-coordinate
     * @Param zin: The ximplex z-coordinate
     * @Return: The simplex noise value computed at the given coordinate
     */
    double Noise(double xin, double yin, double zin);

private:

    /**
     *  Computes the dot product of two vectors
     *  @Param point: The first vector
     *  @Param x: The x-value of the second vector
     *  @Param y: The y-value of the second vector
     *  @Return: The dot product of the two vectors
     */
    double Dot(sf::Vector2i point, double x, double y);

    /**
     * Computes the dot product of two vectors
     * @Param point: The first vector
     * @Param x: The x-value of the second vector
     * @Param y: The y-value of the second vector
     * @Param z: The z-value of the second vector
     * @Return: The dot product of the two vectors
     */
    double Dot(sf::Vector3i point, double x, double y, double z);

    /**
     * Floors a given value
     * @Param x: The value being floored
     * @Return: The truncated value of x
     */
    int FastFloor(double x);

    /**
     * Initializes the point field used for 2D simplex noise
     */
    void InitializePoints2D();

    /**
     * Initializes the point field used for 3D simplex noise
     */
    void InitializePoints3D();

    /**
     * Initializes the point supply used for all simplex noise
     */
    void InitializePointSupply();

    /**
     * Shuffles all values in the point array
     */
    void ShufflePointValues();

    std::default_random_engine mRandom;

    int mNumberOfSwaps = 400;
    sf::Vector2i* mPoints2d;
    sf::Vector3i* mPoints3d;

    static const int sPointFieldSize = 255;
    static short* sPointSupply;
    short* mP;
    short* mPerm;
    short* mPermMod12;

    double mF2;
    double mG2;
    double mF3;
    double mG3;

};

