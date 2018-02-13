#pragma once
class SimplexNoise
{

public:

    SimplexNoise(int numberOfOctaves, double persistence);
    ~SimplexNoise();

    double GetNoise(int x, int y);
    double GetNoise(int x, int y, int z);

    static int SEED;

private:

    std::default_random_engine random;
    SimplexNoiseOctave** octaves;
    double* frequencies;
    double* amplitudes;
    int octaveCount;

};

