CC=gcc
CFLAGS=
LDFLAGS=

.SUFFIXES: .c .o

ALL = stat umask chmod myln mysln myrm mymv mymd myls mycd mypwd mylsl mylsr myrd 

all:$(ALL)

.c .o:
	$(CC) -c $(CFLAGS) $<

stat : stat.o
	$(CC) -o $@ $< $(LDFLAGS)
umask : umask.o
	$(CC) -o $@ $< $(LDFLAGS)
chmod : chmod.o
	$(CC) -o $@ $< $(LDFLAGS)
myln : myln.c	
	$(CC) -o $@ $< $(LDFLAGS)
mysln : mysln.c
	$(CC) -o $@ $< $(LDFLAGS)
myrm : myrm.c
	$(CC) -o $@ $< $(LDFLAGS)
mymv : mymv.c
	$(CC) -o $@ $< $(LDFLAGS)
mymd : mymd.c
	$(CC) -o $@ $< $(LDFLAGS)
myrd : myrm.c
	$(CC) -o $@ $< $(LDFLAGS)
myls : myls.c
	$(CC) -o $@ $< $(LDFLAGS)
mylsr : mylsr.c
	$(CC) -o $@ $< $(LDFLAGS)
mycd : mycd.c
	$(CC) -o $@ $< $(LDFLAGS)
mypwd : mypwd.c
	$(CC) -o $@ $< $(LDFLAGS)
mylsl : mylsl.c
	$(CC) -o $@ $< $(LDFLAGS)
	
xstat :
	./stat stat.c
xumask :
	./umask
xchmod :
	./chmod bar foo
xmyln :
	./myln myln.c myln.c.ln
xmysln :
	./mysln mysln.c mysln.c.ln
xmyrd :
	./myrd test
xmymd :
	./mymd test
xmylsl :
	./mylsl
clean:
	rm -rf *.o $(ALL) bar foo myln.c.ln mysln.c.ln
