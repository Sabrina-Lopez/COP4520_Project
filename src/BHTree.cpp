#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include "BHTree.hpp"
#include <math.h>
#include <iostream> 
#include "gravity.hpp"

// Insert a particle into the quad-tree
void BHTreeNode::insert(sf::Vector2f position, float newMass) {
    if (!containsPosition(position))
        return;

    if (mass == 0) {
        mass = newMass;
        massCenter = position;
        return;
    }

    if (!isExternal) {
        for (BHTreeNode &n : children) {
            n.insert(position, newMass);
        }
    }
    else {
        float childWidth = regionWidth / 2;

        sf::Vector2f nwCenter = regionCenter + sf::Vector2f(-childWidth / 2, -childWidth / 2);
        children.push_back(BHTreeNode(nwCenter, childWidth, theta));

        sf::Vector2f neCenter = regionCenter + sf::Vector2f(childWidth / 2, -childWidth / 2);
        children.push_back(BHTreeNode(neCenter, childWidth, theta));

        sf::Vector2f seCenter = regionCenter + sf::Vector2f(childWidth / 2, childWidth / 2);
        children.push_back(BHTreeNode(seCenter, childWidth, theta));

        sf::Vector2f swCenter = regionCenter + sf::Vector2f(-childWidth / 2, childWidth / 2);   
        children.push_back(BHTreeNode(swCenter, childWidth, theta));

        for (BHTreeNode &n : children) {
            n.insert(position, newMass);
            
            if (position != massCenter)
                n.insert(massCenter, mass);
        }

        isExternal = false;
    }

    massCenter = (position * newMass + massCenter * mass) / (mass + newMass);
    mass += newMass;
}

void BHTreeNode::insert(Particle& particle) {
    insert(particle.getPosition(), particle.getMass());
}

void BHTreeNode::insert(std::vector<Particle>& particles) {
    for (Particle& p : particles) {
        insert(p);
    }
}

sf::Vector2f BHTreeNode::calculateForce(sf::Vector2f position, float newMass) {
    if (isExternal && mass > 0 && mass != newMass) {
        return getGravityForce(massCenter, mass, position, newMass);
    }

    sf::Vector2f force = sf::Vector2f(0, 0);

    sf::Vector2f r = position - massCenter;
    float distance = sqrt(r.x * r.x + r.y * r.y);

    if (regionWidth / distance < theta) {
        return getGravityForce(massCenter, mass, position, newMass);
    }
    else {
        for (BHTreeNode& child : children) {
            force += child.calculateForce(position, newMass);
        }
   }

    return force;
}

void BHTreeNode::applyForce(Particle& particle) {
    auto f = calculateForce(particle.getPosition(), particle.getMass());
    particle.applyForce(f);
}

void applyBHForces(BHTreeNode& bh, std::vector<Particle>& particles, int startIdx, int endIdx) {
    for (int i = startIdx; i < endIdx; i++) {
        bh.applyForce(particles[i]);
    }
}

void applyBHForcesParallel(BHTreeNode& bh, std::vector<Particle>& particles, int threadCount) {
    int numParticles = particles.size();
    int particlesPerThread = numParticles / threadCount;

    std::vector<std::thread> threads;

    for (int i = 0; i < threadCount; ++i)
    {
        int startIdx = i * particlesPerThread;
        int endIdx = (i == threadCount - 1) ? numParticles : startIdx + particlesPerThread;

        threads.emplace_back(applyBHForces, std::ref(bh), std::ref(particles), startIdx, endIdx);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }
}



void BHTreeNode::draw(sf::RenderWindow& window) {
    if (regionWidth < 0.75)
        return;

    sf::Vector2f size = sf::Vector2f(regionWidth, regionWidth);

    sf::RectangleShape s = sf::RectangleShape(size);
    s.setPosition(regionCenter - sf::Vector2f(regionWidth / 2, regionWidth / 2));

    s.setFillColor(sf::Color(0,0,0,0));
    s.setOutlineThickness(1);
    s.setOutlineColor(sf::Color(255,255,255,32));

    window.draw(s);

    for (BHTreeNode& child : children) {
        child.draw(window);
    }
}

int BHTreeNode::getSize() {
    int count = 1;
    for (BHTreeNode& child : children) {
        count += child.getSize();
    }
    return count;
}