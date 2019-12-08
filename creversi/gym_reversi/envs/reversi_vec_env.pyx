import creversi
from creversi.gym_reversi.envs import ReversiEnv

class ReversiVecEnv:
	metadata = {'render.modes': ['line']}

	def __init__(self, int num_envs):
		self.num_envs = num_envs
		self.envs = [ReversiEnv() for _ in range(num_envs)]
		
	def reset(self):
		for env in self.envs:
			env.reset()

	def render(self, mode='line'):
		return [env.render(mode='line') for env in self.envs]

	def step(self, list moves):
		cdef int i
		cdef int move

		rewards = []
		dones = []

		for i, move in enumerate(moves):
			_, reward, done, _ = self.envs[i].step(move)
			rewards.append(reward)
			dones.append(done)
			if done:
				self.envs[i].reset()
		
		return rewards, dones
