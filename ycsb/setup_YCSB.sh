# !/bin/bash

# Install Java and Maven
sudo apt-get update
sudo apt-get install -y openjdk-8-jdk maven

# Check Java version
java -version

# Setup YCSB
# git clone https://github.com/brianfrankcooper/YCSB.git
# cd YCSB
# mvn -pl site.ycsb:rocksdb-binding -am clean package

curl -O --location https://github.com/brianfrankcooper/YCSB/releases/download/0.17.0/ycsb-0.17.0.tar.gz
tar xfvz ycsb-0.17.0.tar.gz
rm ycsb-0.17.0.tar.gz

# cd ycsb-0.17.0
# ./bin/ycsb.sh load basic -P workloads/workloada > outWorkload.txt
# ./bin/ycsb.sh run basic -P workloads/workloada > outWorkloadRun.txt