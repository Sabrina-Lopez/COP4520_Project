#include "gravity.hpp"
#include <math.h>

sf::Vector2f getGravityForce(sf::Vector2f pos1, float mass1, sf::Vector2f pos2, float mass2) {
    sf::Vector2f r = pos2 - pos1;

    float distance = sqrt(r.x * r.x + r.y * r.y);

    if (distance < std::min(mass1, 15.0f))
        return sf::Vector2f(0, 0);

    sf::Vector2f norm_r = r / distance;

    sf::Vector2f force = (-G * ((mass1 * mass2) / (distance * distance))) * norm_r;
    
    return force;
}

sf::Vector2f getGravityForce(Particle &p1, Particle &p2) {
    return getGravityForce(p1.getPosition(), p1.getMass(), p2.getPosition(), p2.getMass());
}