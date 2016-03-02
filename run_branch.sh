#!/bin/bash
echo "Running!"
rm tests.csv
./UrApHMP 5 1 1.0 0.2 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_1_02.txt
echo "5_1_02...ok!"
./UrApHMP 5 1 1.0 0.4 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_1_04.txt
echo "5_1_04...ok!"
./UrApHMP 5 1 1.0 0.6 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_1_06.txt
echo "5_1_06...ok!"
./UrApHMP 5 1 1.0 0.8 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_1_08.txt
echo "5_1_08...ok!"
./UrApHMP 5 1 1.0 1.0 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_1_10.txt
echo "5_1_10...ok!"
./UrApHMP 5 2 1.0 0.2 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_2_02.txt
echo "5_2_02...ok!"
./UrApHMP 5 2 1.0 0.4 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_2_04.txt
echo "5_2_04...ok!"
./UrApHMP 5 2 1.0 0.6 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_2_06.txt
echo "5_2_06...ok!"
./UrApHMP 5 2 1.0 0.8 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_2_08.txt
echo "5_2_08...ok!"
./UrApHMP 5 2 1.0 1.0 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_2_10.txt
echo "5_2_10...ok!"
./UrApHMP 5 3 1.0 0.2 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_3_02.txt
echo "5_3_02...ok!"
./UrApHMP 5 3 1.0 0.4 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_3_04.txt
echo "5_3_04...ok!"
./UrApHMP 5 3 1.0 0.6 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_3_06.txt
echo "5_3_06...ok!"
./UrApHMP 5 3 1.0 0.8 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_3_08.txt
echo "5_3_08...ok!"
./UrApHMP 5 3 1.0 1.0 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_3_10.txt
echo "5_3_10...ok!"
./UrApHMP 5 5 1.0 0.2 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_5_02.txt
echo "5_5_02...ok!"
./UrApHMP 5 5 1.0 0.4 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_5_04.txt
echo "5_5_04...ok!"
./UrApHMP 5 5 1.0 0.6 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_5_06.txt
echo "5_5_06...ok!"
./UrApHMP 5 5 1.0 0.8 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_5_08.txt
echo "5_5_08...ok!"
./UrApHMP 5 5 1.0 1.0 1.0 < instances/CAB25.txt > test_results/model2/CAB25/CAB25_5_5_10.txt
echo "5_5_10...ok!"
# ./UrApHMP 6 1 3.0 0.75 2.0 < AP40.txt > test_results/laptop_executed/AP40/AP40_6_1.txt
# echo "6_1...ok!"
# ./UrApHMP 6 2 3.0 0.75 2.0 < AP40.txt > test_results/laptop_executed/AP40/AP40_6_2.txt
# echo "6_2...ok!"
# ./UrApHMP 6 3 3.0 0.75 2.0 < AP40.txt > test_results/laptop_executed/AP40/AP40_6_3.txt
# echo "6_3...ok!"
# ./UrApHMP 6 4 3.0 0.75 2.0 < AP40.txt > test_results/laptop_executed/AP40/AP40_6_4.txt
# echo "6_4...ok!"
# ./UrApHMP 6 5 3.0 0.75 2.0 < AP40.txt > test_results/laptop_executed/AP40/AP40_6_5.txt
# echo "6_5...ok!"
# ./UrApHMP 6 6 3.0 0.75 2.0 < AP40.txt > test_results/laptop_executed/AP40/AP40_6_6.txt
# echo "6_6...ok!"
