all:
	clang++ -O0 -g -std=c++11 -Isrc src/test/test2.cpp src/test/test.cpp -Wall -Wextra -fmessage-length=0 -o test
#	clang++ -std=c++11 -isystem../threadpool/include -Iinclude -pthread src/test/main.cpp -o test
