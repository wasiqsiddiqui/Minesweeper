#ifndef MINESWEEPER_H_INCLUDE
#define MINESWEEPER_H_INCLUDE

#include "Tile.h"
#include "GameBoard.h"

enum GameState {
    GAME_INIT,
    GAME_PLAYING,
    GAME_OVER,
};

class Mine:public Strategy{
public:
    Mine(GameBoard *g);
    void on_mouse(Tile *cell, MouseButton btn, MouseState st);
    void on_new_game();
    void on_change_level(GameLevel level);
    void on_timer();
    
private:
    GameBoard *win;
    int width;
    int height;
    int num_mines;
    int time;
    int count;
    GameState state;
    GameLevel level;
    void on_mouse_left(Tile *cell, MouseState st);
    void on_mouse_right(Tile *cell, MouseState st);
    void start_game(const Position &pos);
    void init_mine(const Position &pos);
    void open_cell(Tile *cell);
    void highlight(Tile *cell);
    void cancel_highlight(Tile *cell);
    void auto_open(Tile *cell);
    void game_over();
    bool check_game_clear();
    void game_clear();
};

#endif