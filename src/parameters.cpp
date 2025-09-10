#include "parameters.h"

std::map<std::string, std::string> parseParameters(int count, char* argv[]) {
    std::map<std::string, std::string> map;
    std::string lastKey = "";

    for (int i = 1; i < count; i++) {
       std::string str = argv[i];
       bool isParameter = str.at(0) == '-' && str.length() >= 2;

       if (isParameter) {
           std::string key = str.substr(1);

           map[key] = "";
           lastKey = key;
       } else if (lastKey.length()) {
           map[lastKey].append(" " + str);
       }
    }

    return map;
}