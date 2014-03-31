gtk desktop planner application (goes where the desktop icons sometimes go)

theory of operation:
	this software is simple for a few reasons.  i'm just one guy so it's
	hard to add bloaty features anyway and despite the appeal i just have
	to keep it simple stupid.  the interface is under construction.
	"configuration" is done with some other small tools, included.

binaries:
	planner-desktop shows a simple planner in the root window
	options:
		-l	xdg-open/command launcher widget
		-i	cpu/mem info widgets
		-t	system tray widget
		-w	weather widget
		-c	clock widget
		-n	no planner widget
		-b str	background set to file path str

	./ics2plans simply converts .ics files to ~/.plans/ entries
		be careful not to run this more than once

	./location sets location for use by weather widget

