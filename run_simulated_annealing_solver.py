import os
from multiprocessing.pool import ThreadPool
from config import executable_path, time_limit, results_path, instance_paths


def bruteforce_intervention(job_number):
    for base_path in instance_paths:
        for change_count, n_count in [(0.005, 0.0025), (0.015, 0.0025), (0.01, 0.01), (0.005, 0.1), (0.02, 0.015)]:
            for initial_tempreature in [50, 100, 200, 400, 800, 1600, 3200]:
                res_path = os.path.join(results_path,
                                        f'{base_path[-2]}_{job_number}_{change_count}_{n_count}_{initial_tempreature}')
                params = f"./challengeRTE -b {base_path} -t {time_limit} -p {job_number} -o " \
                    f"{res_path} -p1 {change_count} -p2 {n_count} -p3 {initial_tempreature} -out {os.path.join(results_path, 'simulated_annealing.csv')}"

                print(
                    f"Running instance #{job_number} with base_path {base_path}, time limit {time_limit} minutes. Params: "
                    f"change rate {change_count}, neighbors count {n_count} \nParams: {params}\n")

                os.system(
                    params)


if __name__ == "__main__":
    print("Running simulated annealing solver.....")

    results_path = os.path.join(results_path,
                                "results_simulated_annealing")
    os.makedirs(results_path, exist_ok=True)

    pool = ThreadPool(16)
    pool.map(bruteforce_intervention, list(range(1, 16)))
    pool.join()
