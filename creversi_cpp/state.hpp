#pragma once
#include <array>
#include <vector>

#include "board.hpp"
#include "bit_manipulations.hpp"

namespace state {

	uint64_t mobility_pos(const board &);
	__m128i mobility_pos(const double_board &);
	int mobility_count(const board &);
	std::array<int, 2> mobility_count(const double_board &);
	bool is_gameover(const board &);
	board move_rev(const board &, int);
	board move_rev(const board &, int, int);
	board move(const board &, int);
	board move(const board &, int, int);
	std::vector<board> next_states(const board &);
	bool next_states(const board &, std::vector<board> &res);

	template <size_t N>
	int next_states(const board & bd, std::array<board, N> &res) {
		int index = 0;
		bool is_pass = true;
		for (uint64_t bits = ~bit_manipulations::stones(bd); bits; bits = _blsr_u64(bits)) {
			int pos = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			board next = move(bd, pos);
			if (next.player() != bd.opponent()) {
				res[index++] = next;
				is_pass = false;
			}
		}
		if (is_pass) res[index++] = board::reverse_board(bd);
		return is_pass ? 0 : index;
	}
	std::vector<board> next_states(const board &, uint64_t bits);
	bool next_states(const board &, uint64_t bits, std::vector<board> &res);

} // namespace state
