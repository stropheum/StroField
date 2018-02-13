#pragma once
class SimplexNoiseOctave
{
public:
    SimplexNoiseOctave(int seed);
    ~SimplexNoiseOctave();

    double Noise(double xin, double yin);
    double Noise(double xin, double yin, double zin);

private:

    double Dot(sf::Vector2i point, double x, double y);
    double Dot(sf::Vector3i point, double x, double y, double z);

    int FastFloor(double x);

    std::default_random_engine random;

    int numberOfSwaps = 400;
    sf::Vector2i* points2d;
    sf::Vector3i* points3d;

    static short* p_supply;
    short* p;
    short* perm;
    short* permMod12;

    double F2;
    double G2;

    double F3;
    double G3;

};

