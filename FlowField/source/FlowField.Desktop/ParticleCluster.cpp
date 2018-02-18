#include "stdafx.h"
#include "ParticleCluster.h"


ParticleCluster::ParticleCluster(const FlowField* const context, int particleCount) :
    mContext(context)
{
    std::random_device device;
    mRandom = std::default_random_engine(device());
    std::uniform_int_distribution<int> xDis(0, mContext->GetDimensions().x);
    std::uniform_int_distribution<int> yDis(0, mContext->GetDimensions().y);
    std::uniform_int_distribution<int> velDis(-2, 2);

    for (int i = 0; i < particleCount; i++)
    {
        mParticles.push_back(sf::Vertex(sf::Vector2f(xDis(mRandom), yDis(mRandom)), sf::Color(0, 255, 255, 32)));
        mVelocities.push_back(sf::Vector2f(velDis(mRandom), velDis(mRandom)));
    }
}

ParticleCluster::~ParticleCluster()
{
    mParticles.clear();
    mVelocities.clear();
}

void ParticleCluster::Update(sf::RenderWindow& window, const double& deltaTime)
{
    ApplyForcesToParticles(deltaTime);
}

void ParticleCluster::Render(sf::RenderWindow& window)
{
    window.draw(&mParticles[0], mParticles.size(), sf::Points);
}

void ParticleCluster::ApplyForcesToParticles(const double& deltaTime)
{
    for (int i = 0; i < mParticles.size(); i++)
    {
        sf::Vector2f force = mContext->GetForceVectorAt(mParticles[i].position);
        mVelocities[i].x += force.x * deltaTime * 50.0f;
        mVelocities[i].y += force.y * deltaTime * 50.0f;

        // Clamp the velocities
        int maxVelocity = 5.0f;
        if (mVelocities[i].x > maxVelocity)
        {
            mVelocities[i].x = maxVelocity;
        }
        else if (mVelocities[i].x < -maxVelocity)
        {
            mVelocities[i].x = -maxVelocity;
        }

        if (mVelocities[i].y > maxVelocity)
        {
            mVelocities[i].y = maxVelocity;
        }
        else if (mVelocities[i].y < -maxVelocity)
        {
            mVelocities[i].y = -maxVelocity;
        }

        // Move the particle
        mParticles[i].position.x += mVelocities[i].x;
        mParticles[i].position.y += mVelocities[i].y;

        // Wrap around screen
        HandleScreenWrapping(mParticles[i].position);
    }
}

void ParticleCluster::HandleScreenWrapping(sf::Vector2f& position)
{
    sf::Vector2f dimensions = mContext->GetDimensions();
    if (position.x < 0)
    {
        position.x = dimensions.x - 1;
    }
    else if (position.x >= dimensions.x)
    {
        position.x = 0;
    }

    if (position.y < 0)
    {
        position.y = dimensions.y - 1;
    }
    else if (position.y >= dimensions.y)
    {
        position.y = 0;
    }
}