#include "Memory.h"

Memory::Memory(string s, string sel)
{
	subtitle =s;
	selection = sel;
}

void Memory::setNext(Memory* m)
{
	this->next = m;
}

string Memory::getSubtitle()
{
	return subtitle;
}

Memory* Memory::getNext()
{
	return this->next;
}

string Memory::getSel()
{
	return this->selection;
}
