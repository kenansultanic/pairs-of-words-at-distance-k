# Pairs of Words at Distance K

## Overview

This project contains two implementations for finding pairs of words that are exactly K distance apart, based on different distance metrics. The words are read from a large dataset efficiently, and optimized algorithms are used to compute the required word pairs.

## Implementations

### **1. Graph-Based Distance (`generate_pairs_at_distance_k.cpp`)**
- **Distance Definition:** The distance between two words is defined based on graph traversal, where:
  - Two words are considered neighbors if they differ by exactly one insertion, deletion, or substitution of a single letter.
  - The distance between two words is then measured as the number of edges in the graph.
- **Graph Representation:**
  - A graph is built where words are connected if they satisfy the above neighbor condition.
  - The graph is optimized by bucketing words by length and reducing unnecessary comparisons.
- **Graph Search (DFS-Based):**
  - A **Depth-First Search (DFS)** is used to find words that are exactly K edges away from a given word.
  - This approach efficiently finds words within a specified distance without computing all-pairs distances.
- **Optimizations:**
  - **Efficient Graph Construction:** Words are grouped by length to minimize comparisons.
  - **Optimized File Reading:** The file is processed in chunks to handle large datasets efficiently.

### **2. Edit Distance-Based Approach (`generate_pairs_at_levenshtein_distance_k.cpp`)**
- **Distance Definition:** The distance between two words is calculated using the **Levenshtein edit distance**, which counts the minimum number of insertions, deletions, or substitutions required to transform one word into another.
- **Algorithm Used:**
  - A **dynamic programming-based** Levenshtein distance function is used.
  - The implementation is **optimized** by restricting the DP table to the relevant diagonal region (bounded by `k`), reducing the time complexity from **O(m × n)** to **O(k × min(m, n))**.
- **Pair Selection:**
  - Word pairs are randomly selected and checked using the optimized Levenshtein distance function.
  - A **set** is used to avoid duplicate pairs.
- **Optimizations:**
  - **Efficient File Reading:** Words are read in **chunks** for better performance with large files.
  - **Reduced Computation:** The Levenshtein distance algorithm is optimized for small `k` values.

## **Datasets: `dictionary.txt`, `words.txt`**
- The file `dictionary.txt` is an English dictionary containing approximately **500,000 words**, each word separated by a new line.
- The file `words.txt` is a much shorter version of `dictionary.txt`, containing only a few hundred words.
- The files are read in chunks to optimize performance and avoid memory issues.

## **How to Use**

1. **Compile the code** using a C++17 or later compiler:
   ```sh
   g++ -std=c++17 generate_pairs_at_distance_k.cpp -o generate_pairs_at_distance_k
   g++ -std=c++17 generate_pairs_at_levenshtein_distance_k.cpp -o generate_pairs_at_levenshtein_distance_k
   ```

2. **Run the executables** with the word file:
   ```sh
   ./generate_pairs_at_distance_k
   ./generate_pairs_at_levenshtein_distance_k
   ```

## **Performance Considerations**
- The **graph-based approach** is more efficient for small distances (`k ≤ 6`), as DFS only explores relevant neighbors, and smaller datasets.
- The **Levenshtein distance approach** is useful for flexible string distance comparisons, though it requires computing distances explicitly for each pair.

---

This project efficiently finds word pairs at a specified distance using two distinct approaches, demonstrating different algorithmic trade-offs.
