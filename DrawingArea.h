#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <gtkmm.h>
#include "Field.h"

class DrawingArea : public Gtk::DrawingArea
{
public:
    DrawingArea();
	void drawField(Field& f);
protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
private:
    Glib::RefPtr<Gdk::Pixbuf> field;
	Glib::RefPtr<Gdk::Pixbuf> blackC;
	Glib::RefPtr<Gdk::Pixbuf> whiteC;
	Glib::RefPtr<Gdk::Pixbuf> blackQ;
	Glib::RefPtr<Gdk::Pixbuf> whiteQ;
	Glib::RefPtr<Gdk::Pixbuf> ramka;
    Glib::RefPtr<Gdk::Pixbuf> display;
};


#endif // DRAWINGAREA_H
