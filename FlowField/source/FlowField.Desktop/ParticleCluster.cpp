#include "stdafx.h"
#include "ParticleCluster.h"


ParticleCluster::ParticleCluster(const FlowField* const context, int particleCount) :
    mContext(context)
{
    std::random_device device;
    mRandom = std::default_random_engine(device());
    std::uniform_real_distribution<float> xDis(0.0f, mContext->GetDimensions().x);
    std::uniform_real_distribution<float> yDis(0.0f, mContext->GetDimensions().y);
	std::uniform_real_distribution<float> offsetDis(0.0f, 100.0f);

	sf::Vector2f origin(static_cast<float>(xDis(mRandom)), static_cast<float>(yDis(mRandom)));

    for (int i = 0; i < particleCount; i++)
    {
		sf::Vector2f offset(offsetDis(mRandom), offsetDis(mRandom));
		mParticles.push_back(
			sf::Vertex(
				origin + offset,
				//sf::Vector2f(static_cast<float>(xDis(mRandom)), static_cast<float>(yDis(mRandom))),
				sf::Color(0, 255, 255, 255)));
		mVelocities.push_back(sf::Vector2f(0.0f, 0.0f));
    }
}

ParticleCluster::~ParticleCluster()
{
    mParticles.clear();
    mVelocities.clear();
}

void ParticleCluster::Update(sf::RenderWindow& window, const double& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	mLastParticles = mParticles;
	ApplyForcesToParticles(deltaTime);
}

void ParticleCluster::Render(sf::RenderWindow& window)
{
    std::vector<sf::Vertex> mRenderedLines;
    for (int i = 0; i < static_cast<int>(mParticles.size()); i++)
    {   // Zipper up the particles for rendering
        mRenderedLines.push_back(mLastParticles[i]);
        mRenderedLines.push_back(mParticles[i]);
    } 
    window.draw(&mRenderedLines[0], mParticles.size() * 2, sf::Points);
}

void ParticleCluster::ApplyForcesToParticles(const double& deltaTime)
{
    for (int i = 0; i < static_cast<int>(mParticles.size()); i++)
    {
        sf::Vector2f force = mContext->GetForcePercentageAt(mParticles[i].position);
        mVelocities[i].x += static_cast<float>(force.x * deltaTime);
        mVelocities[i].y += static_cast<float>(force.y * deltaTime);

        // Clamp the velocities
		//TODO: This should be moved to an assert, since percentages will clamp this to a percentage of max velocity
        float maxVelocity = 0.25f;
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
        if (HandleScreenWrapping(mParticles[i].position))
        {
            mLastParticles[i] = mParticles[i];
        }
    }
}

bool ParticleCluster::HandleScreenWrapping(sf::Vector2f& position)
{
    bool result = false;
    sf::Vector2f dimensions = mContext->GetDimensions();
    if (position.x < 0)
    {
        position.x = dimensions.x - 1;
        result = true;
    }
    else if (position.x >= dimensions.x)
    {
        position.x = 0;
        result = true;
    }

    if (position.y < 0)
    {
        position.y = dimensions.y - 1;
        result = true;
    }
    else if (position.y >= dimensions.y)
    {
        position.y = 0;
        result = true;
    }

    return result;
}