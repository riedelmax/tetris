
#include "Board.h"

Board::Board(Controller ctrl)
{
    init();
    this.ctrl = ctrl;
}

void Board::run()
{
    bool lost = false;
    while(!lost)
    {
        // item 1 square down
        currentItem.sink();
        
        // check if item is moved
        while(ctrl.isMoving() != 0)
        {
            if(ctrl.isMoving() == -1)
            {
                currentItem.left();
            }
            else
            {
                currentItem.right();
            }
        }

        // check if item is dropped
        if(ctrl.dropButtonPressed())
        {
            drop();
        }

    }
}

void Board::drop()
{
    while(checkCollision())
    {
        currentItem.sink();
    }
}

/*
* returns true if current item can be moved down 1 square
* returns false if current item cant be moved down 1 square
*/
bool Board::checkCollision()
{
    bool item[5][5] = ItemDrawer.items[currentItem.getType()][currentItem.getOrientation()];
    int y = currentItem.getY() + 1;
    int x = currentItem.getX();
    for(int i = x; i < x+5; i++)
    {
        for(int j = y; j < y+5; j++)
        {
            if((board[i][j] != 0) && (item[i-x[j-y] != 0]))
            {
                return false;
            }
        }
    }
    return true;
}

void Board::init() 
{
    for(int i = 0; i < W; i++)
    {
        for(int j = 0; j < H; j++)
        {
            b[i][j] = 0;
        }
    }
}
