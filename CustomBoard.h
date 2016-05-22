#ifndef CUSTOMWINDOW_H_INCLUDE
#define CUSTOMWINDOW_H_INCLUDE

#include <FL/Fl_Window.H>
#include <FL/Fl_Int_Input.H>
#include "GameBoard.h"

class CustomBoard : public Fl_Window{

public:
    CustomBoard(GameBoard *g);
private:
    GameBoard *game;
    Fl_Int_Input *width, *height, *mines;
    Fl_Button *btn;
    static void cb_btn(Fl_Widget *w, void *p);
    
};

#endif
