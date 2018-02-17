#pragma once
class SimComponent
{
public:

#pragma region Construction/Copy/Assignment
    SimComponent() {}
    virtual ~SimComponent() {}
    SimComponent(const SimComponent& rhs) = delete;
    SimComponent& operator=(const SimComponent& rhs) = delete;
    SimComponent(SimComponent&& rhs) = delete;
    SimComponent& operator=(SimComponent&& rhs) = delete;
#pragma endregion

    /**
     * Updates the sim component state
     * @Param window: The window being rendered to
     * @Param deltaTime: The time elased since last update call
     */
    virtual void Update(sf::RenderWindow& window, const double& deltaTime) = 0;

    /**
     * Renders the current sim component state to the screen
     * @Param window: The window being rendered to
     */
    virtual void Render(sf::RenderWindow& window) = 0;
};