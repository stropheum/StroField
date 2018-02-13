#pragma once
#include "SimComponent.h"
#include <random>

class NoiseCube : public SimComponent
{
public:

#pragma region Construction/Copy/Assignment
    NoiseCube(const int& width, const int& height);
    virtual ~NoiseCube();
    NoiseCube(const NoiseCube& rhs) = delete;
    NoiseCube& operator=(const NoiseCube& rhs) = delete;
    NoiseCube(NoiseCube&& rhs) = delete;
    NoiseCube& operator=(NoiseCube&& rhs) = delete;
#pragma endregion

    virtual void Update(sf::RenderWindow& window, const double& deltaTime) override;

    void Render(sf::RenderWindow& window);

    void ClearBackground();

    void GenerateNoise();

private:

    void GenerateNoiseAtDepth(int depth);
    void ColorizeGreyscale(int depth);
    void ColorizeTerrain(int depth);
    void ColorizeTopography(int depth);

    int mWidth;
    int mHeight;
    int mDepth;
    int mDepthIndex;
    bool mPingPongAscending;

    sf::Uint8** mPixelArray;
    sf::Texture mPixelTexture;
    sf::Sprite mPixelSprite;
    sf::Clock mDeltaClock;

    std::default_random_engine mGenerator;

    class SimplexNoise* mSimplexNoise;

};

