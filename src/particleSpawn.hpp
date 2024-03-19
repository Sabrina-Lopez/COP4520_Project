#include "particle.hpp"
#include "gravity.hpp"
#include <math.h>
#include <SFML/Graphics.hpp>

const float PI = 3.1415926535;

float randomFloat()
{
    return (float)(rand()) / (float)(RAND_MAX);
}

void setRandomColor(Particle& p) {
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;
    p.setColor(sf::Color(r,g,b));
}

// Returns a particle with a position the area defined by size
Particle createRandomParticle(sf::Vector2u spawnSize) {
    float x = rand() % spawnSize.x;
    float y = rand() % spawnSize.y;
    float mass = rand() % 1000 + 1;

    Particle p = Particle(mass, sf::Vector2f(x, y));

    // change colors
    setRandomColor(p);

    return p;
}

Particle createRandomOrbitParticle(Particle centerParticle, float minDistance, float maxDistance) {
    float distance = randomFloat() * (maxDistance - minDistance) + minDistance;
    float angle = (randomFloat() * 360) * PI / 180.0;

    float x = distance * cos(angle);
    float y = distance * sin(angle);

    auto center = centerParticle.getPosition();

    sf::Vector2f pos = sf::Vector2f(x, y);

    sf::Vector2f vel = sf::Vector2f(-pos.y, pos.x);
    vel /= sqrt(pos.x * pos.x + pos.y * pos.y);

    Particle p = Particle(1, pos + center, vel * sqrt(G * centerParticle.getMass() / distance));

    // change colors
    p.setColor(sf::Color(194, 245, 255, 32));

    return p;
}

std::vector<Particle> createGalaxyParticles(int count, sf::Vector2f center, float centerMass, int radius) {
    std::vector<Particle> particles;

    sf::Vector2f vel = sf::Vector2f(0,0);

    Particle centerParticle = Particle(centerMass, center, vel);

    sf::CircleShape shape = sf::CircleShape(10);
    shape.setFillColor(sf::Color::Black);
    shape.setOutlineColor(sf::Color(255, 255, 255, 32));
    shape.setOutlineThickness(2);
    centerParticle.setShape(shape);

    particles.push_back(centerParticle);

    for (int i = 0; i < count; i++) {
        Particle p = createRandomOrbitParticle(centerParticle, 15, radius);
        particles.push_back(p);
    }

    return particles;
}

void addParticles(std::vector<Particle>& original, std::vector<Particle> other) {
    for (Particle& p : other) {
        original.push_back(p);
    }
}

std::vector<Particle> createRandomParticles(int count, sf::Vector2u spawnSize) {
    std::vector<Particle> particles;
    
    for (int i = 0; i < count; i++)
        particles.push_back(createRandomParticle(spawnSize));

    return particles;
}