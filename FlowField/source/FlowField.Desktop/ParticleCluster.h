#pragma once
#include "SimComponent.h"

class ParticleCluster : public SimComponent
{
public:

#pragma region Construction/Copy/Assignment
    ParticleCluster(const class FlowField* const context, int particleCount = 100);
    virtual ~ParticleCluster();
    ParticleCluster(const ParticleCluster& rhs) = delete;
    ParticleCluster& operator=(const ParticleCluster& rhs) = delete;
    ParticleCluster(ParticleCluster&& rhs) = delete;
    ParticleCluster& operator=(ParticleCluster&& rhs) = delete;
#pragma endregion

    virtual void Update(sf::RenderWindow& window, const double& deltaTime) override;

    virtual void Render(sf::RenderWindow& window) override;

    void ApplyForcesToParticles(const double& deltaTime);

private:

    void HandleScreenWrapping(sf::Vector2f& position);

    const class FlowField* const mContext;
    std::vector<sf::Vertex> mParticles;
    std::vector<sf::Vector2f> mVelocities;
    std::default_random_engine mRandom;

};

