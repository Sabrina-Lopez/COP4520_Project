#include <iostream> 
#include <math.h>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "particle.hpp"
#include "particleSpawn.hpp"
#include "naive.hpp"
#include "BHTree.hpp"

void handleWindowEvents(sf::RenderWindow &window) {
    sf::Event event;
    while (window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(window, event);
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
    int threadCount = 1;
    int maxThreads = std::thread::hardware_concurrency();
    
    float minFps = 30.0;
    float secondsUnder = 0;

    int useBH = 0;
    bool drawBH = false;

    int useGalaxy = 0; // 0 = random spawn, 1 = spawn galaxies

    sf::Clock frameClock;

    std::vector<Particle> particles;

    sf::RenderWindow window(sf::VideoMode(1600, 1000), "N-Body Simulation");
    window.setFramerateLimit(120);
    auto _ = ImGui::SFML::Init(window);

    sf::Vector2f windowCenter = sf::Vector2f(window.getSize()) / 2.0f;
    Particle galaxyCenter = Particle(1e4, windowCenter + sf::Vector2f(1, 1));
    galaxyCenter.setColor(sf::Color(255, 255, 255, 64));

    if (useGalaxy)
        particles.push_back(galaxyCenter);

    while (window.isOpen())
    {
        handleWindowEvents(window);
        window.clear();

        // calculates the frame or delta time
        sf::Time frameTime = frameClock.restart();
        ImGui::SFML::Update(window, frameTime);

        float dt = frameTime.asSeconds();
        float fps = 1 / dt;

        // Benchmarking
        if (fps < minFps) 
        {
            secondsUnder += dt;

            if (threadCount <= maxThreads && secondsUnder > 5) 
            {
                std::cout << threadCount << " - " << particles.size() << " particles" << std::endl;

                if (threadCount < maxThreads) {
                    secondsUnder = 0;
                    threadCount++;
                }
            }
        }
        else {
            for (int i = 0; i < std::min(ceil(fps - minFps), 60.0f); i++) 
            {
                auto p = createRandomParticle(window.getSize());

                if (useGalaxy)
                    p = createRandomOrbitParticle(particles[0], 15, 100);

                particles.push_back(p);
            }
        }

        sf::Vector2u windowSize = window.getSize();
        sf::Vector2f windowCenter = sf::Vector2f(windowSize.x / 2, windowSize.y / 2);
        BHTreeNode bh = BHTreeNode(windowCenter, windowSize.x * 2, 1);

        if (useBH) {
            bh.insert(particles);
            applyBHForcesParallel(bh, particles, threadCount);

            if (drawBH)
                bh.draw(window);
        }
        else {
            applyParallelGravity(particles, threadCount); // naive (multi-threaded)
        }

        for (Particle &p : particles)
        {
            p.integrate(dt);
            p.draw(window);
        }

        // UI
        ImGui::Begin("N-body Debug");

        ImGui::Text("FPS: %.2f / %.2f", fps, minFps);
        ImGui::Text("Threads: %d / %d", threadCount, maxThreads);

        ImGui::Text("Set"); ImGui::SameLine();
        if (ImGui::SmallButton("|<<"))
            threadCount = 1;
        ImGui::SameLine();

        if (ImGui::SmallButton("-1"))
            threadCount--;
        ImGui::SameLine();

        if (ImGui::SmallButton("+1"))
            threadCount++;
        ImGui::SameLine();

        if (ImGui::SmallButton(">>|"))
            threadCount = maxThreads;

        ImGui::Separator();

        ImGui::Text("Particles: %d", particles.size());

        if (ImGui::SmallButton("Clear")) {
            particles.clear();
            if (useGalaxy)
                particles.push_back(galaxyCenter);
        }

        ImGui::Text("Spawn:"); ImGui::SameLine(); 
        if (ImGui::RadioButton("Random", &useGalaxy, 0)) {
            particles.clear();
        } 
        ImGui::SameLine(); 
        
        if (ImGui::RadioButton("Galaxy", &useGalaxy, 1)) {
            particles.clear();
            if (useGalaxy)
                particles.push_back(galaxyCenter);
        }

        ImGui::Separator();

        ImGui::Text("Algorithm");
        ImGui::RadioButton("Naive", &useBH, 0); ImGui::SameLine(); 
        ImGui::RadioButton("Barnes-Hut", &useBH, 1);

        if (useBH) {
            ImGui::Text("QuadTree Size: %d", bh.getSize());
            ImGui::Checkbox("Visualize Quadtree", &drawBH);
        }

        ImGui::End();
        
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
