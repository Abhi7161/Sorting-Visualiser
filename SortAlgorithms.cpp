#include "SortAlgorithms.hpp"
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cmath>

void sleepForVisualization(float speedMultiplier) {
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(500 / speedMultiplier)));
}

std::string formatFloatArray(const std::vector<float>& arr) {
    std::ostringstream oss;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i != 0) oss << ", ";
        oss << std::fixed << std::setprecision(2) << arr[i];
    }
    return oss.str();
}

void bubbleSort(std::vector<int>& arr, 
               std::function<void(const std::vector<int>&, int, int, const std::string&)> callback) {
    SortStats stats;
    stats.timeComplexity = "O(n^2)";
    stats.spaceComplexity = "O(1)";
    auto startTime = std::chrono::high_resolution_clock::now();

    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            stats.comparisons++;
            callback(arr, j, j + 1, "Comparing elements\nComparisons: " + std::to_string(stats.comparisons));
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                stats.swaps++;
                callback(arr, j, j + 1, "Swapped elements\nComparisons: " + std::to_string(stats.comparisons) + 
                                      "\nSwaps: " + std::to_string(stats.swaps));
            }
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    stats.timeTaken = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    callback(arr, -1, -1, "Bubble Sort Complete\nTime: " + std::to_string(stats.timeTaken) + "ms\n" +
                         "Comparisons: " + std::to_string(stats.comparisons) + "\n" +
                         "Swaps: " + std::to_string(stats.swaps) + "\n" +
                         "Time Complexity: " + stats.timeComplexity + "\n" +
                         "Space Complexity: " + stats.spaceComplexity);
}

void insertionSort(std::vector<int>& arr,
                 std::function<void(const std::vector<int>&, int, int, const std::string&)> callback) {
    SortStats stats;
    stats.timeComplexity = "O(n^2)";
    stats.spaceComplexity = "O(1)";
    auto startTime = std::chrono::high_resolution_clock::now();

    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        stats.comparisons++;
        callback(arr, i, j, "Picked element " + std::to_string(key) + 
                           "\nComparisons: " + std::to_string(stats.comparisons));

        while (j >= 0 && arr[j] > key) {
            stats.comparisons++;
            arr[j + 1] = arr[j];
            stats.swaps++;
            callback(arr, j, j + 1, "Shifting " + std::to_string(arr[j]) + " right\n" +
                                    "Comparisons: " + std::to_string(stats.comparisons) + "\n" +
                                    "Shifts: " + std::to_string(stats.swaps));
            j--;
        }

        arr[j + 1] = key;
        callback(arr, j + 1, i, "Inserted " + std::to_string(key) + " at position " + 
                                std::to_string(j + 1) + "\nComparisons: " + 
                                std::to_string(stats.comparisons) + "\nShifts: " + 
                                std::to_string(stats.swaps));
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    stats.timeTaken = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    callback(arr, -1, -1, "Insertion Sort Completed!\nTime: " + std::to_string(stats.timeTaken) + "ms\n" +
                         "Comparisons: " + std::to_string(stats.comparisons) + "\n" +
                         "Shifts: " + std::to_string(stats.swaps) + "\n" +
                         "Time Complexity: " + stats.timeComplexity + "\n" +
                         "Space Complexity: " + stats.spaceComplexity);
}

void quickSort(std::vector<int>& arr, VisualizerState& state, int low, int high, bool isInitialCall) {
    static SortStats stats;
    static std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    
    if (isInitialCall) {
        stats = SortStats();
        stats.timeComplexity = "O(n log n) avg, O(n^2) worst";
        stats.spaceComplexity = "O(log n)";
        startTime = std::chrono::high_resolution_clock::now();
    }

    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;

        QuickSortStep step;
        step.array = arr;
        step.pivotIndex = high;
        step.comparingIndex = -1;
        step.leftBound = low;
        step.rightBound = high;
        step.explanation = "Selecting pivot: " + std::to_string(pivot) + " (index " + 
                          std::to_string(high) + ")\nComparisons: " + 
                          std::to_string(stats.comparisons);
        state.quickSortSteps.push_back(step);

        for (int j = low; j < high; ++j) {
            stats.comparisons++;
            step.array = arr;
            step.pivotIndex = high;
            step.comparingIndex = j;
            step.leftBound = low;
            step.rightBound = high;
            step.explanation = "Comparing " + std::to_string(arr[j]) + " with pivot (" + 
                              std::to_string(pivot) + ")\nComparisons: " + 
                              std::to_string(stats.comparisons);
            state.quickSortSteps.push_back(step);

            if (arr[j] < pivot) {
                i++;
                if (i != j) {
                    std::swap(arr[i], arr[j]);
                    stats.swaps++;
                    step.array = arr;
                    step.explanation = "Swapped " + std::to_string(arr[i]) + " and " + 
                                      std::to_string(arr[j]) + "\nComparisons: " + 
                                      std::to_string(stats.comparisons) + "\nSwaps: " + 
                                      std::to_string(stats.swaps);
                    state.quickSortSteps.push_back(step);
                }
            }
        }

        if (i + 1 != high) {
            std::swap(arr[i + 1], arr[high]);
            stats.swaps++;
            step.array = arr;
            step.pivotIndex = i + 1;
            step.comparingIndex = -1;
            step.leftBound = low;
            step.rightBound = high;
            step.explanation = "Moved pivot to final position at index " + 
                              std::to_string(i + 1) + "\nComparisons: " + 
                              std::to_string(stats.comparisons) + "\nSwaps: " + 
                              std::to_string(stats.swaps);
            state.quickSortSteps.push_back(step);
        }
        int pivotPos = i + 1;

        step.array = arr;
        step.pivotIndex = pivotPos;
        step.comparingIndex = -1;
        step.leftBound = low;
        step.rightBound = high;
        if (low < pivotPos)
            step.leftPartition = std::vector<int>(arr.begin() + low, arr.begin() + pivotPos);
        if (pivotPos + 1 < high)
            step.rightPartition = std::vector<int>(arr.begin() + pivotPos + 1, arr.begin() + high + 1);
        step.explanation = "Partitioned: left (" + std::to_string(low) + "-" + 
                          std::to_string(pivotPos-1) + "), right (" + 
                          std::to_string(pivotPos+1) + "-" + std::to_string(high) + ")\n" +
                          "Comparisons: " + std::to_string(stats.comparisons) + "\n" +
                          "Swaps: " + std::to_string(stats.swaps);
        state.quickSortSteps.push_back(step);

        quickSort(arr, state, low, pivotPos - 1, false);
        quickSort(arr, state, pivotPos + 1, high, false);

        if (isInitialCall) {
            auto endTime = std::chrono::high_resolution_clock::now();
            stats.timeTaken = std::chrono::duration<double, std::milli>(endTime - startTime).count();
            step.array = arr;
            step.pivotIndex = -1;
            step.comparingIndex = -1;
            step.leftBound = -1;
            step.rightBound = -1;
            step.leftPartition.clear();
            step.rightPartition.clear();
            step.explanation = "QuickSort Complete!\nTime: " + std::to_string(stats.timeTaken) + "ms\n" +
                              "Comparisons: " + std::to_string(stats.comparisons) + "\n" +
                              "Swaps: " + std::to_string(stats.swaps) + "\n" +
                              "Time Complexity: " + stats.timeComplexity + "\n" +
                              "Space Complexity: " + stats.spaceComplexity;
            state.quickSortSteps.push_back(step);
        }
    }
}

void bucketSort(std::vector<float>& arr,
               SortStats& stats,
               VisualizerState& state,
               std::function<void(const std::vector<std::vector<float>>&, const std::string&)> stepCallback) {
    stats = SortStats();
    stats.timeComplexity = "O(n + k) average";
    stats.spaceComplexity = "O(n + k)";
    auto startTime = std::chrono::high_resolution_clock::now();

    int n = arr.size();
    if (n <= 0) return;

    std::vector<std::vector<float>> buckets(n);
    for (int i = 0; i < n; ++i) {
        int index = static_cast<int>(arr[i] * n);
        if (index == n) index = n - 1;
        buckets[index].push_back(arr[i]);
        stats.swaps++;

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) 
            << "Placing " << arr[i] << " into bucket " << index << "\n"
            << "Operations: " << stats.swaps;
        stepCallback(buckets, oss.str());
        sleepForVisualization(state.speed);
    }

    for (int i = 0; i < n; ++i) {
        if (!buckets[i].empty()) {
            std::sort(buckets[i].begin(), buckets[i].end());
            stats.comparisons += buckets[i].size() * log2(buckets[i].size());
            stats.swaps += buckets[i].size() * log2(buckets[i].size());
            
            std::ostringstream oss;
            oss << "Sorting bucket " << i << " (" << buckets[i].size() << " elements)\n"
                << "Comparisons: ~" << stats.comparisons << "\n"
                << "Operations: " << stats.swaps;
            stepCallback(buckets, oss.str());
            sleepForVisualization(state.speed);
        }
    }

    int index = 0;
    for (const auto& bucket : buckets) {
        for (float val : bucket) {
            arr[index++] = val;
            stats.swaps++;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    stats.timeTaken = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    
    stepCallback(buckets, "Final sorted array constructed\nTime: " + 
                         std::to_string(stats.timeTaken) + "ms\n" +
                         "Comparisons: ~" + std::to_string(stats.comparisons) + "\n" +
                         "Operations: " + std::to_string(stats.swaps) + "\n" +
                         "Time Complexity: " + stats.timeComplexity + "\n" +
                         "Space Complexity: " + stats.spaceComplexity);
    sleepForVisualization(state.speed);

    state.array.clear();
    for (float val : arr) {
        state.array.push_back(static_cast<int>(val * 100));
    }
    state.currentStep = "Final Sorted Array: [" + formatFloatArray(arr) + "]";
    state.highlightedIndex = -1;
    state.secondaryIndex = -1;
}

void radixSort(std::vector<int>& arr,
              std::function<void(const std::vector<int>&, int, int, const std::string&)> callback) {
    if (arr.empty()) return;

    SortStats stats;
    stats.timeComplexity = "O(nk)";
    stats.spaceComplexity = "O(n + k)";
    auto startTime = std::chrono::high_resolution_clock::now();

    int maxNum = *std::max_element(arr.begin(), arr.end());
    for (int exp = 1; maxNum / exp > 0; exp *= 10) {
        std::vector<int> output(arr.size());
        std::vector<int> count(10, 0);
        
        for (size_t i = 0; i < arr.size(); i++) {
            int digit = (arr[i] / exp) % 10;
            count[digit]++;
            stats.comparisons++;
            callback(arr, i, -1, "Counting digit " + std::to_string(digit) + 
                                " at position " + std::to_string(i) + "\n" +
                                "Comparisons: " + std::to_string(stats.comparisons));
        }
        
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
            callback(arr, -1, -1, "Calculating cumulative count for digit " + 
                                  std::to_string(i) + "\nComparisons: " + 
                                  std::to_string(stats.comparisons));
        }
        
        for (int i = arr.size() - 1; i >= 0; i--) {
            int digit = (arr[i] / exp) % 10;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
            stats.swaps++;
            callback(arr, i, -1, "Placing " + std::to_string(arr[i]) + 
                                " in output array\nOperations: " + 
                                std::to_string(stats.swaps));
        }
        
        for (size_t i = 0; i < arr.size(); i++) {
            arr[i] = output[i];
            callback(arr, i, -1, "Updating array with sorted digits (exp=" + 
                                std::to_string(exp) + ")\nOperations: " + 
                                std::to_string(stats.swaps));
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    stats.timeTaken = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    callback(arr, -1, -1, "Radix Sort Complete!\nTime: " + 
                         std::to_string(stats.timeTaken) + "ms\n" +
                         "Comparisons: " + std::to_string(stats.comparisons) + "\n" +
                         "Operations: " + std::to_string(stats.swaps) + "\n" +
                         "Time Complexity: " + stats.timeComplexity + "\n" +
                         "Space Complexity: " + stats.spaceComplexity);
}

void countingSort(std::vector<int>& arr,
                std::function<void(const std::vector<int>&, int, int, const std::string&)> callback,
                VisualizerState& state) {
    if (arr.empty()) return;

    SortStats stats;
    stats.timeComplexity = "O(n + k)";
    stats.spaceComplexity = "O(n + k)";
    auto startTime = std::chrono::high_resolution_clock::now();

    int max = *std::max_element(arr.begin(), arr.end());
    std::vector<int> count(max + 1, 0);
    std::vector<int> output(arr.size());

    for (size_t i = 0; i < arr.size(); i++) {
        count[arr[i]]++;
        stats.comparisons++;
        state.countArray = count;
        callback(arr, i, -1, "Counting occurrence of " + std::to_string(arr[i]) + 
                            "\nComparisons: " + std::to_string(stats.comparisons));
    }

    for (size_t i = 1; i < count.size(); i++) {
        count[i] += count[i - 1];
        state.countArray = count;
        callback(arr, -1, -1, "Calculating cumulative count for value " + 
                              std::to_string(i) + "\nComparisons: " + 
                              std::to_string(stats.comparisons));
    }

    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
        stats.swaps++;
        state.countArray = count;
        callback(arr, i, -1, "Placing " + std::to_string(arr[i]) + 
                            " in output array\nOperations: " + 
                            std::to_string(stats.swaps));
    }

    for (size_t i = 0; i < arr.size(); i++) {
        arr[i] = output[i];
        callback(arr, i, -1, "Updating main array with sorted elements\nOperations: " + 
                            std::to_string(stats.swaps));
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    stats.timeTaken = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    callback(arr, -1, -1, "Counting Sort Complete!\nTime: " + 
                         std::to_string(stats.timeTaken) + "ms\n" +
                         "Comparisons: " + std::to_string(stats.comparisons) + "\n" +
                         "Operations: " + std::to_string(stats.swaps) + "\n" +
                         "Time Complexity: " + stats.timeComplexity + "\n" +
                         "Space Complexity: " + stats.spaceComplexity);
}