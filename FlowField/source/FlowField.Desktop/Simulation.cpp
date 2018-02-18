#include "stdafx.h"
#include "Simulation.h"


Simulation::Simulation(sf::VideoMode videoMode, const std::string& title) :
    mWindow(videoMode, title), mFlowField(nullptr), mNoiseCube(nullptr), mDeltaClock()
{
    mFlowField = new FlowField(videoMode.width, videoMode.height, 30);
    //mNoiseCube = new NoiseCube(videoMode.width, videoMode.height);
    mDeltaClock.restart();
}

Simulation::~Simulation()
{
    delete mFlowField;
}

void Simulation::Start()
{
    while (mWindow.isOpen())
    {
        sf::Event event;
        while (mWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                mWindow.close();
        }

        Update(mWindow, mDeltaClock.restart().asSeconds());
        Render(mWindow);
    }
}

void Simulation::Update(sf::RenderWindow& window, const double& deltaTime)
{
    mFlowField->Update(window, deltaTime);
    //mNoiseCube->Update(window, deltaTime);
}

void Simulation::Render(sf::RenderWindow& window)
{
    //mWindow.clear();

    mFlowField->Render(window);
    //mNoiseCube->Render(window);
    
    mWindow.display();
}