#pragma once

#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include "tolower.h"

using Counter = std::unordered_map<std::string, std::size_t>;

class WordsThread {
private:
  std::thread thread;
  std::mutex mutex;
  std::condition_variable conditionVariable;
  std::vector<std::string> wordsQueue;
  Counter wordsCounter;
  std::vector<Counter::const_iterator> words;
public:
  WordsThread() {
    thread = std::thread([&]() {
      std::unique_lock<std::mutex> locker(mutex);
      size_t lastProcessedIndex = 0;
      bool isRunning = true;
      std::string word;

      while (isRunning) {
        //& - this?
        conditionVariable.wait(locker, [&]() {
          return !wordsQueue.empty();
        });

        size_t length = wordsQueue.size();

        for (; lastProcessedIndex < length; lastProcessedIndex++) {
          word = wordsQueue[lastProcessedIndex];

          word = tolower(word);

          ++wordsCounter[word];
        }

        if (word.empty()) {
          isRunning = false;

          wordsCounter.erase(word);
        }
      }
    });
  }

  ~WordsThread() {
    join();
  };

  WordsThread(const WordsThread&) = delete;

  WordsThread& operator=(const WordsThread&) = delete;

  WordsThread(WordsThread&& other) noexcept {
    wordsQueue = std::move(other.wordsQueue);
    thread = std::move(other.thread);
  }

  WordsThread& operator=(WordsThread&& other) noexcept {
    if (this != &other) {
      wordsQueue = std::move(other.wordsQueue);
      thread = std::move(other.thread);
    }

    return *this;
  }

  void reserve(size_t count) {
    wordsQueue.reserve(count);
    wordsCounter.reserve(count);
    words.reserve(count);
  }

  void push_back(const std::string& word) {
    wordsQueue.push_back(word);
    conditionVariable.notify_one();
  }

  void join() {
    if (!thread.joinable()) {
      return;
    }

    push_back("");
    thread.join();
  }

  const Counter& getCounter() const {
    return wordsCounter;
  }
};