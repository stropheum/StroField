#include "stdafx.h"
#include "SimplexNoiseOctave.h"


// pre-shuffled values from 0-255 to allow a more arbitrary start-point
short* SimplexNoiseOctave::sPointSupply = new short[256]{ 42, 222, 46, 158, 150, 17, 224, 11,
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
    mF2(0.5f * (sqrt(3.0f) - 1.0f)), 
    mG2((3.0f - sqrt(3.0f)) / 6.0f),
    mF3(1.0f / 3.0f), 
    mG3(1.0f / 6.0f)
{
    mRandom = std::default_random_engine(seed);
    InitializePoints2D();
    InitializePoints3D();
    InitializePointSupply();
}

SimplexNoiseOctave::~SimplexNoiseOctave()
{
    delete[] mP;
    delete[] mPerm;
    delete[] mPermMod12;
    delete[] mPoints2d;
    delete[] mPoints3d;
}

double SimplexNoiseOctave::Noise(double xin, double yin)
{
    double s = (xin + yin) * mF2;

    int I[2]{ 0 };
    int J[2]{ 0 };
    I[0] = FastFloor(xin + s);
    J[0] = FastFloor(yin + s);

    double T = (I[0] + J[0]) * mG2;
    double X0 =I[0] - T;
    double Y0 = J[0] - T;
    
    double x[3]{ 0.0f };
    double y[3]{ 0.0f };
    x[0] = xin - X0;
    y[0] = yin - Y0;

    if (x[0] > y[0])
    {
        I[1] = 1;
        J[1] = 0;
    }
    else
    {
        I[1] = 0;
        J[1] = 1;
    }

    for (int i = 1; i < 3; i++)
    {
        x[i] = x[0] - (i < 2 ? I[i] : 1.0f) + (i * mG2);
        y[i] = y[0] - (i < 2 ? J[i] : 1.0f) + (i * mG2);
    }

    int ii = I[0] & 255;
    int jj = J[0] & 255;

    int gi[3]{ 0 };
    gi[0] = mPermMod12[ii + mPerm[jj]];
    gi[1] = mPermMod12[ii + I[1] + mPerm[jj + J[1]]];
    gi[2] = mPermMod12[ii + 1 + mPerm[jj + 1]];

    double n[3]{ 0.0f };
    double t[3]{ 0.0f };

    for (int i = 0; i < 3; i++)
    {
        t[i] = 0.5f - (x[i] * x[i]) - (y[i] * y[i]);
        if (t[i] < 0)
        {
            n[i] = 0;
        }
        else
        {
            t[i] *= t[i];
            n[i] = t[i] * t[i] * Dot(mPoints2d[gi[i]], x[i], y[i]);
        }
    }

    return 70.0f * (n[0] + n[1] + n[2]);
}

double SimplexNoiseOctave::Noise(double xin, double yin, double zin)
{
    double n[4]{ 0.0f };
    double s = (xin + yin + zin) * mF3;

    int I[3]{ 0 };
    int J[3]{ 0 };
    int K[3]{ 0 };

    I[0] = FastFloor(xin + s);
    J[0] = FastFloor(yin + s);
    K[0] = FastFloor(zin + s);

    double T = (I[0] + J[0] + K[0]) * mG3;
    double X0 = I[0] - T;
    double Y0 = J[0] - T;
    double Z0 = K[0] - T;

    double x[4]{ 0.0f };
    double y[4]{ 0.0f };
    double z[4]{ 0.0f };

    x[0] = xin - X0;
    y[0] = yin - Y0;
    z[0] = zin - Z0;

    if (x[0] >= y[0])
    {
        if (y[0] >= z[0])
        {
            I[1] = 1;
            J[1] = 0;
            K[1] = 0;
            I[2] = 1; 
            J[2] = 1;
            K[2] = 0;
        }
        else if (x[0] >= z[0])
        {
            I[1] = 1;
            J[1] = 0;
            K[1] = 0;
            I[2] = 1;
            J[2] = 0;
            K[2] = 1;
        }
        else
        {
            I[1] = 0;
            J[1] = 0;
            K[1] = 1;
            I[2] = 1;
            J[2] = 0;
            K[2] = 1;
        }
    }
    else
    {
        if (y[0] < z[0])
        {
            I[1] = 0;
            J[1] = 0;
            K[1] = 1;
            I[2] = 0;
            J[2] = 1;
            K[2] = 1;
        }
        else if (x[0] < z[0])
        {
            I[1] = 0;
            J[1] = 1;
            K[1] = 0;
            I[2] = 0;
            J[2] = 1;
            K[2] = 1;
        }
        else
        {
            I[1] = 0;
            J[1] = 1;
            K[1] = 0;
            I[2] = 1;
            J[2] = 1;
            K[2] = 0;
        }
    }

    for (int i = 1; i < 4; i++)
    {
        x[i] = x[0] - (i < 3 ? I[i] : 1.0f) + (i * mG3);
        y[i] = y[0] - (i < 3 ? J[i] : 1.0f) + (i * mG3);
        z[i] = z[0] - (i < 3 ? K[i] : 1.0f) + (i * mG3);
    }

    int ii = I[0] & 255;
    int jj = J[0] & 255;
    int kk = K[0] & 255;

    int gi[4]{ 0 };
    gi[0] = mPerm[ii + mPerm[jj + mPerm[kk]]] % 12;
    gi[1] = mPerm[ii + I[1] + mPerm[jj + J[1] + mPerm[kk + K[1]]]] % 12;
    gi[2] = mPerm[ii + I[2] + mPerm[jj + J[2] + mPerm[kk + K[2]]]] % 12;
    gi[3] = mPerm[ii + 1 + mPerm[jj + 1 + mPerm[kk + 1]]] % 12;

    double t[4]{ 0.0f };
    for (int i = 0; i < 4; i++)
    {
        t[i] = 0.5f - (x[i] * x[i]) - (y[i] * y[i]) - (z[i] * z[i]);
        if (t[i] < 0)
        {
            n[i] = 0.0f;
        }
        else
        {
            t[i] *= t[i];
            n[i] = t[i] * t[i] * Dot(mPoints3d[gi[i]], x[i], y[i], z[i]);
        }
    }

    return 32.0f * (n[0] + n[1] + n[2] + n[3]);
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

void SimplexNoiseOctave::InitializePoints2D()
{
    mPoints2d = new sf::Vector2i[4];
    mPoints2d[0] = sf::Vector2i(1, 1);
    mPoints2d[1] = sf::Vector2i(-1, 1);
    mPoints2d[2] = sf::Vector2i(1, -1);
    mPoints2d[3] = sf::Vector2i(-1, -1);
}

void SimplexNoiseOctave::InitializePoints3D()
{
    mPoints3d = new sf::Vector3i[12];
    mPoints3d[0] = sf::Vector3i(1, 1, 0);
    mPoints3d[1] = sf::Vector3i(-1, 1, 0);
    mPoints3d[2] = sf::Vector3i(1, -1, 0);
    mPoints3d[3] = sf::Vector3i(-1, -1, 0);
    mPoints3d[4] = sf::Vector3i(1, 0, 1);
    mPoints3d[5] = sf::Vector3i(-1, 0, 1);
    mPoints3d[6] = sf::Vector3i(1, 0, -1);
    mPoints3d[7] = sf::Vector3i(-1, 0, -1);
    mPoints3d[8] = sf::Vector3i(0, 1, 1);
    mPoints3d[9] = sf::Vector3i(0, -1, 1);
    mPoints3d[10] = sf::Vector3i(0, 1, -1);
    mPoints3d[11] = sf::Vector3i(0, -1, -1);
}

void SimplexNoiseOctave::InitializePointSupply()
{
    mP = new short[sPointFieldSize];
    mPerm = new short[sPointFieldSize * 2];
    mPermMod12 = new short[sPointFieldSize * 2];

    for (int i = 0; i < sPointFieldSize; i++)
    {
        mP[i] = sPointSupply[i];
    }

    ShufflePointValues();

    for (int i = 0; i < sPointFieldSize * 2; i++) // length of perm
    {
        mPerm[i] = mP[i & sPointFieldSize - 1];
        mPermMod12[i] = static_cast<short>(mPerm[i] % 4);
    }
}

void SimplexNoiseOctave::ShufflePointValues()
{
    std::uniform_int_distribution<int> range(0, sPointFieldSize - 1);
    for (int i = 0; i < mNumberOfSwaps; i++)
    {
        int from = range(mRandom);
        int to = range(mRandom);

        short temp = mP[from];
        mP[from] = mP[to];
        mP[to] = temp;
    }
}