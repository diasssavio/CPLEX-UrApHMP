#!/bin/bash
echo "Running!"
./UrApHMP 3 2 1.0 0.8 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_3_2_08.ps
echo "3_2_08...ok!"
./UrApHMP 3 2 1.0 1.0 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_3_2_10.ps
echo "3_2_10...ok!"
./UrApHMP 3 3 1.0 0.8 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_3_3_08.ps
echo "3_3_08...ok!"
./UrApHMP 3 3 1.0 1.0 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_3_3_10.ps
echo "3_3_10...ok!"
./UrApHMP 4 2 1.0 0.4 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_4_2_04.ps
echo "4_2_04...ok!"
./UrApHMP 4 2 1.0 0.6 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_4_2_06.ps
echo "4_2_06...ok!"
./UrApHMP 4 2 1.0 0.8 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_4_2_08.ps
echo "4_2_08...ok!"
./UrApHMP 4 2 1.0 1.0 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_4_2_10.ps
echo "4_2_10...ok!"
./UrApHMP 5 1 1.0 1.0 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_5_1_10.ps
echo "5_1_10...ok!"
./UrApHMP 5 2 1.0 0.8 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_5_2_08.ps
echo "5_2_08...ok!"
./UrApHMP 5 2 1.0 1.0 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_5_2_10.ps
echo "5_2_10...ok!"
./UrApHMP 5 3 1.0 0.8 1.0 < CAB25.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/CAB25_5_3_08.ps
echo "5_3_08...ok!"
./UrApHMP 3 2 3.0 0.75 2.0 < AP40.txt > trash.txt
dot -Kfdp -n -Tps out.dot -o plots_2/AP40_3_2.ps
echo "3_2...ok!"