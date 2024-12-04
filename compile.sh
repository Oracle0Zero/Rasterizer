g++ -c main.cpp Camera.cpp Transform.cpp;
g++ main.o Camera.o Transform.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system;
./sfml-app;
