#/bin/sh

echo "Architecture: $(uname -m)"
echo "Processor: $(lscpu | grep 'Model name' | sed -r 's/Model name:\s{1,}//g')" 
echo "Number of Processors: $(grep -c ^processor /proc/cpuinfo)"
echo "Ubuntu Version: $(lsb_release -d | awk '{print $3}')"
echo "Kernel Version: $(uname -r)"
echo "RAM Capacity: $(free -h | awk '/Mem:/ {print $2}')"
