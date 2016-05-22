#include <FL/Fl.H>
#include "Tile.h"
#include <iostream>

static const char *digit_str[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

static const Fl_Color digit_color[] = {
    FL_BLACK,
    FL_BLUE,
    FL_GREEN,
    FL_RED,
    FL_DARK_BLUE,
    FL_YELLOW,
    FL_BLUE,
    FL_DARK_CYAN,
    FL_BLACK,
    FL_DARK_MAGENTA,
};

const int Tile::tile_size = 20;
bool Tile::is_down = false;

Tile::Tile(int x, int y, const Position &p) :
Fl_Box(x, y, 20, 20), pos(p){
    
    labelfont(FL_HELVETICA_BOLD);
    reset();
}

void Tile::reset(){ //Turn tile back to normal state after reset mode
    set_state(Tile_CLOSE);
    color(FL_BACKGROUND_COLOR);
    value = 0;
    is_mine = false;
    is_down = false;
}


void Tile::set_state(TileStatus st){ //Setting tile state
    state = st;
    switch (st) {
        case Tile_OPEN:
            set_state_open();
            break;
        case Tile_CLOSE:
            box(FL_UP_BOX);
            label("");
            break;
        case Tile_FLAG:
            if(is_mine){
                box(FL_UP_BOX);
                labelcolor(FL_BLUE); //Delete if state it does not work correctly
                label("?");
            }
            else{
            box(FL_UP_BOX);
            labelcolor(FL_GREEN);
            label("F");
            }
            break;
        case Tile_TMPOPEN:
            box(FL_THIN_DOWN_BOX);
            label("");
            break;
    }
}

void Tile::set_state_open(){
    box(FL_THIN_DOWN_BOX);
    if (is_mine) {
        box(FL_UP_BOX);
        color(FL_RED);
        labelcolor(FL_BLACK);
        label("");
    }
    else if (value >= 1 && value <= 8) {
        labelcolor(digit_color[value]);
        label(digit_str[value]);
    }
    else
        label(" ");
}

bool debugMode(bool mode){ //Debug Mode. Show mines.
    mode = false;
    if(mode == true){
        return true;
    }
    else{
        return false;
    }
}

void Tile::set_mine(){ //Setting the Mine
    is_mine = true;
    mode = true; // fixed DEBUG MODE. Set Mode = false for debug mode. Set Mode = true to turn off debug mode
    if (is_mine == true && debugMode(mode) == false)  {
        box(FL_UP_BOX);
        color(FL_RED);
        labelcolor(FL_BLACK);
        label("@");
    }
    
}

//Checking tile
TileStatus Tile::get_state() const{
    return state;
}

Position Tile::get_pos() const{
    return pos;
}

int Tile::get_value() const{
    return value;
}

bool Tile::get_is_mine() const{
    return is_mine;
}

bool Tile::get_is_down(){
    return is_down;
}

int Tile::handle(int ev){
    switch (ev) {
        case FL_PUSH:
            is_down = true;
            switch (Fl::event_button()) {
                case FL_LEFT_MOUSE:
                    do_callback();
                    return 1;
                case FL_RIGHT_MOUSE:
                    do_callback();
                    return 1;
            }
        case FL_RELEASE:
            is_down = false;
            switch (Fl::event_button()) {
                case FL_LEFT_MOUSE:
                    do_callback();
                    return 1;
                case FL_RIGHT_MOUSE:
                    do_callback();
                    return 1;
            }
        case FL_DRAG:
        case FL_MOVE:
            do_callback();
            return 1;
    }
    return Fl_Box::handle(ev);
}

void Tile::incr_value(){
    if (!is_mine) ++value;
}
