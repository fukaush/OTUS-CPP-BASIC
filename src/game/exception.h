#pragma once

namespace Game {
    class Exception {
        private:
            const char* message;
        public:
            Exception(const char* msg);
            const char* what() const;
    };
}