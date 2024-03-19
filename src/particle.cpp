#include "particle.hpp"

Particle::Particle(float _mass, sf::Vector2f pos)
{
    mass = _mass;
    position = pos;
}

Particle::Particle(float _mass, sf::Vector2f pos, sf::Vector2f vel)
{
    mass = _mass;
    position = pos;
    velocity = vel;
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
    float radius = std::clamp(mass / 500, 1.0f, 10.0f);
    s.setRadius(radius);
    s.setPosition(position - sf::Vector2f(radius, radius));
    window.draw(s);
}

void Particle::setRadius(float radius)
{
    s.setRadius(radius);
}

void Particle::setColor(sf::Color col)
{
    s.setFillColor(col);
}

void Particle::setShape(sf::CircleShape shape)
{
    s = shape;
}