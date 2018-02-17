#include "stdafx.h"
#include "FlowField.h"
#include <Math.h>


FlowField::FlowField(const int& width, const int& height, const int& gridResolution) :
    mGridResolution(gridResolution),
    mGridWidth(static_cast<int>(width / gridResolution)),
    mGridHeight(static_cast<int>(height / gridResolution)),
    mGridOffset(mGridResolution / 2.0f),
    mSimplexNoise(nullptr),
    mSpriteImage(),
    mVectorField(nullptr),
    mRenderedVectors(nullptr),
    mDeltaClock(),
    mNoiseClock()
{
    mSpriteImage.create(mGridOffset, 1, sf::Color::White);
    mSpriteTexture.loadFromImage(mSpriteImage);

    mSimplexNoise = new SimplexNoise(10, 1.0f);

    mVectorField = new sf::Vector2f*[mGridWidth];
    mRenderedVectors = new sf::Vertex[mGridWidth * mGridHeight * 2];
    for (int i = 0; i < mGridWidth; i++)
    {
        mVectorField[i] = new sf::Vector2f[mGridHeight];
        //mRenderedVectors[i] = new sf::Sprite[mGridHeight];
        for (int j = 0; j < mGridHeight; j++)
        {
            mRenderedVectors[(i + (j * mGridWidth)) * 2].position = 
                sf::Vector2f(i * mGridResolution + mGridOffset, j * mGridResolution + mGridOffset);
            mRenderedVectors[(i + (j * mGridWidth)) * 2 + 1].position = 
                sf::Vector2f(i * mGridResolution + mGridOffset, j * mGridResolution + mGridOffset);

            mRenderedVectors[(i + (j * mGridWidth)) * 2].color = sf::Color(170, 170, 170, 255);
            mRenderedVectors[(i + (j * mGridWidth)) * 2 + 1].color = sf::Color(0, 170, 170, 255);
        }
    }

    mDeltaClock.restart();
    mNoiseClock.restart();
}

FlowField::~FlowField()
{
    for (int i = 0; i < mGridWidth; i++)
    {
        delete mVectorField[i];
    }
    delete[] mVectorField;
    delete[] mRenderedVectors;

    delete mSimplexNoise;
}

void FlowField::Update(sf::RenderWindow& window, const double& deltaTime)
{
    double elapsedTime = mNoiseClock.getElapsedTime().asSeconds() / 1000.0f * 5;
    if (mDeltaClock.getElapsedTime().asSeconds() >= (1.0f / 120.0f))
    {
        for (int y = 0; y < mGridHeight; y++)
        {
            for (int x = 0; x < mGridWidth; x++)
            {
                mVectorField[x][y].x = abs(mSimplexNoise->GetNoise(x / 500.0f, y / 500.0f, elapsedTime)) * 360.0f;
                mVectorField[x][y].y = abs(mSimplexNoise->GetNoise(x / 10.0f + 40000.0f, y / 10.0f + 40000.0f, elapsedTime));

                // Only need to update the odd vertices, because the roots will never move. Decent optimization
                int indexOffset = (x + (y * mGridWidth)) * 2;
                
                double xDif = x * mGridResolution + mGridOffset + mGridResolution * mVectorField[x][y].y - mRenderedVectors[indexOffset].position.x;
                if (xDif > mGridResolution)
                {   // clamp the vector length
                    xDif = mGridResolution;
                }
                double newX = xDif * cos(mVectorField[x][y].x);
                double newY = xDif * sin(mVectorField[x][y].x);
                
                mRenderedVectors[indexOffset + 1].position =
                    sf::Vector2f(mRenderedVectors[indexOffset].position.x + newX, mRenderedVectors[indexOffset].position.y + newY);
            }
        }

        mDeltaClock.restart();
    }
}

void FlowField::Render(sf::RenderWindow& window)
{
    for (int x = 0; x < mGridWidth; x++)
    {
        for (int y = 0; y < mGridHeight; y++)
        {
            window.draw(mRenderedVectors, mGridWidth * mGridHeight * 2, sf::Lines);
        }
    }
}