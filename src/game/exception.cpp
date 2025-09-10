#include "exception.h"

Game::Exception::Exception(const char* msg) {
    message = msg;
};

const char* Game::Exception::what() const {
    return message;
};