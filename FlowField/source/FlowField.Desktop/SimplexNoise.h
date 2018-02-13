#pragma once
class SimplexNoise
{

public:

    SimplexNoise(int numberOfOctaves, double persistence);
    ~SimplexNoise();

    double GetNoise(double x, double y);
    double GetNoise(double x, double y, double z);

    static int SEED;

private:

    std::default_random_engine random;
    SimplexNoiseOctave** octaves;
    double* frequencies;
    double* amplitudes;
    int octaveCount;

};

