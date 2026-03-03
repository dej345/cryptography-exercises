CXX = g++
CXXFLAGS = -std=c++17 -Wall -I lib -I lib/mono -I lib/poly -I lib/xor

SRCS = main.cpp \
       lib/cipher.cpp \
       lib/mono/monoa.cpp \
       lib/poly/polya.cpp \
       lib/xor/xorc.cpp

TARGET = cipher

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
