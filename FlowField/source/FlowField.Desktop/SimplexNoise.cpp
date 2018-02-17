#include "stdafx.h"
#include "SimplexNoise.h"


SimplexNoise::SimplexNoise(int numberOfOctaves, double persistence) :
    mOctaveCount(numberOfOctaves)
{
    std::random_device device;
    mRandom = std::default_random_engine(device());

    mOctaves = new SimplexNoiseOctave*[numberOfOctaves];
    mFrequencies = new double[numberOfOctaves];
    mAmplitudes = new double[numberOfOctaves];

    std::uniform_int_distribution<int> range(INT_MIN, INT_MAX);
    for (int i = 0; i < numberOfOctaves; i++)
    {
        mOctaves[i] = new SimplexNoiseOctave(range(mRandom));
        mFrequencies[i] = pow(2, i);
        mAmplitudes[i] = pow(persistence, numberOfOctaves - i);
    }
}

SimplexNoise::~SimplexNoise()
{
    for (int i = 0; i < mOctaveCount; i++)
    {
        delete mOctaves[i];
    }
    delete[] mOctaves;
    delete[] mFrequencies;
    delete[] mAmplitudes;
}

double SimplexNoise::GetNoise(double x, double y)
{
    double result = 0;

    for (int i = 0; i < mOctaveCount; i++)
    {
        result += (mOctaves[i]->Noise(x / mFrequencies[i], y / mFrequencies[i]) * mAmplitudes[i]);
    }

    return result;
}

double SimplexNoise::GetNoise(double x, double y, double z)
{
    double result = 0;
    
    for (int i = 0; i < mOctaveCount; i++)
    {
        result += mOctaves[i]->Noise(x / mFrequencies[i], y / mFrequencies[i], z / mFrequencies[i]) * mAmplitudes[i];
    }
    
    return result;
}
