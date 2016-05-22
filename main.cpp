#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl.H>
#include "Mine.h"

int main()
{
    GameBoard win;
    Mine mine(&win);
    win.set_strategy(&mine);
    return Fl::run();
}






































