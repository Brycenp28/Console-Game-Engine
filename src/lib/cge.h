// Console graphics engine header.

#ifndef _H_CGE_
#define _H_CGE_

#include <sys/ioctl.h> // system headers.
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <mutex>

#include "vector2.h" // library headers.
#include "cge_map.h"
#include "constants.h"
#include "task.h"

class vector2; // class prototypes.
class CGE;
class cge_map;

class view_port {
	float X, Y, width, height;
	friend CGE;
	CGE *window;
	int window_width, window_height;

public:

	view_port(CGE *win);
	view_port(CGE *win, int x, int y, int w, int h);
	void changeX(int value);
	void changeY(int value);
	void change_view(int x, int y, int w, int h);

};

class CGE {
	
	int screen_width, screen_height;
	float frame_rate;
	char* graph;
	bool border;
	char pixel = 219;
	unsigned int render_mode;
	bool exit_flag = false;

	task tasks[2];

	void display_top_border(view_port *vp);
	void display_side_border(view_port *vp);

public:

	std::mutex mtx;
	view_port main_view_port = view_port(this);
	struct winsize wsize;

	enum {
		single,
		relapse
	};

	CGE(int sw,int sh);
	void set_default_vp(unsigned int x,
						unsigned int y,
						unsigned int width,
						unsigned int height);
	float get_frame_rate(void);
	void bind_task(unsigned int t, void (*func)(task*), bool auto_join);
	void abort_task(unsigned int t);
	void abort_tasks(void);
	void update_task(unsigned int t);
	void update_tasks(void);
	int get_width(void);
	int get_height(void);
	void set_framerate(float fr);
	void set_render_mode(unsigned int render_mode);
	void set_border(bool b);
	void set_pixel(unsigned char c);
	char get_pixel(vector2 &v);
	bool draw_pixel(vector2 &v);
	void draw_line(vector2 &v1, vector2 &v2);
	void draw_text(const char * msg, unsigned int size, unsigned int x, unsigned int y);
	void draw_map(cge_map &map);
	void render_frame(view_port* vp);
	void set_exit_flag(bool b);

	~CGE();
};

#endif // _H_CGE_