#ifndef ITEM_H
#define ITEM_H
//

#include <stdint.h>

class Item
{
    public:
    Item();
    enum Orientation {UP, LEFT, DOWN, RIGHT};
    enum ItemType {SQUARE, L_LEFT, L_RIGHT, S_LEFT, S_RIGHT, BAR, T};
    void sink();
    void left();
    void right();
    uint8_t getX();
    uint8_t getY();
    Orientation getOrientation();
    ItemType getType();

    private:
    Orientation orientation;
    ItemType type;
    uint8_t x;
    uint8_t y;
};

#endif
