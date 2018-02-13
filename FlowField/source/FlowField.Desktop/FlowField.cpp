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

    mSimplexNoise = new SimplexNoise(6, 0.1f);

    mVectorField = new sf::Vector2f*[mGridWidth];
    mRenderedVectors = new sf::Vertex[mGridWidth * mGridHeight * 2];
    for (int i = 0; i < mGridWidth; i++)
    {
        mVectorField[i] = new sf::Vector2f[mGridHeight];
        //mRenderedVectors[i] = new sf::Sprite[mGridHeight];
        for (int j = 0; j < mGridHeight; j++)
        {
            //mRenderedVectors[i][j] = sf::Sprite();
            //mRenderedVectors[i][j].setTexture(mSpriteTexture);
            //mRenderedVectors[i][j].setFillColor(sf::Color::White);
            //mRenderedVectors[i][j].setOutlineColor(sf::Color::White);
            mRenderedVectors[(i + (j * mGridWidth)) * 2].position = 
                sf::Vector2f(i * mGridResolution + mGridOffset, j * mGridResolution + mGridOffset);
            mRenderedVectors[(i + (j * mGridWidth)) * 2 + 1].position = 
                sf::Vector2f(i * mGridResolution + mGridOffset, j * mGridResolution + mGridOffset);
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
    //double elapsedTime = mNoiseClock.getElapsedTime().asSeconds();
    if (mDeltaClock.getElapsedTime().asSeconds() >= (1.0f / 60.0f))
    {
        for (int y = 0; y < mGridHeight; y++)
        {
            for (int x = 0; x < mGridWidth; x++)
            {
                mVectorField[x][y].x = ((1 + mSimplexNoise->GetNoise(
                    x / 50, y / 50, static_cast<int>(mNoiseClock.getElapsedTime().asMilliseconds()))) / 2.0f) * 360.0f;
                mVectorField[x][y].y = (1 + mSimplexNoise->GetNoise(x, y, static_cast<int>(mNoiseClock.getElapsedTime().asSeconds()))) / 2.0f;

                //double rotatedX = ()

                // Only need to update the odd vertices, because the roots will never move. Decent optimization
                int indexOffset = (x + (y * mGridWidth)) * 2;
                mRenderedVectors[indexOffset + 1].position =
                    sf::Vector2f(x * mGridResolution + mGridOffset + 10 * mVectorField[x][y].y * 2, y * mGridResolution + mGridOffset);
                std::cout << "index (" << indexOffset << ", " << indexOffset + 1 << "): ";
                std::cout << mRenderedVectors[indexOffset].position.x << ", " << mRenderedVectors[indexOffset].position.y << ", " 
                    << mRenderedVectors[indexOffset + 1].position.x << ", " << mRenderedVectors[indexOffset + 1].position.y << std::endl;
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