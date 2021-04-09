import os

if __name__ == "__main__":
    try:
        os.mkdir("../results")
    except:
        pass

    for base_path in ["../A_set/A_", "../B_set_rounded/B_"]:
        for instance_num in range(1, 16):
            for population_size in [50, 100, 200]:
                for mutation_rate in [0.03, 0.06, 0.1]:
                    for crossover_rate in [0.75, 1]:
                        for time_limit in [2, 5, 10]:
                            print(
                                f"Running instance #{instance_num} with base_path {base_path}, timie limit {time_limit} minutes. Params: "
                                f"pop_size {population_size}, mutation_rate {mutation_rate}, crossover_rate {crossover_rate}")

                            params = f"./challengeRTE -b {base_path} -t {time_limit} -p {instance_num} -o {os.path.join('../results/', f'{base_path[-2]}_{instance_num}_{population_size}_{mutation_rate}_{crossover_rate}_{time_limit}.out')} -p1 {population_size} -p2 {mutation_rate} -p3 {crossover_rate} -out res.csv"
                            os.system(
                                params)
