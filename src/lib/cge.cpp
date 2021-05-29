// Console game engine source library.
#include "cge.h"
#include <ncurses.h>

CGE::CGE(int sw, int sh) :
	screen_width(sw),
	screen_height(sh)
{

	std::cout << "\033]0;[CGE] ; Console Game engine, Version 1.0.0 Release.\007";

	initscr(); // ncurses config.
	noecho();
	cbreak();
	ioctl(0, TIOCGWINSZ, &wsize);

	graph = new char[sw*sh]; // initgraph.

	for (int i = 0; i < sw*sh; i++) // loads screen white space.
	{
		graph[i] = ' ';
	};
}

void CGE::set_default_vp(unsigned int x,
						unsigned int y,
						unsigned int width,
						unsigned int height)
{
	main_view_port.change_view(x,y,width,height);
}

float CGE::get_frame_rate(void)
{
	return frame_rate;
}


void CGE::bind_task(unsigned int t, void (*func)(task*), bool auto_join = true)
{
	tasks[t].auto_join = auto_join;
	tasks[t].bind(func);
}

void CGE::abort_task(unsigned int t)
{
	tasks[t].status = 0;
	if (tasks[t].thread.joinable())
		tasks[t].thread.join();
}

void CGE::abort_tasks(void)
{
	for(int i = 0; i < 2; i++) {
		tasks[i].status = 0; // abort.
		if (tasks[i].thread.joinable())
			tasks[i].thread.join();
	}
}

void CGE::update_task(unsigned int t)
{
	tasks[t].status = 1;
	tasks[t].update();
}

void CGE::update_tasks(void)
{

	if (exit_flag) {
		abort_tasks();
		std::cout << "exiting..." << std::endl;
		exit(0);
	};

	for(int i = 0; i < 2; i++) {
		if (tasks[i].thread.joinable())
			tasks[i].thread.join();
			//mtx.lock();
			tasks[i].update();
			//mtx.unlock();
	};

	for(int i = 0; i < 2; i++) {
			if (tasks[i].thread.joinable() &&
				tasks[i].auto_join) {
				tasks[i].status = 0; // abort.
				tasks[i].thread.join();
			}
	};

	usleep(frame_rate);

}

int CGE::get_width(void)
{
	return screen_width;
}

int CGE::get_height(void)
{
	return screen_height;
}

void CGE::display_top_border(view_port *vp)
{

	int x, y;
	int centerX = (int)(wsize.ws_col / 2 - vp->width / 2);
	//int centerY = (int)(wsize.ws_row / 2 - vp->height / 2);

	getyx(stdscr,y, x);
	move(y,centerX);
	
	for(int i = 0; i < vp->width + 2; i++) {
		getyx(stdscr,y, x);
		addch('-');
		move(y,x+1);
	}
	move(y+1,centerX);
}

void CGE::display_side_border(view_port *vp)
{
	int x, y;
	int centerX = (int)(wsize.ws_col / 2 - vp->width / 2);
	int centerY = (int)(wsize.ws_row / 2 - vp->height / 2);

	getyx(stdscr,y, x);
	move(y, centerX);
	getyx(stdscr,y, x);

	addch('|');
	move(y,x+1);

	for(int i = 0; i < vp->width; i++) {
		getyx(stdscr,y, x);
		addch(' ');
		move(y,x+1);
	};

	move(y,x+1);
	addch('|');

	move(y,centerX+1);
}

void CGE::set_framerate(float fr)
{
	frame_rate = C_SECOND / fr;
}

void CGE::set_render_mode(unsigned int render_mode)
{
	this->render_mode = render_mode;
}

void CGE::set_border(bool b)
{
	border = b;
}

void CGE::set_pixel(unsigned char c = 219)
{
	pixel = c;
}

char CGE::get_pixel(vector2 &v)
{
	int vector_position = v.X + v.Y * screen_width;
	return graph[vector_position];
}

bool CGE::draw_pixel(vector2 &v)
{
	int vector_position;

	vector_position = (v.X + v.Y * screen_width);

	graph[vector_position] = (
		vector_position < screen_width * screen_height) * pixel
		+ (vector_position >= screen_width * screen_height) *
		graph[vector_position];

	return (vector_position < screen_width * screen_height);

}

void CGE::draw_line(vector2 &v1, vector2 &v2)
{ // draw line. O(n+1); branch count: 1.
	float dx, dy, incx, incy, steps;

	dx = v2.X - v1.X;
	dy = v2.Y - v1.Y;
	steps = (dx > dy) * dx + (dx < dy) * dy + (dx == dy) * dx;
	incx = dx / steps;
	incy = dy / steps;

	vector2 position(v1.X, v1.Y);

	for (int i = 0; i < steps; i++) {
		draw_pixel(position);
		position.move_vector(incx,incy);
	}

}

void CGE::draw_text(const char * msg, unsigned int size, unsigned int x, unsigned int y)
{

	vector2 position(x,y);

	for(int i = 0; i < size; i++) {
		
		if (x >= screen_width)
		{
			x = 0;
			y++;
		};

		if (y >= screen_height)
			break;

		set_pixel(msg[i]);
		draw_pixel(position);

		++x;

		position.set_vector(x,y);
	};
}

void CGE::draw_map(cge_map &map)
{

	vector2 position(0,0);
	char dp;
	unsigned int byte_index = 0;

	for (int y = 0; y < map.height; y++) {
		for (int x = 0; x < map.width; x++) {
			dp = map.bytes[byte_index];
				set_pixel(dp);
				position.set_vector(x,y);
				draw_pixel(position);
				++byte_index;
		};
	};
}

void CGE::render_frame(view_port *vp)
{

	int x, y, pixel_location;
	int offsetX = vp->X, offsetY = vp->Y; // save old values to move the screen
	int centerX = (int)(wsize.ws_col / 2 - vp->width / 2);
	int centerY = (int)(wsize.ws_row / 2 - vp->height / 2);
	getyx(stdscr,y, x);

	if (render_mode != single) // check render mode.
		move(centerY, 0);

	//print the screen border.
	if (border)
		display_top_border(vp);
	else
		move(centerY,centerX);

	for(int iy = vp->Y; iy < vp->height + offsetY; iy++)
	{

		if (border)
			display_side_border(vp);

		for(int ix = vp->X; ix < vp->width + offsetX; ix++)
		{

			pixel_location = ix + iy * screen_width;
			
			if (pixel_location > screen_width * screen_height) {
				addch('$');
				move(y,x+1);
				continue;
			}

			getyx(stdscr,y, x);
			addch(graph[pixel_location]);
			move(y,x+1);
		};

		if (border)
			move(y+1, 0);
		else
			move(y+1, centerX);
	}

	move(y+1, 0);

	if (border)
		display_top_border(vp);
	
	refresh();
}

void CGE::set_exit_flag(bool b)
{
	exit_flag = b;
}

CGE::~CGE()
{
	delete [] graph;
	endwin();
}

view_port::view_port(CGE *win) :
	window(win)
{
	window_width = window->get_width();
	window_height = window->get_height();
}

view_port::view_port(CGE *win, int x, int y, int w, int h) :
	X(x),
	Y(y),
	width(w),
	height(h),
	window(win)
{
	window_width = window->get_width();
	window_height = window->get_height();
}

void view_port::changeX(int value)
{

	if (X+value >= 0 && X+value+width-1 < window->get_width())
		X += value;
}

void view_port::changeY(int value)
{

	if (Y+value >= 0 && Y+value+height-1 < window->get_height())
		Y += value;
}

void view_port::change_view(int x, int y, int w, int h)
{
	X = x;
	Y = y;
	width = w;
	height = h;
}