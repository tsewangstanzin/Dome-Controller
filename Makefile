CC = gcc

CLIBS = `pkg-config libxml-2.0 --cflags --libs `

retrieve_attribute_value: pwi2control.c 
	$(CC) pwi2control.c -o pwi2control.bin -lpthread $(CLIBS) 

clean: 
	rm -f *.o *.bin
