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

Simulation::Simulation() : mainView(sf::Vector2f(0, 0), sf::Vector2f(720, 720)),
                           GUIView(sf::Vector2f(0, 0), sf::Vector2f(240, 720)),
                           clearColor(sf::Color(236, 240, 241)),
                           GUIBackground(sf::Vector2f(240, 720)),
                           frameRate(sf::seconds(1.0/30.0f)),
                           isRunning(false)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 6;
    rWindow.create(sf::VideoMode(960, 720), "TacoTruck Physics Engine Demo",
                   sf::Style::Titlebar | sf::Style::Close, settings);
    mainView.setViewport(sf::FloatRect(0.25f, 0, 0.75f, 1.0f));
    GUIView.setViewport(sf::FloatRect(0, 0, 0.25f, 1.0f));
    GUIBackground.setFillColor(sf::Color(44, 62, 80));
    GUIBackground.setPosition(-120, -360);

    sf::RectangleShape fakeButton(sf::Vector2f(220, 24));
    fakeButton.setPosition(-110, -350);
    fakeButton.setFillColor(sf::Color(149, 165, 166));

    rWindow.clear(clearColor);
    rWindow.setView(GUIView);
    rWindow.draw(GUIBackground);
    rWindow.draw(fakeButton);
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
    Time tBegin;

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
                    isRunning = false;
                    initSimulation();
                    tBegin = realTimeClock.getElapsedTime();
                    isRunning = true;
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
    rwindow.clear(clearColor);

    rwindow.setView(GUIView);
    rwindow.draw(GUIBackground);

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
    particles[0].setPosition(0, 0);
    particles[0].setVelocity(-10, 0);
    particles[0].setAcceleration(-2, 0);
    particles[0].setMass(1.0);
    particles[0].setDamping(0.99f);

    circles[0].setPosition(0, 0);
    circles[0].setFillColor(sf::Color::Black);
    circles[0].setRadius(6);
    circles[0].setPointCount(30);
}

sf::Vector2f Simulation::coordWorldToView(const tacoTruck::Vector2D coord) const {
    return sf::Vector2f(coord.x, -coord.y);
}

tacoTruck::Vector2D Simulation::coordViewToWorld(const sf::View & view, const sf::Vector2f & coord) {
    return tacoTruck::Vector2D();
}
