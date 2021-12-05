import numpy as np

class Processing:
    def __init__(self) -> None:
        pass

    # TODO: concertar o signal gain
    def signal_gain(self, g):
        N = 64
        S = 794
        for c in range(0, N):
            for l in range(0, S):
                y = 100 + 1 / 20 * (l + 1) * np.sqrt(l + 1)
                index = l + (c * S)
                g[index] = g[index] * y
        return g

    # normalize array to 0-1
    def normalize(self, array):
        return (array - array.min()) / (array.max() - array.min())

    def lines_to_array(self, lines):
        return np.array(lines, dtype=np.float32)

    def read_file(self, file_name):
        with open(file_name, 'rb') as f:
            return f.read().decode('utf-8')