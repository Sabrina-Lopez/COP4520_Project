#include "particle.hpp"

Particle::Particle(float _mass, sf::Vector2f initPos)
{
    mass = _mass;
    position = initPos;
}

void Particle::applyForce(sf::Vector2f new_force)
{
    force += new_force;
}

void Particle::resetForce()
{
    force = sf::Vector2f(0, 0);
}

// Performs verlet integration of the change in time (dt)
// Updates the position, velocity, acceleration, and force
void Particle::integrate(float dt)
{
    sf::Vector2f half_vel = velocity + 0.5f * acceleration * dt;
    position += half_vel * dt;

    acceleration = force / mass;
    velocity = half_vel + 0.5f * acceleration * dt;

    resetForce();
}

void Particle::draw(sf::RenderWindow &window)
{
    s.setRadius(std::max(mass / 4.0, 1.0));
    s.setPosition(position);
    window.draw(s);
}

void Particle::render(sf::RenderWindow &wind)
{
    wind.draw(s);
}

void Particle::set_color(sf::Color col)
{
    s.setFillColor(col);
}