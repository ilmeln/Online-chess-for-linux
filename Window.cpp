#include "Window.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>





void Window::reading_thread()
{
	char rcv_buf[6];
	while(true) {
		read(sockfd,(char*)&rcv_buf,sizeof(rcv_buf));
		if(rcv_buf[0]=='E') {
			printf("You enemy disconnect\n");
			close();
			pthread_exit(0);
		}
		char state = rcv_buf[4];
		if(state!='B'&&state!='W'&&state!='D'&&state!='C')
			continue;
		Point from;
		int x,y;
		from.y = rcv_buf[0] - '0';
		from.x = rcv_buf[1] - '0';
		y = rcv_buf[2] - '0';
		x = rcv_buf[3] - '0';
		fld.makeTurn(from,Point(x,y));
		dwg.drawField(fld);
		switch(state) {
			case 'B':
				printf("Black win\n");
				sleep(5);
				close();
				pthread_exit(0);
			case 'W':
				printf("White win\n");
				sleep(5);
				close();
				pthread_exit(0);
			case 'D':
				printf("Draw\n");
				sleep(5);
				close();
				pthread_exit(0);
			case 'C':
				if(clientTurn) {
					printf("Error\n");
					close();
					pthread_exit(0);
				}
				clientTurn  = true;
				break;
			default:
				break;
		}
	}
}


void* Window::reading_thread_wraper(void* obj)
{
	static_cast<Window*>(obj)->reading_thread();
}

Window::Window(char ch,int sk)
{
  	set_title ("Chess");
	if(ch=='W') {
		fld.side = White;
		clientTurn = true;
		printf("You are playing white\n");
	}
	else if(ch=='B') {
		fld.side = Black;
		clientTurn = false;
		printf("You are playing black\n");
	}
  	set_default_size(800, 800);
	add_events(Gdk::BUTTON_PRESS_MASK);
	sockfd = sk;
	dwg.signal_show().connect(sigc::mem_fun(this,&Window::on_my_show));
	add(dwg);
	dwg.drawField(fld);
	show_all();
}



Window::~Window()
{
}


void Window::on_my_show()		//Вызывется когда окна появляется на экране. Запускает читающий поток.
{
	pthread_t tid;
	pthread_create(&tid,0,reading_thread_wraper,this);
	pthread_detach(tid);
}


bool Window::on_delete_event(GdkEventAny* any_event)	//Вызывается при закрытие окна
{
	write(sockfd,"E",sizeof("E"));
	return Gtk::Window::on_delete_event(any_event); 
}

bool Window::on_button_press_event(GdkEventButton* button_event)	//Вызывается при нажатии кнопки мыши на окно
{
	if(clientTurn&&(button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 1)) {
		click_handler(button_event->x/100+1,button_event->y/100+1);
	}
	return true;
}



void Window::click_handler(int x,int y) //Обработка нажатий
{
	fld.click(x,y);
	if(fld.endTurn) {				//При завершении хода перерисовывает доску и проверяет состояние игры.
		dwg.drawField(fld);			//Если игра завершилась, закрывается окно.
		Cell end = fld.gameState();
		char state;
		switch(end) {
			case Black:
				printf("Black win\n");
				state = 'B';
				break;
			case White:
				printf("White win\n");
				state = 'W';
				break;
			case Draw:
				printf("Draw\n");
				state = 'D';
				break;
			default:
				state = 'C';
				break;
		}
		Point from = fld.getFromCoord();
		int coords = from.y*1000+from.x*100+y*10+x;
		sprintf(buf,"%d%c",coords,state);
		write(sockfd,buf,sizeof(buf));
		clientTurn = false;
		if(state=='D'||state=='B'||state=='W') {
			sleep(5);
			close();
		}
	}
	else if(fld.change)
		dwg.drawField(fld);
}


