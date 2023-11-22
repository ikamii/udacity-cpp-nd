#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
    std::unique_lock<std::mutex> mLock(_mutex);
    _condition.wait(mLock, [this] {return !_queue.empty();});

    T message = std::move(_queue.back());
    _queue.pop_back();

    return message;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    std::lock_guard<std::mutex> mLock(_mutex);

    _queue.clear();
    _queue.emplace_back(std::move(msg));
    _condition.notify_one();
}


/* Implementation of class "TrafficLight" */

 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.

    while (true){ // Infinite loop

        TrafficLightPhase message = _queue.receive();
        if (message == TrafficLightPhase::green)
            return;

    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 

    // Reference: en.cppreference.com/w/cpp/numeric/random

    std::random_device rnd;
    std::mt19937 gen(rnd()); // Predefined generator
    std::uniform_real_distribution<double> sec (4000, 6000); // interval in ms
    double cycle = sec(gen); 
    std::chrono::time_point<std::chrono::system_clock> lastTime;

    while (true){ // Infinite Loop

        long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastTime).count();

        if (cycle <= time){

            if (_currentPhase == TrafficLightPhase::green)
                _currentPhase = TrafficLightPhase::red;
            else if (_currentPhase == TrafficLightPhase::red)
                _currentPhase = TrafficLightPhase::green;

            _queue.send(std::move(_currentPhase));

            // Reset timer and get new random cycle time
            lastTime = std::chrono::system_clock::now();
            cycle = sec(gen);
        }

        // Sleep for 1 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    }
}

