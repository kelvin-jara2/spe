import time


for i in range(1000):
    th = time.time()
    x = 0
    for i in range(100000):
        x = x + 1
    print(time.time() - th)