#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <sstream>
#include "GameBoard.h"
#include "HighScore.h"

HighScore::HighScore(GameBoard *g) :
Fl_Window(400, 140, "Highscores"), game(g){
    set_modal();
    begin();
    
    label = new Fl_Box(25, 25, 110, 70);
    label->label("Beginner:\nIntermediate:\nAdvanced:");
    label->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
    
    name = new Fl_Box(220, 25, 120, 70, "");
    name->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
    
    score = new Fl_Box(110, 25, 70, 70, "");
    score->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
    
    clear = new Fl_Button(70, 100, 120, 30, "Clear Highscore"); //Clear HighScore button
    clear->callback(cb_clear, this);
    ok = new Fl_Button(220, 100, 70, 30, "Ok"); //Ok button
    ok->callback(cb_close, this);
    ok->take_focus();
    
    update_score();
    end();
    show();
}

void HighScore::update_score(){
    std::ostringstream str1;
    str1 << game->sfile.beg_score.score << "\n" << game->sfile.int_score.score << "\n" << game->sfile.adv_score.score;
    score->copy_label(str1.str().c_str());
    
    std::ostringstream str2;
    str2 << game->sfile.beg_score.name << "\n" << game->sfile.int_score.name << "\n" << game->sfile.adv_score.name;
    name->copy_label(str2.str().c_str());
}

void HighScore::cb_close(Fl_Widget *w, void *p){
    HighScore *hw = static_cast<HighScore*>(p);
    hw->hide();
}

void HighScore::cb_clear(Fl_Widget *w, void *p){
    HighScore *hw = static_cast<HighScore*>(p);
    hw->game->reset_score();
    hw->update_score();
}