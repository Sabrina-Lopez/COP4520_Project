#pragma once
// Helper code for calculating gravitation forces between a pair of point masses (particles)
// Uses Newton's Law of Universal Gravitation

// All nbody approaches should use this code regardless of method

#include <SFML/Graphics.hpp>
#include "particle.hpp"

const float G = 10.0; // the gravitational constant
const float minDistance = 2.0; // the minimum distance to calculate gravitation forces

sf::Vector2f getGravityForce(sf::Vector2f pos1, float mass1, sf::Vector2f pos2, float mass2);

sf::Vector2f getGravityForce(Particle &p1, Particle &p2);