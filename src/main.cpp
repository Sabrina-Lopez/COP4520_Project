#include <SFML/Graphics.hpp>
#include "particle.h"

sf::Color map_val_to_color(float value) // value is 0-1
{
    if (value < 0.0f) value = 0;
    if (value > 1.0f) value = 1;

    int r = 0, g = 0, b = 0;

    // 0 - 0.5 interp blue to green
    // 0.5 - 1 interp green to red

    if (value < 0.5f)
    {
        b = 255 * (1.0f - 2 * value);
        g = 255 * 2 * value;
    }
    else {
        g = 255 * (2.0f - 2 * value);
        r = 255 * (2 * value - 1);
    }

    return sf::Color(r, g, b);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "SFML works!");
    window.setFramerateLimit(60);

    std::vector<Particle> particles;
    sf::Clock clock;

    int num_particles = 3;

    for (int i = 0; i < num_particles; i++)
    {
        // temporary code to showcase different particles being rendered simultaneously
        switch(i) {
            case 0:
                particles.push_back(Particle(1.0f, sf::Vector2f(400, 500)));
                break;
            case 1:
                particles.push_back(Particle(1.0f, sf::Vector2f(800, 500)));
                break;
            case 2:
                particles.push_back(Particle(1.0f, sf::Vector2f(1200, 500)));
                break;
        }

        // change colors
        float val = (float)i / (float)num_particles;

        sf::Color col = map_val_to_color(val);

        particles[i].set_color(col);
    }

    while (window.isOpen())
    {
        // calculates the frame or delta time (useful for physics!)
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear();

        for (int i = 0; i < particles.size(); i++)
        {
            particles[i].draw(window);
        }

        window.display();
    }

    return 0;
}