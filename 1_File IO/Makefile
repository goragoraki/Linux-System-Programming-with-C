CC = gcc
CFLAGS =
LDFLAGS =

.SUFFIXES: .c .o

ALL = mycp hole list tcp1 tcp2 mycp2 split merge conv access tab upper

all:$(ALL)

.c .o:
	$(CC) -c $(CFLAGS) $<

mycp : mycp.o
	$(CC) -o $@ $< $(LDFLAGS)
hole : hole.o
	$(CC) -o $@ $< $(LDFLAGS)
list : list.o
	$(CC) -o $@ $< $(LDFLAGS)
tcp1 : tcp1.o
	$(CC) -o $@ $< $(LDFLAGS)
tcp2 : tcp2.o
	$(CC) -o $@ $< $(LDFLAGS)
mycp2 : mycp2.o
	$(CC) -o $@ $< $(LDFLAGS)
split :split.o
	$(CC) -o $@ $< $(LDFLAGS)
merge : merge.o
	$(CC) -o $@ $< $(LDFLAGS)
tab : tab.o
	$(CC) -o $@ $< $(LDFLAGS)
conv : conv.o
	$(CC) -o $@ $< $(LDFLAGS)
access : access.o
	$(CC) -o $@ $< $(LDFLAGS)
upper : upper.o
	$(CC) -o $@ $< $(LDFLAGS)
exemycp :
	./mycp mycp.c mycp.bak
exemycp2 :
	./mycp2 mycp2.c mycp2.bak
exehole :
	./hole
exelist :
	./list list.c
exetab :
	./tab tab.c 
exetcp1 :
	./tcp1 tcp1.c tcp1.bak
exetcp2 :
	./tcp2 tcp2.c tcp2.bak
exesplit :
	./split split.c a b
exemerge :
	./merge a b merge.new
execonv :
	./conv test.in test.out
exeaccess :
	./access test.out
exeupper :
	./upper upper.c
clean :
	rm -rf *.o $(ALL) file.hole tab.c.t tcp1.bak tcp2.bak a b merge.new mycp.bak test.out mycp2.bak



