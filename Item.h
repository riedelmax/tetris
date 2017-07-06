#ifndef ITEM_H
#define ITEM_H
//

#include <stdint.h>

class Item
{
    public:
    enum Orientation {UP, LEFT, DOWN, RIGHT};
    enum ItemType {SQUARE, L_LEFT, L_RIGHT, S_LEFT, S_RIGHT, BAR, T};
    void sink();
    void left();
    void right();

    uint8_t getX() {return x;};
    uint8_t getY() {return y;};
    ItemType getType() {return type;};
    Orientation getOrientation() {return orientation;};

<<<<<<< HEAD
    private:
    Orientation orientation;
    ItemType type;
    uint8_t x;
    uint8_t y;
};

#endif
=======
#ENDIF
>>>>>>> branch 'master' of https://github.com/riedelmax/tetris.git
