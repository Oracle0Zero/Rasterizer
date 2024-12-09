g++ -c main.cpp Camera.cpp Cube.cpp Instance.cpp Model.cpp Plane.cpp Point.cpp Scene.cpp Transform.cpp Triangle.cpp;
g++ main.o Camera.o Cube.o Instance.o Model.o Plane.o Point.o Scene.o Transform.o Triangle.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system;
./sfml-app;
