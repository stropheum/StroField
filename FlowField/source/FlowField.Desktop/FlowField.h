#pragma once
#include "SimComponent.h"

class FlowField : public SimComponent
{

public:

#pragma region Construction/Copy/Assignment
    FlowField(const int& width, const int& height, const int& gridResolution);
    virtual ~FlowField();
    FlowField(const FlowField& rhs) = delete;
    FlowField& operator=(const FlowField& rhs) = delete;
    FlowField(FlowField&& rhs) = delete;
    FlowField& operator=(FlowField&& rhs) = delete;
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
    virtual void Render(sf::RenderWindow& window) override;

private:

    int mGridResolution;
    int mGridWidth;
    int mGridHeight;
    int mGridOffset;
    class SimplexNoise* mSimplexNoise;
    sf::Vector2f** mVectorField;
    sf::Image mSpriteImage;
    sf::Texture mSpriteTexture;
    sf::Vertex* mRenderedVectors;
    sf::Clock mDeltaClock;
    sf::Clock mNoiseClock;

};

