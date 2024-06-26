import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import time

def step(x, sigma):
    cov = np.diag(sigma)
    dt = 0.1
    x = x + dt * np.random.multivariate_normal([0, 0], cov, 20)
    return x

def draw(t):
    plt.clf()
    plt.suptitle(f"t={t}")
    ax = plt.gca()
    ax.scatter(x[:10,0], x[:10,1], c="black", s = 10)
    ax.plot(x[10:20,0], x[10:20,1], "r^")
    for i in range(10):
        c = patches.Circle(xy=x[i], radius=0.05, fc='grey', alpha=0.1)
        ax.add_patch(c)
        c = patches.Circle(xy=x[i+10], radius=0.05, fc='pink', alpha=0.2)
        ax.add_patch(c)

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_aspect('equal')
    plt.pause(0.01)

def draw_initial(fig, ax, t):
    text = fig.suptitle(f"t={t}")
    scat = ax.scatter(x[:10,0], x[:10,1], c="black", s = 10)
    line, = ax.plot(x[10:20,0], x[10:20,1], "r^")
    cs = []
    for i in range(10):
        c = patches.Circle(xy=x[i], radius=0.05, fc='grey', alpha=0.1)
        ax.add_patch(c)
        cs.append(c)
    for i in range(10):
        c = patches.Circle(xy=x[i+10], radius=0.05, fc='pink', alpha=0.2)
        ax.add_patch(c)
        cs.append(c)

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_aspect('equal')

    return text, scat, line, cs
 
def draw_succesive(text, scat, line, cs, t):
    text.set_text(f"t={t}")
    scat.set_offsets(x[:10,:])
    line.set_data(x[10:20,0], x[10:20,1])
    for i in range(20):
        cs[i].center = x[i]
    plt.pause(0.01)


if __name__ == '__main__':
    n_iter = 100
    sigma = np.full(2, 0.1 / 3)

    x = np.zeros((20,2))
    fig, ax = plt.subplots()
    
    start = time.time()
    for t in range(n_iter):
        x = step(x,sigma)
        draw(t) 
    elapsed_time1 = time.time() - start
    plt.close()

    fig, ax = plt.subplots()
    x = np.zeros((20,2))
    text, scat, line, cs = draw_initial(fig, ax, t)    
    start2 = time.time()
    for t in range(n_iter):
        x = step(x,sigma)
        draw_succesive(text, scat, line, cs, t)
    elapsed_time2 = time.time() - start2
    print(f"rewrite_all: {elapsed_time1}, rewrite_diff: {elapsed_time2}")
