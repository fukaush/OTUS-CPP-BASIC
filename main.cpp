#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <thread>
#include <atomic>
#include <string>
#include "CRC32.hpp"
#include "IO.hpp"

/// @brief Переписывает последние 4 байта значением value
void replaceLastFourBytes(std::vector<char> &data, uint32_t value) {
    std::copy_n(reinterpret_cast<const char *>(&value), 4, data.end() - 4);
}

std::atomic<bool> hasResult = false;

/**
 * @brief Формирует новый вектор с тем же CRC32, добавляя в конец оригинального
 * строку injection и дополнительные 4 байта
 * @details При формировании нового вектора последние 4 байта не несут полезной
 * нагрузки и подбираются таким образом, чтобы CRC32 нового и оригинального
 * вектора совпадали
 * @param injection произвольная строка, которая будет добавлена после данных
 * @param expectedChecksum ожидаемая чексумма
 * @param begin начало
 * @param end конец
 * оригинального вектора
 * @return новый вектор
 */
std::vector<char> hack(std::vector<char> injection, const uint32_t expectedChecksum, const uint32_t begin, const uint32_t end) {
    const uint32_t prev = ~expectedChecksum;

    for (uint32_t i = begin; i < end; i++) {
        if (hasResult) {
            break;
        }

        replaceLastFourBytes(injection, i);

        if (expectedChecksum == crc32(&injection[0], injection.size(), prev)) {
          hasResult = true;

          return std::move(injection);
        }
    }

    throw std::logic_error("Can't hack");
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "<input file> <output file> <count threads = 1>\n";

        return 1;
    }

    size_t countThreads = 1;

    if (argc == 4) {
        countThreads = std::clamp(atoi(argv[3]), 1, std::numeric_limits<int>::max());
    }

    try {
        std::vector<char> data = readFromFile(argv[1]);
        const uint32_t expectedChecksum = crc32(data.data(), data.size());
        const uint32_t max = std::numeric_limits<uint32_t>::max();
        const uint32_t maxPerThread = static_cast<uint32_t>(trunc(max / countThreads));
        const std::string additionalData = "He-he-he";
        std::vector<char> additionalDataVector(additionalData.begin(), additionalData.end());
        std::vector<std::thread> threads;

        additionalDataVector.resize(additionalDataVector.size() + 4);
        threads.reserve(static_cast<size_t>(countThreads));

        std::vector<char> result;

        for (uint32_t i = 0; i < countThreads; i++) {
            uint32_t rangeBegin = i * maxPerThread;
            uint32_t rangeEnd = (i + 1) * maxPerThread;

            if (i == countThreads - 1) {
              rangeEnd += max % countThreads;
            }

            threads.emplace_back([&result, additionalDataVector, expectedChecksum, rangeBegin, rangeEnd]() {
                try {
                    result = hack(additionalDataVector, expectedChecksum, rangeBegin, rangeEnd);
                } catch (...) {}
            });
        }

        for (auto &thread: threads) {
            thread.join();
        }

        if (result.empty()) {
          throw std::logic_error("Can't hack");
        }

        std::cout << "Success" << std::endl;

        std::copy(result.begin(), result.end(), std::back_inserter(data));
        writeToFile(argv[2], data);
    } catch (std::exception &ex) {
        std::cerr << ex.what() << '\n';

        return 2;
    }
    return 0;
}
