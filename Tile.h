#ifndef CELLBOX_H_INCLUDE
#define CELLBOX_H_INCLUDE

#include <FL/Fl_Box.H>
#include <utility>

enum TileStatus {
    Tile_CLOSE,
    Tile_OPEN,
    Tile_FLAG,
    Tile_TMPOPEN
};

enum MouseButton {
    MOUSE_NONE,
    MOUSE_LEFT,
    MOUSE_RIGHT,
};

typedef std::pair<int,int> Position;

class Tile:public Fl_Box{

public:
    static const int tile_size;
    Tile(int x, int y, const Position &pos);
    void reset();
    void set_state(TileStatus st);
    int handle(int ev);
    void incr_value();
    void set_mine();
    TileStatus get_state() const;
    Position get_pos() const;
    int get_value() const;
    bool get_is_mine() const;
    static bool get_is_down();
    
    bool debugMode(bool mode);
    bool mode;
    bool set_debug();
    
    
private:
    TileStatus state;
    Position pos;
    int value;
    bool is_mine;
    static bool is_down;
    void set_state_open();
};

#endif