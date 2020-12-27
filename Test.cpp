// Ovde ukljuciti Vase .h fajlove
#include <iostream>
#include <sstream>
#include <fstream>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include"SubtitleEditor.h"
using namespace std;

int tests_passed = 0;

// Funkcija koja poredi Vas izlaz sa ocekivanim izlazom za odgovarajuci test primer
// Da biste lakse uocili gresku ukoliko Vas izlaz ne odgovara ocekivanom izlazu, uporediti fajlove na linku https://text-compare.com/
void checkOutput(string& subtitles, int test_number) {

	ifstream correct_output("output_" + to_string(test_number) + ".txt");
	stringstream ss_out;
	ss_out << correct_output.rdbuf(); correct_output.close();
	string correct_subtitles = ss_out.str();

	if (correct_subtitles != "" && correct_subtitles == subtitles) {
		cout << "Test " + to_string(test_number) + ": PASSED" << endl;
		tests_passed++;
	}
	else {
		cout << "Test " + to_string(test_number) + ": FAILED" << endl;
	}

}

void test(int test_number) {

	SubtitleEditor* editor = new SubtitleEditor;

	ifstream input("input.txt");
	stringstream ss;
	ss << input.rdbuf(); input.close();
	string subtitles = ss.str();

	ifstream input_8;
	stringstream ss_8;
	string subtitles_8;

	switch (test_number) {
	case 0://provereno do stilova
		// MyExample
		editor->loadSubtitles(subtitles);
		editor->addToSelection(100, 500);
		editor->addToSelection(1000, 1100);
		editor->outputSelection();
		editor->addToSelection(1500, 1600);
		editor->outputSelection();
		editor->addToSelection(1050, 1200);//s desna//1000 1200
		editor->outputSelection();
		editor->addToSelection(950, 1300);//spolja 950 1300
		editor->outputSelection();
		editor->addToSelection(1100, 1200);//iynutra 950 1300
		editor->outputSelection();
		editor->addToSelection(850, 1000);//sleva 850 1300
		editor->outputSelection();
		
		editor->addToSelection(600, 700);//sortirano
		editor->outputSelection();
		editor->addToSelection(1450, 1550);
		editor->outputSelection();
		editor->addToSelection(1350, 1700);
		editor->outputSelection();
		editor->addToSelection(1600, 1650);
		editor->outputSelection();
		editor->addToSelection(50, 200);
		editor->outputSelection();
		editor->addToSelection(500, 550);
		editor->outputSelection();
		editor->addToSelection(1, 555);
		editor->outputSelection();
		editor->addToSelection(100, 200);
		editor->outputSelection();
		//subtitles = editor->saveSubtitles();

		editor->deleteFromSelection(100, 500);
		editor->deleteFromSelection(1000, 1100);
		editor->outputSelection();
		editor->deleteFromSelection(1500, 1600);
		editor->outputSelection();
		editor->deleteFromSelection(1050, 1200);//s desna//1000 1200
		editor->outputSelection();
		editor->deleteFromSelection(950, 1300);//spolja 950 1300
		editor->outputSelection();
		editor->deleteFromSelection(1100, 1200);//iynutra 950 1300
		editor->outputSelection();
		editor->addToSelection(850, 1000);//sleva 850 1300
		editor->outputSelection();
		cout << "delete" << endl;
		editor->deleteFromSelection(600, 700);//sortirano
		editor->outputSelection();
		editor->deleteFromSelection(1450, 1550);
		editor->outputSelection();
		editor->deleteFromSelection(1350, 1700);
		editor->outputSelection();
		editor->deleteFromSelection(1600, 1650);
		editor->outputSelection();
		editor->deleteFromSelection(50, 200);
		editor->outputSelection();
		editor->deleteFromSelection(500, 550);
		editor->outputSelection();
		editor->deleteFromSelection(1, 555);
		editor->outputSelection();
		editor->deleteFromSelection(100, 200);
		editor->outputSelection();
		
		break;
	case 1:
		// LOAD / SAVE
		editor->loadSubtitles(subtitles);
		editor->shiftForward(500);
		editor->shiftBackward(1000);
		editor->undo();
		editor->undo();
		editor->addToSelection(1, 1000);
		editor->addToSelection(1500,1500);
		editor->undo();
		editor->undo();
		editor->outputSelection();
		editor->removeStyle();
		editor->boldText(0, 100);
		editor->undo();
		editor->undo();
		editor->insertSubtitle(3334000, 2000, "This is sample text.\n");
		editor->deleteSubtitles();
		editor->undo(); editor->undo(); 
		
		editor->fixLongLines(42); editor->undo();
		editor->addToSelection(1, 100);
		editor->fixLongTime(2750);
		editor->undo();
		
		subtitles = editor->saveSubtitles();
		break;
	case 2:
		// LOAD / ONE SELECTION / REMOVE STYLE / SAVE
		editor->loadSubtitles(subtitles);
		editor->addToSelection(1, 1000);
		editor->removeStyle();
		subtitles = editor->saveSubtitles();
		break;
	case 3:
		// LOAD / MULTIPLE SELECTIONS / ADD STYLE / SAVE
		editor->loadSubtitles(subtitles);
		editor->shiftForward(500);
		editor->undo();
		editor->shiftBackward(1000);
		editor->undo();
		editor->addToSelection(1, 1000);
		editor->addToSelection(1500, 1500);
		editor->outputSelection();
		editor->undo();
		editor->undo();
		editor->outputSelection();
		editor->removeStyle();
		editor->undo();
		editor->boldText(0, 100);
		editor->undo();
		editor->insertSubtitle(3334000, 2000, "This is sample text.\n");
		editor->undo();
		editor->deleteSubtitles();
		editor->undo();
		editor->addToSelection(1, 1);
		editor->addToSelection(500, 500);
		editor->addToSelection(1500, 1510);
		editor->boldText(0, 100);
		subtitles = editor->saveSubtitles();
		break;
	case 4:
		// LOAD / SHIFT FORWARD / SAVE
		editor->loadSubtitles(subtitles);
		editor->shiftForward(1000);
		subtitles = editor->saveSubtitles();
		break;
	case 5:
		// LOAD / SHIFT BACKWARD / SAVE
		editor->loadSubtitles(subtitles);
		editor->shiftBackward(60000);
		subtitles = editor->saveSubtitles();
		break;
	case 6:
		// LOAD / INSERT SUBTITLE / SAVE
		editor->loadSubtitles(subtitles);
		editor->insertSubtitle(3334000, 2000, "This is sample text.\n");
		subtitles = editor->saveSubtitles();
		break;
	case 7:
		// LOAD / ONE SELECTION / DELETE SUBTITLES / SAVE
		editor->loadSubtitles(subtitles);
		editor->addToSelection(1000, 1100);
		editor->deleteSubtitles();
		subtitles = editor->saveSubtitles();
		break;
	case 8:
		// LOAD / MERGE / SAVE
		editor->loadSubtitles(subtitles);
		input_8.open("input_8_extra.txt");
		ss_8 << input_8.rdbuf(); input_8.close();
		subtitles_8 = ss_8.str();
		editor->merge(subtitles_8);
		subtitles = editor->saveSubtitles();
		break;
	case 9:
		// LOAD / FIX LONG LINES / SAVE
		editor->loadSubtitles(subtitles);
		editor->fixLongLines(42);
		subtitles = editor->saveSubtitles();
		break;
	case 10:
		// LOAD / ONE SELECTION / FIX LONG TIME / SAVE
		editor->loadSubtitles(subtitles);
		editor->addToSelection(100, 105);
		editor->fixLongTime(2750);
		subtitles = editor->saveSubtitles();
		break;
	case 11:
		// LOAD / SHIFT FORWARD / SHIFT BACKWARD / UNDO / SAVE
		editor->loadSubtitles(subtitles);
		editor->shiftForward(1000);
		editor->shiftBackward(500);
		editor->undo();
		subtitles = editor->saveSubtitles();
		break;
	case 12:
		editor->loadSubtitles(subtitles);
		editor->addToSelection(1, 1);
		editor->addToSelection(540, 550);
		editor->addToSelection(1500, 1510);
		editor->boldText(6, 13);
		editor->removeStyle();
		subtitles = editor->saveSubtitles();
		break;
	case 13:
		editor->loadSubtitles(subtitles);
		editor->addToSelection(1, 1);
		editor->addToSelection(540, 550);
		editor->addToSelection(1889, 1889);
		//editor->deleteSubtitles();
		editor->insertSubtitle(76700, 5000, "This is sample text.\n");
		subtitles = editor->saveSubtitles();
		break;
	case 14:
		editor->loadSubtitles("1\n12:00:00,024 --> 12:00:00,124\nName of <u>film is unknow</u>n\n\n2\n15:20:01,000 --> 15:20:02,000\nPle<i>ase</i> work this t<b>ime pleea</b>se\n\n\n");
		editor->fixLongTime(501);
		//editor->outputSection();
		cout << editor->saveSubtitles() << endl;
		break;
	case 15:
		int position = 0;
		Time* t = new Time();
		t->readTime("02:25:03,004", position);
		cout << t->saveTime() << endl;
		position = 0;
		Title* titl = new Title();
		titl->loadTitle("2\n12:05:03,024 --> 14:08:30,124\nName o<i>f film</i>\n\n", position);
		titl->saveTitle();
		titl->writeStyle();
		titl->addStyle(1, 3, 'u');
		cout << titl->saveTitle();

		position = 0;
		editor->loadSubtitles("1\n12:05:03,024 --> 14:08:30,124\nName of <u>film is unknow</u>n\n\n2\n15:20:03,024 --> 19:18:30,124\nPle<i>ase</i> work<b> this t</b>ime pleease\n\n\n");
		cout << editor->saveSubtitles();
		delete t;
		delete titl;
		break;
	}

	ofstream my_output("my_output_" + to_string(test_number) + ".txt");
	my_output << subtitles; my_output.close();

	checkOutput(subtitles, test_number);

	delete editor;
}

int main() {

	// Izmeniti elemente niza u zavisnosti od testova koje zelite da pokrenete!
	int active_tests[] = {15};
	int number_of_tests = sizeof(active_tests) / sizeof(int);

	cout << "============================================" << endl;
	cout << "TESTING SUBTITLE_EDITOR" << endl << endl;
	cout << "Executing tests: ";
	for (int i = 0; i < number_of_tests; i++) cout << active_tests[i] << (i < number_of_tests - 1 ? ", " : "");
	cout << endl;

	for (int i = 0; i < number_of_tests; i++) test(active_tests[i]);

	cout << "============================================" << endl;
	cout << "Tests passed count: " << tests_passed << endl;

	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	return 0;
}