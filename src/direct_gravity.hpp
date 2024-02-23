#pragma once

#include "particle.hpp"
#include <math.h>

const float G = 50.0;

void applyGravity(std::vector<Particle> &particles)
{
    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = i + 1; j < particles.size(); j++)
        {
            float mass1 = particles[i].getMass();
            float mass2 = particles[j].getMass();

            sf::Vector2f pos1 = particles[i].getPosition();
            sf::Vector2f pos2 = particles[j].getPosition();

            sf::Vector2f r = pos2 - pos1;

            float distance = sqrt(r.x * r.x + r.y * r.y);

            sf::Vector2f norm_r = r / distance;

            sf::Vector2f force = (-G * ((mass1 * mass2) / (distance * distance))) * norm_r;

            if (distance > 2)
            {
                particles[i].applyForce(-force);
                particles[j].applyForce(force);
            }
        }
    }
}