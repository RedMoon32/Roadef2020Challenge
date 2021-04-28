import os
from multiprocessing.pool import ThreadPool

time_limit = 15


def bruteforce_intervention(job_number):
    for base_path in ["./A_set/A_", "./B_set_rounded/B_"]:
            for change_count in [0.0025, 0.005, 0.01, 0.015, 0.02, 0.025]:

                res_path = os.path.join('./results/',
                                        f'{base_path[-2]}_{job_number}_{change_count}')
                params = f"./cmake-build-debug/challengeRTE -b {base_path} -t {time_limit} -p {job_number} -o " \
                         f"{res_path} -p1 {change_count} -out res.csv"

                print(
                    f"Running instance #{job_number} with base_path {base_path}, time limit {time_limit} minutes. Params: "
                    f"change rate {change_count} \nParams: {params}\n")

                os.system(
                    params)


if __name__ == "__main__":
    try:
        os.mkdir("./results")
    except:
        pass

    pool = ThreadPool(2)
    pool.map(bruteforce_intervention, list(range(1, 3)))
    pool.join()
