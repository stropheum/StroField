#pragma once
class SimplexNoise
{

public:

#pragma region Construction/Copy/Assignment
    SimplexNoise(int numberOfOctaves, double persistence);
    ~SimplexNoise();
    SimplexNoise(const SimplexNoise& rhs) = delete;
    SimplexNoise& operator=(const SimplexNoise& rhs) = delete;
    SimplexNoise(SimplexNoise&& rhs) = delete;
    SimplexNoise& operator=(SimplexNoise&& rhs) = delete;
#pragma endregion

    /**
     * Computes 2D simplex noise at the given coordinates
     * @Param x: The simplex x-coordinate
     * @Param y: The simplex y-coordinate
     * @Return: Simplex noise value at the given coordinates
     */
    double GetNoise(double x, double y);
    
    /**
     * Computes 3D simplex noise at the given coordinates
     * @Param x: The simplex x-coordinate
     * @Param y: The simplex y-coordinate
     * @Param z: The simplex z-coordinate
     * @Return: Simplex noise value at the given coordinates
     */
    double GetNoise(double x, double y, double z);

private:

    std::default_random_engine mRandom; // Random number generator used for simplex noise
    SimplexNoiseOctave** mOctaves;      // The array of octaves used to generate the final simplex noise gradient
    double* mFrequencies;               // The frequencies associated with each simplex noise octave
    double* mAmplitudes;                // The amplitudes associated with each simplex noise octave
    int mOctaveCount;                   // The number of octaves that will be generated

};

