#include "Time.h"
#include<iostream>
#include<string>

using namespace std;

//konstruktor
Time::Time()
{   
	this->clearTime();
}

//cita vreme iz stringa
bool Time:: readTime(string subtitles, int& position)
{ 
	bool error=true;
	this->h = this->read2Dec(subtitles, position,error);
	if (subtitles[position] != ':' || this->h > 99) { return writeError("Nema prve dvotacke u vremenu ili su sati veci od 99");  }
	position ++;
	this->min = this->read2Dec(subtitles, position,error);
	if (subtitles[position] != ':' || this->min > 59) { return writeError("Nema druge dvotacke u vremenu ili su minuti veci od 59");  }
	position ++;
	this->s = this->read2Dec(subtitles, position,error);
	if (subtitles[position] != ',' || this->s > 59) { return writeError("Nema zareza pre milisekund ili su sekundi veci od 59");  }
	position ++;
	this->ms = this->read2Dec(subtitles, position,error) *10+ readDigit(subtitles,position, error);
	if (this->ms > 999 || !error) { return writeError("Neki od brojeva u vremenu nije validan"); }
	position++;

	this->count = (((this->h * 60 + this->min) * 60 + this->s) * 1000 + this->ms);

	return true;
	
}

//resetuje vreme
void Time::clearTime()
{
	this->h = 0;
	this->min = 0;
	this->s = 0;
	this->ms = 0;
	this->count = 0;
}

//geteri
int Time::getH()
{
	return this->h;
}

int Time::getS()
{
	return this->s;
}

int Time::getMin()
{
	return this->min;
}

int Time::getMs()
{
	return this->ms;
}

int Time::getCount()
{
	return this->count;
}

//vraca vreme u obliku stringa
string Time::saveTime()
{
	string ret = "";

	this->h < 10 ? ret.append("0" + to_string(this->h)) : ret.append(to_string(this->h));
	ret.append(":");
	this->min < 10 ? ret.append("0" + to_string(this->min)) : ret.append(to_string(this->min));
	ret.append(":");
	this->s < 10 ? ret.append("0" + to_string(this->s)) : ret.append(to_string(this->s));
	ret.append(",");

	if (this->ms < 10)
		ret.append("00" + to_string(this->ms));
	else {
		if (this->ms < 100)
			ret.append("0" + to_string(this->ms));
		else
			ret.append(to_string(this->ms));
	}
		
	return ret;
}

// pomera vreme za ms milisekundi unapred
void Time::shiftForward(int ms)
{
	this->count = this->count+ms;
	convertMsIntoTime();
}

//pomera vreme za ms milisekundi unazad
void Time::shiftBackward(int ms)
{
	if (this->count > ms) {// vreme ne sme da ode u negativno
		this->count = this->count - ms;
		convertMsIntoTime();
	}
}

//cita dve cifre iz stringa i proverava njihovu ispravnost
int Time::read2Dec(string subtitles, int& position, bool &error)
{
	int ret = readDigit(subtitles, position, error);
	position++;
	ret=ret*10 + readDigit(subtitles,position,error);
    position ++;

	return ret;
}

//cita jednu cifru iz stringa i proverava ispravnost
int Time::readDigit(string subtitles, int& position, bool &error)
{
	if (position >= subtitles.length() || subtitles[position] < '0' || subtitles[position]>'9') { error = false; return 0; }
	   
	    return (subtitles[position] - '0');
	
		
}

//ispisuje gresku, resetuje vreme i vraca false
bool Time::writeError(string message)
{
	cout << message << endl;
	this->clearTime();
	return false;
}

//azurira sate minute sekunde i milisekunde iz count-a
void Time::convertMsIntoTime()
{
	this->ms = this->count % 1000;
	this->s = ((this->count - this->ms) / 1000) % 60;
	this->min = (((this->count - this->ms) / 1000) / 60) % 60;
	this->h = ((((this->count - this->ms) / 1000) / 60) / 60) % 24;
}


