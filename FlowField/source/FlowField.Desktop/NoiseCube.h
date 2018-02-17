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

    /**
     * Updates the simulation state
     * @Param window: The window being rendered to
     * @Param deltaTime: The time elased since last update call
     */
    virtual void Update(sf::RenderWindow& window, const double& deltaTime) override;

    /**
     * Renders the current simulation state to the screen
     * @Param window: The window being rendered to
     */
    void Render(sf::RenderWindow& window);

    /**
     * Clears the background and renders a gray color instead
     */
    void ClearBackground();

    /**
     * Pre-generates a 3D simplex noise cube to be animated later
     */
    void GenerateNoise();

private:

    /**
     * Helper method to generate a slice of simplex noise
     * @Param depth: The depth-slice of simplex noise being generated
     */
    void GenerateNoiseAtDepth(int depth);

    /**
     * Renders the simplex noise slice as a greyscale point cloud
     * @Param depth: The depth-slice of simplex noise being colorized
     */
    void ColorizeGreyscale(int depth);
    
    /**
     * Renders the simplex noise slice as a height-based terrain map
     * @Param depth: The depth-slice of simplex noise being colorized
     */
    void ColorizeTerrain(int depth);

    /**
     * Renders the simplex noise slice as thresholded topographical map
     * @Param depth: The depth-slice of simplex noise being colorized
     */
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

