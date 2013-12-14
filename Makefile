CFLAGS=`pkg-config gtk+-3.0 --cflags` -g
LDFLAGS=`pkg-config gtk+-3.0 --libs` -lX11 -lm -g

planner-desktop: main.o clock.o tray.o bubble.o appfinder.o planner.o
	$(CC) $(LDFLAGS) -o planner-desktop main.o clock.o tray.o bubble.o appfinder.o planner.o

clean:
	rm -rf *.o planner-desktop
