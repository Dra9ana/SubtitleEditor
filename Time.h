#ifndef TIME_H
#define TIME_H
#include<iostream>
#include<string>

using namespace std;

class Time {
public:Time();
	  bool readTime(string subtitles, int& position);
	  void clearTime();
	  string saveTime();
	  int getH();
	  int getS();
	  int getMin();
	  int getMs();
	  int getCount();
	  void shiftForward(int ms);
	  void shiftBackward(int ms);
	  

private:
	int h;
	int min;
	int s;
	int ms;
	int count;
	int read2Dec(string subtitles, int& position, bool &error);
	int readDigit(string subtitles, int& position, bool &error);
	bool writeError(string message);
	void convertMsIntoTime();
	

};
#endif
