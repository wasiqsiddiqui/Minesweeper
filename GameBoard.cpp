#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include "Tile.h"
#include "GameBoard.h"
#include "HighScore.h"
#include "CustomBoard.h"
#include "Score.h"
#include <iostream>

const char *GameBoard::score_filename = ".score";

GameBoard::GameBoard(){
    init_gui();
    std::ifstream ist(score_filename);
    if (ist)
        ist >> sfile;
    else
        sfile = ScoreFile();
    ist.close();
}

GameBoard::~GameBoard(){
    delete win;
}

void GameBoard::init_gui(){
    
    //Board
    win = new Fl_Window(100,100,"Minesweeper");
    win->label("Minesweeper");
    
    //Menu
    menu = new Fl_Sys_Menu_Bar(0, 0, 300, 25);
    
    //Game
    menu->add("Game", 0, 0, 0, FL_SUBMENU);
    menu->add("Game/New", 0, cb_new_game, this, FL_MENU_DIVIDER);
    menu->add("Game/Beginner", 0,
              cb_beginner, this, FL_MENU_RADIO | FL_MENU_VALUE);
    menu->add("Game/Intermediate", 0, cb_intermediate, this, FL_MENU_RADIO);
    menu->add("Game/Advanced", 0, cb_advanced, this, FL_MENU_RADIO);
    menu->add("Game/Custom Size", 0,
              cb_custom, this, FL_MENU_DIVIDER | FL_MENU_RADIO);
    menu->add("Game/Highscore", 0,
              cb_highscore, this, FL_MENU_DIVIDER);
    menu->add("Game/Quit", 0, cb_quit, this, 0);
    
    //Help
    menu->add("Help", 0, 0, 0, FL_SUBMENU);
    menu->add("Help/About", 0, cb_about, this, 0);
    
    //Panel
    panel = new Fl_Box(FL_THIN_DOWN_BOX, 0, 0, 0, 0, "");
    count_box = new Fl_Box(FL_THIN_DOWN_BOX, 0, 0, 0, 0, "");
    time_box = new Fl_Box(FL_THIN_DOWN_BOX, 0, 0, 0, 0, "");
    button = new Fl_Button(0, 0, 0, 0);
    button->callback(cb_new_game, this);
    button->labelsize(10);
    win->end();
    win->show();
    
}
     
//Creating the Board
void GameBoard::set_board_size(int w, int h, int mines){
    if (w != 0) {
        width = w;
        height = h;
        num_mines = mines;
    }
    const int pad = 10;
    const int ppad = 5;
#ifdef __APPLE__
    const int menu_h = 0;
#else
    const int menu_h = 25;
#endif
    
    const int panel_h = 40;
    const int cell_s = Tile::tile_size;
    const int win_w = pad + cell_s * width + pad;
    const int win_h = menu_h + pad + panel_h + pad + cell_s * height + pad; //New Line
    const int win_x = win->x_root();
    const int win_y = win->y_root();
    win->resize(win_x, win_y, win_w, win_h);
    menu->resize(0, 0, win_w, menu_h);
    const int panel_x = pad;
    const int panel_y = menu_h + pad;
    const int panel_w = win_w - pad * 2;
    
    panel->resize(panel_x, panel_y, panel_w, panel_h);
    count_box->position(panel_x + ppad, panel_y + ppad);
    count_box->size(40, 30);
    
    const int but_w = 30;
    const int but_x = win_w / 2 - but_w / 2;
    button->position(but_x, panel_y + ppad);
    button->size(but_w, 30);
    set_face(FACE_NORMAL);

    const int time_w = 40;
    const int time_x = win_w - pad - ppad - time_w;
    time_box->position(time_x, panel_y + ppad);
    time_box->size(time_w, 30);
    
    board_x = pad;
    board_y = menu_h + pad + panel_h + pad;
    delete_board();
    init_board();
    win->redraw();
    
}

void GameBoard::delete_board(){
    for (BoardIter it = board.begin(); it != board.end(); ++it)
        delete it->second;
        board.clear();
}

void GameBoard::init_board(){
    
    win->begin();
    
    for (int i = 0; i < height; ++i) {
        
        for (int j = 0; j < width; ++j) {
            
            Position pt = make_pair(j,i);
            const int cell_s = Tile::tile_size;
            const int x = board_x + cell_s * j;
            const int y = board_y + cell_s * i;
            Tile *cell = new Tile(x, y, pt);
            cell->callback(cb_cell, this);
            board.insert(make_pair(pt, cell));
            
        }
    }
    win->end();
}

void GameBoard::set_strategy(Strategy *st){
    strategy = st;
}

Tile *GameBoard::get_cell(const Position &pos){
    return board[pos];
}

void GameBoard::start_timer(){
    stop_timer();
    Fl::repeat_timeout(1.0, cb_timer, this);
}

void GameBoard::stop_timer(){
    Fl::remove_timeout(cb_timer);
}

Tile *GameBoard::get_xy_cell(int x, int y){
    const int cell_s = Tile::tile_size;
    if (x < board_x || x >= board_x + cell_s * width) return 0;
    if (y < board_y || y >= board_y + cell_s * height) return 0;
    int cx = (x - board_x) / cell_s;
    int cy = (y - board_y) / cell_s;
    Position pos = make_pair(cx, cy);
    return board[pos];
}

void GameBoard::get_fringe(const Position &pos, PosSet *set){
    const int x = pos.first;
    const int y = pos.second;
    for (int i = y-1; i <= y+1; ++i) {
        for (int j = x-1; j <= x+1; ++j) {
            Position p = make_pair(j, i);
            if (p != pos && is_valid_pos(p))
                set->insert(p);
        }
    }
}

bool GameBoard::is_valid_pos(const Position &pos){
    return pos.first >= 0 && pos.first < width &&
    pos.second >= 0 && pos.second < height;
}

void GameBoard::set_count(int value){
    static char str[4];
//#ifdef WIN32
    std::cout << str << sizeof(str) << "%03d" << value;
    //sprintf_s(str, sizeof(str), "%03d", value);
//#else
    std::snprintf(str, sizeof(str), "%03d", value);
//#endif
    count_box->label(str);
}


//Set Time
void GameBoard::set_time(int value){
    static char str[4];
#ifdef WIN32
    sprintf_s(str, sizeof(str), "%03d", value);
#else
    std::snprintf(str, sizeof(str), "%03d", value);
#endif
    time_box->label(str);
}

//Timer
void GameBoard::cb_timer(void *p){
    GameBoard *game = static_cast<GameBoard*>(p);
    game->strategy->on_timer();
    Fl::repeat_timeout(1.0, cb_timer, game);
}



Board *GameBoard::get_board(){
    return &board;
}

//Setting Face
void GameBoard::set_face(FaceType type){
    switch (type) {
        case FACE_NORMAL:
            button->label("(ᵔᴥᵔ)");
            break;
        case FACE_CLICK:
            button->label("♥‿♥");
            break;
        case FACE_OVER:
            button->label("X_X");
            break;
        case FACE_CLEAR:
            button->label("⌐╦╦═─");
            break;
    }
}

void GameBoard::game_clear(GameLevel level, int score){
    Score *s = 0;
    switch (level) {
        case GAME_BEG:
            s = &sfile.beg_score;
            break;
        case GAME_INT:
            s = &sfile.int_score;
            break;
        case GAME_ADV:
            s = &sfile.adv_score;
            break;
    }
    if (!s) return;
    if (score < s->score) {
        s->name = input_name();
        s->score = score;
    }
    save_score();
}

void GameBoard::reset_score(){
    sfile = ScoreFile();
    save_score();
}

void GameBoard::save_score(){
    std::ofstream ost(score_filename);
    ost << sfile;
    ost.close();
}

const char* GameBoard::input_name(){
    const char *name =
    fl_input("New HighScore! Enter Name: ");
    if (name) return name;
    else return "no name";
}

void GameBoard::get_board_size(int *w, int *h, int *m){
    *w = width;
    *h = height;
    *m = num_mines;
}

void GameBoard::cb_cell(Fl_Widget *w, void *p){
    GameBoard *game = static_cast<GameBoard*>(p);
    
    static Tile *prev_cell = 0;
    static MouseButton prev_btn = MOUSE_NONE;
    
    Strategy *st = game->strategy;
    int x = Fl::event_x();
    int y = Fl::event_y();
    Tile *curr_cell = game->get_xy_cell(x, y);
    
    MouseButton curr_btn;
    switch (Fl::event_button()) {
        case FL_LEFT_MOUSE:
            curr_btn = MOUSE_LEFT;
            break;
        case FL_RIGHT_MOUSE:
            curr_btn = MOUSE_RIGHT;
            break;
    }
    if (Tile::get_is_down()) { //If tile is down
        if (prev_cell == curr_cell) return;
        if (prev_cell) st->on_mouse(prev_cell, prev_btn, MOUSE_LEAVE);
        if (curr_cell) st->on_mouse(curr_cell, curr_btn, MOUSE_DOWN);
        prev_cell = curr_cell;
        prev_btn = curr_btn;
    }
    else {
        if (prev_cell) st->on_mouse(prev_cell, prev_btn, MOUSE_LEAVE);
        if (curr_cell) st->on_mouse(curr_cell, prev_btn, MOUSE_UP);
        prev_cell = 0;
        prev_btn = MOUSE_NONE;
    }
}



//New Game
void GameBoard::cb_new_game(Fl_Widget *w, void *p){
    GameBoard *game = static_cast<GameBoard*>(p);
    game->strategy->on_new_game();
}


///GAME LEVELS
//Menu-Beginner
void GameBoard::cb_beginner(Fl_Widget *w, void *p){
    GameBoard *game = static_cast<GameBoard*>(p);
    game->strategy->on_change_level(GAME_BEG);
}
//Menu-Intermediate
void GameBoard::cb_intermediate(Fl_Widget *w, void *p){
    GameBoard *game = static_cast<GameBoard*>(p);
    game->strategy->on_change_level(GAME_INT);
}
//Menu-Advanced
void GameBoard::cb_advanced(Fl_Widget *w, void *p){
    GameBoard *game = static_cast<GameBoard*>(p);
    game->strategy->on_change_level(GAME_ADV);
}



//Menu-Custom Size
void GameBoard::cb_custom(Fl_Widget *w, void *p){
    GameBoard *game = static_cast<GameBoard*>(p);
    CustomBoard *custB = new CustomBoard(game); //Custom Board
}

//Menu-Highscore
void GameBoard::cb_highscore(Fl_Widget *w, void *p){
    GameBoard *game = static_cast<GameBoard*>(p);
    HighScore *hw = new HighScore(game); //Board
}

//Menu-Quit
void GameBoard::cb_quit(Fl_Widget *w, void *p){
    std::exit(EXIT_SUCCESS);
}

//Menu-About
void GameBoard::cb_about(Fl_Widget *w, void *p){
    fl_message("Wasiq Siddiqui \nRishi Vaze\nCaleb Williams\nJorge Ramirez");
}

