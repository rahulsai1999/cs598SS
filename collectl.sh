# !/bin/bash

curl -O --location https://cytranet.dl.sourceforge.net/project/collectl/collectl/collectl-4.3.1/collectl-4.3.1.src.tar.gz
tar xfvz collectl-4.3.1.src.tar.gz
rm collectl-4.3.1.src.tar.gz
cd collectl-4.3.1
./INSTALL
cd ..
rm -rf collectl-4.3.1