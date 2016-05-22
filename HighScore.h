#ifndef HIGHSCORE_H_INCLUDE
#define HIGHSCORE_H_INCLUDE

#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include "GameBoard.h"

class HighScore : public Fl_Window{

public:
    HighScore(GameBoard *g);
    
private:
    Fl_Box *label, *score, *name;
    Fl_Button *clear, *ok;
    GameBoard *game;
    void update_score();
    static void cb_close(Fl_Widget *w, void *p);
    static void cb_clear(Fl_Widget *w, void *p);
};

#endif