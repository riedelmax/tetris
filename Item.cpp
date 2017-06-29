include "Item.h"
include "Board.h"
include <cstdlib>
include <ctime>

namespace std;

Item::Item()
{
    x = 5;
    y = 0;
    srand(time(0))
    type = rand() / (RAND_MAX/7));
    orientation = Orientation.UP;
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
