CC ?= gcc

TARGET = my-userapp

all: $(TARGET)

$(TARGET): my_userapp.o
	$(CC) $(LDFLAGS) -o $@ $^

my_userapp.o: my_userapp.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
