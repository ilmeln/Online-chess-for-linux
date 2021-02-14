#ifndef FIELD_H
#define FIELD_H


#include <vector>

enum Cell {Empty,White,Black,WhiteQ,BlackQ,pTurn,Continue,Draw};


struct Point {
    int x;
    int y;
    Point()
    {
        x = y = 0;
    }
    Point(int _x,int _y)
    {
        x = _x;
        y = _y;
    }
};


class Field
{
    public:
	Field();
        void click(int x,int y);
        std::vector<Point> possibleTurns(int x,int y);
        void makeTurn(Point from,Point to);
        bool change;		//Истинно при изменении поля
        bool endTurn;		//Истино при окончании хода
        Point getFromCoord();
        Cell gameState();
        std::vector<Cell> operator[](int y)
        {
            return data[y];
        }
	Cell side;		//Сторона, за которую играет игрок
    private:
        std::vector<std::vector<Cell>> data;
        Point chChess;		//Координаты выбранной игроком шашки
        int sz;
        int whiteChess;     
        int blackChess; 	//Количество белых и черных шашек сооствественно
        
};

#endif // FIELD_H
