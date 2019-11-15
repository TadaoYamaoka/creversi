#include "state.hpp"

#include <cassert>

#include <iomanip>
#include <iostream>

#include "bit_manipulations.hpp"

#include "immintrin.h"

namespace bm = bit_manipulations;

namespace state {


	bool is_gameover(const board &bd) {
		return mobility_pos(bd) == 0 &&
			mobility_pos(board::reverse_board(bd)) == 0;
	}

	std::vector<board> next_states(const board & bd) {
		std::vector<board> res;
		res.reserve(16);
		bool is_pass = true;
		for (uint64_t bits = mobility_pos(bd); bits; bits = _blsr_u64(bits)) {
			int pos = bm::bit_to_pos(_blsi_u64(bits));
			res.emplace_back(move(bd, pos / 8, pos % 8));
			is_pass = false;
		}
		if (is_pass) res.emplace_back(board::reverse_board(bd));
		return res;
	}

	bool next_states(const board & bd, std::vector<board> &res) {
		res.clear();
		bool is_pass = true;
		for (uint64_t bits = ~bm::stones(bd); bits; bits = _blsr_u64(bits)) {
			int pos = bm::bit_to_pos(_blsi_u64(bits));
			board next = move(bd, pos);
			if (next.player() != bd.opponent()) {
				res.emplace_back(next);
				is_pass = false;
			}
		}
		if (is_pass) res.emplace_back(board::reverse_board(bd));
		return is_pass;
	}

	std::vector<board> next_states(const board & bd, uint64_t bits) {
		std::vector<board> res;
		res.reserve(16);
		if (bits == 0) res.emplace_back(board::reverse_board(bd));
		for (; bits; bits = _blsr_u64(bits)) {
			int pos = bm::bit_to_pos(_blsi_u64(bits));
			res.emplace_back(move(bd, pos));
		}
		return res;
	}

	bool next_states(const board & bd, uint64_t bits, std::vector<board> &res) {
		res.clear();
		bool is_pass = bits == 0;
		if (is_pass) res.emplace_back(board::reverse_board(bd));
		for (; bits; bits = _blsr_u64(bits)) {
			int pos = bm::bit_to_pos(_blsi_u64(bits));
			res.emplace_back(move(bd, pos));
		}
		return is_pass;
	}

} // namespace state
