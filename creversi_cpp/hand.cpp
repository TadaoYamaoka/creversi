#include "hand.hpp"

hand hand_from_diff(const board &old_b, const board &new_b) {
	uint64_t old_bits = old_b.player() | old_b.opponent();
	uint64_t new_bits = new_b.player() | new_b.opponent();
	for (hand i = 0; i < 64; ++i)
		if (((old_bits >> i) & 1) == 0 && ((new_bits >> i) & 1) == 1)
			return i;
	return PASS;
}

hand to_hand(const std::string &hand_str) {
	if (hand_str == "ps") return PASS;
	if (hand_str.size() != 2) throw "invalid hand_str";
	if (hand_str[0] < 'a' || 'h' < hand_str[0]) throw "invalid hand_str";
	if (hand_str[1] < '1' || '8' < hand_str[1]) throw "invalid hand_str";
	int i = hand_str[1] - '1';
	int j = hand_str[0] - 'a';
	if (std::min(i, j) < 0 || std::max(i, j) >= 8) throw "invalid hand_str";
	return i * 8 + j;
}

std::string to_s(const hand h) {
	if (h == PASS) return "ps";
	std::string res;
	res += (h % 8) + 'a';
	res += (h / 8) + '1';
	return res;
}
