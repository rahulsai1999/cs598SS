# !/bin/bash

# Install Java and Maven
sudo apt-get update
sudo apt-get install -y openjdk-8-jdk maven

# Check Java version
java -version

# Setup YCSB
curl -O --location https://github.com/brianfrankcooper/YCSB/releases/download/0.17.0/ycsb-0.17.0.tar.gz
tar xfvz ycsb-0.17.0.tar.gz
rm ycsb-0.17.0.tar.gz
