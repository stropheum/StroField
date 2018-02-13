#include "stdafx.h"
#include "SimplexNoiseOctave.h"


short* SimplexNoiseOctave::p_supply = new short[256]{ 42, 222, 46, 158, 150, 17, 224, 11,
252, 133, 220, 190, 93, 151, 161, 243, 54, 48,78, 56, 20, 9, 66, 250, 219, 25, 189, 31,
111, 14, 235, 131, 34, 170, 232, 101, 156, 159, 174, 88, 247, 41, 5, 122, 163, 23, 107,
12, 230, 91, 50, 173, 135, 94, 248, 209, 92, 229, 40, 144, 241, 74, 134, 187, 194, 118,
64, 114, 75, 36, 251, 160, 183, 130, 197, 200,205, 6,191, 166, 70, 188, 45,83, 60, 132,
154, 147, 215, 53,227, 202, 245, 238, 13, 87, 223, 225, 103, 162, 72, 152, 63,120, 171,
168, 108, 115, 117, 180, 110, 7,  121, 82, 15, 116, 149, 139, 207, 97, 3, 213, 112, 39,
196, 172, 126, 30, 206, 179, 10, 102, 28, 164, 138, 236, 231, 59, 193, 24, 32, 226, 73,
68, 182, 192, 100, 2, 181, 255, 167, 175, 155, 153,157, 246, 4, 214, 141, 137, 29, 234,
89, 18, 19, 212, 142, 44, 47, 169, 1, 71, 37, 113, 104, 204, 210, 27, 186, 216, 95, 62,
185, 16, 105, 61, 76,184, 218, 58, 239, 177, 22, 176,96, 195, 199,86, 143, 99, 49, 146,
254, 203, 69, 51, 79, 217, 240, 21, 8, 211, 140, 57, 109, 145, 221, 35, 38, 26, 0, 136,
124, 237, 85, 81, 242, 129, 77, 123, 80, 178, 106, 201, 84, 43, 119, 127, 125, 128, 65,
228, 253, 67, 148, 90, 55, 52, 233, 198, 98, 249, 244, 208, 33, 165 };

SimplexNoiseOctave::SimplexNoiseOctave(int seed) :
    F2(0.5f * (sqrt(3.0f) - 1.0f)), G2((3.0f - sqrt(3.0f)) / 6.0f),
    F3(1.0f / 3.0f), G3(1.0f / 6.0f)
{
    random = std::default_random_engine(seed);

    points2d = new sf::Vector2i[4];
    points2d[0] = sf::Vector2i( 1,  1);
    points2d[1] = sf::Vector2i(-1,  1);
    points2d[2] = sf::Vector2i( 1, -1); 
    points2d[3] = sf::Vector2i(-1, -1);

    points3d = new sf::Vector3i[12];
    points3d[0]  = sf::Vector3i( 1, 1, 0);
    points3d[1]  = sf::Vector3i(-1, 1, 0);
    points3d[2]  = sf::Vector3i( 1,-1, 0);
    points3d[3]  = sf::Vector3i(-1,-1, 0);
    points3d[4]  = sf::Vector3i( 1, 0, 1);
    points3d[5]  = sf::Vector3i(-1, 0, 1);
    points3d[6]  = sf::Vector3i( 1, 0,-1);
    points3d[7]  = sf::Vector3i(-1, 0,-1);
    points3d[8]  = sf::Vector3i( 0, 1, 1);
    points3d[9]  = sf::Vector3i( 0,-1, 1);
    points3d[10] = sf::Vector3i( 0, 1,-1);
    points3d[11] = sf::Vector3i( 0,-1,-1);

    p = new short[256];
    perm = new short[512];
    permMod12 = new short[512];

    // Clone p_supply
    for (int i = 0; i < 256; i++)
    {
        p[i] = p_supply[i];
    }

    std::uniform_int_distribution<int> range(0, 255); // Length of p
    for (int i = 0; i < numberOfSwaps; i++)
    {
        int from = range(random);
        int to = range(random);

        short temp = p[from];
        p[from] = p[to];
        p[to] = temp;
    }

    for (int i = 0; i < 512; i++) // length of perm
    {
        perm[i] = p[i & 255];
        permMod12[i] = static_cast<short>(perm[i] % 4);
    }
}

SimplexNoiseOctave::~SimplexNoiseOctave()
{
    //delete[] points;
    //delete[] p;
    //delete[] perm;
    //delete[] permMod12;
}

double SimplexNoiseOctave::Noise(double xin, double yin)
{
    double s = (xin + yin) * F2;
    int i = FastFloor(xin + s);
    int j = FastFloor(yin + s);
    double t = (i + j) * G2;
    double X0 = i - t;
    double Y0 = j - t;
    double x0 = xin - X0;
    double y0 = yin - Y0;

    int i1 = 0;
    int j1 = 0;
    
    if (x0 > y0)
    {
        i1 = 1;
        j1 = 0;
    }
    else
    {
        i1 = 0;
        j1 = 1;
    }

    double x1 = x0 - i1 + G2;
    double y1 = y0 - j1 + G2;
    double x2 = x0 - 1.0f + 2.0f * G2;
    double y2 = y0 - 1.0f + 2.0f * G2;

    int ii = i & 255;
    int jj = j & 255;

    int gi0 = permMod12[ii + perm[jj]];
    int gi1 = permMod12[ii + i1 + perm[jj + j1]];
    int gi2 = permMod12[ii + 1 + perm[jj + 1]];

    double n0 = 0.0f;
    double n1 = 0.0f;
    double n2 = 0.0f;

    double t0 = 0.5f - (x0 * x0) - (y0 * y0);
    if (t0 < 0)
    {
        n0 = 0;
    }
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * Dot(points2d[gi0], x0, y0);
    }

    double t1 = 0.5f - (x1 * x1) - (y1 * y1);
    if (t1 < 0)
    {
        n1 = 0;
    }
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * Dot(points2d[gi1], x1, y1);
    }

    double t2 = 0.5f - (x2 * x2) - (y2 * y2);
    if (t2 < 0)
    {
        n2 = 0;
    }
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * Dot(points2d[gi2], x2, y2);
    }

    return 70 * (n0 + n1 + n2);
}

double SimplexNoiseOctave::Noise(double xin, double yin, double zin)
{
    double n0 = 0;
    double n1 = 0;
    double n2 = 0;
    double n3 = 0;

    double s = (xin + yin + zin) * F3;
    int i = FastFloor(xin + s);
    int j = FastFloor(yin + s);
    int k = FastFloor(zin + s);

    double t = (i + j + k) * G3;
    double X0 = i - t;
    double Y0 = j - t;
    double Z0 = k - t;
    double x0 = xin - X0;
    double y0 = yin - Y0;
    double z0 = zin - Z0;

    int i1 = 0;
    int j1 = 0;
    int k1 = 0;
    int i2 = 0;
    int j2 = 0;
    int k2 = 0;

    if (x0 >= y0)
    {
        if (y0 >= z0)
        {
            i1 = 1;
            j1 = 0;
            k1 = 0;
            i2 = 1; 
            j2 = 1;
            k2 = 0;
        }
        else if (x0 >= z0)
        {
            i1 = 1;
            j1 = 0;
            k1 = 0;
            i2 = 1;
            j2 = 0;
            k2 = 1;
        }
        else
        {
            i1 = 0;
            j1 = 0;
            k1 = 1;
            i2 = 1;
            j2 = 0;
            k2 = 1;
        }
    }
    else
    {
        if (y0 < z0)
        {
            i1 = 0;
            j1 = 0;
            k1 = 1;
            i2 = 0;
            j2 = 1;
            k2 = 1;
        }
        else if (x0 < z0)
        {
            i1 = 0;
            j1 = 1;
            k1 = 0;
            i2 = 0;
            j2 = 1;
            k2 = 1;
        }
        else
        {
            i1 = 0;
            j1 = 1;
            k1 = 0;
            i2 = 1;
            j2 = 1;
            k2 = 0;
        }
    }

    double x1 = x0 - i1 + G3;
    double y1 = y0 - j1 + G3;
    double z1 = z0 - k1 + G3;
    double x2 = x0 - i2 + 2.0f * G3;
    double y2 = y0 - j2 + 2.0f * G3;
    double z2 = z0 - k2 + 2.0f * G3;
    double x3 = x0 - 1.0f + 3.0f * G3;
    double y3 = y0 - 1.0f + 3.0f * G3;
    double z3 = z0 - 1.0f + 3.0f * G3;

    int ii = i & 255;
    int jj = j & 255;
    int kk = k & 255;
    int gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
    int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
    int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
    int gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;

    double t0 = 0.6f - (x0 * x0) - (y0 * y0) - (z0 * z0);
    if (t0 < 0)
    {
        n0 = 0.0f;
    }
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * Dot(points3d[gi0], x0, y0, z0);
    }

    double t1 = 0.6f - (x1 * x1) - (y1 * y1) - (z1 * z1);
    if (t1 < 0)
    {
        n1 = 0.0f;
    }
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * Dot(points3d[gi1], x1, y1, z1);
    }

    double t2 = 0.6f - (x2 * x2) - (y2 * y2) - (z2 * z2);
    if (t2 < 0)
    {
        n2 = 0.0f;
    }
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * Dot(points3d[gi2], x2, y2, z2);
    }

    double t3 = 0.6f - (x3 * x3) - (y3 * y3) - (z3 * z3);
    if (t3 < 0)
    {
        n3 = 0.0f;
    }
    else
    {
        t3 *= t3;
        n3 = t3 * t3 * Dot(points3d[gi3], x3, y3, z3);
    }

    return 32.0f * (n0 + n1 + n2 + n3);
}

double SimplexNoiseOctave::Dot(sf::Vector2i point, double x, double y)
{
    return point.x * x + point.y * y;
}

double SimplexNoiseOctave::Dot(sf::Vector3i point, double x, double y, double z)
{
    return point.x * x + point.y * y + point.z * z;
}

int SimplexNoiseOctave::FastFloor(double x)
{
    return (x > 0) ? static_cast<int>(x) : static_cast<int>(x - 1);
}
