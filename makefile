main:polynomial.o calculator.o polynomial.h calculator.h main.o
	g++ polynomial.o calculator.o main.o polynomial.h calculator.h -o main
main.o:main.cpp
	g++ -c main.cpp -o main.o
polynomial.o:polynomial.cpp
	g++ -c polynomial.cpp -o polynomial.o
calculator.o:calculator.cpp
	g++ -c calculator.cpp -o calculator.o

clean:
	rm *.o
	rm main
