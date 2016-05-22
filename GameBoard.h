#ifndef __minesweeper__Board__
#define __minesweeper__Board__
//#include <stdio.h>

#include <FL/Fl_Window.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <set> //Mine
#include <map>
#include "Tile.h"
#include "Score.h"
#include <vector>

using std::make_pair;
class HighscoreWindow;
class CustomWindow;

enum MouseState {
    MOUSE_DOWN,
    MOUSE_LEAVE,
    MOUSE_UP,
};

enum GameLevel{
GAME_BEG,
GAME_INT,
GAME_ADV,
GAME_CUSTOM,
};

enum FaceType {
    FACE_NORMAL,
    FACE_CLICK,
    FACE_OVER,
    FACE_CLEAR,
};

class Strategy{
public:
    virtual void on_mouse(Tile *cell, MouseButton btn, MouseState st) = 0;
    virtual void on_new_game() = 0;
    virtual void on_change_level(GameLevel lev) = 0;
    virtual void on_timer() = 0;
};

typedef std::map<Position, Tile*> Board;
typedef Board::iterator BoardIter;
typedef std::set<Position> PosSet;
typedef PosSet::iterator PosSetIter;

class GameBoard{
public:
    Strategy *strategy;
    int width;
    int height;
    int num_mines;
    Board board;
    ScoreFile sfile;
    static const char *score_filename;
    friend class HighscoreWindow;
    friend class CustomWindow;
    
//public:
    
    GameBoard();
    ~GameBoard();
    void set_board_size(int w, int h, int mines);
    void set_strategy(Strategy *st);
    Tile *get_cell(const Position &pos);
    void start_timer();
    void stop_timer();
    Tile *get_xy_cell(int x, int y);
    void get_fringe(const Position &pos, PosSet *set);
    void set_count(int value);
    void set_time(int value);
    Board *get_board();
    void set_face(FaceType type);
    void game_clear(GameLevel level, int score);
    void reset_score();
    void get_board_size(int *w, int *h, int *m);
    
private:
    Fl_Window *win;
    Fl_Sys_Menu_Bar *menu;
    Fl_Box *panel;
    Fl_Box *count_box;
    Fl_Box *time_box;
    Fl_Button *button;
    int board_x, board_y;
    
    void init_gui();
    void delete_board();
    void init_board();
    bool is_valid_pos(const Position &pos);
    const char* input_name();
    void save_score();
    bool mode; // debug
    bool b;
    
    static void cb_cell(Fl_Widget *w, void *p);
    static void cb_new_game(Fl_Widget *w, void *p);
    static void cb_beginner(Fl_Widget *w, void *p);
    static void cb_intermediate(Fl_Widget *w, void *p);
    static void cb_advanced(Fl_Widget *w, void *p);
    static void cb_custom(Fl_Widget *w, void *p);
    static void cb_highscore(Fl_Widget *w, void *p);
    static void cb_quit(Fl_Widget *w, void *p);
    static void cb_about(Fl_Widget *w, void *p);
    static void cb_timer(void *p);
};


#endif