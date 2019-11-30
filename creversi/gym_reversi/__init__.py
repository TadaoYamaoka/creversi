from gym.envs.registration import register

register(
    id='Reversi-v0',
    entry_point='creversi.gym_reversi.envs:ReversiEnv',
)