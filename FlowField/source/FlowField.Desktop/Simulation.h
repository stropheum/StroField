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

    void Start();

    virtual void Update(sf::RenderWindow& window, const double& deltaTime) override;

    virtual void Render(sf::RenderWindow& window) override;

private:

    sf::RenderWindow mWindow;
    class FlowField* mFlowField;
    class NoiseCube* mNoiseCube;

};

