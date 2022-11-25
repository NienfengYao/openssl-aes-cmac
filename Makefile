objects = main.o utils.o

all: $(objects)
	gcc -o signtool $(objects) -lcrypto

# Define a pattern rule that compiles every .c file into a .o file
%.o : %.c
	gcc -c $< -o $@

clean:
	rm *.o
