#pragma once
#include "SimComponent.h"

class Simulation : public SimComponent
{
public:

#pragma region Construction/Copy/Assignment
    Simulation(sf::VideoMode videoMode, const std::string& title);
    virtual ~Simulation();
    Simulation(const Simulation& rhs) = delete;
    Simulation& operator=(const Simulation& rhs) = delete;
    Simulation(Simulation&& rhs) = delete;
    Simulation& operator=(Simulation&& rhs) = delete;
#pragma endregion

    /**
     * Entry point of the simulation
     */
    void Start();

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

    sf::RenderWindow mWindow;       // The window that the simulation will be rendered to
    class FlowField* mFlowField;    // The flowfield being simulated
    class NoiseCube* mNoiseCube;    // The noise cube being simulated
    sf::Clock mDeltaClock;          // The clock for handling universal time steps

};

