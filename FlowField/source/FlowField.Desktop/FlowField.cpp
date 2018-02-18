#include "stdafx.h"
#include "FlowField.h"


FlowField::FlowField(const int& width, const int& height, const int& gridResolution) :
    mGridResolution(gridResolution),
    mGridWidth(static_cast<int>(width / gridResolution)),
    mGridHeight(static_cast<int>(height / gridResolution)),
    mGridOffset(mGridResolution / 2.0f),
    mSimplexNoise(nullptr),
    mSpriteImage(),
    mVectorComponents(nullptr),
    mForceVectors(nullptr),
    mNoiseClock(),
    mParticleCluster(nullptr)
{
    mSpriteImage.create(mGridOffset, 1, sf::Color::White);
    mSpriteTexture.loadFromImage(mSpriteImage);

    mSimplexNoise = new SimplexNoise(10, 1.0f);

    mVectorComponents = new sf::Vector2f*[mGridWidth];
    mForceVectors = new sf::Vertex[mGridWidth * mGridHeight * 2];
    for (int i = 0; i < mGridWidth; i++)
    {
        mVectorComponents[i] = new sf::Vector2f[mGridHeight];
        for (int j = 0; j < mGridHeight; j++)
        {
            mForceVectors[(i + (j * mGridWidth)) * 2].position = 
                sf::Vector2f(i * mGridResolution + mGridOffset, j * mGridResolution + mGridOffset);
            mForceVectors[(i + (j * mGridWidth)) * 2 + 1].position = 
                sf::Vector2f(i * mGridResolution + mGridOffset, j * mGridResolution + mGridOffset);

            mForceVectors[(i + (j * mGridWidth)) * 2].color = sf::Color(0, 170, 170, 255);
            mForceVectors[(i + (j * mGridWidth)) * 2 + 1].color = sf::Color(0, 170, 170, 255);
        }
    }

    mParticleCluster = new ParticleCluster(this, 1000);

    mNoiseClock.restart();
}

FlowField::~FlowField()
{
    delete mParticleCluster;

    for (int i = 0; i < mGridWidth; i++)
    {
        delete mVectorComponents[i];
    }
    delete[] mVectorComponents;
    delete[] mForceVectors;

    delete mSimplexNoise;
}

void FlowField::Update(sf::RenderWindow& window, const double& deltaTime)
{
    double elapsedTime = mNoiseClock.getElapsedTime().asMilliseconds() / 100.0f * 1;
    
    for (int y = 0; y < mGridHeight; y++)
    {
        for (int x = 0; x < mGridWidth; x++)
        {
            mVectorComponents[x][y].x = abs(mSimplexNoise->GetNoise(x / 5000.0f, y / 5000.0f, elapsedTime / 5000.0f)) * 360.0f;
            mVectorComponents[x][y].y = abs(mSimplexNoise->GetNoise(x / 10.0f + 40000.0f, y / 10.0f + 40000.0f, elapsedTime / 10.0f + 40000.0f));

            // Only need to update the odd vertices, because the roots will never move. Decent optimization
            int indexOffset = (x + (y * mGridWidth)) * 2;
                
            double vectorLength = mGridResolution * mVectorComponents[x][y].y;
            if (vectorLength > mGridResolution)
            {   // clamp the vector length to the radius of a node
                vectorLength = mGridResolution;
            }

            double newX = vectorLength * cos(mVectorComponents[x][y].x);
            double newY = vectorLength * sin(mVectorComponents[x][y].x);

            mForceVectors[indexOffset + 1].position =
                sf::Vector2f(mForceVectors[indexOffset].position.x + newX, mForceVectors[indexOffset].position.y + newY);
        }
    }

    mParticleCluster->Update(window, deltaTime);
    
}

void FlowField::Render(sf::RenderWindow& window)
{
    //for (int x = 0; x < mGridWidth; x++)
    //{
    //    for (int y = 0; y < mGridHeight; y++)
    //    {
    //        window.draw(mForceVectors, mGridWidth * mGridHeight * 2, sf::Lines);
    //    }
    //}

    mParticleCluster->Render(window);
}

sf::Vector2f FlowField::GetForceVectorAt(const sf::Vector2f& position) const
{
    int x = static_cast<int>(position.x / mGridResolution);
    int y = static_cast<int>(position.y / mGridResolution);
    int indexOffset = x * y * 2;

    return sf::Vector2f(
        mForceVectors[indexOffset + 1].position.x - mForceVectors[indexOffset].position.x, 
        mForceVectors[indexOffset + 1].position.y - mForceVectors[indexOffset].position.y);
}

sf::Vector2f FlowField::GetDimensions() const
{
    return sf::Vector2f(mGridWidth * mGridResolution, mGridHeight * mGridResolution);
}