CFLAGS=`pkg-config gtk+-3.0 --cflags` -g
LDFLAGS=`pkg-config gtk+-3.0 --libs` -lX11 -lm -g
INSTPATH=/usr/local/bin/

all: planner-desktop ics2plans

ics2plans: ics2plans.c
	$(CC) -o ics2plans ics2plans.c

planner-desktop: main.o clock.o tray.o bubble.o appfinder.o planner.o background.o info.o meter.o
	$(CC) -o planner-desktop main.o clock.o tray.o bubble.o appfinder.o planner.o background.o info.o meter.o $(LDFLAGS)

clean:
	rm -rf *.o planner-desktop ics2plans

install: all
	mkdir -p $(INSTPATH)
	cp planner-desktop $(INSTPATH)
