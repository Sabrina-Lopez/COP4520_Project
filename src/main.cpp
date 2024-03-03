#include <iostream> 
#include <math.h>

#include <SFML/Graphics.hpp>

#include "particle.hpp"
#include "direct_gravity.hpp"


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

// Returns a particle with a position the area defined by size
Particle createRandomParticle(sf::Vector2u spawnSize) {
    float x = rand() % spawnSize.x;
    float y = rand() % spawnSize.y;
    float mass = rand() % 10 + 1;

    Particle p = Particle(mass, sf::Vector2f(x, y));

    // change colors
    sf::Color col = map_val_to_color(x / spawnSize.x);
    p.set_color(col);

    return p;
}

std::vector<Particle> createRandomParticles(int count, sf::Vector2u spawnSize) {
    std::vector<Particle> particles;
    
    for (int i = 0; i < count; i++)
        particles.push_back(createRandomParticle(spawnSize));

    return particles;
}

void handleWindowEvents(sf::RenderWindow &window) {
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
}

// Will attempt to add a single particle given the current framerate
// If framerate is below the given threshold, will print out total particles added
void benchmark(std::vector<Particle> &particles, float fps, float fps_threshold, sf::Vector2u spawnSize) {
    if (fps >= fps_threshold)
        particles.push_back(createRandomParticle(spawnSize));
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "SFML works!");
    window.setFramerateLimit(60);

    sf::Clock clock;

    sf::Font roboto;
    roboto.loadFromFile("./src/fonts/Roboto-Regular.ttf");

    sf::Text bench_text;
    bench_text.setFont(roboto);

    std::vector<Particle> particles;

    while (window.isOpen())
    {
        // calculates the frame or delta time
        float dt = clock.restart().asSeconds();

        handleWindowEvents(window);

        window.clear();

        // Particle handling
        // applyGravity(particles);
        applyParallelGravity(particles, 4);

        for (int i = 0; i < particles.size(); i++)
        {
            particles[i].integrate(dt);
            particles[i].draw(window);
        }

        // Benchmarking
        float fpsThreshold = 30.0;
        benchmark(particles, (1 / dt), fpsThreshold, window.getSize());

        std::string fps_str = "FPS: " + std::to_string((int)(1 / dt)) + " / " + std::to_string((int)fpsThreshold);
        std::string particles_str = "Particles: " + std::to_string(particles.size());
        bench_text.setString(fps_str + "\n" + particles_str);
        window.draw(bench_text);

        window.display();
    }

    return 0;
}