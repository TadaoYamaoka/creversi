#include "hand.hpp"
#include <stdexcept>

hand hand_from_diff(const board &old_b, const board &new_b) {
	uint64_t old_bits = old_b.player() | old_b.opponent();
	uint64_t new_bits = new_b.player() | new_b.opponent();
	for (hand i = 0; i < 64; ++i)
		if (((old_bits >> i) & 1) == 0 && ((new_bits >> i) & 1) == 1)
			return i;
	return PASS;
}

hand to_hand(const std::string &hand_str) {
	if (hand_str == "ps" || hand_str == "pass" || hand_str == "pa" || hand_str == "PS" || hand_str == "PASS" || hand_str == "PA") return PASS;
	if (hand_str.size() != 2) throw std::range_error("invalid hand_str");
	int j;
	if ('a' <= hand_str[0] && hand_str[0] <= 'h')
		j = hand_str[0] - 'a';
	else if ('A' <= hand_str[0] && hand_str[0] <= 'H')
		j = hand_str[0] - 'A';
	else
		throw std::range_error("invalid hand_str");
	if (hand_str[1] < '1' || '8' < hand_str[1]) throw std::range_error("invalid hand_str");
	int i = hand_str[1] - '1';
	return i * 8 + j;
}

std::string to_s(const hand h) {
	if (h == PASS) return "pass";
	std::string res;
	res += (h % 8) + 'a';
	res += (h / 8) + '1';
	return res;
}

std::string to_S(const hand h) {
	if (h == PASS) return "pass";
	std::string res;
	res += (h % 8) + 'A';
	res += (h / 8) + '1';
	return res;
}
