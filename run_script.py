import os
from multiprocessing.pool import ThreadPool

time_limit = 15


def bruteforce_intervention(job_number):
    for base_path in ["./A_set/A_", "./B_set_rounded/B_"]:
        for population_size in [25, 50, 100, 200]:
            for mutation_rate in [0.01, 0.03, 0.05, 0.07]:
                for crossover_rate in [0.85, 1]:

                    res_path = os.path.join('./results/',
                                            f'{base_path[-2]}_{job_number}_{population_size}_{mutation_rate}_{crossover_rate}')
                    params = f"./challengeRTE -b {base_path} -t {time_limit} -p {job_number} -o " \
                             f"{res_path} -p1 {population_size} -p2 {mutation_rate} -p3 {crossover_rate} -out res.csv"

                    print(
                        f"Running instance #{job_number} with base_path {base_path}, time limit {time_limit} minutes. Params: "
                        f"pop_size {population_size}, mutation_rate {mutation_rate}, crossover_rate {crossover_rate}\nParams: {params}\n")

                    os.system(
                        params)


if __name__ == "__main__":
    try:
        os.mkdir("./results")
    except:
        pass

    pool = ThreadPool(16)
    pool.map(bruteforce_intervention, list(range(1, 16)))
    pool.join()
