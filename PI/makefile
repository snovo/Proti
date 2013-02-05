gGui:

pnm.o: pnm.c pnm.h
	gcc -Wall -O2 -g -c pnm.c -o pnm.o
	
pnm_m.o: pnm_manipulation.c pnm_manipulation.h
	gcc -Wall -O2 -g -c pnm_manipulation.c -o pnm_m.o

pauta: pnm.o pauta.o
	gcc -Wall -O2 -g -s pnm.o pauta.o -o pauta
	
pauta.o: pauta.c pauta.h
	gcc -Wall -O2 -g -c pauta.c 

trab.o:	trabalho.c trabalho.h
	gcc -Wall -O2 -g -c trabalho.c -o trab.o

gTrab: callbacks.c interface.c support.c main.c callbacks.h interface.h support.h
	gcc -g -Wall -O2 -c `pkg-config --cflags --libs gtk+-2.0` callbacks.c interface.c support.c main.c 
	
gGui: pnm.o pnm_m.o pauta.o trab.o callbacks.c interface.c support.c main.c callbacks.h interface.h support.h
	gcc -g -Wall -O2 -s `pkg-config --cflags --libs gtk+-2.0` `pkg-config --cflags glib-2.0` pnm.o pnm_m.o pauta.o callbacks.c interface.c support.c main.c trab.o -o proti
	
clear:
	rm *.o
	
clean: 
	rm *.o proti
