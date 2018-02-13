#pragma once
class SimComponent
{
public:

    SimComponent() {}

    virtual ~SimComponent() {}

    virtual void Update(sf::RenderWindow& window, const double& deltaTime) = 0;

    virtual void Render(sf::RenderWindow& window) = 0;
};