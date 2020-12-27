#ifndef SUBTITLE_EDITOR_H
#define SUBTITLE_EDITOR_H
#include "Title.h"
#include"Selection.h"
#include"Memory.h"

class SubtitleEditor {
public:
	   SubtitleEditor();
	  ~SubtitleEditor();
	  int loadSubtitles(string subtitles);
	  string saveSubtitles();
	  void addToSelection(int start, int end);
	  bool deleteFromSelection(int start, int end);
	  void setSelection(string s);
	  string getSelection();
	  void boldText(int start, int end);
	  void underlineText(int start, int end);
	  void italicText(int start, int end);
	  void deselect();
	  void removeStyle();
	  void shiftForward(int ms);
	  void shiftBackward(int ms);
	  void insertSubtitle(int start, int length, string text);
	  bool insertTitle(Title* t);
	  void deleteSubtitles();
	  void merge(string subtitles);
	  Title* getHead();
	  void fixLongLines(int max);
	  void fixLongTime(int ms);
	  void outputSelection();
	  void outputSection();
	  void outputStack();
	  void undo();
	  void clearSubtitle();
	  
private:
	Title* head,*tail;
	Selection* selection;
	Memory*s;
    
	void delHead();
	Title* findTitle(int start);
	void updateIndexes();
	void clearStack();
	void addToStack();
	void deleteFromStack();
	void deleteCurrentSelection(Selection* &beforeTemp, Selection*&temp);
	int writeError(string message, Title* t);
	
	
	


};
#endif
