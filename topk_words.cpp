#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <thread>
#include <vector>
#include "generator.h"
#include "words_thread.h"

const size_t TOPK = 10;

void print_topk(std::ostream& stream, const Counter& counter, const size_t topk) {
  std::vector<Counter::const_iterator> words;

  words.reserve(counter.size());

  for (auto it = std::cbegin(counter); it != std::cend(counter); ++it) {
    words.push_back(it);
  }

  std::partial_sort(
      std::begin(words),
      std::begin(words) + std::min(topk, words.size()),
      std::end(words),
      [](auto lhs, auto &rhs) {
        return lhs->second > rhs->second;
      });

  std::for_each(
      std::begin(words),
      std::begin(words) + std::min(topk, words.size()),
      [&stream](const Counter::const_iterator &pair) {
        stream << std::setw(4) << pair->second << " " << pair->first << '\n';
      });
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: topk_words [FILES...]\n";

    return EXIT_FAILURE;
  }

  auto threadCount = std::thread::hardware_concurrency();
  std::vector<WordsThread> wordsThreads;
  size_t maxWordsCount;
  size_t maxWordsPerThread;
  size_t modWordsPerThread;
  unsigned int wordThreadsBeginIndex;
  unsigned int wordThreadsEndIndex;

  if (!threadCount) {
    threadCount = 4;
  }

  auto start = std::chrono::high_resolution_clock::now();
  auto fileCount = static_cast<unsigned int>(argc - 1);
  Counter totalCounter;

  wordsThreads.reserve(threadCount * fileCount);
  Generator<int> generator(0, threadCount - 1);

  for (auto i = 1; i < argc; ++i) {
    std::ifstream input{argv[i]};

    if (!input.is_open()) {
      std::cerr << "Failed to open file " << argv[i] << '\n';

      return EXIT_FAILURE;
    }

    input.seekg(0, std::ios::end);
    std::streampos size = input.tellg();
    input.seekg(0, std::ios::beg);

    maxWordsCount = static_cast<size_t>(size / 2);

    maxWordsPerThread = maxWordsCount / threadCount;
    modWordsPerThread = maxWordsCount % threadCount;
    wordThreadsBeginIndex = static_cast<unsigned int>(i - 1) * threadCount;
    wordThreadsEndIndex = static_cast<unsigned int>(i) * threadCount;

    for (auto t = wordThreadsBeginIndex; t < wordThreadsEndIndex; t++) {
      wordsThreads.emplace_back();
      wordsThreads[t].reserve(maxWordsPerThread);
    }

    wordsThreads[wordThreadsBeginIndex].reserve(maxWordsPerThread + modWordsPerThread);

    std::for_each(std::istream_iterator<std::string>(input),
                  std::istream_iterator<std::string>(),
                  [&](const std::string &s) {
                    auto threadIndex = wordThreadsBeginIndex + generator.next();

                    wordsThreads[threadIndex].push_back(s);
                  });

    for (auto t = wordThreadsBeginIndex; t < wordThreadsEndIndex; t++) {
      wordsThreads[t].join();

      const auto& counter = wordsThreads[t].getCounter();

      for (const auto& pair : counter) {
        totalCounter[pair.first] += pair.second;
      }
    }

    generator.reset();
  }

  print_topk(std::cout, totalCounter, TOPK);

  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Elapsed time is " << elapsed_ms.count() << " us\n";
}