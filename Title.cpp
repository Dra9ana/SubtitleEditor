#include "Title.h"
#include"Time.h"
#include<iostream>

using namespace std;

//konstruktor bez parametara
Title::Title()
{
    this->begTime= new Time();
    this->endTime= new Time();
    this->clearTitle();

}

//konstruktor zadat pocetkom trajanjem i opisom
Title::Title(int start, int length, string text)
{
    this->index = 0;

    this->begTime = new Time();
    this->begTime->shiftForward(start);

    this->endTime = new Time();
    this->endTime->shiftForward(start+length);

    this->text = text;

    this->next = nullptr;
    this->styles = nullptr;
}

//konstruktor zadat pocetkom i trajanjem
Title::Title(int start, int length)
{
    
    this->begTime = new Time();
    this->endTime = new Time();
    this->clearTitle();

    this->begTime->shiftForward(start);
    this->endTime->shiftForward(start + length);
    
}

//destruktor
Title::~Title()
{
    this->clearStyle();
    delete (this->begTime);
    delete (this->endTime);
}

//ucitava jedan titl iz stringa i vraca da li je ispravno ucitan (-1 ako jeste, indeks ako nije)
int Title::loadTitle(string subtitles, int& position)
{

     this->clearTitle();

    
     if (!this->readIndex(subtitles, position)) {return writeError("Broj unosa nije validan");}
     if (subtitles[position] != '\n') { return writeError("Fali novi red nakon indeksa"); }
     position++;

    
     if (!this->begTime->readTime(subtitles, position)) {return writeError("Vreme pocetka nije validno");}
     if (!this->checkArrow(subtitles, position)) { return writeError("Nema strelice izmedju vremena"); }
     position += 5;
    
    
    if (!(this->endTime->readTime(subtitles, position))) { return writeError("Vreme kraja nije validno"); }
    if (subtitles[position] != '\n') { return writeError("Fali novi red nakon vremena"); }
    position++;
    
    if (this->begTime->getCount() > this->endTime->getCount()) { return writeError("vreme pocetka titla je vece of vremena zavrsetka"); }
    
    if (!this->readText(subtitles, position)) { return writeError("Tekst unutar titla ne postoji"); }
    
    if (!this->readStyle()) { return writeError("Greska u stilu"); }
        
    return true;
}

//getter metode
int Title::getIndex()
{
    return this->index;
}

Title* Title::getNext()
{
    return this->next;
}

//setter metode
void Title::setIndex(int i)
{
    this->index = i;
}

void Title::setNext(Title* other)
{
    this->next = other;
}

void Title::setTime(int start, int length)
{
    this->begTime->clearTime();
    this->endTime->clearTime();
    this->begTime->shiftForward(start);
    this->endTime->shiftForward(start + length);
}

//vraca jedan titl u obliku stringa
string Title::saveTitle()
{
    string ret = "";
    ret.append(to_string(this->index));
    ret.append("\n");
    ret.append(this->begTime->saveTime());
    ret.append(" --> ");
    ret.append(this->endTime->saveTime());
    ret.append("\n");
    ret.append(this->saveText());
    ret.append("\n");
    
    return ret ;
}

//ubacuje stil u opis
string Title::saveText()
{
    Style* temp = this->styles;
    string styledText = this->text;//krece od opisa bez stila
    while (temp) {//dokle god postoji stil u listi ubacuje ga
        temp->writeStyle(styledText);//upisuje se od zadnjeg ka prvom jer su citani od prvog ka zadnjem
        temp = temp->getNext();
    }
    return styledText;
}

//dodaje stil u listu stilova sortirano opadajuce
bool Title::addStyle(int start, int end, char c)
{
    bool success = true;
    if (start < 0 || start>=this->text.length())start = 0;//ako je manji od 0 stavi ga na nulu
    if (end >= (this->text.length()-1))end = (this->text.length() - 2);//ako je veci od kraja stavi ga na kraj

    Style* temp = this->styles;

    while (temp && temp->getNext() && temp->getNext()->getOpenPosition()>end)//idemo niz stek dokle god ne naidje na prvi pre njega
    { temp = temp->getNext();}//stek je sortiran opadajuce
                                                                                                        
    if (temp == nullptr) {
        this->styles = new Style(start, end, c);//ako je prvi posavi ga
    }
    else if (!temp->styleOverlapping(start, end) && (temp->getNext() == nullptr || !temp->getNext()->styleOverlapping(start, end))) {//ako se ne preklapa

        Style* node = new Style(start, end, c);//umetni ga izmedju prvog pre i prvog posle
        node->setNext(temp->getNext());
        temp->setNext(node);
    }
    else return false;

    return true;

}

void Title::setStyle(Style* s)
{
    this->styles = s;
}

//dealocira listu stilova
bool Title::clearStyle()
{
    Style* temp;
    if (this->styles == nullptr)return false;

    while (styles) {
        temp = styles;
        styles = styles->getNext();
        delete temp;
    }
    styles = nullptr;
    return true;
}

//pomocna funkcija za debagovanje
void Title::writeStyle()
{
    Style* temp=this->styles;

    while (temp) {
        temp->tellStyle();
        temp = temp->getNext();
    }
}

//pomera vreme pocetka i vreme kraja za ms milisekundi unapred
void Title::shiftForward(int ms)
{
   this->begTime->shiftForward(ms);
   this->endTime->shiftForward(ms);
}

//pomera vreme pocetka i vreme kraja za ms milisekundi unazad
void Title::shiftBackward(int ms)
{
    this->begTime->shiftBackward(ms);
    this->endTime->shiftBackward(ms);
}

//vraca vreme pocetka u milisekundama
int Title::getBegTime()
{
    return this->begTime->getCount();
}

//vraca vreme kraja u milisekundama
int Title::getEndTime()
{
    return this->endTime->getCount();
}


//prebrojava reci u celom tekstu opisa
int Title::countWords()
{
    int count = 0,i=1;
    
    while (i < this->text.length()) {
        if ((this->text[i - 1] != '\t' && this->text[i - 1] != ' ') && (this->text[i] == '\t' || this->text[i] == ' ' || this->text[i] == '\n'))
            count++;
        i++;
    }
    return count;
}

//upisuje prvih m reci iz stringa od indeksa i na kraj opisa
void Title::readMWords(string s, int &i,int m)
{
    int count = 0;
    
    
    while (i < (s.length()-1) && count < m) {//cita sve sem novog reda
        if (s[i] == '\n')// ako je naisao na  novi red menja ga sa space
            s[i] = ' ';
        
        this->text.push_back(s[i]);

        if ( s[i] != '\t' && s[i] != ' ' &&  (s[(i + 1)] == '\t' || s[(i + 1)] == ' '|| s[(i+1)]=='\n')) 
        {count++;}//naisao je na kraj nove reci

        i++;
    }
    this->text.push_back('\n');
    i++;
    
}

//pravi stek stilova koje cita iz teksta
bool Title::readStyle()
{
    bool error = true;
    Style* node, *last=nullptr;
    node = new Style();
    while (node->readStyle(this->text, error)) {//pravi se stek
        node->setNext(this->styles);
        this->styles = node;
        node = new Style();
    }
    delete node;
    return error;
}

//postavlja sve  parametre titla na 0 i dealocira listu 
void Title::clearTitle()
{
    this->index = 0;

    this->begTime->clearTime(); 
    this->endTime->clearTime();

    if (this->styles != nullptr) {
        this->clearStyle();
    }
    this->styles = nullptr;
    this->text="";
    this->next = nullptr;
}

//ako neki red titla ima vise od max nebelih karaktera, deli titl na linije od po najvise max nebelih karaktera
bool Title::fixLongLines(int max)
{
    int notSpaces= 0,lastSpace=0, i=0;

    if (checkMaxChar(max))return false;//proverava da li postoji linija sa vise od max nebelih karaktera

    while (i < (this->text.length()-1)) {//dok ne prelomi poslednju liniju

        notSpaces = 0;//promenljiva koja broji nebele karaktere nakon mesta gde je poslednji put prelomljena linija

        while (i<this->text.length() && notSpaces <= max) {//dok ne prodje kroz max nebelih karaktera ili do kraja

            if (this->text[i] == '\n')this->text[i]=' ';//ako je naisao na novi red brise ga i stavlja prazninu

            (this->text[i] != '\t' && this->text[i] !=' ') ? notSpaces++ : lastSpace = i;//ako naidje na tab ili space pamti ga kao potencijalno mesto za deljenje linije
                                                                                         // ako naidje na nebeli karakter povecava not Spaces

            i++;//ide dalje kroz text
        } 

     this->text[lastSpace] = '\n';//prelamanje se uvek vrsi na poslednjoj belini->postavljamo lastSpace na novi red
     i = lastSpace+1;//zapocinjemo novu iteraciju od karaktera nakon prelamanja
       
    }
    return true;
}

//
bool Title::fixLongTime(Title*prev,int ms)
{
    int duration = this->getEndTime() - this->getBegTime();
    Title* node = nullptr;
    
    if (duration > ms) {
        int n = ceil(duration / (ms - 1.0));//minimalan broj titlova koji moze da obezbedi vreme manje od ms
        int i = 0;//pozicija u tekstu
        int start = this->getBegTime();//pocetak narednog titla koji se pravi
        int numOfWords = ceil(this->countWords() / (n + 0.0));//broj reci koje ce imati novi titlovi
        int gap = floor(duration / (n + 0.0));//broj ms koji ce imati novi titlovi

        while (n > 1) {//dokle god ne dodjes do poslednjeg novog
            node = new Title(start, gap);
            start = start + gap;//azuriraj pocetak
            int pos = i;
            node->readMWords(this->text, i, numOfWords);//procitaj numOfWords reci iz teksta
            this->transferStyle(node, i,pos);
            node->setNext(this);//postavljaj uvek ovaj titl za poslednji 

            if (prev != nullptr)//ako ovaj titl nije glava
            {
                prev->setNext(node);
            }//ulancaj ga na prethodni
            prev = node;//azuriraj prethodni

            n--;

        }
        this->transferStyle(node,this->text.length(),i );
        this->text = (&this->text[i]);//azuriraj test poslednjem
        this->setTime(start, (duration - n * gap));//azuriraj vreme poslednjem
        
    }
    else return false;

    return true;
    
}

//metode za ucitavanje

bool Title::readIndex(string subtitles, int& position)
{
    
    while (position < subtitles.length() && subtitles[position] <= '9' && subtitles[position]>='0')//ide dalje sve dok ne naidje na nesto sto
                                                                                                   //nije broj ili kraj stringa
    {
        this->index = this->index *10 + (subtitles[position] - '0');
        position++;
    }
    
    if (this->index == 0)//ako ni jednom nije usao u petlju ili je indeks nula on je nevalidan
        return false;
    else  //u suprotnom indeks je validan
         return true;
}

bool Title::readText(string subtitles, int& position)
{
   
    while (readLine(subtitles, position));//cita linije dokle god ne naidje na prazan red
    if (this->text == "")return false;//ako nije ucitano nista to ej greska
    return true;//ako je ucitano bilo sta opis je validan

}

bool Title::readLine(string subtitles, int& position)
{
    if (subtitles[position] == '\n' || position >= subtitles.length()) {//ako je prazan red ili kraj stringa javi kraj unosa
        position++;
        return false;
    }
    else 
    {
        while (position<(subtitles.length()-1) && subtitles[position] != '\n') {//u suprotnom prepisuje karakter po karakter do novog reda
            this->text+=(subtitles[position]); 
            position++;
        }
        this->text+= subtitles[position];//prepisuje novi red
   
        position++;//prelazi na poziciju nakon novog reda
        return true;//nije doslo do kraja unosa
    }
}

bool Title::checkArrow(string subtitles, int& position)
{
 //proverava da li postoji strelica izmedju vremena u zadatom formatu
 return position<subtitles.length()&& subtitles[position]==' ' && subtitles[position+1]=='-' && subtitles[position + 2] == '-' && subtitles[position + 3] == '>' && subtitles[position + 4] == ' ';
    
}

//proverava da li postoji red sa manje od max nebelih karaktera u text-u
bool Title::checkMaxChar(int max)
{
    int i = 0;//indeks
    int notSpaces = 0;//broj nebelih karatera

    while (i < this->text.length()) {//ide dok ne dodje do kraja stringa
        notSpaces = 0;

        while (i<this->text.length() && this->text[i] != '\n')//ide dok ne dodje do kraja reda ili stringa
        {  
            if (this->text[i] != '\t' && this->text[i] != ' ') {//sko nije belina inkrementira nodSpaces
                notSpaces++;
            }
            i++;//proverava sledeci char
        }

        if (notSpaces > max)return false;//ako u ovom redu ima vise od max nebelih karaktera, treba preseci linije teksta
        i++;
        
    }
    return true;//ako nije naisao ni na jednu liniju duzu od max ne treba vrsiti presecanje
}

//ispis greske 
bool Title::writeError(string message)
{
    cout << message << endl;
    return false;
}

//proverava da li se dva titla preklapaju vremenom
bool Title::titleOverlapping(unsigned start, unsigned end)
{
    //ne mogu da se mnoze brojevi direktno jer dolazi do prekoracenja ali je dovoljno proveriti samo znakove razlika
    int a = (this->getEndTime() >= end)  ? 1 : -1;//proverava znak razlike  krajeva
    int b = (this->getBegTime() >= start)  ? 1 : -1;//proverava znak razlike pocetaka
    int c = (this->getBegTime() <= end) ? 1 : -1;//proveravaju znakove pocetka jednog i razlike drugog i obrnuto
    int d= (start <= this->getEndTime() )  ? 1 : -1;

    if (a * b < 0 || c*d>0)//ako je proizvod razlika krajeva i razlika pocetaka negativan to znaci da se titlovi preklapaju odzfo ili odozgo
    {                      //ako je prozivod razlika pocetka jednog i kraja drugog titla pozitivan to znaci da se neki od ta dva titla nalazi unutar drugog tj dolazi do ugnjezdavanja
        return true;
    }
    return false;//ako se nisu ni preklopili ni ugnjezdili to znaci da nemaju presek 


}

void Title::transferStyle(Title* other,int i,int pos)
{
    Style* s=this->styles ,*prev=nullptr;
    //stek je sortiran opadajuce
    while (s && s->getClosedPosition() > i) {
        prev = s;
        s = s->getNext();
    }
    other->setStyle(s);
    while (s) {
        s->updatePositions(pos);
        //s->tellStyle();
        s = s->getNext();
    }
    
    if (prev)
    {
        prev->setNext(nullptr);
    }
}
