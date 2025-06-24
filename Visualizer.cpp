#include "Visualizer.hpp"
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <SFML/System/String.hpp>

void drawArray(sf::RenderWindow& window, const std::vector<int>& array, 
              int highlightIndex, int secondHighlight, const sf::Font& font) {
    const float boxWidth = 50.f;
    const float boxHeight = 50.f;
    const float spacing = 10.f;
    const float startX = 20.f;
    const float yPos = 190.f;  // Increased from 100 to 150

    for (size_t i = 0; i < array.size(); ++i) {
        float x = startX + i * (boxWidth + spacing);

        sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
        box.setPosition(x, yPos);
        
        if (static_cast<int>(i) == highlightIndex) {
            box.setFillColor(sf::Color::Yellow);
        } 
        else if (static_cast<int>(i) == secondHighlight) {
            box.setFillColor(sf::Color::Cyan);
        }
        else {
            box.setFillColor(sf::Color::White);
        }

        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);
        window.draw(box);

        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(array[i]));
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2, bounds.height / 2);
        text.setPosition(x + boxWidth / 2, yPos + boxHeight / 2);
        window.draw(text);
    }
}
void drawQuickSortVisualization(sf::RenderWindow& window, const QuickSortStep& step, 
                              const VisualizerState& state, const SortStats& stats,
                              const sf::Font& font) {
    const float startX = 50.f;
    const float startY = 100.f;
    const float boxSize = 40.f;
    const float spacing = 10.f;
    const float verticalSpacing = 80.f;
    const float infoSpacing = 30.f;

    // Draw the main array
    for (size_t i = 0; i < step.array.size(); ++i) {
        sf::RectangleShape box(sf::Vector2f(boxSize, boxSize));
        box.setPosition(startX + i * (boxSize + spacing), startY);
        
        // Color coding
        if (i == step.pivotIndex) {
            box.setFillColor(sf::Color::Red);
        } 
        else if (i == step.comparingIndex) {
            box.setFillColor(sf::Color::Yellow);
        }
        else if (step.leftBound != -1 && i >= step.leftBound && i <= step.rightBound) {
            box.setFillColor(sf::Color(200, 255, 200));
        }
        else {
            box.setFillColor(sf::Color(220, 220, 220));
        }

        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);
        window.draw(box);

        // Draw array value
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(step.array[i]));
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2, bounds.height / 2);
        text.setPosition(box.getPosition().x + boxSize / 2, box.getPosition().y + boxSize / 2);
        window.draw(text);
    }

    // Current step explanation
    sf::Text stepText;
    stepText.setFont(font);
    stepText.setString(step.explanation);
    stepText.setCharacterSize(20);
    stepText.setFillColor(sf::Color::Black);
    stepText.setPosition(startX, startY + verticalSpacing);
    window.draw(stepText);

    // Navigation instructions (always shown)
    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("LEFT/RIGHT: Navigate | SPACE: Auto-play | R: Reset");
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(startX, window.getSize().y - 100);
    window.draw(instructions);

    // Step counter (always shown)
    sf::Text stepCounter;
    stepCounter.setFont(font);
    stepCounter.setString("Step " + std::to_string(state.currentQuickStep + 1) +
                        " of " + std::to_string(static_cast<int>(state.quickSortSteps.size())));
    stepCounter.setCharacterSize(20);
    stepCounter.setFillColor(sf::Color::Black);
    stepCounter.setPosition(startX, window.getSize().y - 70);
    window.draw(stepCounter);

    // Completion message (only shown on final step)
    if (state.currentQuickStep == static_cast<int>(state.quickSortSteps.size()) - 1) {
        sf::Text completionText;
        completionText.setFont(font);
        completionText.setString("QuickSort Complete!\n" +
                                std::to_string(stats.timeTaken) + "ms\n" +
                               "Comparisons: " + std::to_string(stats.comparisons) + "\n" +
                               "Swaps: " + std::to_string(stats.swaps) + "\n" +
                               "Time Complexity: O(n log n) avg, O(n²) worst\n" +
                               "Space Complexity: O(log n)");
        completionText.setCharacterSize(20);
        completionText.setFillColor(sf::Color::Green);
        completionText.setPosition(startX, startY + verticalSpacing + infoSpacing * 2);
        window.draw(completionText);
    }
}
void drawBuckets(sf::RenderWindow& window, const std::vector<std::vector<float>>& buckets,
                const std::string& currentStep, const sf::Font& font) {
    const float startX = 100.f;
    const float startY = 110.f;  // Increased from 50 to 100
    const float rectWidth = 60.f;
    const float rectHeight = 30.f;
    const float spacingX = 10.f;
    const float spacingY = 60.f;  // Increased from 50 to 60

    // Draw each bucket
    for (size_t i = 0; i < buckets.size(); ++i) {
        float y = startY + i * spacingY;

        // Bucket label
        sf::Text indexText;
        indexText.setFont(font);
        indexText.setString("Bucket " + std::to_string(i) + ":");
        indexText.setCharacterSize(16);
        indexText.setFillColor(sf::Color::Black);
        indexText.setPosition(20, y);
        window.draw(indexText);

        // Bucket elements
        for (size_t j = 0; j < buckets[i].size(); ++j) {
            float x = startX + j * (rectWidth + spacingX);

            sf::RectangleShape rect(sf::Vector2f(rectWidth, rectHeight));
            rect.setPosition(x, y);
            rect.setFillColor(sf::Color(100, 150, 250));
            rect.setOutlineColor(sf::Color::Black);
            rect.setOutlineThickness(1);
            window.draw(rect);

            // Value text
            sf::Text text;
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << buckets[i][j];
            text.setFont(font);
            text.setString(ss.str());
            text.setCharacterSize(14);
            text.setFillColor(sf::Color::Black);

            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.width / 2, bounds.height / 2);
            text.setPosition(x + rectWidth / 2, y + rectHeight / 2);
            window.draw(text);
        }
    }

    drawExplanation(window, currentStep, font);
}

void drawCountingSort(sf::RenderWindow& window, const std::vector<int>& array,
                    const std::vector<int>& countArray, int highlightedIndex,
                    const sf::Font& font) {
    const float windowWidth = window.getSize().x;
    const float arrayStartY = 200.f;  // Increased from 100 to 150
    const float countStartY = 300.f;  // Increased from 200 to 250
    const float boxWidth = 50.f;
    const float boxHeight = 50.f;
    const float countBoxHeight = 30.f;
    const float spacing = 10.f;
    const float countSpacing = 5.f;
    float startX = 20.f;

    // Draw main array
    for (size_t i = 0; i < array.size(); ++i) {
        float x = startX + i * (boxWidth + spacing);
        
        sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
        box.setPosition(x, arrayStartY);
        box.setFillColor(static_cast<int>(i) == highlightedIndex ? sf::Color::Yellow : sf::Color::White);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);
        window.draw(box);

        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(array[i]));
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2, bounds.height / 2);
        text.setPosition(x + boxWidth / 2, arrayStartY + boxHeight / 2);
        window.draw(text);
    }

    // Draw count array
    const float maxBoxWidth = (windowWidth - 40.f) / countArray.size() - countSpacing;
    const float countBoxWidth = std::min(30.f, maxBoxWidth);
    
    startX = 20.f;
    for (size_t i = 0; i < countArray.size(); ++i) {
        float x = startX + i * (countBoxWidth + countSpacing);
        
        if (x + countBoxWidth > windowWidth - 20.f) break;
        
        sf::RectangleShape box(sf::Vector2f(countBoxWidth, countBoxHeight));
        box.setPosition(x, countStartY);
        box.setFillColor(sf::Color(180, 220, 255));
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(1);
        window.draw(box);

        sf::Text countText;
        countText.setFont(font);
        countText.setString(std::to_string(countArray[i]));
        countText.setCharacterSize(16);
        countText.setFillColor(sf::Color::Black);
        
        sf::FloatRect bounds = countText.getLocalBounds();
        countText.setOrigin(bounds.width / 2, bounds.height / 2);
        countText.setPosition(x + countBoxWidth / 2, countStartY + countBoxHeight / 2);
        window.draw(countText);

        if (countBoxWidth > 20.f) {
            sf::Text indexText;
            indexText.setFont(font);
            indexText.setString(std::to_string(i));
            indexText.setCharacterSize(14);
            indexText.setFillColor(sf::Color::Black);
            indexText.setPosition(x + countBoxWidth / 2, countStartY + countBoxHeight + 10);  // Increased from 5 to 10
            window.draw(indexText);
        }
    }
}

void drawExplanation(sf::RenderWindow& window, const std::string& stepText, 
                    const sf::Font& font) {
    sf::Text explanation;
    explanation.setFont(font);
    explanation.setString(stepText);
    explanation.setCharacterSize(20);
    explanation.setFillColor(sf::Color::Black);
    
    // Wrap text if it's too long
    float maxWidth = window.getSize().x - 80.f;
    sf::FloatRect bounds = explanation.getLocalBounds();
    if (bounds.width > maxWidth) {
        std::string wrappedText;
        std::string currentLine;
        std::istringstream iss(stepText);
        std::string word;
        
        while (iss >> word) {
            sf::Text testText = explanation;
            testText.setString(currentLine + word + " ");
            if (testText.getLocalBounds().width < maxWidth) {
                currentLine += word + " ";
            } else {
                wrappedText += currentLine + "\n";
                currentLine = word + " ";
            }
        }
        wrappedText += currentLine;
        explanation.setString(wrappedText);
    }
    
    explanation.setPosition(20.f, 20.f);
    window.draw(explanation);
}