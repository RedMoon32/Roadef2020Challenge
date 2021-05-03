import os
from multiprocessing.pool import ThreadPool
from config import executable_path, time_limit, results_path, instance_paths


def bruteforce_intervention(job_number):
    for base_path in instance_paths:
        for population_size in [25, 50, 100, 200]:
            for mutation_rate in [0.01, 0.03, 0.05, 0.07]:
                for crossover_rate in [0.85, 1]:

                    res_path = os.path.join(results_path,
                                            f'{base_path[-2]}_{job_number}_{population_size}_{mutation_rate}_{crossover_rate}')

                    params = f"{executable_path} -b {base_path} -t {time_limit} -p {job_number} -o " \
                             f"{res_path} -p1 {population_size} -p2 {mutation_rate} -p3 {crossover_rate} -out {os.path.join(results_path, 'genetic_results.csv')}"

                    print(
                        f"Running instance #{job_number} with base_path {base_path}, time limit {time_limit} minutes. Params: "
                        f"pop_size {population_size}, mutation_rate {mutation_rate}, crossover_rate {crossover_rate}\nParams: {params}\n")

                    os.system(
                        params)


if __name__ == "__main__":
    print("Running genetic solver.....")

    os.makedirs(os.path.join(results_path, "results_genetic"), exist_ok=True)

    pool = ThreadPool(16)
    pool.map(bruteforce_intervention, list(range(1, 16)))
    pool.join()
