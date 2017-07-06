#ifndef BOARD_H
#define BOARD_H

#define W 10
#define H 20
//

#include "Item.h"
#include "Controller.h"
#include "ItemDrawer.h"

class Board
{
    private:
        Item currentItem;
        uint8_t b[W][H];
        Controller ctrl;
        ItemDrawer iDraw;
        void drop();
        void init();

    public:
        Board(Controller ctrl);
        void run();
        bool checkCollision();
        void draw();
        
};


#endif
