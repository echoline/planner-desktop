CFLAGS=`pkg-config gtk+-3.0 --cflags` -g `pkg-config gweather-3.0 --cflags`
LDFLAGS=`pkg-config gtk+-3.0 --libs` `pkg-config libsoup-2.4 --libs` -lX11 -lm -g `pkg-config gweather-3.0 --libs`
INSTPATH=/usr/local/bin/

all: planner-desktop ics2plans location

location: location.c
	$(CC) -o location location.c $(LDFLAGS) $(CFLAGS)

ics2plans: ics2plans.c
	$(CC) -o ics2plans ics2plans.c

planner-desktop: main.o clock.o tray.o bubble.o appfinder.o planner.o background.o weather.o info.o meter.o
	$(CC) -o planner-desktop main.o clock.o tray.o bubble.o appfinder.o planner.o background.o info.o meter.o weather.o $(LDFLAGS)

clean:
	rm -rf *.o planner-desktop ics2plans location

install: all
	mkdir -p $(INSTPATH)
	cp planner-desktop $(INSTPATH)
