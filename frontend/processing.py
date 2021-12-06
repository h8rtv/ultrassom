import numpy as np

class Processing:
    def __init__(self) -> None:
        pass

    def signal_gain(self, g):
        N = 64
        S = 794
        g = np.reshape(g, (S, N))
        for c in range(0, N):
            for l in range(0, S):
                y = 100 + (1 / 20) * (c + 1) * np.sqrt(c + 1)
                g[l, c] = g[l, c] * y
        return g.flatten()

    # normalize array to 0-1
    def normalize(self, array):
        return (array - array.min()) / (array.max() - array.min())

    def lines_to_array(self, lines):
        return np.array(lines, dtype=np.float32)

    def read_file(self, file_name):
        with open(file_name, 'rb') as f:
            return f.read().decode('utf-8')