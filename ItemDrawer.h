

#IFNDEF ITEMDRAWER_H
#DEFINE ITEMDRAWER_H

#DEFINE NB_TYPES 7
#DEFINE NB_ORI 4

class ItemDrawer
{
    public:
        bool item[NB_TYPES][NB_ORI][5][5];

        static void drawItem();

    private:

};

#ENDIF