CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o random.o Trip.o Pop.o city.o
	$(CC) main.o random.o Trip.o Pop.o city.o -o main.exe

main.o : main.cpp Pop.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

Trip.o : Trip.cpp Trip.h
	$(CC) -c Trip.cpp -o Trip.o $(CFLAGS)

Pop.o : Pop.cpp Pop.h
	$(CC) -c Pop.cpp -o Pop.o $(CFLAGS)

city.o : city.cpp city.h
	$(CC) -c city.cpp -o city.o $(CFLAGS)

clean :
	rm *.o main.exe seed.out