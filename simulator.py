import numpy as np

def random_environment(size = (50,50, 20), density = 0.20):
    X, Y, Z = size

    grid = np.zeros(size, dtype=np.uint8)

    obstacles = np.random.random(size) < density

    grid[obstacles] = 1
    return grid


grid = random_environment(density=0.20)

def corridor_environment(size=(50, 50, 20)):
    X, Y, Z = size
    grid = np.zeros(size, dtype=np.uint8)

    # Side walls
    grid[15:20, :, :] = 1
    grid[30:35, :, :] = 1

    return grid


def cluttered_environment(size = (50, 50, 20), num_clusters=40):
    X, Y, Z = size
    grid = np.zeros(size, dtype=np.uint8)

    for _ in range(num_clusters):
        x = np.random.randint(0, X)
        y = np.random.randint(0, Y)
        z = np.random.randint(0, Z)

        sx = np.random.randint(2, 6)
        sy = np.random.randint(2, 6)
        sz = np.random.randint(2, 5)

        grid[
            x:min(x+sx, X),
            y:min(y+sy, Y),
            z:min(z+sz, Z)
        ] = 1

    return grid
    
def generate_environments(environment_type, density = 0.20):
    if(environment_type == "random"):
        return random_environment(density=density)

    elif environment_type == "corridor":
        return corridor_environment()

    elif environment_type == "cluttered":
        return cluttered_environment()

    else:
        raise ValueError(f"Unknown environment type: {environment_type}")


