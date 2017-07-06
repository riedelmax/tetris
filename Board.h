#DEFINE W 10
#DEFINE H 20

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
