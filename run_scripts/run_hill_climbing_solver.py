import os
from multiprocessing.pool import ThreadPool
from config import executable_path, time_limit, results_path, instance_paths


def bruteforce_intervention(job_number):
    for base_path in instance_paths:
        for change_count in [0.0025, 0.005, 0.01, 0.015, 0.02, 0.025]:
            for n_count in [0.0025, 0.005, 0.01, 0.015, 0.3, 0.5, 0.1]:

                res_path = os.path.join(results_path,
                                        f'{base_path[-2]}_{job_number}_{change_count}_{n_count}')
                params = f"./challengeRTE -b {base_path} -t {time_limit} -p {job_number} -o " \
                    f"{res_path} -p1 {change_count} -p2 {n_count} -out {os.path.join(results_path, 'hill_climbing.csv')}"

                print(
                    f"Running instance #{job_number} with base_path {base_path}, time limit {time_limit} minutes. Params: "
                    f"change rate {change_count}, neighbors count {n_count} \nParams: {params}\n")

                os.system(
                    params)


if __name__ == "__main__":
    print("Running hil climbing solver.....")

    results_path = os.path.join(results_path, "results_hill_climbing")
    os.makedirs(results_path, exist_ok=True)

    pool = ThreadPool(16)
    pool.map(bruteforce_intervention, list(range(1, 16)))
    pool.join()
