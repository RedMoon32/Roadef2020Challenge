for num in $(seq -w 01 15);
do
	/mnt/c/Users/rinat/Projects/RoadefThesis/cmake-build-debug/CheckerBenchmark -t 15 -p "/mnt/c/Users/rinat/Projects/RoadefThesis/A_Set/A_$num.json" -o "/mnt/c/Users/rinat/Projects/RoadefThesis/outputs/output_$num.txt" -name -s 1000;
done
