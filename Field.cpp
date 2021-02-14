#include "Field.h"

#include <cmath>
#include <stdio.h>


Field::Field()
{
	sz = 10;
    chChess.x = chChess.y = -1;
    blackChess = whiteChess = 12;
    data.resize(sz);
	for(int i=0;i<sz;++i)
		data[i].resize(sz);
    for(int i=1;i<sz-1;++i) {
        for(int j=1;j<sz-1;++j) {
            if(((i%2==0)&&(j%2==1))||((i%2==1)&&(j%2==0))) {
                if(i<4)
                    data[i][j] = Black;
                else if(i>5)
                    data[i][j] = White;
                else
                    data[i][j] = Empty;
            }
            else
                data[i][j] = Empty;
        }
    }
}





std::vector<Point> Field::possibleTurns(int x,int y)		//Поиск возможных ходов для шашки
{
    std::vector<Point> pT;
    if(data[y][x]==White) {							
    	if(data[y-1][x+1]==Empty)
    		pT.push_back(Point(x+1,y-1));
    	else if(((data[y-1][x+1]==Black)||(data[y-1][x+1]==BlackQ))&&(data[y-2][x+2]==Empty))
    		pT.push_back(Point(x+2,y-2));
    	if(data[y-1][x-1]==Empty)
    		pT.push_back(Point(x-1,y-1));
    	else if(((data[y-1][x-1]==Black)||(data[y-1][x-1]==BlackQ))&&(data[y-2][x-2]==Empty))
    		pT.push_back(Point(x-2,y-2));
   		if(((data[y+1][x+1]==Black)||(data[y+1][x+1]==BlackQ))&&(data[y+2][x+2]==Empty))			//Если это обычная шашка, то в зависимости от цвета просматриваются клетки в напрвлении движения
			pT.push_back(Point(x+2,y+2));															//Если они пустые, то добавляются в вектор ходов, если там стоит шашка проивоположного цвета, то
		if(((data[y+1][x-1]==Black)||(data[y+1][x-1]==BlackQ))&&(data[y+2][x-2]==Empty))            //смотрится следующая за ней, и в случае если она пустая, добавляется в вектор
			pT.push_back(Point(x-2,y+2));															//Так же просматриваются клетки сзади, и при наличии в них шашек противоположного цвета,в вектор 
	}																								//добавляются пустые клетки за ними, если они есть
	else if(data[y][x]==Black) {
		if(data[y+1][x+1]==Empty)
			pT.push_back(Point(x+1,y+1));
		else if(((data[y+1][x+1]==White)||(data[y+1][x+1]==WhiteQ))&&(data[y+2][x+2]==Empty))
			pT.push_back(Point(x+2,y+2));
		if(data[y+1][x-1]==Empty)
			pT.push_back(Point(x-1,y+1));
		else if(((data[y+1][x-1]==White)||(data[y+1][x-1]==WhiteQ))&&(data[y+2][x-2]==Empty))
			pT.push_back(Point(x-2,y+2));
		if(((data[y-1][x+1]==White)||(data[y-1][x+1]==WhiteQ))&&(data[y-2][x+2]==Empty))
			pT.push_back(Point(x+2,y-2));
		if(((data[y-1][x-1]==White)||(data[y-1][x-1]==WhiteQ))&&(data[y-2][x-2]==Empty))
			pT.push_back(Point(x-2,y-2));
    }
    if(data[y][x]==WhiteQ||data[y][x]==BlackQ) {					//В случае если это дамка, запускается цикл, который идет в четыре возможные стороны.Сначала он смотрит ближайшую клетку, и
        Cell enemy = White;											//если в ней шашка противника, а за ней пустая клетка,она добавляется в вектор.Иначе добавляются все пустые клетки, пока цикл
        Cell enemyQ = WhiteQ;										//не дойдет до края доски или пока не наткнется на шашку.
        if(data[y][x]==WhiteQ)
            enemy = Black;
            enemyQ = BlackQ;
		for(int dY = -1;dY<=1;dY+=2) {
			for(int dX = -1;dX<=1;dX+=2) {
				if((data[y+dY][x+dX]==enemy||data[y+dY][x+dX]==enemyQ)&&data[y+dY*2][x+dX*2]==Empty)
					pT.push_back(Point(x+dX*2,y+dY*2));
				else if(data[y+dY][x+dX]==Empty) {
				    int _x = x +dX;
				    int _y = y+ dY;
					int pY = (dY>0)? 9 : 0;
					int pX = (dX>0)? 9 : 0;
				    while(_x!=pX&&_y!=pY&&data[_y][_x]==Empty) {
				        pT.push_back(Point(_x,_y));
				        _x+=dX;
				        _y+=dY;
				    }
				}
			}
		}
    }
    std::vector<Point> vec;
    for(auto p : pT) {
        if(p.x>0&&p.x<9&&p.y>0&&p.y<9)		
            vec.push_back(p);
    }
    return vec;
}


Point Field::getFromCoord()		//Используется в классе Window для получения исходной координаты, после того, как закончится ход
{
	Point p = chChess;
    chChess.x = chChess.y = -1;
    return p;
}

void Field::makeTurn(Point from,Point to)	//Функция хода
{
    if(std::abs(from.x-to.x)==2) {		//Если происходит ход через клетку, проверяется есть ли между ней и исходной 
        int checkX = (to.x - from.x)/2+from.x;  //шашка, и если есть то уменьшается счетчик шашек ее цвета и она убирается
        int checkY = (to.y - from.y)/2+from.y;
        if(data[checkY][checkX]==Black||data[checkY][checkX]==BlackQ)
            --blackChess;
        else if(data[checkY][checkX]==White||data[checkY][checkX]==White)
            --whiteChess;
        data[checkY][checkX]=Empty;
    }
    Cell c = data[from.y][from.x];
    if(to.y==1&&c==White)		//Если шашка достигла противоположного конца доски, то она становится дамкой
        c = WhiteQ;
    else if(to.y==8&&c==Black)
        c = BlackQ;
    data[to.y][to.x] = c;
    data[from.y][from.x] = Empty;

}


Cell Field::gameState()		//Возвращает текущее состояние игры
{
    if(blackChess==0)
        return White;
    else if(whiteChess==0)
        return Black;
    bool whiteTurn = false;
    bool blackTurn = false;
    for(int i = 1;i<9;++i) {							//Этот цикл пробегается по всему полю, и для найденных шашек смотрит количество возможных ходов.
        for(int j=1;j<9;++j) {							//Если есть хотя бы по одному ходу у каждой из сторон значит игра продолжается.
            if(data[i][j]!=Empty&&data[i][j]!=pTurn) {			
                auto vec = possibleTurns(j,i);
                if(vec.size()!=0) {
                    if(data[i][j]==White||data[i][j]==WhiteQ)
                        whiteTurn = true;
                    else if(data[i][j]==Black||data[i][j]==BlackQ)
                        blackTurn = true;
					if(whiteTurn&&blackTurn)
    					return Continue;
                }
            }
        }
    }
    return Draw; //Если шашки есть у обоих игроков, но для них нет ходов, значит ничья
}


void Field::click(int x,int y)	//Основная функция
{
    change = endTurn = false;
    if((data[y][x]==side)||(side==White&&data[y][x]==WhiteQ)||(side==Black&&data[y][x]==BlackQ)) {		
        if(chChess.x!=-1||chChess.y!=-1) {		//Если была выбрана шашка, на доске могут остаться возможные ходы для нее, которые убираются в этом цикле
            for(int i=0;i<sz;++i) {
                for(int j=0;j<sz;++j) {
                    if(data[i][j]==pTurn) {
                        data[i][j] = Empty;
                        change = true;
                    }
                }
            }
        }
        auto vec = possibleTurns(x,y);
        if(vec.size()==0)
            return;
        for(auto p : vec)
            data[p.y][p.x] = pTurn;		//Если есть ходы для этой шашки,то они записываются на доску и запоминаются ее координаты 
        change = true;
        chChess.x = x;
        chChess.y = y;
        return;
    }
    else if(data[y][x]==pTurn) {  //Если нажатие происходит на клетку помеченную на возможный ход, то вызывается функция для хода	
        Point to(x,y);
        makeTurn(chChess,to);						
        endTurn = true;
    }
    for(int i=0;i<sz;++i) {	 //При нажатие в любое другое место и после хода с доски убираются все возможные ходы
        for(int j=0;j<sz;++j) {
            if(data[i][j]==pTurn) {
                data[i][j] = Empty;
                if(!endTurn)
                    change = true;
            }
        }
    }
}




