#pragma once

#include "particle.hpp"
#include <math.h>

#include <thread>
#include <mutex>
#include <vector>

const float G = 10.0;

// New Code to Deal with Potential Contention
struct ForceAccumulator
{
    sf::Vector2f force;
    std::mutex mutex;
};

std::mutex mutex;

// New code...
// Using extra function to not condense code too much
void calculateGravity(std::vector<Particle> &particles, int startIdx, int endIdx, std::vector<ForceAccumulator> &forceAccumulators)
{
    int numParticles = particles.size();

    for (int i = startIdx; i < endIdx; i++)
    {
        for (int j = i + 1; j < i + floor(numParticles / 2); j++)
        {
            int curIndex = j % numParticles;

            float mass1 = particles[i].getMass();
            float mass2 = particles[curIndex].getMass();

            sf::Vector2f pos1 = particles[i].getPosition();
            sf::Vector2f pos2 = particles[curIndex].getPosition();

            sf::Vector2f r = pos2 - pos1;

            float distance = sqrt(r.x * r.x + r.y * r.y);

            if (distance > 2)
            {
                sf::Vector2f norm_r = r / distance;
                sf::Vector2f force = (-G * ((mass1 * mass2) / (distance * distance))) * norm_r;

                // Use a lock only when updating the accumulator
                std::lock_guard<std::mutex> lock(forceAccumulators[i].mutex);
                forceAccumulators[i].force -= force;
                forceAccumulators[curIndex].force += force;
            }
        }
    }
}


// New Code
void applyParallelGravity(std::vector<Particle> &particles, int numThreads)
{
    int numParticles = particles.size();
    int particlesPerThread = numParticles / numThreads;

    std::vector<std::thread> threads;
    std::vector<ForceAccumulator> forceAccumulators(numParticles);

    for (int i = 0; i < numThreads; ++i)
    {
        int startIdx = i * particlesPerThread;
        int endIdx = (i == numThreads - 1) ? numParticles : startIdx + particlesPerThread;

        threads.emplace_back(calculateGravity, std::ref(particles), startIdx, endIdx, std::ref(forceAccumulators));
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    // Apply accumulated forces outside the critical section
    for (int i = 0; i < numParticles; ++i)
    {
        particles[i].applyForce(forceAccumulators[i].force);
    }
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