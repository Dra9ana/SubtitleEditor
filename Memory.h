#ifndef MEMORY_H
#define MEMORY_H

#include"Selection.h"


class Memory {
public:
	Memory(string subtitles, string sel);
	void setNext(Memory* m);
	string getSubtitle();
	Memory* getNext();
	string getSel();
	
private:
	string selection;
	string subtitle;
	Memory* next;


};
#endif
