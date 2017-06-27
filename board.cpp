#DEFINE W 10
#DEFINE H 20

class board
{
    // koordinatenfeld 0 <= x <=  9
    //                 0 <= y <= 19

    private currentItem;
    private uint8_t b[W][H];

    public board()
    {
        init();
    }

    public void run()
    {
        bool lost = false;
        while(!lost)
        {
            currentItem.sink();
        }
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