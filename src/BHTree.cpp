#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include "BHTree.hpp"
#include <math.h>
#include <iostream> 
#include "gravity.hpp"

// Insert a particle into the quad-tree
void BHTree::insert(Node& node, sf::Vector2f position, float mass) {
    if (!node.containsPosition(position))
        return;

    // if node is not empty and has no children
    if (node.isExternal) {
        float childWidth = node.regionWidth / 2;

        // Create all children

        sf::Vector2f nwCenter = node.regionCenter + sf::Vector2f(-childWidth / 2, -childWidth / 2);
        node.children.push_back(Node(nwCenter, childWidth, nwCenter, 0));

        sf::Vector2f neCenter = node.regionCenter + sf::Vector2f(childWidth / 2, -childWidth / 2);
        node.children.push_back(Node(neCenter, childWidth, neCenter, 0));

        sf::Vector2f seCenter = node.regionCenter + sf::Vector2f(childWidth / 2, childWidth / 2);
        node.children.push_back(Node(seCenter, childWidth, seCenter, 0));

        sf::Vector2f swCenter = node.regionCenter + sf::Vector2f(-childWidth / 2, childWidth / 2);   
        node.children.push_back(Node(swCenter, childWidth, swCenter, 0));
        
        node.isExternal = false;
    }

    // if this node is empty
    if (node.mass == 0) {
        node.mass = mass;
        node.massCenter = position;
        return;
    }
    
    node.massCenter = (position * mass + node.massCenter * node.mass) / (node.mass + mass);
    node.mass += mass;

    // Attempt to insert into all children
    for (Node &n : node.children) {
        insert(n, position, mass);
    }
}

sf::Vector2f BHTree::calculate_force(Node& node, sf::Vector2f position, float mass) {
    if (node.isExternal) 
        return getGravityForce(node.massCenter, node.mass, position, mass);

    sf::Vector2f force = sf::Vector2f(0, 0);

    sf::Vector2f r = position - node.massCenter;
    float distance = sqrt(r.x * r.x + r.y * r.y);

    if (node.regionWidth / distance < theta) {
        return getGravityForce(node.massCenter, node.mass, position, mass);
    }
    else {
        for (Node& child : node.children) {
            force += calculate_force(child, position, mass);
        }
    }

    return force;
}