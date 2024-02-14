#include "particle.h"

Particle::Particle(float _mass, sf::Vector2f initPos) {
    mass = _mass;
    position = initPos;
}

void Particle::applyForce(sf::Vector2f new_force) {
    force += new_force;
}

void Particle::resetForce() {
    force -= force;
}

// Performs verlet integration of the change in time (dt)
// Updates the position, velocity, acceleration, and force
void Particle::integrate(float dt) {
    sf::Vector2f half_vel = velocity + 0.5f * acceleration * dt;
    position += half_vel * dt;

    acceleration = force / mass;
    velocity = half_vel + 0.5f * acceleration * dt;

    resetForce();
}

void Particle::draw(sf::RenderWindow &window) {
    sf::CircleShape circle(5);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(position);
    window.draw(circle);
}

void Particle::render(sf::RenderWindow& wind)
{
    wind.draw(s);
}

void Particle::set_color(sf::Color col)
{
    s.setFillColor(col);
}