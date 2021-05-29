// in console graphics program.
// made by brycen pina.

#include <iostream>
#include <string>
#include <vector>
#include "lib/cge.h"
#include "lib/constants.h"
#include <ncurses.h>

#define width 100
#define height 100

CGE graphics(width, height); // graphics object.

void Draw(task* t)
{

	static unsigned int frame_count = 0; // frame iteration.

	do {

		graphics.render_frame(&graphics.main_view_port); // Render frame from the main viewport.

		usleep(graphics.get_frame_rate()); // manual frame-rate control.

		move(1,0);//								Print the frame-rate
		printw("frame_count: %d.", frame_count);//  to the screen.

		++frame_count;

	} while (t->status != 0); // run until abort_task(DRAW) is called.
};

void Key_pressed(task* t)
{

	int c = getchar(); //getch();
	keypad(stdscr, FALSE); // disable characters buffering.

	graphics.abort_task(DRAW); // Interupt the drawing process.

	if (c == UP_KEY)
		graphics.main_view_port.changeY(-1);
	if (c == DOWN_KEY)
		graphics.main_view_port.changeY(1);
	if (c == LEFT_KEY)
		graphics.main_view_port.changeX(-1);
	if (c == RIGHT_KEY)
		graphics.main_view_port.changeX(1);
	if (c == EXIT_KEY) // exits the window (non blocking).
		graphics.set_exit_flag(true);

};

int main() { // Test program.

	
	// Properties & event binding.

	graphics.set_default_vp(0,0,65,20);
	graphics.set_framerate(15);
	graphics.set_render_mode(CGE::relapse);
	graphics.set_border(true);
	graphics.bind_task(DRAW, Draw, false);
	graphics.bind_task(KEY_PRESSED, Key_pressed, true);
	cge_map map1("src/maps/test.cgem");
	vector2 v1(0,6), v2(7,8);

	try { // 			Attempts to load in a map.

		map1.load(); // The load method will return an exception if the load was unsuccessful.

	} catch(...) {
		graphics.set_exit_flag(true); // Non-blocking way to exit the program.
	};

	// Drawing graphics to the screen.

	graphics.draw_map(map1);
	graphics.draw_text("Hello, World!", 13, 0,3);
	graphics.draw_line(v1,v2);

	while(true) { // main graphics loop.

		graphics.update_tasks();
		keypad(stdscr, TRUE); // Listen for keys to be pressed again.
	};

	return 0;
}