#pragma once

#include <SFML/Graphics.hpp>
#include "particle.hpp"

class BHTree {
    public:
        // Represents a node in the quad-tree
        struct Node {
            bool is_external; // Refers to whether this is a leaf / external node (represents a single particle)
            sf::Vector2f region_center;
            float region_width;
            sf::Vector2f mass_center;
            float mass;
            
            // The four child nodes in each quadrant
            std::vector<Node> nw; // north west
            std::vector<Node> ne; // north east
            std::vector<Node> se; // south east
            std::vector<Node> sw; // south west

            // Constructor
            Node(sf::Vector2f region_center, float region_size, sf::Vector2f mass_center, float mass):
                region_center(region_center), region_width(region_width), mass_center(), mass(mass), is_external(true) {}
        };

        // Constructor
        BHTree(sf::Vector2f region_center, float region_size):
            root(Node(region_center, region_size, region_center, 0)) {}
        
        // Insert a particle into a specific node
        void insertParticle(const Particle& particle) {
            insert(root, particle.getPosition(), particle.getMass());
        }

        // Force calculation between a particle and the quad-tree
        sf::Vector2f calculate_particle_force(const Particle& particle) {
            return calculate_force(root, particle.getPosition(), particle.getMass());
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
