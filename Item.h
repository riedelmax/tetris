#IFNDEF ITEM_H
#DEFINE ITEM_H

class item
{
    private:
    Orientation orientation;
    ItemType type;
    uint8_t x;
    uint8_t y;

    public:
    enum Orientation {UP, LEFT, DOWN, RIGHT};
    enum ItemType {SQUARE, L_LEFT, L_RIGHT, S_LEFT, S_RIGHT, BAR, T};
    void sink();

    uint8_t getX() {return x;};
    uint8_t getY() {return y;};
    ItemType getType() {return type;};
    Orientation getOrientation() {return orientation;};
}

#ENDIF