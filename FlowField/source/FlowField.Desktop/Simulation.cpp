#include "stdafx.h"
#include "Simulation.h"


Simulation::Simulation(sf::VideoMode videoMode, const std::string& title) :
    mWindow(videoMode, title), 
	mFlowField(nullptr), 
	mNoiseCube(nullptr), 
	mDeltaClock(), 
	mFullScreenEnabled(false), 
	mTitle(title), 
	mVideoMode(videoMode)
{
    mFlowField = new FlowField(videoMode.width, videoMode.height, 50);
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
            {
                mWindow.close();
            }
            
            if (event.type == sf::Event::KeyPressed)
            {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					mWindow.close();
				}

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
					mFullScreenEnabled = !mFullScreenEnabled;
					mWindow.create(mVideoMode, mTitle, mFullScreenEnabled ? sf::Style::Fullscreen : sf::Style::Default);
                }
            }
        }

        Update(mWindow, mDeltaClock.restart().asSeconds());
        Render(mWindow);
    }
}

void Simulation::Update(sf::RenderWindow& window, const double& deltaTime)
{
    mFlowField->Update(window, deltaTime);
}

void Simulation::Render(sf::RenderWindow& window)
{
    mWindow.clear();

    mFlowField->Render(window);
    
    mWindow.display();
}