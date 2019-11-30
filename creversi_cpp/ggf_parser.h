#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <iostream>

#include "hand.hpp"

enum ResultFlag {
	NONE = 0, RESIGNED, TIME_OUT, MUTUAL_SCORE
};

class __GgfParser {
public:
	std::vector<std::vector<std::string>> names;
	std::vector<std::vector<float>> ratings;
	std::vector<int> results;
	std::vector<int> result_flags;
	std::vector<std::vector<int>> moves;
	std::vector<std::vector<float>> evaluations;

	void parse_file(const std::string& path) {
		std::ifstream ifs(path);
		if (ifs) {
			std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
			parse_str(str);
		}
		else
			throw std::ios_base::failure("open failed");
	}

	void parse_str(const std::string& str) {
		names.clear();
		ratings.clear();
		results.clear();
		result_flags.clear();
		moves.clear();
		evaluations.clear();

		int game_n = 0;
		int line = 1;
		size_t line_pos = 0;
		bool inside = false;
		for (size_t i = 0; i < str.size(); ++i) {
			// skip white spaces
			if (str[i] == ' ' || str[i] == '\r')
				continue;
			else if (str[i] == '\n') {
				++line;
				line_pos = i;
				continue;
			}

			if (str.compare(i, 2, "B[") == 0 || str.compare(i, 2, "W[") == 0) {
				i += 2;
				size_t pos = i;
				for (; i < str.size() && str[i] != ']'; ++i)
					;
				size_t n = i - pos;
				std::string s = str.substr(pos, n);
				auto n2 = s.find_first_of('/');
				if (n2 != std::string::npos) {
					n = n2;
					auto n3 = s.find_first_of('/', n2 + 1);
					if (n3 == std::string::npos)
						n3 = s.size();
					if (n3 - n2 - 1 == 0)
						evaluations[game_n].emplace_back(std::numeric_limits<float>::quiet_NaN());
					else
						evaluations[game_n].emplace_back(std::stof(s.substr(n2 + 1, n3 - n2 - 1)));
				}
				try {
					moves[game_n].emplace_back(to_hand(str.substr(pos, n)));
					continue;
				}
				catch (std::runtime_error) {

				}
			}
			else if (str.compare(i, 2, "(;") == 0) {
				names.emplace_back(std::vector<std::string>{"", ""});
				ratings.emplace_back(std::vector<float>{0.0f, 0.0f});
				results.emplace_back();
				result_flags.emplace_back();
				moves.emplace_back();
				evaluations.emplace_back();
				inside = true;
				++i;
				continue;
			}
			else if (str.compare(i, 2, ";)") == 0) {
				++game_n;
				inside = false;
				++i;
				continue;
			}
			else if (str.compare(i, 11, "GM[Othello]") == 0) {
				i += 10;
				continue;
			}
			else if (str.compare(i, 3, "PC[") == 0 || str.compare(i, 3, "DT[") == 0 || str.compare(i, 3, "TI[") == 0 || str.compare(i, 3, "TB[") == 0 || str.compare(i, 3, "TW[") == 0) {
				for (i += 3; i < str.size() && str[i] != ']'; ++i)
					;
				continue;
			}
			else if (str.compare(i, 3, "PB[") == 0) {
				i += 3;
				size_t pos = i;
				for (; i < str.size() && str[i] != ']'; ++i)
					;
				names[game_n][0] = str.substr(pos, i - pos);
				continue;
			}
			else if (str.compare(i, 3, "PW[") == 0) {
				i += 3;
				size_t pos = i;
				for (; i < str.size() && str[i] != ']'; ++i)
					;
				names[game_n][1] = str.substr(pos, i - pos);
				continue;
			}
			else if (str.compare(i, 3, "RB[") == 0) {
				i += 3;
				size_t pos = i;
				for (; i < str.size() && str[i] != ']'; ++i)
					;
				ratings[game_n][0] = std::stof(str.substr(pos, i - pos));
				continue;
			}
			else if (str.compare(i, 3, "RW[") == 0) {
				i += 3;
				size_t pos = i;
				for (; i < str.size() && str[i] != ']'; ++i)
					;
				ratings[game_n][1] = std::stof(str.substr(pos, i - pos));
				continue;
			}
			else if (str.compare(i, 5, "TY[8]") == 0) {
				i += 4;
				continue;
			}
			else if (str.compare(i, 3, "RE[") == 0) {
				i += 3;
				size_t pos = i;
				for (; i < str.size() && str[i] != ']'; ++i)
					;
				std::string s = str.substr(pos, i - pos);
				auto n = s.find_first_of(':');
				if (n != std::string::npos) {
					std::string flag = s.substr(n + 1, s.size() - n);
					if (flag.compare("r"))
						result_flags[game_n] = (int)RESIGNED;
					else if (flag.compare("t"))
						result_flags[game_n] = (int)TIME_OUT;
					else if (flag.compare("s"))
						result_flags[game_n] = (int)MUTUAL_SCORE;
				}
				else
					n = i - pos;
				results[game_n] = int(std::stof(str.substr(pos, n)));
				continue;
			}
			else if (str.compare(i, 79, "BO[8 -------- -------- -------- ---O*--- ---*O--- -------- -------- -------- *]") == 0) {
				i += 78;
				continue;
			}

			//std::cerr << "unexpected string at [" + std::to_string(line) + ":" + std::to_string(i - line_pos + 1) + "]" << std::endl;
			if (inside) {
				for (; i < str.size(); ++i) {
					if (str.compare(i, 2, ";)") == 0) {
						names.pop_back();
						ratings.pop_back();
						results.pop_back();
						result_flags.pop_back();
						moves.pop_back();
						evaluations.pop_back();
						inside = false;
						++i;
						break;
					}
				}
			}
			else {
				for (; i < str.size(); ++i) {
					if (str.compare(i, 2, "(;") == 0) {
						--i;
						break;
					}
				}
			}
		}
	}

	int game_num() const {
		return (int)names.size();
	}
};