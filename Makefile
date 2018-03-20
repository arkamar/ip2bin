CFLAGS += -Wall -pedantic -O2
TARGET  = ip2bin

all: $(TARGET)

clean:
	$(RM) *.o $(TARGET)
