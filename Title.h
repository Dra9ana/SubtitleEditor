#ifndef TITLE_H
#define TITLE_H
#include "Time.h"
#include "Style.h"
class Title {
public:
	Title();
	Title(int start, int length, string text);
	Title(int start, int length);
	~Title();
	int loadTitle(string subtitles, int& position);
	int getIndex();
	Title* getNext();
	void setIndex(int i);
	void setNext(Title* other);
	void setTime(int start, int length);
	string saveTitle();
	string saveText();
	bool addStyle(int start, int end, char c);
	void setStyle(Style* s);
	bool clearStyle();
	void writeStyle();
	void shiftForward(int ms);
	void shiftBackward(int ms);
	int getBegTime();
	int getEndTime();
	int countWords();
	void readMWords(string text, int& i,int m);
	bool readStyle();
	void clearTitle();
	bool fixLongLines(int max);
	bool fixLongTime(Title*prev,int ms);
    bool titleOverlapping(unsigned start, unsigned end);
	
private:
	int index;
	Time* begTime;
	Time* endTime;
	string text;
	Style* styles;
	Title* next;
	void transferStyle(Title* t,int i,int pos);
	bool readIndex(string subtitles, int& position);
	bool readText(string subtitles, int& position);
	bool readLine(string subtitles, int& position);
	bool checkArrow(string subtitles, int& position);
	bool checkMaxChar(int max);
	bool writeError(string message);
	

	

};
#endif
