#include <iostream> 
#include <math.h>

#include <SFML/Graphics.hpp>

#include "particle.hpp"
#include "naive_nbody.hpp"
#include "BHTree.hpp"

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


void calculateBHGravity(BHTree& bh, std::vector<Particle>& particles, int startIdx, int endIdx) {
    for (int i = startIdx; i < endIdx; i++) {
        sf::Vector2f force = bh.calculate_particle_force(particles[i]);
        particles[i].applyForce(force);
    }
}

float furthestDistance(std::vector<Particle>& particles) {
    int largestX = 0;
    for (Particle p : particles) {
        if (p.getPosition().x > largestX) {
            largestX = p.getPosition().x;
        }
    }
    return largestX;
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

    float threadCount = 1;
    float maxThreads = 20;
    
    float minFps = 30.0;
    float secondsUnder = 0;

    bool wasMousePressed = false;

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

        // Particle handling
        // applyParallelGravity(particles, threadCount); // naive (multi-threaded)
        // applyGravity(particles); // naive (single threaded)
        
        sf::Vector2f windowCenter = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);
        BHTree bh = BHTree(windowCenter, furthestDistance(particles) * 2, 1);
        
        for (Particle &p : particles) {
            bh.insertParticle(p);
        }

        int numParticles = particles.size();
        int particlesPerThread = numParticles / threadCount;

        std::vector<std::thread> threads;

        for (int i = 0; i < threadCount; ++i)
        {
            int startIdx = i * particlesPerThread;
            int endIdx = (i == threadCount - 1) ? numParticles : startIdx + particlesPerThread;

            threads.emplace_back(calculateBHGravity, std::ref(bh), std::ref(particles), startIdx, endIdx);
        }

        for (auto &thread : threads)
        {
            thread.join();
        }

        for (Particle &p : particles)
        {
            p.integrate(dt);
            p.draw(window);
        }

        std::string fps_str = "FPS: " + std::to_string((int)(1 / dt)) + " / " + std::to_string((int)minFps);
        std::string particles_str = "Particles: " + std::to_string(particles.size());
        bench_text.setString(fps_str + "\n" + particles_str);
        window.draw(bench_text);

        window.display();
    }

    return 0;
}
