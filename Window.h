#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>
#include "Field.h"
#include "DrawingArea.h"


class Window : public Gtk::Window
{
public:
	Window(char ch,int sk);
	virtual ~Window();
protected:
	void on_my_show();
	bool on_button_press_event(GdkEventButton* button_event);
	bool on_delete_event(GdkEventAny* any_event);
	static void* reading_thread_wraper(void* obj);
	DrawingArea dwg;
  	Field fld;
	void click_handler(int x,int y);
	int sockfd;
	char buf[6];
	void reading_thread();
	bool clientTurn; //Истинно, если сейчас ход игрока
};

#endif //GTKMM_EXAMPLEWINDOW_H
