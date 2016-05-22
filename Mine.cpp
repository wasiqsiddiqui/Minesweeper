#include <FL/fl_ask.H>
#include <ctime>
#include <cstdlib>
#include <set>
#include "Tile.h"
#include "Mine.h"

Mine::Mine(GameBoard *w) :
Strategy(), win(w){
    on_change_level(GAME_BEG);
}

void Mine::on_mouse(Tile *cell, MouseButton btn, MouseState st){
    switch (btn) {
        case MOUSE_LEFT:
            on_mouse_left(cell, st);
            break;
        case MOUSE_RIGHT:
            on_mouse_right(cell, st);
            break;
    }
}

void Mine::on_mouse_left(Tile *cell, MouseState st){
    switch (st) {
        case MOUSE_DOWN:
            if (state == GAME_PLAYING || state == GAME_INIT) {
                win->set_face(FACE_CLICK);
                if (cell->get_state() == Tile_CLOSE)
                    cell->set_state(Tile_TMPOPEN);
                else
                    highlight(cell);
            }
            break;
        case MOUSE_LEAVE:
            if (cell->get_state() == Tile_TMPOPEN)
                cell->set_state(Tile_CLOSE);
            else {
                cancel_highlight(cell);
                auto_open(cell);
            }
            break;
        case MOUSE_UP:
            if (state == GAME_INIT) start_game(cell->get_pos());
            if (state == GAME_PLAYING) {
                win->set_face(FACE_NORMAL);
                if (cell->get_state() == Tile_CLOSE) open_cell(cell);
            }
            break;
    }
}

void Mine::on_mouse_right(Tile *cell, MouseState st){
    switch (st) {
        case MOUSE_UP:
            if (state == GAME_INIT)
                start_game(cell->get_pos());
            if (state == GAME_PLAYING) {
                if (cell->get_state() == Tile_CLOSE) {
                    win->set_count(--count);
                    cell->set_state(Tile_FLAG);
                }
                else if (cell->get_state() == Tile_FLAG) {
                    win->set_count(++count);
                    cell->set_state(Tile_CLOSE);
                }
                if (check_game_clear()) game_clear();
            }
            break;
    }
}

void Mine::on_new_game(){
    win->stop_timer();
    win->set_time(time = 0);
    win->set_count(count = num_mines);
    state = GAME_INIT;
    win->set_face(FACE_NORMAL);
    
    Board *board = win->get_board();
    for (BoardIter it = board->begin(); it != board->end(); ++it)
        it->second->reset();
}

void Mine::on_change_level(GameLevel lev){
    level = lev;
    switch (lev) {
        case GAME_BEG:
            width = 9;
            height = 9;
            num_mines = 10;
            break;
        case GAME_INT:
            width = 16;
            height = 16;
            num_mines = 40;
            break;
        case GAME_ADV:
            width = 16;
            height = 30;
            num_mines = 99;
            break;
        case GAME_CUSTOM:
            win->get_board_size(&width, &height, &num_mines);
            break;
    }
    win->set_board_size(width, height, num_mines);
    
    win->stop_timer();
    state = GAME_INIT;
    time = 0;
    win->set_time(time);
    count = num_mines;
    win->set_count(count);
}

void Mine::on_timer(){
    if (++time > 999) time = 999;
    win->set_time(time);
}

bool Tile::debugMode(bool mode){
    if(mode == true){
        return true;
    }
    else{
        return false;
    }
}

void Mine::start_game(const Position &pos){
    state = GAME_PLAYING;
    init_mine(pos);
    win->start_timer();
}

void Mine::init_mine(const Position &pos){
    std::srand(std::time(0));
    PosSet set;
    set.insert(pos);
    int n = 0;
    while (n < num_mines) {
        int x = rand() % width;
        int y = rand() % height;
        Position p = make_pair(x, y);
        if (set.find(p) == set.end()) { //
            set.insert(p);
            win->get_cell(p)->set_mine();
            PosSet fringe;
            win->get_fringe(p, &fringe);
            for (PosSetIter it = fringe.begin(); it != fringe.end(); ++it)
                win->get_cell(*it)->incr_value();
            ++n;
        }
    }
}

void Mine::open_cell(Tile *cell){
    if (cell->get_is_mine()) {
        game_over();
        return;
    }
    
    cell->set_state(Tile_OPEN);
    if (cell->get_value() > 0){
        if (check_game_clear()) game_clear();
        return;
    }
    
    PosSet visit;
    PosSet checked;
    
    Position pos = cell->get_pos();
    checked.insert(pos);
    win->get_fringe(pos, &visit);
    
    while (!visit.empty()) {
        PosSetIter it = visit.begin();
        if (checked.find(*it) == checked.end()){
            Tile *cell = win->get_cell(*it);
            cell->set_state(Tile_OPEN);
            if (cell->get_value() == 0) win->get_fringe(*it, &visit);
            checked.insert(*it);
        }
        visit.erase(it);
    }
    
    if (check_game_clear()) game_clear();
}

void Mine::highlight(Tile *cell){
    if (cell->get_state() == Tile_CLOSE)
        cell->set_state(Tile_TMPOPEN);
    PosSet set;
    win->get_fringe(cell->get_pos(), &set);
    for (PosSetIter it = set.begin(); it != set.end(); ++it)
        if (win->get_cell(*it)->get_state() == Tile_CLOSE)
            win->get_cell(*it)->set_state(Tile_TMPOPEN);
}


void Mine::cancel_highlight(Tile *cell){
    if (cell->get_state() == Tile_TMPOPEN)
        cell->set_state(Tile_CLOSE);
    PosSet set;
    win->get_fringe(cell->get_pos(), &set);
    for (PosSetIter it = set.begin(); it != set.end(); ++it)
        if (win->get_cell(*it)->get_state() == Tile_TMPOPEN)
            win->get_cell(*it)->set_state(Tile_CLOSE);
}

void Mine::auto_open(Tile *cell){
    PosSet set;
    win->get_fringe(cell->get_pos(), &set);
    
    if (cell->get_value() == 0) return;
    
    int n = 0;
    for (PosSetIter it = set.begin(); it != set.end(); ++it)
        if (win->get_cell(*it)->get_state() == Tile_FLAG)
            ++n;
    
    if (n == cell->get_value())
        for (PosSetIter it = set.begin(); it != set.end(); ++it)
            if (win->get_cell(*it)->get_state() == Tile_CLOSE)
                open_cell(win->get_cell(*it));
}


void Mine::game_over(){
    win->stop_timer();
    win->set_face(FACE_OVER);
    state = GAME_OVER;
    Board *board = win->get_board();
    for (BoardIter it = board->begin(); it != board->end(); ++it) {
        if (it->second->get_is_mine() &&
            it->second->get_state() != Tile_FLAG)
            it->second->set_state(Tile_OPEN);
    }
}

bool Mine::check_game_clear(){
    if (count != 0) return false;
    Board *board = win->get_board();
    for (BoardIter it = board->begin(); it != board->end(); ++it)
        if (it->second->get_state() == Tile_CLOSE) return false;
    return true;
}

void Mine::game_clear(){
    win->stop_timer();
    win->set_face(FACE_CLEAR);
    state = GAME_OVER;
    win->game_clear(level, time);
}