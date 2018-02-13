#include "stdafx.h"
#include "SimplexNoise.h"


int SimplexNoise::SEED = 6199519165653446891;

SimplexNoise::SimplexNoise(int numberOfOctaves, double persistence) :
    octaveCount(numberOfOctaves)
{
    random = std::default_random_engine(SEED);

    octaves = new SimplexNoiseOctave*[numberOfOctaves];
    frequencies = new double[numberOfOctaves];
    amplitudes = new double[numberOfOctaves];

    std::uniform_int_distribution<int> range(INT_MIN, INT_MAX);
    for (int i = 0; i < numberOfOctaves; i++)
    {
        octaves[i] = new SimplexNoiseOctave(range(random));
        frequencies[i] = pow(2, i);
        amplitudes[i] = pow(persistence, numberOfOctaves - i);
    }
}

SimplexNoise::~SimplexNoise()
{
    for (int i = 0; i < octaveCount; i++)
    {
        delete octaves[i];
    }
    delete[] octaves;
    delete[] frequencies;
    delete[] amplitudes;
}

double SimplexNoise::GetNoise(double x, double y)
{
    double result = 0;

    for (int i = 0; i < octaveCount; i++)
    {
        result += (octaves[i]->Noise(x / frequencies[i], y / frequencies[i]) * amplitudes[i]);
    }

    return result;
}

double SimplexNoise::GetNoise(double x, double y, double z)
{
    double result = 0;
    
    for (int i = 0; i < octaveCount; i++)
    {
        result += octaves[i]->Noise(x / frequencies[i], y / frequencies[i], z / frequencies[i]) * amplitudes[i];
    }
    
    return result;
}
