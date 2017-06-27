include "Item.h"
include <cstdlib>
include <ctime>

namespace std;

class Item
{
    public Item()
    {
        x = 5;
        y = 0;
        srand(time(0))
        type = rand() / (RAND_MAX/7));
        orientation = Orientation.UP;
    }

    private void sink()
    {
        y++;
    }
}