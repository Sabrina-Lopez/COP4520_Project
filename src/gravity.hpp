// Helper code for calculating gravitation forces between a pair of point masses (particles)
// Uses Newton's Law of Universal Gravitation

// All nbody approaches should use this code regardless of method

#include <SFML/Graphics.hpp>
#include "particle.hpp"

const float G = 10.0; // the gravitational constant
const float minDistance = 2.0; // the minimum distance to calculate gravitation forces

sf::Vector2f getGravityForce(sf::Vector2f pos1, float mass1, sf::Vector2f pos2, float mass2) {
    sf::Vector2f r = pos2 - pos1;

    float distance = sqrt(r.x * r.x + r.y * r.y);

    if (distance < minDistance)
        return sf::Vector2f(0, 0);

    sf::Vector2f norm_r = r / distance;

    sf::Vector2f force = (-G * ((mass1 * mass2) / (distance * distance))) * norm_r;
    
    return force;
}

sf::Vector2f getGravityForce(Particle &p1, Particle &p2) {
    return getGravityForce(p1.getPosition(), p1.getMass(), p2.getPosition(), p2.getMass());
}