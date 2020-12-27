#include "Style.h"

//konstruktori
Style::Style()
{
    
        this->next = nullptr;
        this->clearStyle();
        
}

Style::Style(int beg, int end, char c)
{
    this->openPosition = beg;
    this->closedPosition = end+1;
    this->type = c;
    this->next = nullptr;
}

//retetuje style
void Style::clearStyle()
{
    this->closedPosition = this->openPosition = 0;
     this->type = 'b';
}

//ispisuje style
void Style::writeStyle(string& text)
{
    string t;
    //kovertovanje char u strin radi lakseg upisa
    switch (this->type) {
    case'i': t = "i"; break;
    case'u': t = "u"; break;
    case'b': t = "b"; break;
    
    }
    text.insert(closedPosition, "</" + t + ">");
    text.insert(openPosition, "<" + t+ ">");
    //izbrisano je prvo otvoren pa zatvoren zato moramo da upisemo zatvoren pa otvoren 
    
}

//cita style iz teksta
bool Style::readStyle(string& text, bool& error)
{
    
    this->type = type;
    int a = text.find("<");//pronalazi prvi potencijalni znak stila
    if (a != -1) {//ako ga je nasao
        if ((text[a + 1] != 'i' && text[a + 1] != 'u' && text[a + 1] != 'b')||text[a+2]!='>') {//i nije validnog oblika
            cout << "Nepostojeci stil ili je yatvoren stil pre otvorenog" << endl; //prijavi gresku i prekini citanje
            error = false; 
            return false; 
        }
        else {//ako jeste neki od validnih stilova
              this->type = text[a + 1];//sacuvaj tip stila
              text.erase(a, 3);//obrisi otvoren stil iz teksta
             }
    }
    else return false;//nije nasao ni jedan stil ->prekid citanja
    
    int b = text.find("<" );//nalazi sledeci potencijalni stil
    if (b == -1) { cout << "Neki od stilova nije uparen" << endl; error = false; return false; }//ako stil nije zatvoren javlja gresku i prekida citanje
    else  {
               if (text[b+1]!='/' || text[b + 2] != this->type || text[b+3]!='>')//ako je stil otvoren ili drugog tipa ili nije validan vraca gresku
               { cout << "Ugnjezdavanje stilova ili nevalidan zatvoren stil" << endl; error = false; return false; }
               
               text.erase(b,4);//ako do tog momenta nije vracena greska stil je validan i zatvoren i ne preklapa se ni sa jednim
                               //brise se iz teksta zatvoren deo       
               this->openPosition = a;//cuva pociiciju pocetka stila
               this->closedPosition = b;//cuva poziciju kraja stila
        
               return true;//vraca validnost
              }
    
}

//getter metode
int Style::getOpenPosition()
{
    return this->openPosition;
}

int Style::getClosedPosition()
{
    return this->closedPosition;
}

Style* Style::getNext()
{
    return this->next;
}

//setter metode
void Style::setNext(Style* s)
{
    this->next = s;
}

//proverava da li se stilovi preklapaju
bool Style::styleOverlapping(int start, int end)
{
    int a = (this->closedPosition >= end) ? 1 : -1;
    int b = (this->openPosition >= start) ? 1 : -1;
    int c = (this->openPosition <= end) ? 1 : -1;
    int d = (start <= this->closedPosition) ? 1 : -1;

    if (a * b < 0 || c * d>0)
    {
        return true;
    }
    return false;
    
}

//pomocna funkcija za debagovanje
void Style::tellStyle()
{
    cout << this->openPosition << "," << this->closedPosition << endl;
}

//
void Style::updatePositions(int pos)
{
    if (this->openPosition >= pos)this->openPosition-=pos;
    if (this->closedPosition >= pos)this->closedPosition-=pos;
}


