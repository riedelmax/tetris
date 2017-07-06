
#include "Item.h"
#include <cstdlib>
#include <ctime>

#include "Board.h"

using namespace std;

Item::Item()
{
    x = 5;
    y = 0;
    srand(time(0));
    type = (ItemType) ( rand() / (RAND_MAX/7));
    orientation = Orientation::UP;
}

void Item::left()
{
    if(x-1 >= 0)
    {
        x--;
    }
}

void Item::right()
{
    if(x+1 < W)
    {
        x++;
    }
}

void Item::sink()
{
    y++;
}


uint8_t Item::getX() {return x;};
uint8_t Item::getY() {return y;};
Item::ItemType Item::getType() {return type;};
Item::Orientation Item::getOrientation() {return orientation;};
