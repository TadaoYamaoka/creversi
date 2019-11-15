#pragma once

#include "board.hpp"

using hand = int;

constexpr hand PASS = -1;
constexpr hand NOMOVE = -2;

hand hand_from_diff(const board &old_b, const board &new_b);
hand to_hand(const std::string &);
std::string to_s(const hand);
