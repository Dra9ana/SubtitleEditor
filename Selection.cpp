#include "Selection.h"

//konstruktori
Selection::Selection()
{
	this->beforeStart = nullptr;
	this->start = this->end = nullptr;
	this->next = nullptr;
}

Selection::Selection(Title* start, Title* end)
{
	this->start = start;
	this->end = end;
	this->next = nullptr;
	this->beforeStart = nullptr;
}

Selection::Selection(Selection* sel)
{
	this->start = sel->getStart();
	this->end = sel->getEnd();
	this->beforeStart = sel->getBeforeStart();
	this->next = nullptr;
}

string Selection::saveSelection()
{
	return "(" + to_string(this->getStartID()) + "," + to_string(this->getEndID()) + ")";
}


//setter metode
void Selection::setStart(Title* t)
{
	this->start = t;
}

void Selection::setStart(Selection* s)
{
	this->start = s->getStart();
	this->beforeStart = s->getBeforeStart();
}

void Selection::setEnd(Title* t)
{
	this->end = t;
}

void Selection::setEnd(Selection* s)
{
	this->end = s->getEnd();
}

void Selection::setNext(Selection* other)
{
	this->next = other;
}

void Selection::setBeforeStart(Title* bStart)
{
	this->beforeStart = bStart;
}

//getter metode
Title* Selection::getBeforeStart()
{
	return this->beforeStart;
}

Title* Selection::getStart()
{
	return this->start;
}

int Selection::getStartID()
{
	return this->start->getIndex();

}

Title* Selection::getEnd()
{
	return this->end;
}

int Selection::getEndID()
{
	return this->end->getIndex();

}

Selection* Selection::getNext()
{
	return this->next;
}

//dodaje novi stil u selekciju
bool Selection::styleSelection(int first, int last, char c)
{
	bool success = false;
	Title* temp = start;
	while (temp != end->getNext()) {
		if (temp->addStyle(first, last, c)) success = true;;
		temp = temp->getNext();
	}
	return success;
}

//nrise listu stilova iz svih titlova u selekciji
bool Selection::removeStyle()
{
	bool success = false;
	Title* temp = start;
	while (temp != end->getNext()) {
		if(temp->clearStyle())success= true;
		temp = temp->getNext();
	}
	return success;
}

//pomera vreme unazad za ms milisekundi u svim titlovima u selekciji
bool Selection::shiftBackward(int ms)
{
	Title* temp = this->start->getNext();
	if (beforeStart == nullptr || beforeStart->getEndTime() < (start->getBegTime() - ms))
	{
		this->start->shiftBackward(ms);
	}
	else return false;//titl bi se preklopio sa prethodnim->zahtevana operacija nije dozvoljena i ne izvrsava se

	while (temp != this->end->getNext()) {
		temp->shiftBackward(ms);
		temp = temp->getNext();
	}
	return true;//metoda se validno izvrsava
}


bool Selection::shiftForward(int ms)
{
	Title* temp = this->start;
	if (this->end->getNext() == nullptr || (this->end->getEndTime() + ms) < this->end->getNext()->getBegTime())
	{
		this->end->shiftForward(ms);
	}
	else return false;//preklapanje titla sa sledecim->zahtevana operacija nije dozvoljena i ne izvrsava se

	while (temp != this->end) {
		temp->shiftForward(ms);
		temp = temp->getNext();
	}
	return true;//metoda se validno izvrsava

}

//brise titlova koji su u selekciji iz sekvence titlova
void Selection::deleteSelection(Title*& head)
{
	Title* temp;
	while (start && start != end) {//obrisi sve do poslednjeg ako selekcija nije prazna
		temp = start;
		start = start->getNext();
		delete temp;

	}

	if (this->beforeStart != nullptr)//ako start nije bio glava sekvence prelancaj ga na titl posle kraja selekcije
		this->beforeStart->setNext(end->getNext());
	else 
		head = end->getNext();//ako jeste glava sekvence postavi glavu na titl posle kraja selekcije

	delete end;//obrisi i poslednji

	beforeStart = start = end = nullptr;//resetuj selekciju

}

//primenjuje fixLonglinse na titlove u selekciji
bool Selection::fixLongLines(int max)
{
	bool success = false;
	Title* temp = start;
	while (temp != end->getNext()) {
		if (temp->fixLongLines(max)) success = true;
		temp = temp->getNext();
	}
	return success;
}

//primenjuje fixLongTime na titlove u selekciji
bool Selection::fixLongTime(int ms)
{
	bool success = false;
	Title* temp = this->start, * prev = beforeStart, * prev1;//prev cuva titl pre titla koji se trenutno obradjuje

	while (temp != this->end->getNext()) {
		 if(temp->fixLongTime(prev, ms))success=true;
		 //ako je izvrsena operacija prev je poslednji novonastali titl
		 prev = temp; //ako operacija nije  mogla da se izvrsi prev je temp
		temp = prev->getNext();//temp postaje prvi posle prethodnog
	}

	return success;

}

bool Selection::isInSelection(Title* t)
{
	return t->getIndex()<=getEndID() &&  t->getIndex()>=getStartID();
}


