import gym
from gym import spaces
import numpy as np
import creversi
from collections import defaultdict

class ReversiEnv(gym.Env):
	metadata = {'render.modes': ['human', 'svg', 'ansi', 'line']}

	def __init__(self):
		super().__init__()

		self.board = creversi.Board()

		self.observation_space = spaces.Box(0, 2, (8, 8), dtype=np.uint8)

		# actionはmoveを直接受け付ける
		# sample()は非合法手も含む
		self.action_space = gym.spaces.Discrete(65)

	def reset(self, line=None):
		if line:
			self.board.set_line(line)
		else:
			self.board.reset()

		return self.board

	def render(self, str mode='human'):
		if mode == 'svg':
			return self.board.to_svg()
		elif mode == 'ansi':
			print(self.board)
		elif mode == 'line':
			print(self.board.to_line())
		else:
			return self.board

	def step(self, int move):
		assert self.board.is_legal(move)

		self.board.move(move)

		done = self.board.is_game_over()
		if done:
			reward = 1.0 if self.board.diff_num() < 0 else (-1.0 if self.board.diff_num() > 0 else 0.0)
		else:
			reward = 0.0

		return self.board, reward, done, None
