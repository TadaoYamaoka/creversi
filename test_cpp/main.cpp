#include <iostream>

#include "board.hpp"
#include "state.hpp"
#include "hand.hpp"
#include "utils.hpp"
#include "bit_manipulations.hpp"

void test() {
	board bd = board::initial_board();

	std::cout << utils::to_s<true>(bd) << std::endl;

	bool is_pass = true;
	//for (uint64_t bits = ~bit_manipulations::stones(bd); bits; bits = _blsr_u64(bits)) {
	for (uint64_t bits = state::mobility_pos(bd); bits; bits = _blsr_u64(bits)) {
		int pos = bit_manipulations::bit_to_pos(_blsi_u64(bits));
		board next = state::move(bd, pos);
		if (next.player() != bd.opponent()) {
			std::cout << pos << "\t" << to_s(pos) << std::endl;
			is_pass = false;
		}
	}

	hand h = to_hand("c4");
	bd = state::move(bd, h);

	std::cout << utils::to_s<false>(bd) << std::endl;
}

int main() {
	utils::init_all();
	test();
}