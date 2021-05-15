import os
from multiprocessing.pool import ThreadPool
from itertools import product

time_limit = 15


def bruteforce_intervention(fs):
    job_number, base_path = fs

    for _ in range(10):
        for solver, config in [("sa", [0.02, 0.015, 1600.0]), ("hc", [0.005, 0.0025, -1.0]), ("ga", [50.0, 0.05, 1.0])]:
            p1, p2, p3 = config
            res_path = os.path.join("./results/",
                                    f'{base_path[-2]}_{job_number}_{p1}_{p2}_{p3}')

            params = f"./challengeRTE -b {base_path} -t {time_limit} -p {job_number} -o " \
                     f"{res_path} -p1 {p1} -p2 {p2} -p3 {p3}  -solver {solver} -out res.csv"

            print(
                f"Running instance #{job_number} with base_path {base_path}, time limit {time_limit} minutes. Params: {params}\n")

            os.system(
                params)


if __name__ == "__main__":
    try:
        os.mkdir("./results")
    except:
        pass

    pool = ThreadPool(24)
    pool.map(bruteforce_intervention, list(product(list(range(1, 16)), ["./A_set/A_", "./B_set_rounded/B_"])))
    pool.join()
