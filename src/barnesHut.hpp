#pragma once

#include <SFML/Graphics.hpp>
#include "particle.hpp"

class BarnesHut {
    public:
        // Represents a node in the quad-tree
        struct Node {
            double x, y, mass;
            bool is_leaf; // True if the node is a leaf node (contains a single particle)
            std::vector<Node> children;

            // Constructor
            Node(double x, double y, double mass):
                x(x), y(y), mass(mass), is_leaf(true) {}
        };

        // Constructor
        BarnesHut(double x_min, double x_max, double y_min, double y_max):
            root(Node(x_min + (x_max - x_min) / 2, y_min + (y_max - y_min) / 2, 0.0)) {}
        
        // Insert a particle into a specific node
        void insertParticle(const Particle& particle) {
            insert(root, particle.getPosition().x, particle.getPosition().y, particle.getMass());
        }

        // Force calculation between a particle and the quad-tree
        double calculate_particle_force(const Particle& particle) {
            return calculate_force(root, particle.getPosition().x, particle.getPosition().y, particle.getMass());
        }

    private:
        Node root; // Root node of the quad-tree
        static const double G; // Gravitational constant

        // Insert a particle into the quad-tree
        void insert(Node& node, double x, double y, double mass);

        // Force calculation between a particle and the quad-tree
        double calculate_force(Node& node, double x, double y, double mass);

        // Get the index of the child node that the particle belongs to
        int get_child_index(double x, double y, double center_x, double center_y);
};
