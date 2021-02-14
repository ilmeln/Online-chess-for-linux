#include "DrawingArea.h"
#include <stdio.h>


DrawingArea::DrawingArea()
{
    field = Gdk::Pixbuf::create_from_file("field.bmp");
	blackC = Gdk::Pixbuf::create_from_file("black.bmp");
	whiteC = Gdk::Pixbuf::create_from_file("white.bmp");
	blackQ = Gdk::Pixbuf::create_from_file("blackQ.bmp");
	whiteQ = Gdk::Pixbuf::create_from_file("whiteQ.bmp");
	ramka = Gdk::Pixbuf::create_from_file("4.bmp");
    display = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB,0,8,800,800);
}


void DrawingArea::drawField(Field& f)
{
	field->copy_area(0,0,800,800,display,0,0);
	for(int i=1;i<9;++i) {
		for(int j=1;j<9;++j) {
			if(f[i][j]==Black)
				blackC->copy_area(0,0,100,100,display,(j-1)*100,(i-1)*100);
			else if(f[i][j]==White)
				whiteC->copy_area(0,0,100,100,display,(j-1)*100,(i-1)*100);
			else if(f[i][j]==pTurn)
				ramka->copy_area(0,0,100,100,display,(j-1)*100,(i-1)*100);
			else if(f[i][j]==WhiteQ)
				whiteQ->copy_area(0,0,100,100,display,(j-1)*100,(i-1)*100);
			else if(f[i][j]==BlackQ)
				blackQ->copy_area(0,0,100,100,display,(j-1)*100,(i-1)*100);
		}
	}
	queue_draw();
}




bool DrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gdk::Cairo::set_source_pixbuf(cr, display, 0,0);
    cr->rectangle(0, 0, 800, 800);
    cr->paint();
    return true;
}





