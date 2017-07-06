#define W 10
#define H 20
//

#include "Item.h"
#include "Controller.h"

class Board
{
    private:
        Item currentItem;
        uint8_t b[W][H];
        Controller ctrl;
        void drop();
        void init();

    public:
        Board(Controller ctrl);
        void run();
        bool checkCollision();
        void draw();
        
};
