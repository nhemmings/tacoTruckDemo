#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include "Vector2D.hpp"
#include "pfgen.hpp"

/**
 *  The primary simulation logic and entry point.
 *  Initializes the simulation's subsystems, manages the viewing window, and processes user input.
 */
class Simulation
{
private:
    sf::RenderWindow rWindow;
    sf::View mainView;
    const sf::Time frameRate;
    bool isRunning;

    void initSimulation();
    void runClientFrame(const sf::Time & dt, sf::RenderWindow & rwindow);

    void dtSMAinit(const sf::Time & dtIdeal);
    sf::Time dtSMAupdate(const sf::Time & dtCurrent);

    sf::CircleShape* circles;
    tacoTruck::Particle* particles;
    tacoTruck::ParticleForceRegistry pfReg;

    sf::Vector2f coordWorldToView(const tacoTruck::Vector2D coord) const;
    tacoTruck::Vector2D coordViewToWorld(const sf::View & view, const sf::Vector2f & coord);
public:
    Simulation();
    virtual ~Simulation();

    int run();
};
#endif // GAME_HPP
