#pragma once

#include <SFML/Graphics.hpp>
#include "particle.hpp"

class BHTree {
    public:
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
        BHTree(sf::Vector2f regionCenter, float regionSize):
            root(Node(regionCenter, regionSize, regionCenter, 0)) {}
        
        // Insert a particle into a specific node
        void insertParticle(const Particle& particle) {
            insert(root, particle.getPosition(), particle.getMass());
        }

        // Force calculation between a particle and the quad-tree
        sf::Vector2f calculate_particle_force(const Particle& particle) {
            return calculate_force(root, particle.getPosition(), particle.getMass());
        }

        int getChildCount(Node& node, sf::RenderWindow& window) {
            int count = 1;
            sf::Vector2f size = sf::Vector2f(node.regionWidth, node.regionWidth);
            sf::RectangleShape s = sf::RectangleShape(size);
            s.setPosition(node.regionCenter - sf::Vector2f(node.regionWidth / 2, node.regionWidth / 2));
            s.setFillColor(sf::Color(0,0,0,0));
            s.setOutlineThickness(1);
            s.setOutlineColor(sf::Color(255,255,255,10));
            window.draw(s);

            for (Node& child : node.children) {
                count += getChildCount(child, window);
            }
            return count;
        }

        int getTotalNodeSize(sf::RenderWindow& window) {
            return getChildCount(root, window);
        }

    private:
        Node root; // Root node of the quad-tree
        static const double G; // Gravitational constant

        // Insert a particle into the quad-tree
        void insert(Node& node, sf::Vector2f position, float mass);

        // Force calculation between a particle and the quad-tree
        sf::Vector2f calculate_force(Node& node, sf::Vector2f position, float mass);

        // Get the index of the child node that the particle belongs to
        int get_child_index(double x, double y, double center_x, double center_y);
};
