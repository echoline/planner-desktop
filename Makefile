CFLAGS=`pkg-config gtk+-3.0 --cflags` -g
LDFLAGS=`pkg-config gtk+-3.0 --libs` -lm -g

planner-desktop: main.o clock.o planner.o
	$(CC) $(LDFLAGS) -o planner-desktop main.o clock.o planner.o

clean:
	rm -rf *.o planner-desktop
