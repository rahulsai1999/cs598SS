# !/bin/bash

cp -r workloads ycsb-0.17.0

cd ycsb-0.17.0

./bin/ycsb.sh load basic -P workloads/workloada > outWorkloadA.txt
./bin/ycsb.sh run basic -P workloads/workloada > outWorkloadRunA.txt

./bin/ycsb.sh load basic -P workloads/workloadb > outWorkloadB.txt
./bin/ycsb.sh run basic -P workloads/workloadb > outWorkloadRunB.txt

./bin/ycsb.sh load basic -P workloads/workloadc > outWorkloadC.txt
./bin/ycsb.sh run basic -P workloads/workloadc > outWorkloadRunC.txt

./bin/ycsb.sh load basic -P workloads/workloadd > outWorkloadD.txt
./bin/ycsb.sh run basic -P workloads/workloadd > outWorkloadRunD.txt

./bin/ycsb.sh load basic -P workloads/workloade > outWorkloadE.txt
./bin/ycsb.sh run basic -P workloads/workloade > outWorkloadRunE.txt

./bin/ycsb.sh load basic -P workloads/workloadf > outWorkloadF.txt
./bin/ycsb.sh run basic -P workloads/workloadf > outWorkloadRunF.txt

# Copy the results to ycsb folder
cp outWorkloadA.txt ../
cp outWorkloadRunA.txt ../
cp outWorkloadB.txt ../
cp outWorkloadRunB.txt ../
cp outWorkloadC.txt ../
cp outWorkloadRunC.txt ../
cp outWorkloadD.txt ../
cp outWorkloadRunD.txt ../
cp outWorkloadE.txt ../
cp outWorkloadRunE.txt ../
cp outWorkloadF.txt ../
cp outWorkloadRunF.txt ../