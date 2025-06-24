#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>

struct QuickSortStep {
    std::vector<int> array;
    int pivotIndex;
    int comparingIndex;
    int leftBound;
    int rightBound;
    std::string explanation;
    std::vector<int> leftPartition;
    std::vector<int> rightPartition;
};

struct SortStats {
    int comparisons = 0;
    int swaps = 0;
    double timeTaken = 0.0;
    std::string timeComplexity;
    std::string spaceComplexity;
};

struct VisualizerState {
    std::vector<int> array;
    std::vector<float> floatArray;
    std::vector<std::vector<float>> bucketData;
    std::vector<QuickSortStep> quickSortSteps;
    std::vector<int> countArray;
    int currentQuickStep = 0;
    int currentDigit = -1;
    int highlightedIndex = -1;
    int secondaryIndex = -1;
    float speed = 1.0f;
    bool isSorting = false;
    std::string currentStep;
};

void drawArray(sf::RenderWindow& window, const std::vector<int>& array, 
             int highlightedIndex, int secondaryIndex, const sf::Font& font);
void drawQuickSortVisualization(sf::RenderWindow& window, const QuickSortStep& step, 
                              const VisualizerState& state, const SortStats& stats,
                              const sf::Font& font);
void drawBuckets(sf::RenderWindow& window, const std::vector<std::vector<float>>& buckets,
               const std::string& currentStep, const sf::Font& font);
void drawCountingSort(sf::RenderWindow& window, const std::vector<int>& array,
                    const std::vector<int>& countArray, int highlightedIndex,
                    const sf::Font& font);
void drawExplanation(sf::RenderWindow& window, const std::string& stepText, 
                   const sf::Font& font);