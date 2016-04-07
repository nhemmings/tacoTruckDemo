#include <cstdlib>
#include "Simulation.hpp"
#include <ctime>

#include <iostream>

#define numParticles 2

/** Program entry point. */
int main(int argc, char** argv)
{
    Simulation sim;
    return sim.run();
}

Simulation::Simulation() : mainView(sf::Vector2f(0, 0), sf::Vector2f(800, 800)),
                           frameRate(sf::seconds(1.0/30.0f)),
                           isRunning(false)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 6;
    rWindow.create(sf::VideoMode(800, 800), "TacoTruck Physics Engine Demo", sf::Style::Default, settings);
    rWindow.setView(mainView);
    rWindow.clear();
    rWindow.display();

    circles = new sf::CircleShape[40];
    particles = new tacoTruck::Particle[40];
}

Simulation::~Simulation()
{
}

int Simulation::run() {
    using sf::Time;
    using sf::Clock;

    initSimulation();

    // Initialize clocks
    Time dtReal = frameRate;           // The real frame delta time, initialized to ideal value
    Time dtFrame = sf::Time::Zero;
    Clock realTimeClock;
    Time tBegin = realTimeClock.getElapsedTime();

    // MAIN SIMULATION LOOP
    while (rWindow.isOpen())
    {
        // System message pump
        sf::Event event;
        while (rWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                rWindow.close();
            else if (event.type == sf::Event::Resized) {
                mainView.setCenter(rWindow.getView().getCenter());
                mainView.setSize(event.size.width, event.size.height);
                rWindow.setView(mainView);
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    rWindow.close();
                else if (event.key.code == sf::Keyboard::R) {
                    isRunning = !isRunning;
                }
            }
        }

        if (isRunning) {
            // Frame update
            dtFrame += dtReal;
            if (dtFrame >= frameRate) {
                runClientFrame(frameRate, rWindow);
                dtFrame -= frameRate;
            }

            // Timer update
            Time tEnd = realTimeClock.getElapsedTime();
            dtReal = tEnd - tBegin;
            tBegin = tEnd;
        }
    }
    return 0;
}

void Simulation::runClientFrame(const sf::Time & dt, sf::RenderWindow & rwindow) {
    rwindow.clear();
    rwindow.setView(mainView);

    pfReg.updateForces(dt.asSeconds());
    for (int i = 0; i < numParticles; i++) {
        particles[i].integrate(dt.asSeconds());
        circles[i].setPosition(coordWorldToView((particles[i].getPosition())));
        rwindow.draw(circles[i]);
    }
    rwindow.display();
}

void Simulation::initSimulation() {

}

sf::Vector2f Simulation::coordWorldToView(const tacoTruck::Vector2D coord) const {
    return sf::Vector2f(coord.x, -coord.y);
}

tacoTruck::Vector2D Simulation::coordViewToWorld(const sf::View & view, const sf::Vector2f & coord) {
    return tacoTruck::Vector2D();
}
