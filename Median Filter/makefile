CXX=g++
CFLAGS  += -std=c++11
all: image imagestack

image: image.cpp
	$(CXX) $(CFLAGS) image.cpp -o image

imagestack: imagestack.cpp
	$(CXX) $(CFLAGS) imagestack.cpp -o imagestack

clean: 
	rm -f image imagestack

