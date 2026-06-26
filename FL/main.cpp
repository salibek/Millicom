#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#pragma comment(lib, "fltk.lib")

int main()
{
	Fl_Window* window = new Fl_Window(300, 300, "test window");
	window->show();

	Fl::run();

	return 0;
}