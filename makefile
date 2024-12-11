vpath %.cpp src
vpath %.h include

main.exec: main.o Utils.o Camera.o Point.o Triangle.o Plane.o Model.o Transform.o Cube.cpp Instance.o Scene.o
	g++ $^ -o $@ -fopenmp -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU -lSOIL -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi;

main.o: main.cpp Camera.h Triangle.h Plane.h Model.h Instance.h Scene.h Utils.h
	g++ -c -fopenmp $<

Utils.o: Utils.cpp Utils.h
	g++ -c -fopenmp $<
	
Camera.o: Camera.cpp Camera.h Transform.h
	g++ -c -fopenmp $<

Point.o: Point.cpp Point.h
	g++ -c -fopenmp $<

Triangle.o: Triangle.cpp Point.cpp Triangle.h Point.h
	g++ -c -fopenmp $<

Plane.o: Plane.cpp Plane.h
	g++ -c -fopenmp $<

Model.o: Model.cpp Model.h Cube.h
	g++ -c -fopenmp $<

Transform.o: Transform.cpp Transform.h
	g++ -c -fopenmp $<

Cube.o: Cube.cpp Cube.h
	g++ -c -fopenmp $<

Instance.o: Instance.cpp Transform.cpp Point.cpp Instance.h Transform.h Cube.h
	g++ -c -fopenmp $<	

Scene.o: Scene.cpp Instance.cpp Scene.h Instance.h
	g++ -c -fopenmp $<