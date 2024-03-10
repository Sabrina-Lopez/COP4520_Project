#pragma once

#include "particle.hpp"
#include "gravity.hpp"
#include <math.h>

#include <thread>
#include <mutex>
#include <vector>

// New Code to Deal with Potential Contention
struct ForceAccumulator
{
    sf::Vector2f force;
    std::mutex mutex;
};

// Using extra function to not condense code too much
void calculateGravity(std::vector<Particle> &particles, int startIdx, int endIdx, std::vector<ForceAccumulator> &forceAccumulators)
{
    int numParticles = particles.size();

    for (int i = startIdx; i < endIdx; i++)
    {
        for (int j = i + 1; j < i + floor(numParticles / 2); j++)
        {
            int jIndex = j % numParticles;
            sf::Vector2f force = getGravityForce(particles[i], particles[jIndex]);

            // Use a lock only when updating the accumulator
            std::lock_guard<std::mutex> lock(forceAccumulators[i].mutex);
            forceAccumulators[i].force -= force;
            forceAccumulators[jIndex].force += force;
        }
    }
}


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
            sf::Vector2f force = getGravityForce(particles[i], particles[j]);

            particles[i].applyForce(-force);
            particles[j].applyForce(force);
        }
    }
}