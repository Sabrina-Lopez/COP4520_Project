#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include "BHTree.hpp"
#include <math.h>

// Define the gravitational constant
const double BHTree::G = 10.0;

// Insert a particle into the quad-tree
void BHTree::insert(Node& node, sf::Vector2f position, float mass) {

}

sf::Vector2f BHTree::calculate_force(Node& node, sf::Vector2f position, float mass) {


    // sf::Vector2f d_vec = 

    // if (node.is_external && node.mass_center != position) {

    // }

    return sf::Vector2f(0, 0);
}

int BHTree::get_child_index(double x, double y, double center_x, double center_y) {
    int child_index = 0;
    
    if (x > center_x) {
        child_index += 1;
    }

    if (y > center_y) {
        child_index += 2;
    }

    return child_index;
}