#pragma once

#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include "barnesHut.hpp"

// Define the gravitational constant
const double BarnesHut::G = 6.6743e-11;

// Insert a particle into the quad-tree
void BarnesHut::insert(Node& node, double x, double y, double mass) {
    if (node.is_leaf) {
        // If the node is a leaf, create a new node for the new particle and the existing particle
        double new_x = (node.x + x) / 2;
        double new_y = (node.y + y) / 2;
        double new_mass = node.mass + mass;
        
        node.is_leaf = false;
        node.children.emplace_back(node.x, node.y, node.mass);
        node.children.emplace_back(new_x, new_y, new_mass);
        insert(node.children[0], node.x, node.y, node.mass);
        insert(node.children[1], new_x, new_y, new_mass);
    } 
    else {
        // If the node is not a leaf, choose the appropriate child to insert the new particle
        int child_index = get_child_index(x, y, node.x, node.y);
        insert(node.children[child_index], x, y, mass);
    }
}

double BarnesHut::calculate_force(Node& node, double x, double y, double mass) {
    double dx = x - node.x;
    double dy = y - node.y;
    double distance = std::sqrt(dx * dx + dy * dy);

    if (node.is_leaf || distance * node.mass / (distance * distance) < 0.1) {
        // If the node is a leaf or the far-field approximation is valid, use the direct force calculation
        return G * mass * node.mass / (distance * distance);
    } 
    else {
        // Otherwise, recursively calculate the force from the children
        double total_force = 0.0;
        for (auto& child : node.children) {
            total_force += calculate_force(child, x, y, mass);
        }
        return total_force;
    }
}

int BarnesHut::get_child_index(double x, double y, double center_x, double center_y) {
    int child_index = 0;
    
    if (x > center_x) {
        child_index += 1;
    }

    if (y > center_y) {
        child_index += 2;
    }

    return child_index;
}