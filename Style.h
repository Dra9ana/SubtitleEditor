#ifndef STYLE_H
#define STYLE_H
#include<iostream>
#include<string>

using namespace std;
class Style {
public:
	Style();
	Style(int beg, int end, char c);
	void clearStyle();
	void writeStyle(string& text);
	bool readStyle(string& text, bool& error);
	int getOpenPosition();
	int getClosedPosition();
	Style* getNext();
	void setNext(Style* s);
	bool styleOverlapping(int start, int end);
	void tellStyle();
	void updatePositions(int pos);
private:
	char type;
	int  openPosition;
	int  closedPosition;
	Style* next;
	
};
#endif
