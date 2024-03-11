#pragma once

#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include <thread>

class BHTreeNode {
    public:
        float theta = 1;

        bool isExternal; // Refers to whether this is a leaf / external node (represents a single particle)

        sf::Vector2f regionCenter;
        float regionWidth;

        sf::Vector2f massCenter;
        float mass = 0;
        
        // The four child nodes in each quadrant
        std::vector<BHTreeNode> children;

        // Constructor
        BHTreeNode(sf::Vector2f regionCenter, float regionWidth, float theta):
            regionCenter(regionCenter), regionWidth(regionWidth), massCenter(regionCenter), isExternal(true), theta(theta) {}

        bool containsPosition(sf::Vector2f position) {
            if (std::abs(position.x - regionCenter.x) >= regionWidth / 2)
                return false;

            if (std::abs(position.y - regionCenter.y) >= regionWidth / 2)
                return false;

            return true;
        }

        void insert(Particle& particle);
        void insert(std::vector<Particle>& particles);

        void applyForce(Particle& particle);

        void draw(sf::RenderWindow& window);
        int getSize();

    private:
        // Insert a particle into the quad-tree
        void insert(sf::Vector2f position, float newMass);

        // Force calculation between a particle and the quad-tree
        sf::Vector2f calculateForce(sf::Vector2f position, float newMass);
};

void applyBHForcesParallel(BHTreeNode& bh, std::vector<Particle>& particles, int threadCount);