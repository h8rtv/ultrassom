import numpy as np

class Processing:
    def __init__(self) -> None:
        pass

    def signal_gain(self, g):
        # N = 64
        # S = 794
        # for c in range(0, N):
        #     for l in range(0, S):
        #         y = 100 + 1 / 20 * (l) * np.sqrt(l)
        #         index = l + S * c
        #         g[index] = g[index] * y
        return g

    def lines_to_array(self, lines):
        return np.array(lines, dtype=np.float32)

    def read_file(self, file_name):
        with open(file_name, 'rb') as f:
            return f.read().decode('utf-8')