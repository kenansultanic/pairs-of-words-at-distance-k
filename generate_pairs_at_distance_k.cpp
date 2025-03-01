#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <queue>
#include <set>


std::vector<std::string> readFileInChunks(const std::string& fileName, int bufferSize) {
    std::ifstream file(fileName);

    if (!file.is_open())
        throw std::runtime_error("Unable to open file");

    std::string buffer;
    buffer.reserve(bufferSize);

    std::string leftover;
    std::vector<std::string> words;

    while (file.good()) {
        char chunk[bufferSize + 1] = {0};
        file.read(chunk, bufferSize);
        std::streamsize bytesRead = file.gcount();

        std::string currentChunk = leftover + std::string(chunk, bytesRead);

        size_t lastSpace = currentChunk.find_last_of('\n');
        if (lastSpace != std::string::npos) {
            std::istringstream stream(currentChunk.substr(0, lastSpace));
            std::string word;
            while (stream >> word) {
                words.push_back(word);
            }

            leftover = currentChunk.substr(lastSpace + 1);
        } else {
            leftover += currentChunk;
        }
    }

    if (!leftover.empty()) {
        std::istringstream stream(leftover);
        std::string word;
        while (stream >> word) {
            words.push_back(word);
        }
    }

    file.close();

    return words;
}

bool isSingleInsertionOrDeletion(const std::string& word1, const std::string& word2) {

    if (word1.size() > word2.size())
        return isSingleInsertionOrDeletion(word2, word1);

    if (word2.size() != word1.size() + 1)
        return false;

    size_t i = 0, j = 0;
    bool foundDifference = false;

    while (i < word1.size() && j < word2.size()) {
        if (word1[i] != word2[j]) {
            if (foundDifference) return false;
            foundDifference = true;
            j++;
        } else {
            i++;
            j++;
        }
    }

    return true;
}


bool isSingleSubstitution(const std::string& word1, const std::string& word2) {
    if (word1.size() != word2.size()) return false;

    int differences = 0;
    for (size_t i = 0; i < word1.size(); i++) {
        if (word1[i] != word2[i]) {
            differences++;
            if (differences > 1) return false;
        }
    }

    return differences == 1;
}

std::unordered_map<std::string, std::vector<std::string>> buildGraph(const std::vector<std::string>& words) {

    std::unordered_map<int, std::vector<std::string>> wordBuckets;
    std::unordered_map<std::string, std::vector<std::string>> graph;

    for (const std::string& word : words)
        wordBuckets[word.size()].push_back(word);

    for (const auto& [length, bucket] : wordBuckets) {

        for (size_t i = 0; i < bucket.size(); i++) {
            for (size_t j = i + 1; j < bucket.size(); j++) {
                if (isSingleSubstitution(bucket[i], bucket[j])) {
                    graph[bucket[i]].push_back(bucket[j]);
                    graph[bucket[j]].push_back(bucket[i]);
                }
            }
        }

        for (int adjLength : {length - 1, length + 1}) {
            if (wordBuckets.find(adjLength) != wordBuckets.end()) {
                const std::vector<std::string>& adjBucket = wordBuckets[adjLength];
                for (const auto& i : bucket) {
                    for (const auto& j : adjBucket) {
                        if (isSingleInsertionOrDeletion(i, j)) {
                            graph[i].push_back(j);
                            graph[j].push_back(i);
                        }
                    }
                }
            }
        }
    }

    return graph;
}

std::vector<std::string> getNeighborsAtDistanceK(
        const std::unordered_map<std::string, std::vector<std::string>>& graph,
        const std::string& startNode,
        int k) {

    std::unordered_set<std::string> visited;
    std::queue<std::pair<std::string, int>> toVisit;
    std::vector<std::string> result;

    toVisit.emplace(startNode, 0);
    visited.insert(startNode);

    while (!toVisit.empty()) {
        auto [currentNode, currentDepth] = toVisit.front();
        toVisit.pop();

        if (currentDepth == k)
            result.push_back(currentNode);

        if (currentDepth >= k || graph.find(currentNode) == graph.end())
            continue;

        for (const std::string& neighbor : graph.at(currentNode)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                toVisit.emplace(neighbor, currentDepth + 1);
            }
        }
    }

    return result;
}

std::vector<std::pair<std::string, std::string>> findRandomPairs(
        const std::unordered_map<std::string, std::vector<std::string>>& graph,
        int numPairs,
        int k) {

    std::set<std::pair<std::string, std::string>> seenPairs;
    std::vector<std::string> nodes;
    std::vector<std::pair<std::string, std::string>> result;
    result.reserve(numPairs);

    for (const auto& node : graph)
        nodes.push_back(node.first);

    std::shuffle(nodes.begin(), nodes.end(), std::mt19937(std::random_device()()));

    for (const std::string& node : nodes) {
        std::vector<std::string> neighborsAtDistanceK = getNeighborsAtDistanceK(graph, node, k);
        for (const std::string& neighbor : neighborsAtDistanceK) {
            if (result.size() >= numPairs) break;
            if (seenPairs.count({node, neighbor}) || seenPairs.count({neighbor, node}))
                continue;
            result.emplace_back(node, neighbor);
            seenPairs.emplace(node, neighbor);
        }
        if (result.size() >= numPairs) break;
    }

    return result;
}

int main() {

    int k = 3;
    int numPairs = 100;

    std::vector<std::string> words = readFileInChunks("words1.txt", 4096);
    auto graph = buildGraph(words);
    auto pairs = findRandomPairs(graph, numPairs, k);

    std::cout << std::setw(30) << std::left << "Word 1"
              << std::setw(30) << std::left << "Word 2"
              << std::endl
              << std::string(50, '-')
              << std::endl;

    for (const auto& pair: pairs) {
        std::cout << std::setw(30) << std::left << pair.first
                  << std::setw(30) << std::left << pair.second
                  << std::endl;
    }

    return 0;
}
