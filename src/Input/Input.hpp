#pragma once
#include <unordered_map>

namespace Silicon {
    class Input {
    private:
        static inline std::unordered_map<int, bool> keys;
    public:
        static void setKey(int key, bool state) { keys[key] = state; }
        static bool isKeyDown(int key) { return keys[key]; }
    };
}
