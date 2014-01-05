CFLAGS=`pkg-config gtk+-3.0 --cflags` -g
LDFLAGS=`pkg-config gtk+-3.0 --libs` -lX11 -lm -g
INSTPATH=/usr/local/bin/

planner-desktop: main.o clock.o tray.o bubble.o appfinder.o planner.o background.o 
	$(CC) -o planner-desktop main.o clock.o tray.o bubble.o appfinder.o planner.o background.o $(LDFLAGS)

clean:
	rm -rf *.o planner-desktop

install: planner-desktop
	mkdir -p $(INSTPATH)
	cp planner-desktop $(INSTPATH)
