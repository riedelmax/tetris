#DEFINE W 10
#DEFINE H 20

class Board
{
    // koordinatenfeld 0 <= x <=  9
    //                 0 <= y <= 19

    private Item currentItem;
    private uint8_t b[W][H];
    private Controller ctrl;

    public Board(Controller ctrl)
    {
        init();
        this.ctrl = ctrl;
    }

    public void run()
    {
        bool lost = false;
        while(!lost)
        {
            // item 1 square down
            currentItem.sink();
            
            // check if item is moved
            while(ctrl.isMoving())
            {

            }

            // check if item is dropped
            if(ctrl.dropButtonPressed())
            {
                drop();
            }

        }
    }

    private void drop()
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
    public bool checkCollision()
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

    private void init() 
    {
        for(int i = 0; i < W; i++)
        {
            for(int j = 0; j < H; j++)
            {
                b[i][j] = 0;
            }
        }
    }

}