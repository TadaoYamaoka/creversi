from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool

import numpy as np
cimport numpy as np

PIECES = [NONE, BLACK, WHITE] = range(3)

cdef extern from "utils.hpp" namespace "utils":
	void init_all()

init_all()

cdef extern from "creversi.h":
	cdef cppclass __Board:
		__Board() except +
		__Board(const string &line, const bool is_black_turn) except +
		__Board(const __Board& board, const bool is_black_turn) except +
		void set(const string &line, const bool is_black_turn)
		void reset()
		string dump()
		void move(const int move)
		void move_from_str(const string& str)
		void move_pass()
		bool is_game_over()
		int stone_sum()
		int diff_num()
		int puttable_diff()
		bool turn()
		string to_s_ffo()
		int piece(const int sq)
		void piece_planes(char* mem)

cdef class Board:
	cdef __Board __board

	def __cinit__(self, str line=None, bool is_black_turn=True):
		cdef string line_b
		if line:
			line_b = line.encode('ascii')
			self.__board = __Board(line_b, is_black_turn)
		else:
			self.__board = __Board()

	def __copy__(self):
		return Board(board=self)

	def copy(self):
		return Board(board=self)

	def set_line(self, str line, bool is_black_turn=True):
		cdef string line_b = line.encode('ascii')
		self.__board.set(line_b, is_black_turn)

	def reset(self):
		self.__board.reset()

	def __repr__(self):
		return self.__board.dump().decode('ascii')

	def move(self, int move):
		self.__board.move(move)

	def move_from_str(self, str str):
		cdef string str_b = str.encode('ascii')
		self.__board.move_from_str(str_b)

	def move_pass(self):
		self.__board.move_pass()

	@property
	def legal_moves(self):
		return LegalMoveList(self)

	def is_game_over(self):
		return self.__board.is_game_over()

	def stone_sum(self):
		return self.__board.stone_sum()

	def diff_num(self):
		return self.__board.diff_num()

	def puttable_diff(self):
		return self.__board.puttable_diff()

	@property
	def turn(self):
		return self.__board.turn()

	def to_line(self):
		return self.__board.to_s_ffo().decode('ascii')

	def piece(self, int sq):
		return self.__board.piece(sq)

	def piece_planes(self, np.ndarray features):
		return self.__board.piece_planes(features.data)

cdef extern from "creversi.h":
	cdef cppclass __LegalMoveList:
		__LegalMoveList() except +
		__LegalMoveList(const __Board& board) except +
		bool end()
		int next()
		int size()

	string __move_to_str(const int move)

cdef class LegalMoveList:
	cdef __LegalMoveList __ml

	def __cinit__(self, Board board):
		self.__ml = __LegalMoveList(board.__board)

	def __iter__(self):
		return self

	def __next__(self):
		if self.__ml.end():
			raise StopIteration()
		return self.__ml.next()

	def __len__(self):
		return self.__ml.size()

def move_to_str(int move):
	return __move_to_str(move).decode('ascii')
