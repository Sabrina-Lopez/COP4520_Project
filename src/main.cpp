#include <SFML/Graphics.hpp>
#include "particle.h"

#include <math.h>

const float G = 100.0;

sf::Color map_val_to_color(float value) // value is 0-1
{
    if (value < 0.0f)
        value = 0;
    if (value > 1.0f)
        value = 1;

    int r = 0, g = 0, b = 0;

    // 0 - 0.5 interp blue to green
    // 0.5 - 1 interp green to red

    if (value < 0.5f)
    {
        b = 255 * (1.0f - 2 * value);
        g = 255 * 2 * value;
    }
    else
    {
        g = 255 * (2.0f - 2 * value);
        r = 255 * (2 * value - 1);
    }

    return sf::Color(r, g, b);
}

void applyGravity(std::vector<Particle> &particles)
{
    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = i + 1; j < particles.size(); j++)
        {
            float mass1 = particles[i].getMass();
            float mass2 = particles[j].getMass();

            sf::Vector2f pos1 = particles[i].getPosition();
            sf::Vector2f pos2 = particles[j].getPosition();

            sf::Vector2f r = pos2 - pos1;

            float distance = sqrt(r.x * r.x + r.y * r.y);

            sf::Vector2f norm_r = r / distance;

            sf::Vector2f force = (-G * ((mass1 * mass2) / (distance * distance))) * norm_r;

            if (distance > 2)
            {
                particles[i].applyForce(-force);
                particles[j].applyForce(force);
            }
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "SFML works!");
    window.setFramerateLimit(60);

    std::vector<Particle> particles;
    sf::Clock clock;

    int num_particles = 1000;

    for (int i = 0; i < num_particles; i++)
    {
        float x = rand() % window.getSize().x;
        float y = rand() % window.getSize().y;
        float mass = rand() % 10 + 1;

        particles.push_back(Particle(mass, sf::Vector2f(x, y)));

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

            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        window.clear();

        applyGravity(particles);

        for (int i = 0; i < particles.size(); i++)
        {
            particles[i].integrate(dt);
            particles[i].draw(window);
        }

        window.display();
    }

    return 0;
}