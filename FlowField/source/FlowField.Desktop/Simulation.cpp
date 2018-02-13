#include "stdafx.h"
#include "Simulation.h"


Simulation::Simulation(sf::VideoMode videoMode, const std::string& title) :
    mWindow(videoMode, title), mFlowField(nullptr), mNoiseCube(nullptr)
{
    //FlowField = new FlowField(videoMode.width, videoMode.height, 50);
    mNoiseCube = new NoiseCube(videoMode.width, videoMode.height);
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

        Update(mWindow, 0.0f);
        Render(mWindow);
    }
}

void Simulation::Update(sf::RenderWindow& window, const double& deltaTime)
{
    //FlowField->Update(window, deltaTime);
    mNoiseCube->Update(window, deltaTime);
}

void Simulation::Render(sf::RenderWindow& window)
{
    mWindow.clear();

    //mFlowField->Render(window);
    mNoiseCube->Render(window);
    
    mWindow.display();
}