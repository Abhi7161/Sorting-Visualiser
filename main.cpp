#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <thread>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>
#include "Visualizer.hpp"
#include "SortAlgorithms.hpp"

std::vector<int> generateRandomIntArray(int size, int min, int max) {
    std::vector<int> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    for (int& num : arr) {
        num = dist(gen);
    }
    return arr;
}

std::vector<float> generateRandomFloatArray(int size, float min, float max) {
    std::vector<float> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    for (float& num : arr) {
        num = dist(gen);
    }
    return arr;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 700), "Sorting Visualizer");
    sf::Font font;
    if (!font.loadFromFile("Fonts/Roboto-Regular.ttf")) {
        return -1;
    }

    VisualizerState state;
    state.array = generateRandomIntArray(10, 1, 99);
    state.floatArray = generateRandomFloatArray(10, 0.0f, 1.0f);
    state.currentStep = "Press S:Bubble | I:Insertion | Q:Quick | 4:Bucket | 5:Radix | 6:Counting";
    SortStats stats;

    std::function<void()> sortFunction;
    bool sortRequested = false;
    bool bucketView = false;
    bool isQuickSortActive = false;
    bool isCountingSortActive = false;

    auto intCallback = [&](const std::vector<int>& arr, int i, int j, const std::string& explanation) {
        state.array = arr;
        state.highlightedIndex = i;
        state.secondaryIndex = j;
        state.currentStep = explanation;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color(230, 230, 230));
        
        if (isCountingSortActive) {
            drawCountingSort(window, state.array, state.countArray, 
                           state.highlightedIndex, font);
        } else {
            drawArray(window, state.array, state.highlightedIndex, state.secondaryIndex, font);
        }
        
        drawExplanation(window, state.currentStep, font);
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(500 / state.speed)));
    };

    auto bucketCallback = [&](const std::vector<std::vector<float>>& buckets, const std::string& explanation) {
        state.bucketData = buckets;
        state.currentStep = explanation;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color(230, 230, 230));
        drawBuckets(window, state.bucketData, state.currentStep, font);
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(500 / state.speed)));
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    state.array = generateRandomIntArray(10, 1, 99);
                    state.floatArray = generateRandomFloatArray(10, 0.0f, 1.0f);
                    state.isSorting = false;
                    sortRequested = false;
                    bucketView = false;
                    isQuickSortActive = false;
                    isCountingSortActive = false;
                    state.bucketData.clear();
                    state.quickSortSteps.clear();
                    state.currentQuickStep = 0;
                    state.currentDigit = -1;
                    state.countArray.clear();
                    state.currentStep = "Press S:Bubble | I:Insertion | Q:Quick | 4:Bucket | 5:Radix | 6:Counting";
                }
                else if (event.key.code == sf::Keyboard::S && !state.isSorting) {
                    sortFunction = [&]() { bubbleSort(state.array, intCallback); };
                    state.isSorting = true;
                    sortRequested = true;
                    bucketView = false;
                    isQuickSortActive = false;
                    isCountingSortActive = false;
                }
                else if (event.key.code == sf::Keyboard::I && !state.isSorting) {
                    sortFunction = [&]() { insertionSort(state.array, intCallback); };
                    state.isSorting = true;
                    sortRequested = true;
                    bucketView = false;
                    isQuickSortActive = false;
                    isCountingSortActive = false;
                }
                else if (event.key.code == sf::Keyboard::Q && !state.isSorting) {
                    state.currentStep = "Generating Quick Sort steps...";
                    state.quickSortSteps.clear();
                    state.currentQuickStep = 0;
                    isQuickSortActive = true;
                    state.isSorting = true;
                    bucketView = false;
                    isCountingSortActive = false;

                    std::vector<int> tempArray = state.array;
                    quickSort(tempArray, state, 0, tempArray.size() - 1);
                    
                    if (!state.quickSortSteps.empty()) {
                        state.array = state.quickSortSteps.back().array;
                        state.currentQuickStep = 0;
                        state.currentStep = state.quickSortSteps[0].explanation;
                    } else {
                        state.currentStep = "No Quick Sort steps were generated.";
                    }
                    state.isSorting = false;
                }
                else if (event.key.code == sf::Keyboard::Num4 && !state.isSorting) {
                    sortFunction = [&]() {
                        state.bucketData.clear();
                        state.currentStep = "Starting Bucket Sort...";
                        state.floatArray = generateRandomFloatArray(10, 0.0f, 1.0f);
                        window.clear(sf::Color(230, 230, 230));
                        drawBuckets(window, state.bucketData, state.currentStep, font);
                        window.display();
                        std::this_thread::sleep_for(std::chrono::milliseconds(400));
                        bucketSort(state.floatArray, stats, state, bucketCallback);
                    };
                    state.isSorting = true;
                    sortRequested = true;
                    bucketView = true;
                    isQuickSortActive = false;
                    isCountingSortActive = false;
                }
                else if (event.key.code == sf::Keyboard::Num5 && !state.isSorting) {
                    sortFunction = [&]() { radixSort(state.array, intCallback); };
                    state.isSorting = true;
                    sortRequested = true;
                    bucketView = false;
                    isQuickSortActive = false;
                    isCountingSortActive = false;
                }
                else if (event.key.code == sf::Keyboard::Num6 && !state.isSorting) {
                    for (auto& num : state.array) {
                        num = std::min(num, 30);
                    }
                    
                    sortFunction = [&]() { 
                        state.countArray.clear();
                        countingSort(state.array, intCallback, state); 
                    };
                    state.isSorting = true;
                    sortRequested = true;
                    bucketView = false;
                    isQuickSortActive = false;
                    isCountingSortActive = true;
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    state.speed = std::min(state.speed + 0.5f, 5.0f);
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    state.speed = std::max(state.speed - 0.5f, 0.5f);
                }
                else if (event.key.code == sf::Keyboard::Right && isQuickSortActive) {
                    if (state.currentQuickStep < state.quickSortSteps.size() - 1) {
                        state.currentQuickStep++;
                    }
                }
                else if (event.key.code == sf::Keyboard::Left && isQuickSortActive) {
                    if (state.currentQuickStep > 0) {
                        state.currentQuickStep--;
                    }
                }
                else if (event.key.code == sf::Keyboard::Space && isQuickSortActive) {
                    while (state.currentQuickStep < state.quickSortSteps.size() - 1) {
                        state.currentQuickStep++;

                        window.clear(sf::Color(230, 230, 230));
                        drawQuickSortVisualization(window, state.quickSortSteps[state.currentQuickStep], 
                         state, stats, font);

                        sf::Text instructions;
                        instructions.setFont(font);
                        instructions.setString("LEFT/RIGHT: Navigate | SPACE: Auto-play | R: Reset");
                        instructions.setCharacterSize(20);
                        instructions.setFillColor(sf::Color::Black);
                        instructions.setPosition(20, 550);
                        window.draw(instructions);

                        sf::Text explanation;
                        explanation.setFont(font);
                        explanation.setString(state.quickSortSteps[state.currentQuickStep].explanation);
                        explanation.setCharacterSize(22);
                        explanation.setFillColor(sf::Color::Black);
                        explanation.setPosition(20, 580);
                        window.draw(explanation);

                        sf::Text stepCounter;
                        stepCounter.setFont(font);
                        stepCounter.setString("Step " + std::to_string(state.currentQuickStep + 1) +
                                            " of " + std::to_string(state.quickSortSteps.size()));
                        stepCounter.setCharacterSize(20);
                        stepCounter.setFillColor(sf::Color::Black);
                        stepCounter.setPosition(20, 620);
                        window.draw(stepCounter);

                        window.display();
                        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000 / state.speed)));

                        sf::Event quickEvent;
                        while (window.pollEvent(quickEvent)) {
                            if (quickEvent.type == sf::Event::KeyPressed &&
                                quickEvent.key.code == sf::Keyboard::Space) {
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (sortRequested && sortFunction && !isQuickSortActive) {
            sortFunction();
            sortRequested = false;
            state.isSorting = false;
        }

        window.clear(sf::Color(230, 230, 230));

        if (isQuickSortActive) {
            if (!state.quickSortSteps.empty()) {
                state.currentQuickStep = std::min(state.currentQuickStep, (int)state.quickSortSteps.size() - 1);

                const auto& currentStep = state.quickSortSteps[state.currentQuickStep];
                drawQuickSortVisualization(window, currentStep, state, stats, font);

                sf::Text instructions;
                instructions.setFont(font);
                instructions.setString("LEFT/RIGHT: Navigate | SPACE: Auto-play | R: Reset");
                instructions.setCharacterSize(20);
                instructions.setFillColor(sf::Color::Black);
                instructions.setPosition(20, 550);
                window.draw(instructions);

                sf::Text explanation;
                explanation.setFont(font);
                explanation.setString(currentStep.explanation);
                explanation.setCharacterSize(22);
                explanation.setFillColor(sf::Color::Black);
                explanation.setPosition(20, 580);
                window.draw(explanation);

                sf::Text stepCounter;
                stepCounter.setFont(font);
                stepCounter.setString("Step " + std::to_string(state.currentQuickStep + 1) +
                                    " of " + std::to_string(state.quickSortSteps.size()));
                stepCounter.setCharacterSize(20);
                stepCounter.setFillColor(sf::Color::Black);
                stepCounter.setPosition(20, 620);
                window.draw(stepCounter);
            } else {
                static sf::Clock loadingClock;
                if (loadingClock.getElapsedTime().asSeconds() < 2.0f) {
                    sf::Text loadingText;
                    loadingText.setFont(font);
                    loadingText.setString("Generating Quick Sort steps...");
                    loadingText.setCharacterSize(24);
                    loadingText.setFillColor(sf::Color::Black);
                    loadingText.setPosition(20, 580);
                    window.draw(loadingText);
                } else {
                    state.currentStep = "Failed to generate Quick Sort steps";
                    isQuickSortActive = false;
                }
            }
        }
        else if (bucketView) {
            drawBuckets(window, state.bucketData, state.currentStep, font);
        }
        else if (isCountingSortActive) {
            drawCountingSort(window, state.array, state.countArray, 
                            state.highlightedIndex, font);
        }
        else {
            drawArray(window, state.array, state.highlightedIndex, state.secondaryIndex, font);
        }

        drawExplanation(window, state.currentStep, font);
        window.display();
    }

    return 0;
}