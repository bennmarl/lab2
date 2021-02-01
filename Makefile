#
# R Jesse Chaney
# 

CC = gcc
DEBUG = -g
DEFINES =
#DEFINES += -DMIN_SBRK_SIZE=1024
DEFINES += -DMIN_SBRK_SIZE=1024

CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
        -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
        -Wdeclaration-after-statement $(DEFINES)
PROG = beavalloc


all: $(PROG)


$(PROG): $(PROG).o main.o
	$(CC) $(CFLAGS) -o $@ $^
	chmod a+rx,g-w $@

$(PROG).o: $(PROG).c $(PROG).h
	$(CC) $(CFLAGS) -c $<

main.o: main.c $(PROG).h
	$(CC) $(CFLAGS) -c $<

opt: clean
	make DEBUG=-O3

tar: clean
	tar cvfz $(PROG).tar.gz *.[ch] ?akefile

# clean up the compiled files and editor chaff
clean cls:
	rm -f $(PROG) *.o *~ \#*

# I use RCS. If you'd prefer to use something else (like SCCS), go
# ahead. Version control is a good thing.
# If you put your code in GitHub, make sure it is not publicly
# viewable. That would be a violation of student conduct.
ci:
	if [ ! -d RCS ] ; then mkdir RCS; fi
	ci -t-none -m"lazy-checkin" -l *.[ch] ?akefile
