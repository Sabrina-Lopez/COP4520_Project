#include <iostream> 
#include <math.h>

#include <SFML/Graphics.hpp>

#include "particle.hpp"
#include "naive_nbody.hpp"
#include "BHTree.hpp"

// Returns a particle with a position the area defined by size
Particle createRandomParticle(sf::Vector2u spawnSize) {
    float x = rand() % spawnSize.x;
    float y = rand() % spawnSize.y;
    float mass = rand() % 10 + 1;

    Particle p = Particle(mass, sf::Vector2f(x, y));

    // change colors
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;
    p.set_color(sf::Color(r,g,b));

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

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "N-Body Simulation");
    window.setFramerateLimit(120);

    sf::Clock clock;

    sf::Font roboto;
    roboto.loadFromFile("./src/fonts/Roboto-Regular.ttf");

    sf::Text bench_text;
    bench_text.setScale(sf::Vector2f(0.75, 0.75));
    bench_text.setFont(roboto);

    std::vector<Particle> particles;

    int threadCount = 4;
    float maxThreads = 4;
    
    float minFps = 30.0;
    float secondsUnder = 0;

    while (window.isOpen())
    {
        // calculates the frame or delta time
        float dt = clock.restart().asSeconds();
        float fps = 1 / dt;

        handleWindowEvents(window);
        window.clear();

        // Benchmarking
        secondsUnder += dt;
        if (fps >= minFps) 
        {
            for (int i = 0; i < std::min(ceil(fps - minFps), 60.0f); i++) 
            {
                particles.push_back(createRandomParticle(window.getSize()));
                secondsUnder = 0;
            }
        }
        else if (threadCount <= maxThreads && secondsUnder > 5) 
        {
            std::cout << threadCount << " - " << particles.size() << " particles" << std::endl;
            threadCount++;
            secondsUnder = 0;
        }

        sf::Vector2u windowSize = window.getSize();
        sf::Vector2f windowCenter = sf::Vector2f(windowSize.x / 2, windowSize.y / 2);
        BHTreeNode bh = BHTreeNode(windowCenter, windowSize.x * 2, 1);

        bh.insert(particles);
        applyBHForcesParallel(bh, particles, threadCount);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            bh.draw(window);

        //applyParallelGravity(particles, threadCount); // naive (multi-threaded)

        for (Particle &p : particles)
        {
            p.integrate(dt);
            p.draw(window);
        }

        std::string fps_str = "FPS: " + std::to_string((int)(1 / dt)) + " / " + std::to_string((int)minFps);
        std::string particles_str = "Particles: " + std::to_string(particles.size());
        std::string threads_str = "Threads: " + std::to_string(threadCount);
        std::string bh_str = "BH Size: " + std::to_string(bh.getSize());
        bench_text.setString(fps_str + "\n" + particles_str + "\n" + threads_str + "\n" + bh_str);
        window.draw(bench_text);

        window.display();
    }

    return 0;
}
