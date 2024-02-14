#include <SFML/Graphics.hpp>

class Particle {
    private:
        float mass;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
        sf::Vector2f force;
        sf::CircleShape s;

    public:
        Particle(float _mass, sf::Vector2f initPos);

        float getMass() const { return mass; }
        sf::Vector2f getPosition() const { return position; }
        sf::Vector2f getVelocity() const { return velocity; }
        sf::Vector2f getAcceleration() const { return acceleration; }
        sf::Vector2f getForce() const { return force; }

        void applyForce(sf::Vector2f new_force);
        void resetForce();
        
        void integrate(float dt);

        void render(sf::RenderWindow& wind);
        void set_color(sf::Color col);
        void draw(sf::RenderWindow &window);
};