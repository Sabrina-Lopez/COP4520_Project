#pragma once

#include <SFML/Graphics.hpp>
#include "particle.hpp"

class BHTree {
    public:
        float theta = 1;

        // Represents a node in the quad-tree
        struct Node {
            bool isExternal; // Refers to whether this is a leaf / external node (represents a single particle)

            sf::Vector2f regionCenter;
            float regionWidth;

            sf::Vector2f massCenter;
            float mass;
            
            // The four child nodes in each quadrant
            std::vector<Node> children;

            // Constructor
            Node(sf::Vector2f regionCenter, float regionWidth, sf::Vector2f massCenter, float mass):
                regionCenter(regionCenter), regionWidth(regionWidth), massCenter(regionCenter), mass(mass), isExternal(true) {}

            bool containsPosition(sf::Vector2f position) {
                if (std::abs(position.x - regionCenter.x) > regionWidth / 2)
                    return false;

                if (std::abs(position.y - regionCenter.y) > regionWidth / 2)
                    return false;

                return true;
            }
        };

        // Constructor
        BHTree(sf::Vector2f regionCenter, float regionSize, float theta):
            root(Node(regionCenter, regionSize, regionCenter, 0)), theta(theta) {}
        
        // Insert a particle into a specific node
        void insertParticle(const Particle& particle) {
            insert(root, particle.getPosition(), particle.getMass());
        }

        // Force calculation between a particle and the quad-tree
        sf::Vector2f calculate_particle_force(const Particle& particle) {
            return calculate_force(root, particle.getPosition(), particle.getMass());
        }

        void draw(Node& node, sf::RenderTexture& window) {
            sf::Vector2f size = sf::Vector2f(node.regionWidth, node.regionWidth);
            sf::RectangleShape s = sf::RectangleShape(size);
            s.setPosition(node.regionCenter - sf::Vector2f(node.regionWidth / 2, node.regionWidth / 2));
            s.setFillColor(sf::Color(0,0,0,0));
            s.setOutlineThickness(1);
            s.setOutlineColor(sf::Color(255,255,255,10));
            window.draw(s);

            for (Node& child : node.children) {
                draw(child, window);
            }
        }

        void draw(sf::RenderTexture& window) {
            draw(root, window);
        }

        int getChildCount(Node& node) {
            int count = 1;
            for (Node& child : node.children) {
                count += getChildCount(child);
            }
            return count;
        }

        int getSize() {
            return getChildCount(root);
        }

    private:
        Node root; // Root node of the quad-tree

        // Insert a particle into the quad-tree
        void insert(Node& node, sf::Vector2f position, float mass);

        // Force calculation between a particle and the quad-tree
        sf::Vector2f calculate_force(Node& node, sf::Vector2f position, float mass);
};
