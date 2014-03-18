CFLAGS=`pkg-config gtk+-3.0 --cflags` -g
LDFLAGS=`pkg-config gtk+-3.0 --libs` `pkg-config libsoup-2.4 --libs` -lX11 -lm -g
INSTPATH=/usr/local/bin/

planner-desktop: main.o clock.o tray.o bubble.o appfinder.o planner.o background.o weather.la info.o meter.o
	$(CC) -o planner-desktop main.o clock.o tray.o bubble.o appfinder.o planner.o background.o info.o meter.o weather.la $(LDFLAGS)

weather.la:
	$(MAKE) -C weather/

clean:
	$(MAKE) -C weather/ clean
	rm -rf *.o planner-desktop weather.la

install: planner-desktop
	mkdir -p $(INSTPATH)
	cp planner-desktop $(INSTPATH)
