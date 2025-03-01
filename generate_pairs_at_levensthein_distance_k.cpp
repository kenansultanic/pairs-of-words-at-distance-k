#include <fstream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>
#include <iomanip>


bool isLevenshteinDistanceK(const std::string& s1, const std::string& s2, int k) {
    int m = s1.size();
    int n = s2.size();

    if (std::abs(m - n) > k) {
        return false;
    }

    if (m > n) return isLevenshteinDistanceK(s2, s1, k);

    std::vector<int> prev(n + 1, 0), curr(n + 1, k+1);

    for (int j = 0; j <= n; ++j)
        prev[j] = j;

    for (int i = 1; i <= m; ++i) {
        curr[0] = i;

        for (int j = std::max(1, i - k); j <= std::min(n, i + k); ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = 1 + std::min({prev[j],
                                        curr[j - 1],
                                        prev[j - 1]
                                       });
            }
        }

        std::swap(prev, curr);
    }

    return prev[n] == k;
}

std::vector<std::string> readFileInChunksWithWords(const std::string& filePath, size_t bufferSize) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

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
            while (stream >> word)
                words.push_back(word);

            leftover = currentChunk.substr(lastSpace + 1);
        } else {
            leftover += currentChunk;
        }
    }

    if (!leftover.empty()) {
        std::istringstream stream(leftover);
        std::string word;
        while (stream >> word)
            words.push_back(word);
    }

    file.close();

    return words;
}

std::vector<std::pair<std::string, std::string>> findRandomPairsWithLevenshteinDistanceK(
        const std::string& filePath, int k, size_t numPairs, size_t bufferSize) {

    std::vector<std::string> words = readFileInChunksWithWords(filePath, bufferSize);
    std::vector<std::pair<std::string, std::string>> result;
    std::set<std::pair<size_t, size_t>> seenPairs;

    while (result.size() < numPairs) {
        size_t i = std::rand() % words.size();
        size_t j = std::rand() % words.size();

        if (i == j || seenPairs.count({i, j}) || seenPairs.count({j, i}))
            continue;

        seenPairs.insert({i, j});
        if (isLevenshteinDistanceK(words[i], words[j], k)) {
            result.emplace_back(words[i], words[j]);
        }
    }

    return result;
}


int main() {
    const std::string filePath = "dictionary.txt";
    const size_t bufferSize = 4096;
    const size_t numPairs = 100;
    const int distance = 7;

    try {
        auto pairs = findRandomPairsWithLevenshteinDistanceK(filePath, distance, numPairs, bufferSize);
        std::cout << std::setw(20) << std::left << "Word 1"
                  << std::setw(20) << std::left << "Word 2"
                  << '\n'
                  << std::string(44, '-')
                  << std::endl;

        for (const auto& pair : pairs) {
            std::cout << std::setw(20) << std::left << pair.first
                      << std::setw(20) << std::left << pair.second
                      << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
