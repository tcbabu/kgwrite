KULINA=/usr
#CC	=g++ -pthread
CC	=cc -pthread
kgsld	: kgsld.o
	 $(CC)  -o kgsld kgsld.o  -I$(KULINA)/include $(KULINA)/lib/libkulina.a $(KULINA)/lib/libgm.a -L/usr/X11R6/lib -lX11 -lXext -lm -lpthread -lz -lbz2 -lGL
kgsld.o	: kgsld.c 
	 $(CC) -c kgsld.c
clean	:  
	   rm -f *.o kgsld
install	: kgsld
	 cp kgsld $(KULINA)/bin/kgwrite
