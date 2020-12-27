#include "SubtitleEditor.h"
#include<iostream>
#include<string>
#include"Memory.h"
using namespace std;

//konstruktori
SubtitleEditor::SubtitleEditor()
{
    this->s = nullptr;
    this->head=this->tail = nullptr;
    this->selection = nullptr;
}

SubtitleEditor::~SubtitleEditor()
{
    this->clearSubtitle(); 
    this->clearStack();
}

//ucitavanje
int SubtitleEditor::loadSubtitles(string subtitles)
{
    this->clearSubtitle();
    int position = 0;
    int id = 0;//ako zatreba nisam razumela koji indeks se kad vraca
    while (position<(subtitles.length()-1)) {

        id++;
        Title* node = new Title();

        if (!(node->loadTitle(subtitles, position)))return writeError("Greska unutar titla", node);
        else {
               if (this->head != nullptr) {

                   if (this->tail->titleOverlapping( node->getBegTime(),node->getEndTime())) { return writeError("Preklapanje titlova", node);}

                   if (this->tail->getIndex() != (node->getIndex() - 1)) {return writeError("Indeksi nisu uzastopni", node);}

                   this->tail->setNext(node);
                   this->tail = node;
                }
               else{
                   if (node->getIndex() != 1) { return writeError("Prvi indeks nije jednak jedan", node); }

                   this->head = this->tail = node;
                }
        }
    }
    
    addToStack();
    return -1;
}

//cuvanje u obliku stringa
string SubtitleEditor::saveSubtitles()
{
    Title* temp = this->head;
    string ret = "";
    while (temp != nullptr) {
        ret.append(temp->saveTitle());
        temp = temp->getNext();
    }
    return ret;
}

//dodavanje u selekciju
void SubtitleEditor::addToSelection(int start, int end)
{
    Selection* node = new Selection(), * temp = this->selection, * beforeTemp = nullptr;
    if (start < 1)start = 1;//ako je manji od 1 stavi na 1

    if (this->tail != nullptr && end > this->tail->getIndex()) end = this->tail->getIndex();//ako je veci od max stavi na poslednji

    if (this->selection == nullptr) { this->selection = node; }//ako nista nije u selekciji stavi prvi element
    else {
        while (temp && temp->getEndID() < start) {//idi dok ne dodjes do titla koji se ili preklapa odozdo ili je veci
            beforeTemp = temp;
            temp = temp->getNext();
        }

        Selection* firstSmaller = beforeTemp;//upamti prvi pre selekcije

        if (temp && temp->getStartID() < start) {//promeni pocetak ako ima preklapanja odzdo
            node->setStart(temp);
        }

        while (temp && temp->getEndID() < end) {//dokle god se preklapa odozdo ili iznutra obrisi ga
            deleteCurrentSelection(beforeTemp, temp);
        }

        if (temp && temp->getStartID() < end) {//ako se poslednji preklapa odozgo postavi kraj i izbrisi ga
            node->setEnd(temp);
            this->deleteCurrentSelection(beforeTemp, temp);
        }

        //ulancaj node izmadju porvog manjeg i poslednjeg veceg
        node->setNext(temp);
        if (firstSmaller)firstSmaller->setNext(node);
        else { this->selection = node; }
    }

    if ( node->getStart() == nullptr) {//nadji start ako nije vec postavljen
        Title* bStart = findTitle(start - 1);
        node->setBeforeStart(bStart);
        node->setStart(bStart?bStart->getNext():findTitle(start));   
        }
    
    //nadji kraj ako nije vec postavljen
    if (node->getEnd() == nullptr) {node->setEnd(findTitle(end));}

    addToStack();
    
}

//brisanje iz selekcije titlova koji pripadaju [start,end] i u selekciji su
bool SubtitleEditor::deleteFromSelection(int start, int end)
{
    bool success = false;
    Selection* temp = this->selection,*beforeTemp=nullptr;
    while (temp && start > temp->getEndID()) {//idi dok ne naidjes na prvi koji se potencijalno preklapa odzdo
        beforeTemp = temp;
        temp = temp->getNext();
    }
    if (temp && (temp->getStartID() == start) && (temp->getEndID() == end)) {
            deleteCurrentSelection(beforeTemp, temp);
            return true;
    }
    if (temp && temp->getEndID() <= end && temp->getStartID()!=start) {//ako se preklapa odozdo
        success = true;
         temp->setEnd(findTitle(start - 1));
         beforeTemp = temp;
         temp = temp->getNext();
    }
    while (temp &&temp->getEndID() <= end)//brisi sve koji se preklapaju iznutra
    { 
        success = true;
        deleteCurrentSelection(beforeTemp, temp);
    }
    if (temp &&  temp->getStartID() <= end) {//ako se poslednji preklapa promeni mu kraj
      
        if (!success)//ako nije do sad pomerio ili izbrisao nista onda se preklapa okolo
        {
            Selection* node = new Selection(temp);//napravi novi node sa istim int kao temp
            temp->setEnd(findTitle(start - 1));//pomeri kraj za temp
            node->setNext(temp->getNext());//ulancaj novi
            temp->setNext(node);
            temp = node;//pomeri temp 
        }
            Title* bStart = findTitle(end);
            temp->setBeforeStart(bStart);
            temp->setStart(bStart ? bStart->getNext() : findTitle(end + 1));
        
    }
    return success;
}

void SubtitleEditor::setSelection(string s)
{
    if (this->selection != nullptr) {
        deselect();
        deleteFromStack();
    }

    int i = 0,start=0,end=0;
    
    while (i < s.length()) {  
        i++;//preskakanje prve zagrade
        
        while (s[i] <= '9' && s[i] >= '0') {//ucitavanje starta
               start = start * 10 + (s[i] - '0');
               i++;
        }

        i++;//preskakanje zareza
        
        while (s[i] <= '9' && s[i] >= '0') {//ucitavanje end-a
            end = end * 10 + (s[i] - '0');
            i++;
        }
        i++;//preskakanje druge zagrade
        
        addToSelection(start, end);
        deleteFromStack();
        
    }
}

string SubtitleEditor::getSelection()
{
    Selection* sel = this->selection;
    string ret="";
    while (sel) {
        ret = ret+sel->saveSelection();
        sel = sel->getNext();
    }
    return ret;
    
    
}

//stilizovanje
void SubtitleEditor::boldText(int start, int end)
{
    bool success = false;
    Selection* temp = selection;
    while (temp) {
      if(temp->styleSelection(start, end,'b')) success=true;
      temp = temp->getNext();
    } 

    if (success) addToStack();
    
}

void SubtitleEditor::underlineText(int start, int end)
{
    bool success = false;
    Selection* temp = selection;
    while (temp) {
        if(temp->styleSelection(start, end, 'u')) success=true;
        temp = temp->getNext();
    }
    if (success)addToStack();
    
}

void SubtitleEditor::italicText(int start, int end)
{
    bool success = false;
    Selection* temp = selection;
    while (temp) {
        if(temp->styleSelection(start, end, 'i')) success=true;
        temp = temp->getNext();
    }

    if (success) addToStack();
    
}

//brisanje selekcije
void SubtitleEditor::deselect()
{
    if (this->selection) {
        delete this->selection;
        this->selection = nullptr;
        addToStack();
    }
    
}

//brisanje stila
void SubtitleEditor::removeStyle()
{
    bool success = false;
    Selection* temp = this->selection;
    while (temp) {
        if(temp->removeStyle())success=true;
        temp = temp->getNext();
    }
    if (success)addToStack();
}

//pomeranje vremena
void SubtitleEditor::shiftForward(int ms)
{   
    bool success = false;
    Selection* temp = this->selection;

    if (temp == nullptr) {
        Selection*s = new Selection(this->head, this->tail);//selektuj celu listu
        if (s->shiftForward(ms))success = true;//primeni operaciju nad celom listom
        delete s;//obrisi selekciju
    }
    while (temp != nullptr) {
        if (temp->shiftForward(ms))success = true;
        temp = temp->getNext();
    }
    if (success)addToStack();
    
}

void SubtitleEditor::shiftBackward(int ms)
{   
    bool success = false;
    Selection* temp = this->selection;

    if (temp == nullptr) {
        Selection* s = new Selection(this->head, this->tail);//selektuj sve
        if(s->shiftBackward(ms))success=true;//pomeri selektovano
        delete s;//obrisi selekciju
    }
    while (temp != nullptr) {
        if(temp->shiftBackward(ms))success=true;
        temp = temp->getNext();     
    }
    if (success)addToStack();
}

//umetanje titla 
void SubtitleEditor::insertSubtitle(int start, int length, string text)
{
    
    Title* t = new Title( start,  length, text);//alociraj memoriju
    bool success=insertTitle(t);//ubaci
    updateIndexes();//update-uj indekse

    if (success)addToStack();
    
}

bool SubtitleEditor::insertTitle(Title* t)
{
    Title* temp = head;

    if (head == nullptr) { head = tail = t; }//ako je lista prazna postavi prvi
    else if (head->getBegTime() >= t->getEndTime()){ //ako je pre prvog postavi ga na prvo mesto
                  t->setNext(head);
                  head = t;
              } 
            
         else {
                 while (temp->getNext()!=nullptr && temp->getNext()->getBegTime() < t->getBegTime()) { temp = temp->getNext(); }//dokle god je trenutni pre novog idi dalje
        
                 if (t->getBegTime() >= temp->getEndTime()) {//ako se ne preklapa sa prethodnim pokusaj dalje
            
                     if (temp->getNext() == nullptr) { //ako je poslednji postavi rep
                         tail->setNext(t);
                         tail = t;
                     }

                     else if (t->getEndTime() < temp->getNext()->getBegTime()) {//ako se ne preklapa sa sledecim postavi ga izmedju

                         t->setNext(temp->getNext());
                         temp->setNext(t);
                     }
                     else return false;
                  }
          }
    return true;
}

Title* SubtitleEditor::findTitle(int index)
{
    Title* temp = this->head;

    if (index < temp->getIndex())return nullptr;

    while (temp && temp->getIndex()<index) {
        temp=temp->getNext();
    }

    return temp;
}

//azuriranje indeksa
void SubtitleEditor::updateIndexes()
{
    int i = 1;
    Title* temp = this->head;
    while (temp) {
        temp->setIndex(i);
        i++;
        temp = temp->getNext();
    }
}

//brisanje selekcije
void SubtitleEditor::deleteSubtitles()
{
    bool success = (this->selection != nullptr);

    Selection* temp;
    while (this->selection!=nullptr) {
        temp = this->selection;
        this->selection = this->selection->getNext();
        temp->deleteSelection(this->head);
        delete temp;
    }
    this->selection = nullptr;

    if (success) {
       updateIndexes();
       addToStack();
    }
    


    
}

//pomocna funkcija koja brise poslednji sa steka kada je metoda pozvana unutar ove klase a ne spolja
void SubtitleEditor::deleteFromStack()
{
    Memory* m = this->s;
    if (m != nullptr) {
        m = this->s;
        this->s = this->s->getNext();
        delete m;
    }
}

void SubtitleEditor::outputSelection()//pomocna funkcija za debagovanje
{
    Selection* temp = this->selection;
    while (temp!=nullptr) {
        cout << temp->getStartID()<<","<< temp->getEndID() << endl;
        temp = temp->getNext();
    }
    cout << "END" << endl;
}

//spajanje dva subtitla sortirano
void SubtitleEditor::merge(string subtitles)
{   
    SubtitleEditor *editor2 = new SubtitleEditor();//this je editor1
    editor2->loadSubtitles(subtitles);
    deleteFromStack();
    
    Title *temp, *title=editor2->getHead();

    while (title != nullptr) {//umetanje titlova iz editora 2 u editor 1 sortirano
        temp = title;
        title = title->getNext();//sledeci titl iz editora2
        temp->setIndex(0);//resetuj indeks za svaki slucaj
        temp->setNext(nullptr);//izlancaj ga iz editora2 da bi ga ulancao u editor1
        this->insertTitle(temp); //ubaci ga u editor1
    }
    editor2->delHead();//postavi glavu na nullptr da se ne bi dealocirali titlovi (koji su sad deo editora2)prilikom dealokacije editora
    delete editor2;
    updateIndexes();//azuriraj indekse
    addToStack();
    
}

void SubtitleEditor::clearSubtitle()
{
    if (head!=nullptr ){//dealokacija sekvence
        Title* temp;
        while (this->head) {
            temp = this->head;
            this->head = temp->getNext();
            delete temp;
        }
        this->tail=this->head = nullptr;
    }

    if (this->selection!=nullptr) {//dealokacija selekcije

        Selection* temp=nullptr;
        while (this->selection) {
            temp = this->selection;
            this->selection = temp->getNext();
            delete temp;
        }

        this->selection = nullptr;
    }
}

//obrisi selekciju posle beforeTemp iz liste selekcija
void SubtitleEditor::deleteCurrentSelection(Selection* &beforeTemp, Selection* &temp)
{
    Selection* sel = temp;
    if (beforeTemp != nullptr) { beforeTemp->setNext(sel->getNext()); }
    else this->selection = temp->getNext();
    temp = temp->getNext();
    
    delete sel;
}

//ispis greske
int SubtitleEditor::writeError(string message, Title* t)
{
    int i = t->getIndex();
    t->clearTitle();
    cout << message << endl;
    this->clearSubtitle();
    this->clearStack();
    return i;
}

//pomocna funkcija za debagovanje
void SubtitleEditor::outputSection()
{
    Title* temp = head;
    while (temp) {
        cout<<temp->getIndex()<<endl;
        temp = temp->getNext();
    }
}

void SubtitleEditor::undo()
{
    if (this->s != nullptr && this->s->getNext()!=nullptr)
    {
        //skini operaciju pre undo sa steka
         deleteFromStack();
         
        //vrati na stanje operacije pre te
        this->loadSubtitles(this->s->getSubtitle());
        deleteFromStack();
        this->setSelection(this->s->getSel());
        
        
        
    }
}

void SubtitleEditor::addToStack()
{
    string subtitles = this->saveSubtitles();
    string selection = getSelection();

    Memory* m = new Memory(subtitles, selection);
    m->setNext(this->s);
    this->s = m;
}

void SubtitleEditor::outputStack()
{
    Memory* m = this->s;
    while (m) {
       cout<< m->getSel()<<endl;
        m = m->getNext();
    }
}

void SubtitleEditor::clearStack()
{
    if (s != nullptr) {

        Memory* temp;

        while (this->s) {
            temp = this->s;
            this->s = temp->getNext();
            delete temp;
        }

        this->s = nullptr;
    }
}

Title* SubtitleEditor::getHead()
{
    return (this->head);
}

void SubtitleEditor::delHead()
{
    this->head = nullptr;
}

void SubtitleEditor::fixLongLines(int max)
{
    bool success = false;
    Selection* sel=this->selection;
   
    if (sel == nullptr) {
        Selection* s = new Selection(this->head, this->tail);
        if(s->fixLongLines(max)) success=true;
        delete s;
    }
    while (sel) {
        if(sel->fixLongLines(max))success=true;
        sel = sel->getNext();
    }
    if (success)addToStack();
    
}

void SubtitleEditor::fixLongTime(int ms)
{
    bool success = false;
    Selection* s = this->selection;
    
    if (s == nullptr) {
        Selection* sel = new Selection(this->head, this->tail);
        if(sel->fixLongTime(ms)) success=true;
        delete sel;
    }
    while (s != nullptr) {
        if(s->fixLongTime(ms)) success=true;
        s = s->getNext();

    }
    if (success) {
        updateIndexes();
        addToStack();
    }
    
}



