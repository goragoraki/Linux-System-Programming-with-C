CC = gcc
CFLAGS =
LDFLAGS = -lpthread
.SUFFIXES : .c .o
.c .o :
	$(CC) -c $(CFLAGS) $<

ALL = calc del pattern upper size app rpc

all:$(ALL)
calc:calc.o
	$(CC) -o $@ $< $(LDFLAGS)
del:del.o
	$(CC) -o $@ $< $(LDFLAGS)
pattern:pattern.o
	$(CC) -o $@ $< $(LDFLAGS)
upper:upper.o
	$(CC) -o $@ $< $(LDFLAGS)
size:size.o
	$(CC) -o $@ $< $(LDFLAGS)
app:app.o
	$(CC) -o $@ $< $(LDFLAGS)
rpc:rpc.o
	$(CC) -o $@ $< $(LDFLAGS)	
clean : 
	rm -rf *.o $(ALL)
