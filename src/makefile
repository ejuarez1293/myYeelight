# Makefile for programs in driver1 && driver2
# ***************************************************************
PROGRAMS =  Discover_Bulbs TCPSocket

# ***************************************************************
# Parameters to control Makefile operation

CC = g++
CFLAGS = -Wall -lnsl -std=c++0x -pthread


# ***************************************************************
# Entry to bring the package up to date
all: $(PROGRAMS)


# ***************************************************************
# C compilations
#sender.o: sender.cpp
#	$(CC) $(CFLAGS) -c sender.cpp

TCPSocket.o: TCPSocket.cpp
	$(CC) $(CFLAGS) -c TCPSocket.cpp

Discover_Bulbs.o: Discover_Bulbs.cpp
	$(CC) $(CFLAGS) -c Discover_Bulbs.cpp

main.o: main.cpp 
	$(CC) $(CFLAGS) -c main.cpp

# ***************************************************************
# Executable programs
#sender: sender.o
#	$(CC) $(CFLAGS) -o sender sender.o

TCPSocket: TCPSocket.o 
	$(CC) $(CFLAGS) -o TCPSocket TCPSocket.o


Discover_Bulbs: Discover_Bulbs.o main.o 
	$(CC) $(CFLAGS) -o Discover_Bulbs Discover_Bulbs.o main.o



clean:
	rm *.o