#include "stdafx.h"


using namespace std;

int main()
{
    Simulation simulation(sf::VideoMode(1600, 900), "FlowField");
    simulation.Start();
    //SimplexNoise noise(1, 1.0f);
    //double min = 100.0f;
    //double max = -100.0f;
    //
    //for (int z = 0; z < 100; z++)
    //{
    //    for (int y = 0; y < 900; y++)
    //    {
    //        for (int x = 0; x < 1600; x++)
    //        {
    //            double noiseValue = abs(noise.GetNoise(x, y, z));
    //            if (noiseValue < min)
    //            {
    //                min = noiseValue;
    //            }
    //            if (noiseValue > max)
    //            {
    //                max = noiseValue;
    //            }
    //        }
    //    }
    //}
    //
    //cout << "min: " << min << endl;
    //cout << "max: " << max << endl;
    //
    //system("pause");

    return 0;
}

