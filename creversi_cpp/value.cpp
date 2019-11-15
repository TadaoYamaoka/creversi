#include "value.hpp"

#include "bit_manipulations.hpp"
#include "state.hpp"
#include "utils.hpp"

namespace value {

	int diff_num(const board &bd) {
		int bn = (int)_popcnt64(bd.player());
		int wn = (int)_popcnt64(bd.opponent());
		return bn - wn;
	}

	int puttable_diff(const board &bd) {
		int b = state::mobility_count(bd);
		int w = state::mobility_count(board::reverse_board(bd));
		return b - w;
	}

} // namespace value
