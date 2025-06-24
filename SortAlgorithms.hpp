#pragma once

#ifndef SORT_ALGORITHMS_HPP
#define SORT_ALGORITHMS_HPP

#include <vector>
#include <functional>
#include <string>
#include "Visualizer.hpp"

void bubbleSort(
    std::vector<int>& arr,
    std::function<void(const std::vector<int>&, int, int, const std::string&)> callback
);

void insertionSort(
    std::vector<int>& arr,
    std::function<void(const std::vector<int>&, int, int, const std::string&)> callback
);

void quickSort(
    std::vector<int>& arr,
    VisualizerState& state,
    int low,
    int high,
    bool isInitialCall = true
);

void bucketSort(
    std::vector<float>& arr,
    SortStats& stats,
    VisualizerState& state,
    std::function<void(const std::vector<std::vector<float>>&, const std::string&)> stepCallback
);

void radixSort(
    std::vector<int>& arr,
    std::function<void(const std::vector<int>&, int, int, const std::string&)> callback
);

void countingSort(
    std::vector<int>& arr,
    std::function<void(const std::vector<int>&, int, int, const std::string&)> callback,
    VisualizerState& state
);

std::string formatFloatArray(const std::vector<float>& arr);

#endif // SORT_ALGORITHMS_HPP