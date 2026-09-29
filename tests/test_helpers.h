#pragma once

#include <vector>
#include <string>
#include <algorithm>

inline bool appears_before(const std::vector<std::string>& order,
                            const std::string& before,
                            const std::string& after) {
    auto pos_before = std::find(order.begin(), order.end(), before);
    auto pos_after = std::find(order.begin(), order.end(), after);
    return pos_before < pos_after;
}