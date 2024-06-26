#pragma once

#include <SFML/Graphics.hpp>

class Particle {
    private:
        float mass;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
        sf::Vector2f force;
        sf::CircleShape s;

    public:
        Particle(float _mass, sf::Vector2f pos);
        Particle(float _mass, sf::Vector2f pos, sf::Vector2f vel);

        float getMass() const { return mass; }
        sf::Vector2f getPosition() const { return position; }
        sf::Vector2f getVelocity() const { return velocity; }
        sf::Vector2f getAcceleration() const { return acceleration; }
        sf::Vector2f getForce() const { return force; }

        void applyForce(sf::Vector2f new_force);
        void resetForce();
        
        void integrate(float dt, int steps = 1);

        void setColor(sf::Color col);
        void setRadius(float radius);
        void setShape(sf::CircleShape shape);
        void draw(sf::RenderWindow &window);
};