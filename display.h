#ifndef DISPLAY_H
#define DISPLAY_H

#include "scene.h"

using namespace std;

class Display final
{
private:
	ALLEGRO_EVENT_QUEUE *_events;
	ALLEGRO_DISPLAY *_display;
	
	Scene _scene;
	
	bool _show_info;
	
	//
	
	void showsInfo(); // shows info in screen
	
	Display(); // starts the game
	
	Display(const Display &) = delete;
	Display(Display &&) = delete;
	
	void operator=(const Display &) = delete;
	void operator=(Display &&) = delete;
public:
	// this function stays in the END of the game loop
	void update(); // updates the display buffer
	
	~Display();
	
	const bool &showInfo();
	
	/// Gets singleton instance
	static Display &instance();
};

#endif
