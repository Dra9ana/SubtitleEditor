#ifndef SELECTION_H
#define SELECTION_H
#include"Title.h"
class Selection {
public:
	Selection();
	Selection(Title*start,Title*end);
	Selection(Selection* sel);
	string saveSelection();
	void setStart(Title* t);
	void setStart(Selection* s);
	void setEnd(Title* t);
	void setEnd(Selection* s);
	void setNext(Selection* other);
	void setBeforeStart(Title* bStart);
	Title* getBeforeStart();
	Title* getStart();
	int getStartID();
	Title* getEnd();
	int getEndID();
	Selection* getNext();
	
	bool styleSelection(int first, int last, char c);
	bool removeStyle();
	bool shiftBackward(int ms);
	bool shiftForward(int ms);
	void deleteSelection(Title*&head);
	bool fixLongLines(int max);
	bool fixLongTime(int ms);
	bool isInSelection(Title* t);
private:
	Title* start, *end, *beforeStart;
	Selection* next;
};
#endif
