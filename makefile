all:
	g++ src/lib/cge.cpp src/lib/vector2.cpp src/lib/cge_map.cpp src/main.cpp -std=c++14 -pthread -lcurses -o graphics