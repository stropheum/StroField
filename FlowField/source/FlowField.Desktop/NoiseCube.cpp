#include "stdafx.h"
#include "NoiseCube.h"


using namespace std;

NoiseCube::NoiseCube(const int& width, const int& height) :
    mWidth(width), mHeight(height), mDepth(100), mDepthIndex(0), mPingPongAscending(true || mDepthIndex >= (mDepth - 1)), 
    mPixelArray(0), mPixelTexture(), mPixelSprite(), mDeltaClock(), mGenerator(), mSimplexNoise(nullptr)
{
    mSimplexNoise = new SimplexNoise(7, 0.5f);

    std::random_device device;
    mGenerator = std::default_random_engine(device());

    mPixelArray = new sf::Uint8*[mDepth];
    for (int i = 0; i < mDepth; i++)
    {
        mPixelArray[i] = new sf::Uint8[mHeight * mWidth * 4];
    }

    mPixelTexture.create(mWidth, mHeight);
    mPixelSprite.setTexture(mPixelTexture);

    GenerateNoise();
    mDeltaClock.restart();
}

NoiseCube::~NoiseCube()
{
    delete mSimplexNoise;
    delete[] mPixelArray;
}

void NoiseCube::Update(sf::RenderWindow& window, const double& deltaTime)
{
    if (mDeltaClock.getElapsedTime().asSeconds() > (1.0f / 30.0f))
    {
        mDepthIndex += mPingPongAscending ? 1 : -1;
        
        if (mPingPongAscending && mDepthIndex >= (mDepth - 1))
        {
            mPingPongAscending = false;
        }
        else if (!mPingPongAscending && mDepthIndex <= 0)
        {
            mPingPongAscending = true;
        }

        mDeltaClock.restart();
    }
}

void NoiseCube::Render(sf::RenderWindow& window)
{
    mPixelTexture.update(mPixelArray[mDepthIndex]);
    window.draw(mPixelSprite);
}

void NoiseCube::ClearBackground()
{
    for (int i = 0; i < mWidth * mHeight * 4; i += 4)
    {
        mPixelArray[0][i + 0] = 124;
        mPixelArray[0][i + 1] = 124;
        mPixelArray[0][i + 2] = 124;
        mPixelArray[0][i + 3] = 255;
    }
}

void NoiseCube::GenerateNoise()
{
    std::vector<std::thread> sliceThread;

    int batchSize = 10;

    for (int i = 0; i < mDepth; i += batchSize)
    {   
        for (int j = 0; j < batchSize; j++)
        {
            sliceThread.push_back(std::thread(&NoiseCube::GenerateNoiseAtDepth, this, i + j));
        }

        for (int j = 0; j < batchSize; j++)
        {   // Merge all threads before next iteration
            sliceThread[j].join();
        }

        // Clear the thread vector before next iteration
        sliceThread.clear();
    }
}

void NoiseCube::GenerateNoiseAtDepth(int depth)
{
    //ColorizeGreyscale(depth);
    //ColorizeTerrain(depth);
    ColorizeTopography(depth);
}

void NoiseCube::ColorizeGreyscale(int depth)
{
    for (int i = 0; i < mHeight; i++)
    {
        for (int j = 0; j < mWidth; j++)
        {
            int indexOffset = ((i * mWidth) + j) * 4;

            double noise = (1 + mSimplexNoise->GetNoise(i, j, depth)) / 2;

            mPixelArray[depth][indexOffset + 0] = noise * 255;
            mPixelArray[depth][indexOffset + 1] = noise * 255;
            mPixelArray[depth][indexOffset + 2] = noise * 255;
            mPixelArray[depth][indexOffset + 3] = 255;
        }
    }
}

void NoiseCube::ColorizeTerrain(int depth)
{
    for (int i = 0; i < mHeight; i++)
    {
        for (int j = 0; j < mWidth; j++)
        {
            int indexOffset = ((i * mWidth) + j) * 4;

            double noise = (1 + mSimplexNoise->GetNoise(i, j, depth)) / 2;

            if (noise < 0.45f)
            {   // Deep water
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 0;
                mPixelArray[depth][indexOffset + 2] = 124;
                mPixelArray[depth][indexOffset + 3] = 255;
            }
            else if (noise < 0.475f)
            {   // Shallow water
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 0;
                mPixelArray[depth][indexOffset + 2] = 190;
                mPixelArray[depth][indexOffset + 3] = 255;
            }
            else if (noise < 0.55f)
            {   // Grass
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 124;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255;
            }
            else
            {   // Mountain
                mPixelArray[depth][indexOffset + 0] = 120;
                mPixelArray[depth][indexOffset + 1] = 10;
                mPixelArray[depth][indexOffset + 2] = 10;
                mPixelArray[depth][indexOffset + 3] = 255;
            }
        }
    }
}

void NoiseCube::ColorizeTopography(int depth)
{
    for (int i = 0; i < mHeight; i++)
    {
        for (int j = 0; j < mWidth; j++)
        {
            int indexOffset = ((i * mWidth) + j) * 4;

            double noise = (1 + mSimplexNoise->GetNoise(i, j, depth)) / 2;

            if (noise > 0.00 && noise <= 0.01f)
            {   
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 255 * 0.15f;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255 * 0.15f;
            }
            else if (noise > 0.1 && noise <= 0.11f)
            {
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 255 * 0.3f;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255 * 0.3f;
            }
            else if (noise > 0.2 && noise <= 0.21f)
            {
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 255 * 0.45f;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255 * 0.45f;
            }
            else if (noise > 0.3 && noise <= 0.31f)
            {
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 255 * 0.6f;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255 * 0.6f;
            }
            else if (noise > 0.4 && noise <= 0.41f)
            {
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 255 * 0.75f;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255 * 0.75f;
            }
            else if (noise > 0.5 && noise <= 0.51f)
            {
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 255 * 0.9f;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255 * 0.9f;
            }
            else if (noise > 0.6 && noise <= 0.61f)
            {
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 255;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255;
            }
            else if (noise > 0.7 && noise <= 0.71f)
            {
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 255;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255;
            }
            else if (noise > 0.8 && noise <= 0.81f)
            {
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 255;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255;
            }
            else if (noise > 0.9 && noise <= 0.91f)
            {
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 255;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255;
            }
            else
            {
                mPixelArray[depth][indexOffset + 0] = 0;
                mPixelArray[depth][indexOffset + 1] = 0;
                mPixelArray[depth][indexOffset + 2] = 0;
                mPixelArray[depth][indexOffset + 3] = 255;
            }
        }
    }
}