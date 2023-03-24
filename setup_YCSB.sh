# !/bin/bash

# Install Java and Maven
sudo apt-get update
sudo apt-get install -y openjdk-8-jdk maven

# Check Java version
java -version

# Setup YCSB
git clone https://github.com/brianfrankcooper/YCSB.git
cd YCSB
mvn -pl site.ycsb:rocksdb-binding -am clean package

# Setup Redis
sudo apt-get install -y redis-server
sudo service redis-server start

# Check Redis status
sudo service redis-server status