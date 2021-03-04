#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

bool gameOn = true;
bool win = false;

class Card {
	char cardColour = '0';
	char cardType = '0';		// H D C S
	int cardNumber = 00;	// 1-13
	string cardStatus = "unknown";	// open or closed or unkown

public:

	void setType(char type) {cardType = type;}
	void setColour(char type) {
		if (type == 'H' || type == 'D') {
			cardColour = 'R';
		} else if (type == 'S' || type == 'C') {
			cardColour = 'B';
		}
	}
	void setNumber(int num) {cardNumber = num;}
	void setStatus(string status) {cardStatus = status;}
	char getType() {return cardType;}
	char getColour() {return cardColour;}
	int getNumber() {return cardNumber;}
	string getStatus() {return cardStatus;}
};

void exitGame() {
	gameOn = false;
}

int largest(int arr[]){
	int max = arr[0];
	for (int i = 0; i < 7; i++){
		if (arr[i] > max) {
			max = arr[i];
		}
	}
	return max;
}

void checkWin(int fhc, int fdc, int fcc, int fsc) {
	if (fhc + fdc + fcc + fsc == 52) {
		win = true;
		gameOn = false;
	}
}

int main(int argc, char *argv[]) {

	Card deck[52];
	Card stock[24];
	Card waste[3];
	Card wasteHelper[24];
	Card foundationH[13];
	Card foundationD[13];
	Card foundationC[13];
	Card foundationS[13];
	Card pile0[20];
	Card pile1[20];
	Card pile2[20];
	Card pile3[20];
	Card pile4[20];
	Card pile5[20];
	Card pile6[20];
	int pile0cntr;
	int pile1cntr;
	int pile2cntr;
	int pile3cntr;
	int pile4cntr;
	int pile5cntr;
	int pile6cntr;
	int FHcntr = 0;
	int FDcntr = 0;
	int FCcntr = 0;
	int FScntr = 0;
	int stockCntr = 0;
	int wasteCntr = 0;
	int wasteHelperCntr = 0;

	ifstream ReadFileDeck(argv[1]);
	int i = 0;
	string deckline;
	while (getline (ReadFileDeck, deckline)) {
		deck[i].setType(deckline.at(0));
		deck[i].setColour(deckline.at(0));
		deck[i].setNumber(stoi(deckline.substr(1,3)));
		deck[i].setStatus("closed");
		i++;
	}
	ReadFileDeck.close();

	// initializing piles
	pile0[0] = deck[51];
	pile0[0].setStatus("open");
	pile1[0] = deck[50];
	pile2[0] = deck[49];
	pile3[0] = deck[48];
	pile4[0] = deck[47];
	pile5[0] = deck[46];
	pile6[0] = deck[45];

	pile1[1] = deck[44];
	pile1[1].setStatus("open");
	pile2[1] = deck[43];
	pile3[1] = deck[42];
	pile4[1] = deck[41];
	pile5[1] = deck[40];
	pile6[1] = deck[39];

	pile2[2] = deck[38];
	pile2[2].setStatus("open");
	pile3[2] = deck[37];
	pile4[2] = deck[36];
	pile5[2] = deck[35];
	pile6[2] = deck[34];

	pile3[3] = deck[33];
	pile3[3].setStatus("open");
	pile4[3] = deck[32];
	pile5[3] = deck[31];
	pile6[3] = deck[30];

	pile4[4] = deck[29];
	pile4[4].setStatus("open");
	pile5[4] = deck[28];
	pile6[4] = deck[27];

	pile5[5] = deck[26];
	pile5[5].setStatus("open");
	pile6[5] = deck[25];

	pile6[6] = deck[24];
	pile6[6].setStatus("open");

	pile0cntr = 1;
	pile1cntr = 2;
	pile2cntr = 3;
	pile3cntr = 4;
	pile4cntr = 5;
	pile5cntr = 6;
	pile6cntr = 7;

	//initializing stock
	for (int i=0; i < 24; i++){
		stock[i] = deck[i];
		stockCntr++;
	}

	ofstream output;
	output.open(argv[3]);
	output << endl;

	ifstream ReadFileCommands(argv[2]);
	while (gameOn && !win) {

		//	BOARD
		//	top part
		if (stockCntr > 0) { //	stock part in board
			output << "@@@ ";
		} else {
			output << "___ ";
		}

		if (wasteCntr == 0 && wasteHelperCntr > 0) {

			waste[0] = wasteHelper[wasteHelperCntr-1];
			wasteHelper[wasteHelperCntr-1].setNumber(00);
			wasteHelper[wasteHelperCntr-1].setType('0');
			wasteHelper[wasteHelperCntr-1].setStatus("unknown");
			wasteCntr++;
			wasteHelperCntr--;
		}
		for (int i = 0; i < 3; i++) { // waste part in board
			if (waste[i].getType() == '0'){
				output << "___ ";
			} else {
				output << waste[i].getType() << setw(2) << setfill('0') << waste[i].getNumber() << " ";
			}
		}

		// Foundation part in board
		output << "        ";
		if (FHcntr == 0){
			output << "___ ";
		} else {
			output << "H" << setw(2) << setfill('0') << FHcntr << " ";
		}
		if (FDcntr == 0){
			output << "___ ";
		} else {
			output << "D" << setw(2) << setfill('0') << FDcntr << " ";
		}
		if (FScntr == 0){
			output << "___ ";
		} else {
			output << "S" << setw(2) << setfill('0') << FScntr << " ";
		}
		if (FCcntr == 0){
			output << "___ ";
		} else {
			output << "C" << setw(2) << setfill('0') << FCcntr << " ";
		}
		output << endl;
		output << endl;

		int pileCntrs[] = {pile0cntr, pile1cntr, pile2cntr, pile3cntr, pile4cntr, pile5cntr, pile6cntr};

		// bottom part
		for (int i = 0; i < largest(pileCntrs); i++){  // finds largest pile so board looks good :)
			if ((pile0[i].getType() != '0') && (pile0[i].getStatus() == "open")) {
				output << pile0[i].getType() << setw(2) << setfill('0') << pile0[i].getNumber() << "   ";
			} else if (pile0[i].getType() != '0' && pile0[i].getStatus() == "closed"){
				output << "@@@   ";
			} else {
				output << "      ";
			}
			if (pile1[i].getType() != '0' && pile1[i].getStatus() == "open") {
				output << pile1[i].getType() << setw(2) << setfill('0') << pile1[i].getNumber() << "   ";
			} else if (pile1[i].getType() != '0' && pile1[i].getStatus() == "closed"){
				output << "@@@   ";
			} else {
				output << "      ";
			}
			if (pile2[i].getType() != '0' && pile2[i].getStatus() == "open") {
				output << pile2[i].getType() << setw(2) << setfill('0') << pile2[i].getNumber() << "   ";
			} else if (pile2[i].getType() != '0' && pile2[i].getStatus() == "closed"){
				output << "@@@   ";
			} else {
				output << "      ";
			}
			if (pile3[i].getType() != '0' && pile3[i].getStatus() == "open") {
				output << pile3[i].getType() << setw(2) << setfill('0') << pile3[i].getNumber() << "   ";
			} else if (pile3[i].getType() != '0' && pile3[i].getStatus() == "closed"){
				output << "@@@   ";
			} else {
				output << "      ";
			}
			if (pile4[i].getType() != '0' && pile4[i].getStatus() == "open") {
				output << pile4[i].getType() << setw(2) << setfill('0') << pile4[i].getNumber() << "   ";
			} else if (pile4[i].getType() != '0' && pile4[i].getStatus() == "closed"){
				output << "@@@   ";
			} else {
				output << "      ";
			}
			if (pile5[i].getType() != '0' && pile5[i].getStatus() == "open") {
				output << pile5[i].getType() << setw(2) << setfill('0') << pile5[i].getNumber() << "   ";
			} else if (pile5[i].getType() != '0' && pile5[i].getStatus() == "closed"){
				output << "@@@   ";
			} else {
				output << "      ";
			}
			if (pile6[i].getType() != '0' && pile6[i].getStatus() == "open") {
				output << pile6[i].getType() << setw(2) << setfill('0') << pile6[i].getNumber() << "   ";
			} else if (pile6[i].getType() != '0' && pile6[i].getStatus() == "closed"){
				output << "@@@   ";
			} else {
				output << "      ";
			}
			output << endl;
		}
		output << endl;

		if (FHcntr + FDcntr + FScntr + FCcntr == 52) { // CHECKS WIN CONDITION
			output << endl;
			output << "****************************************" << endl;
			output << endl;
			output << "You Win!" << endl << endl;
			output << "Game Over!" << endl ;
			break;
		}

		string comma1;
		string comma2;
		string comma3;
		string comma4;
		string comma5;
		ReadFileCommands >> comma1;
		if (comma1 == "exit") { // EXIT COMMAND
			exitGame();
			output << comma1 << endl;
			output << endl;
			output << "****************************************" << endl;
			output << endl;
			output << "Game Over!" << endl;
			break;
		} else if (comma1 == "open") { // first string is open
			ReadFileCommands >> comma2;
			if (comma2 == "from") { // second string is stock
				ReadFileCommands >> comma3; // OPEN FROM STOCK
				output << comma1 << " " << comma2 << " " << comma3 << endl;
				if (stockCntr >= 3) {
					switch (wasteCntr) {
					case 0: {
						for (int i=0; i<3; i++) {
							stock[stockCntr-1].setStatus("open");
							waste[i] = stock[stockCntr-1];
							wasteCntr++;
							stock[stockCntr-1].setNumber(00);
							stock[stockCntr-1].setType('0');
							stock[stockCntr-1].setStatus("unknown");
							stockCntr--;
						}
						break;
					}
					case 1: {
						waste[0].setStatus("closed");
						wasteHelper[wasteHelperCntr] = waste[0];
						wasteHelperCntr++;
						waste[0].setNumber(00);
						waste[0].setType('0');
						waste[0].setStatus("unknown");
						wasteCntr--;
						for (int i=0; i<3; i++) {
							stock[stockCntr-1].setStatus("open");
							waste[i] = stock[stockCntr-1];
							wasteCntr++;
							stock[stockCntr-1].setNumber(00);
							stock[stockCntr-1].setType('0');
							stock[stockCntr-1].setStatus("unknown");
							stockCntr--;
						}
						break;
					}
					case 2: {
						waste[0].setStatus("closed");
						wasteHelper[wasteHelperCntr] = waste[0];
						wasteHelperCntr++;
						waste[0].setNumber(00);
						waste[0].setType('0');
						waste[0].setStatus("unknown");
						wasteCntr--;
						waste[1].setStatus("closed");
						wasteHelper[wasteHelperCntr] = waste[1];
						wasteHelperCntr++;
						waste[1].setNumber(00);
						waste[1].setType('0');
						waste[1].setStatus("unknown");
						wasteCntr--;
						for (int i=0; i<3; i++) {
							stock[stockCntr-1].setStatus("open");
							waste[i] = stock[stockCntr-1];
							wasteCntr++;
							stock[stockCntr-1].setNumber(00);
							stock[stockCntr-1].setType('0');
							stock[stockCntr-1].setStatus("unknown");
							stockCntr--;
						}
						break;
					}
					case 3: {
						waste[0].setStatus("closed");
						wasteHelper[wasteHelperCntr] = waste[0];
						wasteHelperCntr++;
						waste[0].setNumber(00);
						waste[0].setType('0');
						waste[0].setStatus("unknown");
						wasteCntr--;
						waste[1].setStatus("closed");
						wasteHelper[wasteHelperCntr] = waste[1];
						wasteHelperCntr++;
						waste[1].setNumber(00);
						waste[1].setType('0');
						waste[1].setStatus("unknown");
						wasteCntr--;
						waste[2].setStatus("closed");
						wasteHelper[wasteHelperCntr] = waste[2];
						wasteHelperCntr++;
						waste[2].setNumber(00);
						waste[2].setType('0');
						waste[2].setStatus("unknown");
						wasteCntr--;
						for (int i=0; i<3; i++) {
							stock[stockCntr-1].setStatus("open");
							waste[i] = stock[stockCntr-1];
							wasteCntr++;
							stock[stockCntr-1].setNumber(00);
							stock[stockCntr-1].setType('0');
							stock[stockCntr-1].setStatus("unknown");
							stockCntr--;
						}
						break;
					}
					}
				} else if (stockCntr == 2) {
					int tempWasteCntr = wasteCntr;
					for (int i = 0; i<tempWasteCntr; i++) {
						waste[i].setStatus("closed");
						wasteHelper[wasteHelperCntr] = waste[i];
						wasteHelperCntr++;
						waste[i].setNumber(00);
						waste[i].setType('0');
						waste[i].setStatus("unknown");
						wasteCntr--;
					}
					for (int i=0; i<2; i++) {
						stock[stockCntr-1].setStatus("open");
						waste[i] = stock[stockCntr-1];
						wasteCntr++;
						stock[stockCntr-1].setNumber(00);
						stock[stockCntr-1].setType('0');
						stock[stockCntr-1].setStatus("unknown");
						stockCntr--;
					}
				} else if (stockCntr == 1) {
					int tempWasteCntr = wasteCntr;
					for (int i = 0; i<tempWasteCntr; i++) {
						waste[i].setStatus("closed");
						wasteHelper[wasteHelperCntr] = waste[i];
						wasteHelperCntr++;
						waste[i].setNumber(00);
						waste[i].setType('0');
						waste[i].setStatus("unknown");
						wasteCntr--;
					}
					for (int i=0; i<1; i++) {
						stock[stockCntr-1].setStatus("open");
						waste[i] = stock[stockCntr-1];
						wasteCntr++;
						stock[stockCntr-1].setNumber(00);
						stock[stockCntr-1].setType('0');
						stock[stockCntr-1].setStatus("unknown");
						stockCntr--;
					}

				} else if (stockCntr == 0) {
					int tempWasteCntr = wasteCntr;
					for (int i = 0; i<tempWasteCntr; i++) {
						waste[i].setStatus("closed");
						wasteHelper[wasteHelperCntr] = waste[i];
						wasteHelperCntr++;
						waste[i].setNumber(00);
						waste[i].setType('0');
						waste[i].setStatus("unknown");
						wasteCntr--;
					}
					int tempWasteHelperCntr = wasteHelperCntr;
					for (int i=0; i < tempWasteHelperCntr; i++) {
						stock[i] = wasteHelper[wasteHelperCntr-1];
						stockCntr++;
						wasteHelper[wasteHelperCntr-1].setNumber(00);
						wasteHelper[wasteHelperCntr-1].setType('0');
						wasteHelper[wasteHelperCntr-1].setStatus("unknown");
						wasteHelperCntr--;
					}
				}
			} else if (comma2 != "from") { // OPEN PILENUMBER
				int pileNum = stoi(comma2);
				output << comma1 << " " << comma2 << endl;
				switch(pileNum){
				case 0:{
					if (pile0cntr != 0 && pile0[pile0cntr-1].getStatus() == "closed") {
						pile0[pile0cntr-1].setStatus("open");
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 1: {
					if (pile1cntr != 0 && pile1[pile1cntr-1].getStatus() == "closed") {
						pile1[pile1cntr-1].setStatus("open");
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 2: {
					if (pile2cntr != 0 && pile2[pile2cntr-1].getStatus() == "closed") {
						pile2[pile2cntr-1].setStatus("open");
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 3: {
					if (pile3cntr != 0 && pile3[pile3cntr-1].getStatus() == "closed") {
						pile3[pile3cntr-1].setStatus("open");
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 4: {
					if (pile4cntr != 0 && pile4[pile4cntr-1].getStatus() == "closed") {
						pile4[pile4cntr-1].setStatus("open");
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 5: {
					if (pile5cntr != 0 && pile5[pile5cntr-1].getStatus() == "closed") {
						pile5[pile5cntr-1].setStatus("open");
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 6: {
					if (pile6cntr != 0 && pile6[pile6cntr-1].getStatus() == "closed") {
						pile6[pile6cntr-1].setStatus("open");
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				}
			}
		} else if (comma1 == "move") { // If first String is "move"
			ReadFileCommands >> comma2;
			if (comma2 == "to") { // If second string "to"
				ReadFileCommands >> comma3; // move to foundation
				ReadFileCommands >> comma4; // pile or waste
				if (comma4 == "pile") { // move to foundation pile
					ReadFileCommands >> comma5; // pile number = comma5
					output << comma1 << " " << comma2 << " " << comma3 << " " << comma4 << " " << comma5 << endl;
					switch(stoi(comma5)) {
					case 0:
					{
						if (pile0cntr == 0) {
							output << endl;
							output << "Invalid Move!" << endl;
						}

						char tempCardType = pile0[pile0cntr-1].getType();
						int tempCardNum = pile0[pile0cntr-1].getNumber();
						switch(tempCardType) {
						case 'H':
							if (FHcntr == 0 && tempCardNum == 01) {
								foundationH[0] = pile0[pile0cntr-1];
								FHcntr++;
								pile0[pile0cntr-1].setType('0');
								pile0[pile0cntr-1].setNumber(00);
								pile0[pile0cntr-1].setStatus("unknown");
								pile0cntr--;
							} else if (FHcntr > 0 && tempCardNum-1 == FHcntr) {
								foundationH[FHcntr] = pile0[pile0cntr-1];
								FHcntr++;
								pile0[pile0cntr-1].setType('0');
								pile0[pile0cntr-1].setNumber(00);
								pile0[pile0cntr-1].setStatus("unknown");
								pile0cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'D':
							if (FDcntr == 0 && tempCardNum == 01) {
								foundationD[0] = pile0[pile0cntr-1];
								FDcntr++;
								pile0[pile0cntr-1].setType('0');
								pile0[pile0cntr-1].setNumber(00);
								pile0[pile0cntr-1].setStatus("unknown");
								pile0cntr--;
							} else if (FDcntr > 0 && tempCardNum-1 == FDcntr) {
								foundationD[FDcntr] = pile0[pile0cntr-1];
								FDcntr++;
								pile0[pile0cntr-1].setType('0');
								pile0[pile0cntr-1].setNumber(00);
								pile0[pile0cntr-1].setStatus("unknown");
								pile0cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'C':
							if (FCcntr == 0 && tempCardNum == 01) {
								foundationC[0] = pile0[pile0cntr-1];
								FCcntr++;
								pile0[pile0cntr-1].setType('0');
								pile0[pile0cntr-1].setNumber(00);
								pile0[pile0cntr-1].setStatus("unknown");
								pile0cntr--;
							} else if (FCcntr > 0 && tempCardNum-1 == FCcntr) {
								foundationC[FCcntr] = pile0[pile0cntr-1];
								FCcntr++;
								pile0[pile0cntr-1].setType('0');
								pile0[pile0cntr-1].setNumber(00);
								pile0[pile0cntr-1].setStatus("unknown");
								pile0cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'S':
							if (FScntr == 0 && tempCardNum == 01) {
								foundationS[0] = pile0[pile0cntr-1];
								FScntr++;
								pile0[pile0cntr-1].setType('0');
								pile0[pile0cntr-1].setNumber(00);
								pile0[pile0cntr-1].setStatus("unknown");
								pile0cntr--;
							} else if (FScntr > 0 && tempCardNum-1 == FScntr) {
								foundationS[FScntr] = pile0[pile0cntr-1];
								FScntr++;
								pile0[pile0cntr-1].setType('0');
								pile0[pile0cntr-1].setNumber(00);
								pile0[pile0cntr-1].setStatus("unknown");
								pile0cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
							}
						break;

					}

					case 1:
					{
						if (pile1cntr == 0) {
							output << endl;
							output << "Invalid Move!" << endl;
						}

						char tempCardType = pile1[pile1cntr-1].getType();
						int tempCardNum = pile1[pile1cntr-1].getNumber();
						switch(tempCardType) {
						case 'H':
							if (FHcntr == 0 && tempCardNum == 01) {
								foundationH[0] = pile1[pile1cntr-1];
								FHcntr++;
								pile1[pile1cntr-1].setType('0');
								pile1[pile1cntr-1].setNumber(00);
								pile1[pile1cntr-1].setStatus("unknown");
								pile1cntr--;
							} else if (FHcntr > 0 && tempCardNum-1 == FHcntr) {
								foundationH[FHcntr] = pile1[pile1cntr-1];
								FHcntr++;
								pile1[pile1cntr-1].setType('0');
								pile1[pile1cntr-1].setNumber(00);
								pile1[pile1cntr-1].setStatus("unknown");
								pile1cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'D':
							if (FDcntr == 0 && tempCardNum == 01) {
								foundationD[0] = pile1[pile1cntr-1];
								FDcntr++;
								pile1[pile1cntr-1].setType('0');
								pile1[pile1cntr-1].setNumber(00);
								pile1[pile1cntr-1].setStatus("unknown");
								pile1cntr--;
							} else if (FDcntr > 0 && tempCardNum-1 == FDcntr) {
								foundationD[FDcntr] = pile1[pile1cntr-1];
								FDcntr++;
								pile1[pile1cntr-1].setType('0');
								pile1[pile1cntr-1].setNumber(00);
								pile1[pile1cntr-1].setStatus("unknown");
								pile1cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'C':
							if (FCcntr == 0 && tempCardNum == 01) {
								foundationC[0] = pile1[pile1cntr-1];
								FCcntr++;
								pile1[pile1cntr-1].setType('0');
								pile1[pile1cntr-1].setNumber(00);
								pile1[pile1cntr-1].setStatus("unknown");
								pile1cntr--;
							} else if (FCcntr > 0 && tempCardNum-1 == FCcntr) {
								foundationC[FCcntr] = pile1[pile1cntr-1];
								FCcntr++;
								pile1[pile1cntr-1].setType('0');
								pile1[pile1cntr-1].setNumber(00);
								pile1[pile1cntr-1].setStatus("unknown");
								pile1cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'S':
							if (FScntr == 0 && tempCardNum == 01) {
								foundationS[0] = pile1[pile1cntr-1];
								FScntr++;
								pile1[pile1cntr-1].setType('0');
								pile1[pile1cntr-1].setNumber(00);
								pile1[pile1cntr-1].setStatus("unknown");
								pile1cntr--;
							} else if (FScntr > 0 && tempCardNum-1 == FScntr) {
								foundationS[FScntr] = pile1[pile1cntr-1];
								FScntr++;
								pile1[pile1cntr-1].setType('0');
								pile1[pile1cntr-1].setNumber(00);
								pile1[pile1cntr-1].setStatus("unknown");
								pile1cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
							}
						break;

					}

					case 2:
					{
						if (pile2cntr == 0) {
							output << endl;
							output << "Invalid Move!" << endl;
						}

						char tempCardType = pile2[pile2cntr-1].getType();
						int tempCardNum = pile2[pile2cntr-1].getNumber();
						switch(tempCardType) {
						case 'H':
							if (FHcntr == 0 && tempCardNum == 01) {
								foundationH[0] = pile2[pile2cntr-1];
								FHcntr++;
								pile2[pile2cntr-1].setType('0');
								pile2[pile2cntr-1].setNumber(00);
								pile2[pile2cntr-1].setStatus("unknown");
								pile2cntr--;
							} else if (FHcntr > 0 && tempCardNum-1 == FHcntr) {
								foundationH[FHcntr] = pile2[pile2cntr-1];
								FHcntr++;
								pile2[pile2cntr-1].setType('0');
								pile2[pile2cntr-1].setNumber(00);
								pile2[pile2cntr-1].setStatus("unknown");
								pile2cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'D':
							if (FDcntr == 0 && tempCardNum == 01) {
								foundationD[0] = pile2[pile2cntr-1];
								FDcntr++;
								pile2[pile2cntr-1].setType('0');
								pile2[pile2cntr-1].setNumber(00);
								pile2[pile2cntr-1].setStatus("unknown");
								pile2cntr--;
							} else if (FDcntr > 0 && tempCardNum-1 == FDcntr) {
								foundationD[FDcntr] = pile2[pile2cntr-1];
								FDcntr++;
								pile2[pile2cntr-1].setType('0');
								pile2[pile2cntr-1].setNumber(00);
								pile2[pile2cntr-1].setStatus("unknown");
								pile2cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'C':
							if (FCcntr == 0 && tempCardNum == 01) {
								foundationC[0] = pile2[pile2cntr-1];
								FCcntr++;
								pile2[pile2cntr-1].setType('0');
								pile2[pile2cntr-1].setNumber(00);
								pile2[pile2cntr-1].setStatus("unknown");
								pile2cntr--;
							} else if (FCcntr > 0 && tempCardNum-1 == FCcntr) {
								foundationC[FCcntr] = pile2[pile2cntr-1];
								FCcntr++;
								pile2[pile2cntr-1].setType('0');
								pile2[pile2cntr-1].setNumber(00);
								pile2[pile2cntr-1].setStatus("unknown");
								pile2cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'S':
							if (FScntr == 0 && tempCardNum == 01) {
								foundationS[0] = pile2[pile2cntr-1];
								FScntr++;
								pile2[pile2cntr-1].setType('0');
								pile2[pile2cntr-1].setNumber(00);
								pile2[pile2cntr-1].setStatus("unknown");
								pile2cntr--;
							} else if (FScntr > 0 && tempCardNum-1 == FScntr) {
								foundationS[FScntr] = pile2[pile2cntr-1];
								FScntr++;
								pile2[pile2cntr-1].setType('0');
								pile2[pile2cntr-1].setNumber(00);
								pile2[pile2cntr-1].setStatus("unknown");
								pile2cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
							}
						break;

					}

					case 3:
					{
						if (pile3cntr == 0) {
							output << endl;
							output << "Invalid Move!" << endl;
						}

						char tempCardType = pile3[pile3cntr-1].getType();
						int tempCardNum = pile3[pile3cntr-1].getNumber();
						switch(tempCardType) {
						case 'H':
							if (FHcntr == 0 && tempCardNum == 01) {
								foundationH[0] = pile3[pile3cntr-1];
								FHcntr++;
								pile3[pile3cntr-1].setType('0');
								pile3[pile3cntr-1].setNumber(00);
								pile3[pile3cntr-1].setStatus("unknown");
								pile3cntr--;
							} else if (FHcntr > 0 && tempCardNum-1 == FHcntr) {
								foundationH[FHcntr] = pile3[pile3cntr-1];
								FHcntr++;
								pile3[pile3cntr-1].setType('0');
								pile3[pile3cntr-1].setNumber(00);
								pile3[pile3cntr-1].setStatus("unknown");
								pile3cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'D':
							if (FDcntr == 0 && tempCardNum == 01) {
								foundationD[0] = pile3[pile3cntr-1];
								FDcntr++;
								pile3[pile3cntr-1].setType('0');
								pile3[pile3cntr-1].setNumber(00);
								pile3[pile3cntr-1].setStatus("unknown");
								pile3cntr--;
							} else if (FDcntr > 0 && tempCardNum-1 == FDcntr) {
								foundationD[FDcntr] = pile3[pile3cntr-1];
								FDcntr++;
								pile3[pile3cntr-1].setType('0');
								pile3[pile3cntr-1].setNumber(00);
								pile3[pile3cntr-1].setStatus("unknown");
								pile3cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'C':
							if (FCcntr == 0 && tempCardNum == 01) {
								foundationC[0] = pile3[pile3cntr-1];
								FCcntr++;
								pile3[pile3cntr-1].setType('0');
								pile3[pile3cntr-1].setNumber(00);
								pile3[pile3cntr-1].setStatus("unknown");
								pile3cntr--;
							} else if (FCcntr > 0 && tempCardNum-1 == FCcntr) {
								foundationC[FCcntr] = pile3[pile3cntr-1];
								FCcntr++;
								pile3[pile3cntr-1].setType('0');
								pile3[pile3cntr-1].setNumber(00);
								pile3[pile3cntr-1].setStatus("unknown");
								pile3cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'S':
							if (FScntr == 0 && tempCardNum == 01) {
								foundationS[0] = pile3[pile3cntr-1];
								FScntr++;
								pile3[pile3cntr-1].setType('0');
								pile3[pile3cntr-1].setNumber(00);
								pile3[pile3cntr-1].setStatus("unknown");
								pile3cntr--;
							} else if (FScntr > 0 && tempCardNum-1 == FScntr) {
								foundationS[FScntr] = pile3[pile3cntr-1];
								FScntr++;
								pile3[pile3cntr-1].setType('0');
								pile3[pile3cntr-1].setNumber(00);
								pile3[pile3cntr-1].setStatus("unknown");
								pile3cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
							}
						break;

					}

					case 4:
					{
						if (pile4cntr == 0) {
							output << endl;
							output << "Invalid Move!" << endl;
						}

						char tempCardType = pile4[pile4cntr-1].getType();
						int tempCardNum = pile4[pile4cntr-1].getNumber();
						switch(tempCardType) {
						case 'H':
							if (FHcntr == 0 && tempCardNum == 01) {
								foundationH[0] = pile4[pile4cntr-1];
								FHcntr++;
								pile4[pile4cntr-1].setType('0');
								pile4[pile4cntr-1].setNumber(00);
								pile4[pile4cntr-1].setStatus("unknown");
								pile4cntr--;
							} else if (FHcntr > 0 && tempCardNum-1 == FHcntr) {
								foundationH[FHcntr] = pile4[pile4cntr-1];
								FHcntr++;
								pile4[pile4cntr-1].setType('0');
								pile4[pile4cntr-1].setNumber(00);
								pile4[pile4cntr-1].setStatus("unknown");
								pile4cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'D':
							if (FDcntr == 0 && tempCardNum == 01) {
								foundationD[0] = pile4[pile4cntr-1];
								FDcntr++;
								pile4[pile4cntr-1].setType('0');
								pile4[pile4cntr-1].setNumber(00);
								pile4[pile4cntr-1].setStatus("unknown");
								pile4cntr--;
							} else if (FDcntr > 0 && tempCardNum-1 == FDcntr) {
								foundationD[FDcntr] = pile4[pile4cntr-1];
								FDcntr++;
								pile4[pile4cntr-1].setType('0');
								pile4[pile4cntr-1].setNumber(00);
								pile4[pile4cntr-1].setStatus("unknown");
								pile4cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'C':
							if (FCcntr == 0 && tempCardNum == 01) {
								foundationC[0] = pile4[pile4cntr-1];
								FCcntr++;
								pile4[pile4cntr-1].setType('0');
								pile4[pile4cntr-1].setNumber(00);
								pile4[pile4cntr-1].setStatus("unknown");
								pile4cntr--;
							} else if (FCcntr > 0 && tempCardNum-1 == FCcntr) {
								foundationC[FCcntr] = pile4[pile4cntr-1];
								FCcntr++;
								pile4[pile4cntr-1].setType('0');
								pile4[pile4cntr-1].setNumber(00);
								pile4[pile4cntr-1].setStatus("unknown");
								pile4cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'S':
							if (FScntr == 0 && tempCardNum == 01) {
								foundationS[0] = pile4[pile4cntr-1];
								FScntr++;
								pile4[pile4cntr-1].setType('0');
								pile4[pile4cntr-1].setNumber(00);
								pile4[pile4cntr-1].setStatus("unknown");
								pile4cntr--;
							} else if (FScntr > 0 && tempCardNum-1 == FScntr) {
								foundationS[FScntr] = pile4[pile4cntr-1];
								FScntr++;
								pile4[pile4cntr-1].setType('0');
								pile4[pile4cntr-1].setNumber(00);
								pile4[pile4cntr-1].setStatus("unknown");
								pile4cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
							}
						break;

					}

					case 5:
					{
						if (pile5cntr == 0) {
							output << endl;
							output << "Invalid Move!" << endl;
						}

						char tempCardType = pile5[pile5cntr-1].getType();
						int tempCardNum = pile5[pile5cntr-1].getNumber();
						switch(tempCardType) {
						case 'H':
							if (FHcntr == 0 && tempCardNum == 01) {
								foundationH[0] = pile5[pile5cntr-1];
								FHcntr++;
								pile5[pile5cntr-1].setType('0');
								pile5[pile5cntr-1].setNumber(00);
								pile5[pile5cntr-1].setStatus("unknown");
								pile5cntr--;
							} else if (FHcntr > 0 && tempCardNum-1 == FHcntr) {
								foundationH[FHcntr] = pile5[pile5cntr-1];
								FHcntr++;
								pile5[pile5cntr-1].setType('0');
								pile5[pile5cntr-1].setNumber(00);
								pile5[pile5cntr-1].setStatus("unknown");
								pile5cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'D':
							if (FDcntr == 0 && tempCardNum == 01) {
								foundationD[0] = pile5[pile5cntr-1];
								FDcntr++;
								pile5[pile5cntr-1].setType('0');
								pile5[pile5cntr-1].setNumber(00);
								pile5[pile5cntr-1].setStatus("unknown");
								pile5cntr--;
							} else if (FDcntr > 0 && tempCardNum-1 == FDcntr) {
								foundationD[FDcntr] = pile5[pile5cntr-1];
								FDcntr++;
								pile5[pile5cntr-1].setType('0');
								pile5[pile5cntr-1].setNumber(00);
								pile5[pile5cntr-1].setStatus("unknown");
								pile5cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'C':
							if (FCcntr == 0 && tempCardNum == 01) {
								foundationC[0] = pile5[pile5cntr-1];
								FCcntr++;
								pile5[pile5cntr-1].setType('0');
								pile5[pile5cntr-1].setNumber(00);
								pile5[pile5cntr-1].setStatus("unknown");
								pile5cntr--;
							} else if (FCcntr > 0 && tempCardNum-1 == FCcntr) {
								foundationC[FCcntr] = pile5[pile5cntr-1];
								FCcntr++;
								pile5[pile5cntr-1].setType('0');
								pile5[pile5cntr-1].setNumber(00);
								pile5[pile5cntr-1].setStatus("unknown");
								pile5cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'S':
							if (FScntr == 0 && tempCardNum == 01) {
								foundationS[0] = pile5[pile5cntr-1];
								FScntr++;
								pile5[pile5cntr-1].setType('0');
								pile5[pile5cntr-1].setNumber(00);
								pile5[pile5cntr-1].setStatus("unknown");
								pile5cntr--;
							} else if (FScntr > 0 && tempCardNum-1 == FScntr) {
								foundationS[FScntr] = pile5[pile5cntr-1];
								FScntr++;
								pile5[pile5cntr-1].setType('0');
								pile5[pile5cntr-1].setNumber(00);
								pile5[pile5cntr-1].setStatus("unknown");
								pile5cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
							}
						break;
					}
					case 6:
					{
						if (pile6cntr == 0) {
							output << endl;
							output << "Invalid Move!" << endl;
						}

						char tempCardType = pile6[pile6cntr-1].getType();
						int tempCardNum = pile6[pile6cntr-1].getNumber();
						switch(tempCardType) {
						case 'H':
							if (FHcntr == 0 && tempCardNum == 01) {
								foundationH[0] = pile6[pile6cntr-1];
								FHcntr++;
								pile6[pile6cntr-1].setType('0');
								pile6[pile6cntr-1].setNumber(00);
								pile6[pile6cntr-1].setStatus("unknown");
								pile6cntr--;
							} else if (FHcntr > 0 && tempCardNum-1 == FHcntr) {
								foundationH[FHcntr] = pile6[pile6cntr-1];
								FHcntr++;
								pile6[pile6cntr-1].setType('0');
								pile6[pile6cntr-1].setNumber(00);
								pile6[pile6cntr-1].setStatus("unknown");
								pile6cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'D':
							if (FDcntr == 0 && tempCardNum == 01) {
								foundationD[0] = pile6[pile6cntr-1];
								FDcntr++;
								pile6[pile6cntr-1].setType('0');
								pile6[pile6cntr-1].setNumber(00);
								pile6[pile6cntr-1].setStatus("unknown");
								pile6cntr--;
							} else if (FDcntr > 0 && tempCardNum-1 == FDcntr) {
								foundationD[FDcntr] = pile6[pile6cntr-1];
								FDcntr++;
								pile6[pile6cntr-1].setType('0');
								pile6[pile6cntr-1].setNumber(00);
								pile6[pile6cntr-1].setStatus("unknown");
								pile6cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'C':
							if (FCcntr == 0 && tempCardNum == 01) {
								foundationC[0] = pile6[pile6cntr-1];
								FCcntr++;
								pile6[pile6cntr-1].setType('0');
								pile6[pile6cntr-1].setNumber(00);
								pile6[pile6cntr-1].setStatus("unknown");
								pile6cntr--;
							} else if (FCcntr > 0 && tempCardNum-1 == FCcntr) {
								foundationC[FCcntr] = pile6[pile6cntr-1];
								FCcntr++;
								pile6[pile6cntr-1].setType('0');
								pile6[pile6cntr-1].setNumber(00);
								pile6[pile6cntr-1].setStatus("unknown");
								pile6cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
						case 'S':
							if (FScntr == 0 && tempCardNum == 01) {
								foundationS[0] = pile6[pile6cntr-1];
								FScntr++;
								pile6[pile6cntr-1].setType('0');
								pile6[pile6cntr-1].setNumber(00);
								pile6[pile6cntr-1].setStatus("unknown");
								pile6cntr--;
							} else if (FScntr > 0 && tempCardNum-1 == FScntr) {
								foundationS[FScntr] = pile6[pile6cntr-1];
								FScntr++;
								pile6[pile6cntr-1].setType('0');
								pile6[pile6cntr-1].setNumber(00);
								pile6[pile6cntr-1].setStatus("unknown");
								pile6cntr--;
							} else {
								output << endl;
								output << "Invalid Move!" << endl;
							}
							break;
							}
						break;
					}
					}

				} else { // move to foundation waste
					output << comma1 << " " << comma2 << " " << comma3 << " " << comma4 << endl;
					if (wasteCntr == 0) {output << "Invalid Move!" << endl;}
					char tempCardType = waste[wasteCntr-1].getType();
					int tempCardNum = waste[wasteCntr-1].getNumber();

					switch(tempCardType) {
					case 'H': {
						if (FHcntr == 0 && tempCardNum == 01) {
							foundationH[0] = waste[wasteCntr-1];
							FHcntr++;
							waste[wasteCntr-1].setType('0');
							waste[wasteCntr-1].setNumber(00);
							waste[wasteCntr-1].setStatus("unknown");
							wasteCntr--;
						} else if (FHcntr > 0 && tempCardNum-1 == FHcntr) {
							foundationH[FHcntr] = waste[wasteCntr-1];
							FHcntr++;
							waste[wasteCntr-1].setType('0');
							waste[wasteCntr-1].setNumber(00);
							waste[wasteCntr-1].setStatus("unknown");
							wasteCntr--;
						} else {
							output << endl;
							output << "Invalid Move!" << endl;
						}
						break;
					}
					case 'D': {
						if (FDcntr == 0 && tempCardNum == 01) {
							foundationD[0] = waste[wasteCntr-1];
							FDcntr++;
							waste[wasteCntr-1].setType('0');
							waste[wasteCntr-1].setNumber(00);
							waste[wasteCntr-1].setStatus("unknown");
							wasteCntr--;
						} else if (FDcntr > 0 && tempCardNum-1 == FDcntr) {
							foundationD[FDcntr] = waste[wasteCntr-1];
							FDcntr++;
							waste[wasteCntr-1].setType('0');
							waste[wasteCntr-1].setNumber(00);
							waste[wasteCntr-1].setStatus("unknown");
							wasteCntr--;
						} else {
							output << endl;
							output << "Invalid Move!" << endl;
						}
						break;
					}
					case 'C': {
						if (FCcntr == 0 && tempCardNum == 01) {
							foundationC[0] = waste[wasteCntr-1];
							FCcntr++;
							waste[wasteCntr-1].setType('0');
							waste[wasteCntr-1].setNumber(00);
							waste[wasteCntr-1].setStatus("unknown");
							wasteCntr--;
						} else if (FCcntr > 0 && tempCardNum-1 == FCcntr) {
							foundationC[FCcntr] = waste[wasteCntr-1];
							FCcntr++;
							waste[wasteCntr-1].setType('0');
							waste[wasteCntr-1].setNumber(00);
							waste[wasteCntr-1].setStatus("unknown");
							wasteCntr--;
						} else {
							output << endl;
							output << "Invalid Move!" << endl;
						}
						break;
					}
					case 'S': {
						if (FScntr == 0 && tempCardNum == 01) {
							foundationS[0] = waste[wasteCntr-1];
							FScntr++;
							waste[wasteCntr-1].setType('0');
							waste[wasteCntr-1].setNumber(00);
							waste[wasteCntr-1].setStatus("unknown");
							wasteCntr--;
						} else if (FScntr > 0 && tempCardNum-1 == FScntr) {
							foundationS[FScntr] = waste[wasteCntr-1];
							FScntr++;
							waste[wasteCntr-1].setType('0');
							waste[wasteCntr-1].setNumber(00);
							waste[wasteCntr-1].setStatus("unknown");
							wasteCntr--;
						} else {
							output << endl;
							output << "Invalid Move!" << endl;
						}
						break;
						}
					}

				}


			} else if (comma2 == "pile") { // move pile num num num
				ReadFileCommands >> comma3;
				ReadFileCommands >> comma4;
				ReadFileCommands >> comma5;
				output << comma1 << " " << comma2 << " " << comma3 << " " << comma4 << " " << comma5 <<endl;
				int alPile = stoi(comma3);
				int cardCount = stoi(comma4);
				int koyPile = stoi(comma5);
				if (alPile == koyPile) {
					output << endl;
					output << "Invalid Move!" << endl;
				} else {
					switch (alPile) {
					case 0: { // from pile0
						if (pile0cntr >= (cardCount+1) && pile0[pile0cntr-1-cardCount].getStatus() == "open") { // if take card is open and pile have enough cards
							int alCardNum = pile0[pile0cntr-1-cardCount].getNumber();
							char alCardColour = pile0[pile0cntr-1-cardCount].getColour();

							switch (koyPile) {
							case 0: {
								break;
							}
							case 1: {
								string koyCardStatus = pile1[pile1cntr-1].getStatus();
								int koyCardNum = pile1[pile1cntr-1].getNumber();
								char koyCardColour = pile1[pile1cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1 && koyCardStatus == "open") {
									for (int i=(cardCount+1); i>0; i--) {
										pile1[pile1cntr-1+i] = pile0[pile0cntr-1-((cardCount+1)-i)];
										pile0[pile0cntr-1-((cardCount+1)-i)].setType('0');
										pile0[pile0cntr-1-((cardCount+1)-i)].setNumber(00);
										pile0[pile0cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr - (cardCount+1);
									pile1cntr = pile1cntr + ((cardCount+1));
								} else if (pile1cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile1[i] = pile0[pile0cntr-1-(cardCount-i)];
										pile0[pile0cntr-1-(cardCount-i)].setType('0');
										pile0[pile0cntr-1-(cardCount-i)].setNumber(00);
										pile0[pile0cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr + (cardCount+1);
									pile0cntr = pile0cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 2: {
								string koyCardStatus = pile2[pile2cntr-1].getStatus();
								int koyCardNum = pile2[pile2cntr-1].getNumber();
								char koyCardColour = pile2[pile2cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile2[pile2cntr-1+i] = pile0[pile0cntr-1-((cardCount+1)-i)];
										pile0[pile0cntr-1-((cardCount+1)-i)].setType('0');
										pile0[pile0cntr-1-((cardCount+1)-i)].setNumber(00);
										pile0[pile0cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr - (cardCount+1);
									pile2cntr = pile2cntr + ((cardCount+1));
								} else if (pile2cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile2[i] = pile0[pile0cntr-1-(cardCount-i)];
										pile0[pile0cntr-1-(cardCount-i)].setType('0');
										pile0[pile0cntr-1-(cardCount-i)].setNumber(00);
										pile0[pile0cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr + (cardCount+1);
									pile0cntr = pile0cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 3: {
								string koyCardStatus = pile3[pile3cntr-1].getStatus();
								int koyCardNum = pile3[pile3cntr-1].getNumber();
								char koyCardColour = pile3[pile3cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile3[pile3cntr-1+i] = pile0[pile0cntr-1-((cardCount+1)-i)];
										pile0[pile0cntr-1-((cardCount+1)-i)].setType('0');
										pile0[pile0cntr-1-((cardCount+1)-i)].setNumber(00);
										pile0[pile0cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr - (cardCount+1);
									pile3cntr = pile3cntr + ((cardCount+1));
								} else if (pile3cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile3[i] = pile0[pile0cntr-1-(cardCount-i)];
										pile0[pile0cntr-1-(cardCount-i)].setType('0');
										pile0[pile0cntr-1-(cardCount-i)].setNumber(00);
										pile0[pile0cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr + (cardCount+1);
									pile0cntr = pile0cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 4: {
								string koyCardStatus = pile4[pile4cntr-1].getStatus();
								int koyCardNum = pile4[pile4cntr-1].getNumber();
								char koyCardColour = pile4[pile4cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile4[pile4cntr-1+i] = pile0[pile0cntr-1-((cardCount+1)-i)];
										pile0[pile0cntr-1-((cardCount+1)-i)].setType('0');
										pile0[pile0cntr-1-((cardCount+1)-i)].setNumber(00);
										pile0[pile0cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr - (cardCount+1);
									pile4cntr = pile4cntr + ((cardCount+1));
								} else if (pile4cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile4[i] = pile0[pile0cntr-1-(cardCount-i)];
										pile0[pile0cntr-1-(cardCount-i)].setType('0');
										pile0[pile0cntr-1-(cardCount-i)].setNumber(00);
										pile0[pile0cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr + (cardCount+1);
									pile0cntr = pile0cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 5: {
								string koyCardStatus = pile5[pile5cntr-1].getStatus();
								int koyCardNum = pile5[pile5cntr-1].getNumber();
								char koyCardColour = pile5[pile5cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile5[pile5cntr-1+i] = pile0[pile0cntr-1-((cardCount+1)-i)];
										pile0[pile0cntr-1-((cardCount+1)-i)].setType('0');
										pile0[pile0cntr-1-((cardCount+1)-i)].setNumber(00);
										pile0[pile0cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr - (cardCount+1);
									pile5cntr = pile5cntr + ((cardCount+1));
								} else if (pile5cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile5[i] = pile0[pile0cntr-1-(cardCount-i)];
										pile0[pile0cntr-1-(cardCount-i)].setType('0');
										pile0[pile0cntr-1-(cardCount-i)].setNumber(00);
										pile0[pile0cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr + (cardCount+1);
									pile0cntr = pile0cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 6: {
								string koyCardStatus = pile6[pile6cntr-1].getStatus();
								int koyCardNum = pile6[pile6cntr-1].getNumber();
								char koyCardColour = pile6[pile6cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile6[pile6cntr-1+i] = pile0[pile0cntr-1-((cardCount+1)-i)];
										pile0[pile0cntr-1-((cardCount+1)-i)].setType('0');
										pile0[pile0cntr-1-((cardCount+1)-i)].setNumber(00);
										pile0[pile0cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr - (cardCount+1);
									pile6cntr = pile6cntr + ((cardCount+1));
								} else if (pile6cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile6[i] = pile0[pile0cntr-1-(cardCount-i)];
										pile0[pile0cntr-1-(cardCount-i)].setType('0');
										pile0[pile0cntr-1-(cardCount-i)].setNumber(00);
										pile0[pile0cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr + (cardCount+1);
									pile0cntr = pile0cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							}
						} else { // if there is an open card
							output << endl;
							output << "Invalid Move!" << endl;
						}
						break;
					}
					case 1: { //from pile 1 to ...
						if (pile1cntr >= (cardCount+1) && pile1[pile1cntr-1-cardCount].getStatus() == "open") {
							int alCardNum = pile1[pile1cntr-1-cardCount].getNumber();
							char alCardColour = pile1[pile1cntr-1-cardCount].getColour();

							switch (koyPile) {
							case 0: {
								string koyCardStatus = pile0[pile0cntr-1].getStatus();
								int koyCardNum = pile0[pile0cntr-1].getNumber();
								char koyCardColour = pile0[pile0cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile0[pile0cntr-1+i] = pile1[pile1cntr-1-((cardCount+1)-i)];
										pile1[pile1cntr-1-((cardCount+1)-i)].setType('0');
										pile1[pile1cntr-1-((cardCount+1)-i)].setNumber(00);
										pile1[pile1cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr - (cardCount+1);
									pile0cntr = pile1cntr + ((cardCount+1));
								} else if (pile0cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile0[i] = pile1[pile1cntr-1-(cardCount-i)];
										pile1[pile1cntr-1-(cardCount-i)].setType('0');
										pile1[pile1cntr-1-(cardCount-i)].setNumber(00);
										pile1[pile1cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr + (cardCount+1);
									pile1cntr = pile1cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 1: {
								break;
							}
							case 2: {
								string koyCardStatus = pile2[pile2cntr-1].getStatus();
								int koyCardNum = pile2[pile2cntr-1].getNumber();
								char koyCardColour = pile2[pile2cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile2[pile2cntr-1+i] = pile1[pile1cntr-1-((cardCount+1)-i)];
										pile1[pile1cntr-1-((cardCount+1)-i)].setType('0');
										pile1[pile1cntr-1-((cardCount+1)-i)].setNumber(00);
										pile1[pile1cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr - (cardCount+1);
									pile2cntr = pile2cntr + ((cardCount+1));
								} else if (pile2cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile2[i] = pile1[pile1cntr-1-(cardCount-i)];
										pile1[pile1cntr-1-(cardCount-i)].setType('0');
										pile1[pile1cntr-1-(cardCount-i)].setNumber(00);
										pile1[pile1cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr + (cardCount+1);
									pile1cntr = pile1cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 3: {
								string koyCardStatus = pile3[pile3cntr-1].getStatus();
								int koyCardNum = pile3[pile3cntr-1].getNumber();
								char koyCardColour = pile3[pile3cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile3[pile3cntr-1+i] = pile1[pile1cntr-1-((cardCount+1)-i)];
										pile1[pile1cntr-1-((cardCount+1)-i)].setType('0');
										pile1[pile1cntr-1-((cardCount+1)-i)].setNumber(00);
										pile1[pile1cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr - (cardCount+1);
									pile3cntr = pile3cntr + ((cardCount+1));
								} else if (pile3cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile3[i] = pile1[pile1cntr-1-(cardCount-i)];
										pile1[pile1cntr-1-(cardCount-i)].setType('0');
										pile1[pile1cntr-1-(cardCount-i)].setNumber(00);
										pile1[pile1cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr + (cardCount+1);
									pile1cntr = pile1cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 4: {
								string koyCardStatus = pile4[pile4cntr-1].getStatus();
								int koyCardNum = pile4[pile4cntr-1].getNumber();
								char koyCardColour = pile4[pile4cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile4[pile4cntr-1+i] = pile1[pile1cntr-1-((cardCount+1)-i)];
										pile1[pile1cntr-1-((cardCount+1)-i)].setType('0');
										pile1[pile1cntr-1-((cardCount+1)-i)].setNumber(00);
										pile1[pile1cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr - (cardCount+1);
									pile4cntr = pile4cntr + ((cardCount+1));
								} else if (pile4cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile4[i] = pile1[pile1cntr-1-(cardCount-i)];
										pile1[pile1cntr-1-(cardCount-i)].setType('0');
										pile1[pile1cntr-1-(cardCount-i)].setNumber(00);
										pile1[pile1cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr + (cardCount+1);
									pile1cntr = pile1cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 5: {
								string koyCardStatus = pile5[pile5cntr-1].getStatus();
								int koyCardNum = pile5[pile5cntr-1].getNumber();
								char koyCardColour = pile5[pile5cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile5[pile5cntr-1+i] = pile1[pile1cntr-1-((cardCount+1)-i)];
										pile1[pile1cntr-1-((cardCount+1)-i)].setType('0');
										pile1[pile1cntr-1-((cardCount+1)-i)].setNumber(00);
										pile1[pile1cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr - (cardCount+1);
									pile5cntr = pile5cntr + ((cardCount+1));
								} else if (pile5cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile5[i] = pile1[pile1cntr-1-(cardCount-i)];
										pile1[pile1cntr-1-(cardCount-i)].setType('0');
										pile1[pile1cntr-1-(cardCount-i)].setNumber(00);
										pile1[pile1cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr + (cardCount+1);
									pile1cntr = pile1cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 6: {
								string koyCardStatus = pile6[pile6cntr-1].getStatus();
								int koyCardNum = pile6[pile6cntr-1].getNumber();
								char koyCardColour = pile6[pile6cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile6[pile6cntr-1+i] = pile1[pile1cntr-1-((cardCount+1)-i)];
										pile1[pile1cntr-1-((cardCount+1)-i)].setType('0');
										pile1[pile1cntr-1-((cardCount+1)-i)].setNumber(00);
										pile1[pile1cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr - (cardCount+1);
									pile6cntr = pile6cntr + ((cardCount+1));
								} else if (pile6cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile6[i] = pile1[pile1cntr-1-(cardCount-i)];
										pile1[pile1cntr-1-(cardCount-i)].setType('0');
										pile1[pile1cntr-1-(cardCount-i)].setNumber(00);
										pile1[pile1cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr + (cardCount+1);
									pile1cntr = pile1cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							}
						} else { // can't move closed card
							output << endl;
							output << "Invalid Move!" << endl;
						}
						break;
					}
					case 2: {
						if (pile2cntr >= (cardCount+1) && pile2[pile2cntr-1-cardCount].getStatus() == "open") { // if card is open
							int alCardNum = pile2[pile2cntr-1-cardCount].getNumber();
							char alCardColour = pile2[pile2cntr-1-cardCount].getColour();

							switch (koyPile) {
							case 0: {
								string koyCardStatus = pile0[pile0cntr-1].getStatus();
								int koyCardNum = pile0[pile0cntr-1].getNumber();
								char koyCardColour = pile0[pile0cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile0[pile0cntr-1+i] = pile2[pile2cntr-1-((cardCount+1)-i)];
										pile2[pile2cntr-1-((cardCount+1)-i)].setType('0');
										pile2[pile2cntr-1-((cardCount+1)-i)].setNumber(00);
										pile2[pile2cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr - (cardCount+1);
									pile0cntr = pile0cntr + ((cardCount+1));
								} else if (pile0cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile0[i] = pile2[pile2cntr-1-(cardCount-i)];
										pile2[pile2cntr-1-(cardCount-i)].setType('0');
										pile2[pile2cntr-1-(cardCount-i)].setNumber(00);
										pile2[pile2cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr + (cardCount+1);
									pile2cntr = pile2cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 1: {
								string koyCardStatus = pile1[pile1cntr-1].getStatus();
								int koyCardNum = pile1[pile1cntr-1].getNumber();
								char koyCardColour = pile1[pile1cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile1[pile1cntr-1+i] = pile2[pile2cntr-1-((cardCount+1)-i)];
										pile2[pile2cntr-1-((cardCount+1)-i)].setType('0');
										pile2[pile2cntr-1-((cardCount+1)-i)].setNumber(00);
										pile2[pile2cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr - (cardCount+1);
									pile1cntr = pile1cntr + ((cardCount+1));
								} else if (pile1cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile1[i] = pile2[pile2cntr-1-(cardCount-i)];
										pile2[pile2cntr-1-(cardCount-i)].setType('0');
										pile2[pile2cntr-1-(cardCount-i)].setNumber(00);
										pile2[pile2cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr + (cardCount+1);
									pile2cntr = pile2cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 2: {
								break;
							}
							case 3: {
								string koyCardStatus = pile3[pile3cntr-1].getStatus();
								int koyCardNum = pile3[pile3cntr-1].getNumber();
								char koyCardColour = pile3[pile3cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile3[pile3cntr-1+i] = pile2[pile2cntr-1-((cardCount+1)-i)];
										pile2[pile2cntr-1-((cardCount+1)-i)].setType('0');
										pile2[pile2cntr-1-((cardCount+1)-i)].setNumber(00);
										pile2[pile2cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr - (cardCount+1);
									pile3cntr = pile3cntr + ((cardCount+1));
								} else if (pile3cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile3[i] = pile2[pile2cntr-1-(cardCount-i)];
										pile2[pile2cntr-1-(cardCount-i)].setType('0');
										pile2[pile2cntr-1-(cardCount-i)].setNumber(00);
										pile2[pile2cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr + (cardCount+1);
									pile2cntr = pile2cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 4: {
								string koyCardStatus = pile4[pile4cntr-1].getStatus();
								int koyCardNum = pile4[pile4cntr-1].getNumber();
								char koyCardColour = pile4[pile4cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile4[pile4cntr-1+i] = pile2[pile2cntr-1-((cardCount+1)-i)];
										pile2[pile2cntr-1-((cardCount+1)-i)].setType('0');
										pile2[pile2cntr-1-((cardCount+1)-i)].setNumber(00);
										pile2[pile2cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr - (cardCount+1);
									pile4cntr = pile4cntr + ((cardCount+1));
								} else if (pile4cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile4[i] = pile2[pile2cntr-1-(cardCount-i)];
										pile2[pile2cntr-1-(cardCount-i)].setType('0');
										pile2[pile2cntr-1-(cardCount-i)].setNumber(00);
										pile2[pile2cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr + (cardCount+1);
									pile2cntr = pile2cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 5: {
								string koyCardStatus = pile5[pile5cntr-1].getStatus();
								int koyCardNum = pile5[pile5cntr-1].getNumber();
								char koyCardColour = pile5[pile5cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile5[pile5cntr-1+i] = pile2[pile2cntr-1-((cardCount+1)-i)];
										pile2[pile2cntr-1-((cardCount+1)-i)].setType('0');
										pile2[pile2cntr-1-((cardCount+1)-i)].setNumber(00);
										pile2[pile2cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr - (cardCount+1);
									pile5cntr = pile5cntr + ((cardCount+1));
								} else if (pile5cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile5[i] = pile2[pile2cntr-1-(cardCount-i)];
										pile2[pile2cntr-1-(cardCount-i)].setType('0');
										pile2[pile2cntr-1-(cardCount-i)].setNumber(00);
										pile2[pile2cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr + (cardCount+1);
									pile2cntr = pile2cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 6: {
								string koyCardStatus = pile6[pile6cntr-1].getStatus();
								int koyCardNum = pile6[pile6cntr-1].getNumber();
								char koyCardColour = pile6[pile6cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile6[pile6cntr-1+i] = pile2[pile2cntr-1-((cardCount+1)-i)];
										pile2[pile2cntr-1-((cardCount+1)-i)].setType('0');
										pile2[pile2cntr-1-((cardCount+1)-i)].setNumber(00);
										pile2[pile2cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr - (cardCount+1);
									pile6cntr = pile6cntr + ((cardCount+1));
								} else if (pile6cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile6[i] = pile2[pile2cntr-1-(cardCount-i)];
										pile2[pile2cntr-1-(cardCount-i)].setType('0');
										pile2[pile2cntr-1-(cardCount-i)].setNumber(00);
										pile2[pile2cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr + (cardCount+1);
									pile2cntr = pile2cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							}
						} else { // card is closed
							output << endl;
							output << "Invalid Move!" << endl;
						}
						break;
					}
					case 3: {
						if (pile3cntr >= (cardCount+1) && pile3[pile3cntr-1-cardCount].getStatus() == "open") {
							int alCardNum = pile3[pile3cntr-1-cardCount].getNumber();
							char alCardColour = pile3[pile3cntr-1-cardCount].getColour();

							switch (koyPile) {
							case 0: {
								string koyCardStatus = pile0[pile0cntr-1].getStatus();
								int koyCardNum = pile0[pile0cntr-1].getNumber();
								char koyCardColour = pile0[pile0cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile0[pile0cntr-1+i] = pile3[pile3cntr-1-((cardCount+1)-i)];
										pile3[pile3cntr-1-((cardCount+1)-i)].setType('0');
										pile3[pile3cntr-1-((cardCount+1)-i)].setNumber(00);
										pile3[pile3cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr - (cardCount+1);
									pile0cntr = pile0cntr + ((cardCount+1));
								} else if (pile0cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile0[i] = pile3[pile3cntr-1-(cardCount-i)];
										pile3[pile3cntr-1-(cardCount-i)].setType('0');
										pile3[pile3cntr-1-(cardCount-i)].setNumber(00);
										pile3[pile3cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr + (cardCount+1);
									pile3cntr = pile3cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 1: {
								string koyCardStatus = pile1[pile1cntr-1].getStatus();
								int koyCardNum = pile1[pile1cntr-1].getNumber();
								char koyCardColour = pile1[pile1cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile1[pile1cntr-1+i] = pile3[pile3cntr-1-((cardCount+1)-i)];
										pile3[pile3cntr-1-((cardCount+1)-i)].setType('0');
										pile3[pile3cntr-1-((cardCount+1)-i)].setNumber(00);
										pile3[pile3cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr - (cardCount+1);
									pile1cntr = pile1cntr + ((cardCount+1));
								} else if (pile1cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile1[i] = pile3[pile3cntr-1-(cardCount-i)];
										pile3[pile3cntr-1-(cardCount-i)].setType('0');
										pile3[pile3cntr-1-(cardCount-i)].setNumber(00);
										pile3[pile3cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr + (cardCount+1);
									pile3cntr = pile3cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 2: {
								string koyCardStatus = pile2[pile2cntr-1].getStatus();
								int koyCardNum = pile2[pile2cntr-1].getNumber();
								char koyCardColour = pile2[pile2cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile2[pile2cntr-1+i] = pile3[pile3cntr-1-((cardCount+1)-i)];
										pile3[pile3cntr-1-((cardCount+1)-i)].setType('0');
										pile3[pile3cntr-1-((cardCount+1)-i)].setNumber(00);
										pile3[pile3cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr - (cardCount+1);
									pile2cntr = pile2cntr + ((cardCount+1));
								} else if (pile2cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile2[i] = pile3[pile3cntr-1-(cardCount-i)];
										pile3[pile3cntr-1-(cardCount-i)].setType('0');
										pile3[pile3cntr-1-(cardCount-i)].setNumber(00);
										pile3[pile3cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr + (cardCount+1);
									pile3cntr = pile3cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 3: {
								break;
							}
							case 4: {
								string koyCardStatus = pile4[pile4cntr-1].getStatus();
								int koyCardNum = pile4[pile4cntr-1].getNumber();
								char koyCardColour = pile4[pile4cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile4[pile4cntr-1+i] = pile3[pile3cntr-1-((cardCount+1)-i)];
										pile3[pile3cntr-1-((cardCount+1)-i)].setType('0');
										pile3[pile3cntr-1-((cardCount+1)-i)].setNumber(00);
										pile3[pile3cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr - (cardCount+1);
									pile4cntr = pile4cntr + ((cardCount+1));
								} else if (pile4cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile4[i] = pile3[pile3cntr-1-(cardCount-i)];
										pile3[pile3cntr-1-(cardCount-i)].setType('0');
										pile3[pile3cntr-1-(cardCount-i)].setNumber(00);
										pile3[pile3cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr + (cardCount+1);
									pile3cntr = pile3cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 5: {
								string koyCardStatus = pile5[pile5cntr-1].getStatus();
								int koyCardNum = pile5[pile5cntr-1].getNumber();
								char koyCardColour = pile5[pile5cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile5[pile5cntr-1+i] = pile3[pile3cntr-1-((cardCount+1)-i)];
										pile3[pile3cntr-1-((cardCount+1)-i)].setType('0');
										pile3[pile3cntr-1-((cardCount+1)-i)].setNumber(00);
										pile3[pile3cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr - (cardCount+1);
									pile5cntr = pile5cntr + ((cardCount+1));
								} else if (pile5cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile5[i] = pile3[pile3cntr-1-(cardCount-i)];
										pile3[pile3cntr-1-(cardCount-i)].setType('0');
										pile3[pile3cntr-1-(cardCount-i)].setNumber(00);
										pile3[pile3cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr + (cardCount+1);
									pile3cntr = pile3cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 6: {
								string koyCardStatus = pile6[pile6cntr-1].getStatus();
								int koyCardNum = pile6[pile6cntr-1].getNumber();
								char koyCardColour = pile6[pile6cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile6[pile6cntr-1+i] = pile3[pile3cntr-1-((cardCount+1)-i)];
										pile3[pile3cntr-1-((cardCount+1)-i)].setType('0');
										pile3[pile3cntr-1-((cardCount+1)-i)].setNumber(00);
										pile3[pile3cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr - (cardCount+1);
									pile6cntr = pile6cntr + ((cardCount+1));
								} else if (pile6cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile6[i] = pile3[pile3cntr-1-(cardCount-i)];
										pile3[pile3cntr-1-(cardCount-i)].setType('0');
										pile3[pile3cntr-1-(cardCount-i)].setNumber(00);
										pile3[pile3cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr + (cardCount+1);
									pile3cntr = pile3cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							}
						} else {
							output << endl;
							output << "Invalid Move!" << endl;
						}
						break;
					}
					case 4: {
						if (pile4cntr >= (cardCount+1) && pile4[pile4cntr-1-cardCount].getStatus() == "open") {
							int alCardNum = pile4[pile4cntr-1-cardCount].getNumber();
							char alCardColour = pile4[pile4cntr-1-cardCount].getColour();

							switch (koyPile) {
							case 0: {
								string koyCardStatus = pile0[pile0cntr-1].getStatus();
								int koyCardNum = pile0[pile0cntr-1].getNumber();
								char koyCardColour = pile0[pile0cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile0[pile0cntr-1+i] = pile4[pile4cntr-1-((cardCount+1)-i)];
										pile4[pile4cntr-1-((cardCount+1)-i)].setType('0');
										pile4[pile4cntr-1-((cardCount+1)-i)].setNumber(00);
										pile4[pile4cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr - (cardCount+1);
									pile0cntr = pile0cntr + ((cardCount+1));
								} else if (pile0cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile0[i] = pile4[pile4cntr-1-(cardCount-i)];
										pile4[pile4cntr-1-(cardCount-i)].setType('0');
										pile4[pile4cntr-1-(cardCount-i)].setNumber(00);
										pile4[pile4cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr + (cardCount+1);
									pile4cntr = pile4cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 1: {
								string koyCardStatus = pile1[pile1cntr-1].getStatus();
								int koyCardNum = pile1[pile1cntr-1].getNumber();
								char koyCardColour = pile1[pile1cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile1[pile1cntr-1+i] = pile4[pile4cntr-1-((cardCount+1)-i)];
										pile4[pile4cntr-1-((cardCount+1)-i)].setType('0');
										pile4[pile4cntr-1-((cardCount+1)-i)].setNumber(00);
										pile4[pile4cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr - (cardCount+1);
									pile1cntr = pile1cntr + ((cardCount+1));
								} else if (pile1cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile1[i] = pile4[pile4cntr-1-(cardCount-i)];
										pile4[pile4cntr-1-(cardCount-i)].setType('0');
										pile4[pile4cntr-1-(cardCount-i)].setNumber(00);
										pile4[pile4cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr + (cardCount+1);
									pile4cntr = pile4cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 2: {
								string koyCardStatus = pile2[pile2cntr-1].getStatus();
								int koyCardNum = pile2[pile2cntr-1].getNumber();
								char koyCardColour = pile2[pile2cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile2[pile2cntr-1+i] = pile4[pile4cntr-1-((cardCount+1)-i)];
										pile4[pile4cntr-1-((cardCount+1)-i)].setType('0');
										pile4[pile4cntr-1-((cardCount+1)-i)].setNumber(00);
										pile4[pile4cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr - (cardCount+1);
									pile2cntr = pile2cntr + ((cardCount+1));
								} else if (pile2cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile2[i] = pile4[pile4cntr-1-(cardCount-i)];
										pile4[pile4cntr-1-(cardCount-i)].setType('0');
										pile4[pile4cntr-1-(cardCount-i)].setNumber(00);
										pile4[pile4cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr + (cardCount+1);
									pile4cntr = pile4cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 3: {
								string koyCardStatus = pile3[pile3cntr-1].getStatus();
								int koyCardNum = pile3[pile3cntr-1].getNumber();
								char koyCardColour = pile3[pile3cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile3[pile3cntr-1+i] = pile4[pile4cntr-1-((cardCount+1)-i)];
										pile4[pile4cntr-1-((cardCount+1)-i)].setType('0');
										pile4[pile4cntr-1-((cardCount+1)-i)].setNumber(00);
										pile4[pile4cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr - (cardCount+1);
									pile3cntr = pile3cntr + ((cardCount+1));
								} else if (pile3cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile3[i] = pile4[pile4cntr-1-(cardCount-i)];
										pile4[pile4cntr-1-(cardCount-i)].setType('0');
										pile4[pile4cntr-1-(cardCount-i)].setNumber(00);
										pile4[pile4cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr + (cardCount+1);
									pile4cntr = pile4cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 4: {
								break;
							}
							case 5: {
								string koyCardStatus = pile5[pile5cntr-1].getStatus();
								int koyCardNum = pile5[pile5cntr-1].getNumber();
								char koyCardColour = pile5[pile5cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile5[pile5cntr-1+i] = pile4[pile4cntr-1-((cardCount+1)-i)];
										pile4[pile4cntr-1-((cardCount+1)-i)].setType('0');
										pile4[pile4cntr-1-((cardCount+1)-i)].setNumber(00);
										pile4[pile4cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr - (cardCount+1);
									pile5cntr = pile5cntr + ((cardCount+1));
								} else if (pile5cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile5[i] = pile4[pile4cntr-1-(cardCount-i)];
										pile4[pile4cntr-1-(cardCount-i)].setType('0');
										pile4[pile4cntr-1-(cardCount-i)].setNumber(00);
										pile4[pile4cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr + (cardCount+1);
									pile4cntr = pile4cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 6: {
								string koyCardStatus = pile6[pile6cntr-1].getStatus();
								int koyCardNum = pile6[pile6cntr-1].getNumber();
								char koyCardColour = pile6[pile6cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile6[pile6cntr-1+i] = pile4[pile4cntr-1-((cardCount+1)-i)];
										pile4[pile4cntr-1-((cardCount+1)-i)].setType('0');
										pile4[pile4cntr-1-((cardCount+1)-i)].setNumber(00);
										pile4[pile4cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr - (cardCount+1);
									pile6cntr = pile6cntr + ((cardCount+1));
								} else if (pile6cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile6[i] = pile4[pile4cntr-1-(cardCount-i)];
										pile4[pile4cntr-1-(cardCount-i)].setType('0');
										pile4[pile4cntr-1-(cardCount-i)].setNumber(00);
										pile4[pile4cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr + (cardCount+1);
									pile4cntr = pile4cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							}
						} else {
							output << endl;
							output << "Invalid Move!" << endl;
						}
						break;
					}
					case 5: { // take from pile 5
						if (pile5cntr >= (cardCount+1) && pile5[pile5cntr-1-cardCount].getStatus() == "open") {
							int alCardNum = pile5[pile5cntr-1-cardCount].getNumber();
							char alCardColour = pile5[pile5cntr-1-cardCount].getColour();

							switch (koyPile) {
							case 0: {
								string koyCardStatus = pile0[pile0cntr-1].getStatus();
								int koyCardNum = pile0[pile0cntr-1].getNumber();
								char koyCardColour = pile0[pile0cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile0[pile0cntr-1+i] = pile5[pile5cntr-1-((cardCount+1)-i)];
										pile5[pile5cntr-1-((cardCount+1)-i)].setType('0');
										pile5[pile5cntr-1-((cardCount+1)-i)].setNumber(00);
										pile5[pile5cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr - (cardCount+1);
									pile0cntr = pile0cntr + ((cardCount+1));
								} else if (pile0cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile0[i] = pile5[pile5cntr-1-(cardCount-i)];
										pile5[pile5cntr-1-(cardCount-i)].setType('0');
										pile5[pile5cntr-1-(cardCount-i)].setNumber(00);
										pile5[pile5cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr + (cardCount+1);
									pile5cntr = pile5cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 1: {
								string koyCardStatus = pile1[pile1cntr-1].getStatus();
								int koyCardNum = pile1[pile1cntr-1].getNumber();
								char koyCardColour = pile1[pile1cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile1[pile1cntr-1+i] = pile5[pile5cntr-1-((cardCount+1)-i)];
										pile5[pile5cntr-1-((cardCount+1)-i)].setType('0');
										pile5[pile5cntr-1-((cardCount+1)-i)].setNumber(00);
										pile5[pile5cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr - (cardCount+1);
									pile1cntr = pile1cntr + ((cardCount+1));
								} else if (pile1cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile1[i] = pile5[pile5cntr-1-(cardCount-i)];
										pile5[pile5cntr-1-(cardCount-i)].setType('0');
										pile5[pile5cntr-1-(cardCount-i)].setNumber(00);
										pile5[pile5cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr + (cardCount+1);
									pile5cntr = pile5cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 2: {
								string koyCardStatus = pile2[pile2cntr-1].getStatus();
								int koyCardNum = pile2[pile2cntr-1].getNumber();
								char koyCardColour = pile2[pile2cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile2[pile2cntr-1+i] = pile5[pile5cntr-1-((cardCount+1)-i)];
										pile5[pile5cntr-1-((cardCount+1)-i)].setType('0');
										pile5[pile5cntr-1-((cardCount+1)-i)].setNumber(00);
										pile5[pile5cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr - (cardCount+1);
									pile2cntr = pile2cntr + ((cardCount+1));
								} else if (pile2cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile2[i] = pile5[pile5cntr-1-(cardCount-i)];
										pile5[pile5cntr-1-(cardCount-i)].setType('0');
										pile5[pile5cntr-1-(cardCount-i)].setNumber(00);
										pile5[pile5cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr + (cardCount+1);
									pile5cntr = pile5cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 3: {
								string koyCardStatus = pile3[pile3cntr-1].getStatus();
								int koyCardNum = pile3[pile3cntr-1].getNumber();
								char koyCardColour = pile3[pile3cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile3[pile3cntr-1+i] = pile5[pile5cntr-1-((cardCount+1)-i)];
										pile5[pile5cntr-1-((cardCount+1)-i)].setType('0');
										pile5[pile5cntr-1-((cardCount+1)-i)].setNumber(00);
										pile5[pile5cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr - (cardCount+1);
									pile3cntr = pile3cntr + ((cardCount+1));
								} else if (pile3cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile3[i] = pile5[pile5cntr-1-(cardCount-i)];
										pile5[pile5cntr-1-(cardCount-i)].setType('0');
										pile5[pile5cntr-1-(cardCount-i)].setNumber(00);
										pile5[pile5cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr + (cardCount+1);
									pile5cntr = pile5cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 4: {
								string koyCardStatus = pile4[pile4cntr-1].getStatus();
								int koyCardNum = pile4[pile4cntr-1].getNumber();
								char koyCardColour = pile4[pile4cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile4[pile4cntr-1+i] = pile5[pile5cntr-1-((cardCount+1)-i)];
										pile5[pile5cntr-1-((cardCount+1)-i)].setType('0');
										pile5[pile5cntr-1-((cardCount+1)-i)].setNumber(00);
										pile5[pile5cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr - (cardCount+1);
									pile4cntr = pile4cntr + ((cardCount+1));
								} else if (pile4cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile4[i] = pile5[pile5cntr-1-(cardCount-i)];
										pile5[pile5cntr-1-(cardCount-i)].setType('0');
										pile5[pile5cntr-1-(cardCount-i)].setNumber(00);
										pile5[pile5cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr + (cardCount+1);
									pile5cntr = pile5cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 5: {
								break;
							}
							case 6: {
								string koyCardStatus = pile6[pile6cntr-1].getStatus();
								int koyCardNum = pile6[pile6cntr-1].getNumber();
								char koyCardColour = pile6[pile6cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile6[pile6cntr-1+i] = pile5[pile5cntr-1-((cardCount+1)-i)];
										pile5[pile5cntr-1-((cardCount+1)-i)].setType('0');
										pile5[pile5cntr-1-((cardCount+1)-i)].setNumber(00);
										pile5[pile5cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr - (cardCount+1);
									pile6cntr = pile6cntr + ((cardCount+1));
								} else if (pile6cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile6[i] = pile5[pile5cntr-1-(cardCount-i)];
										pile5[pile5cntr-1-(cardCount-i)].setType('0');
										pile5[pile5cntr-1-(cardCount-i)].setNumber(00);
										pile5[pile5cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr + (cardCount+1);
									pile5cntr = pile5cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							}
						} else {
							output << endl;
							output << "Invalid Move!" << endl;
						}
						break;
					}
					case 6: { //6dan aldım
						if (pile6cntr >= (cardCount+1) && pile6[pile6cntr-1-cardCount].getStatus() == "open") { // alınacak kart open ise
							int alCardNum = pile6[pile6cntr-1-cardCount].getNumber();
							char alCardColour = pile6[pile6cntr-1-cardCount].getColour();

							switch (koyPile) {
							case 0: {
								string koyCardStatus = pile0[pile0cntr-1].getStatus();
								int koyCardNum = pile0[pile0cntr-1].getNumber();
								char koyCardColour = pile0[pile0cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile0[pile0cntr-1+i] = pile6[pile6cntr-1-((cardCount+1)-i)];
										pile6[pile6cntr-1-((cardCount+1)-i)].setType('0');
										pile6[pile6cntr-1-((cardCount+1)-i)].setNumber(00);
										pile6[pile6cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr - (cardCount+1);
									pile0cntr = pile0cntr + ((cardCount+1));
								} else if (pile0cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile0[i] = pile6[pile6cntr-1-(cardCount-i)];
										pile6[pile6cntr-1-(cardCount-i)].setType('0');
										pile6[pile6cntr-1-(cardCount-i)].setNumber(00);
										pile6[pile6cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile0cntr = pile0cntr + (cardCount+1);
									pile6cntr = pile6cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 1: {
								string koyCardStatus = pile1[pile1cntr-1].getStatus();
								int koyCardNum = pile1[pile1cntr-1].getNumber();
								char koyCardColour = pile1[pile1cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile1[pile1cntr-1+i] = pile6[pile6cntr-1-((cardCount+1)-i)];
										pile6[pile6cntr-1-((cardCount+1)-i)].setType('0');
										pile6[pile6cntr-1-((cardCount+1)-i)].setNumber(00);
										pile6[pile6cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr - (cardCount+1);
									pile1cntr = pile1cntr + ((cardCount+1));
								} else if (pile1cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile1[i] = pile6[pile6cntr-1-(cardCount-i)];
										pile6[pile6cntr-1-(cardCount-i)].setType('0');
										pile6[pile6cntr-1-(cardCount-i)].setNumber(00);
										pile6[pile6cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile1cntr = pile1cntr + (cardCount+1);
									pile6cntr = pile6cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;
							}
							case 2: {
								string koyCardStatus = pile2[pile2cntr-1].getStatus();
								int koyCardNum = pile2[pile2cntr-1].getNumber();
								char koyCardColour = pile2[pile2cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile2[pile2cntr-1+i] = pile6[pile6cntr-1-((cardCount+1)-i)];
										pile6[pile6cntr-1-((cardCount+1)-i)].setType('0');
										pile6[pile6cntr-1-((cardCount+1)-i)].setNumber(00);
										pile6[pile6cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr - (cardCount+1);
									pile2cntr = pile2cntr + ((cardCount+1));
								} else if (pile2cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile2[i] = pile6[pile6cntr-1-(cardCount-i)];
										pile6[pile6cntr-1-(cardCount-i)].setType('0');
										pile6[pile6cntr-1-(cardCount-i)].setNumber(00);
										pile6[pile6cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile2cntr = pile2cntr + (cardCount+1);
									pile6cntr = pile6cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 3: {
								string koyCardStatus = pile3[pile3cntr-1].getStatus();
								int koyCardNum = pile3[pile3cntr-1].getNumber();
								char koyCardColour = pile3[pile3cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile3[pile3cntr-1+i] = pile6[pile6cntr-1-((cardCount+1)-i)];
										pile6[pile6cntr-1-((cardCount+1)-i)].setType('0');
										pile6[pile6cntr-1-((cardCount+1)-i)].setNumber(00);
										pile6[pile6cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr - (cardCount+1);
									pile3cntr = pile3cntr + ((cardCount+1));
								} else if (pile3cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile3[i] = pile6[pile6cntr-1-(cardCount-i)];
										pile6[pile6cntr-1-(cardCount-i)].setType('0');
										pile6[pile6cntr-1-(cardCount-i)].setNumber(00);
										pile6[pile6cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile3cntr = pile3cntr + (cardCount+1);
									pile6cntr = pile6cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 4: {
								string koyCardStatus = pile4[pile4cntr-1].getStatus();
								int koyCardNum = pile4[pile4cntr-1].getNumber();
								char koyCardColour = pile4[pile4cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile4[pile4cntr-1+i] = pile6[pile6cntr-1-((cardCount+1)-i)];
										pile6[pile6cntr-1-((cardCount+1)-i)].setType('0');
										pile6[pile6cntr-1-((cardCount+1)-i)].setNumber(00);
										pile6[pile6cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr - ((cardCount+1));
									pile4cntr = pile4cntr + ((cardCount+1));
								} else if (pile4cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile4[i] = pile6[pile6cntr-1-(cardCount-i)];
										pile6[pile6cntr-1-(cardCount-i)].setType('0');
										pile6[pile6cntr-1-(cardCount-i)].setNumber(00);
										pile6[pile6cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile4cntr = pile4cntr + (cardCount+1);
									pile6cntr = pile6cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 5: {
								string koyCardStatus = pile5[pile5cntr-1].getStatus();
								int koyCardNum = pile5[pile5cntr-1].getNumber();
								char koyCardColour = pile5[pile5cntr-1].getColour();
								if (alCardColour != koyCardColour && koyCardNum == alCardNum+1) {
									for (int i=(cardCount+1); i>0; i--) {
										pile5[pile5cntr-1+i] = pile6[pile6cntr-1-((cardCount+1)-i)];
										pile6[pile6cntr-1-((cardCount+1)-i)].setType('0');
										pile6[pile6cntr-1-((cardCount+1)-i)].setNumber(00);
										pile6[pile6cntr-1-((cardCount+1)-i)].setStatus("unknown");
									}
									pile6cntr = pile6cntr - ((cardCount+1));
									pile5cntr = pile5cntr + ((cardCount+1));
								} else if (pile5cntr == 0 && alCardNum == 13) {
									for(int i=0; i<cardCount+1; i++) {
										pile5[i] = pile6[pile6cntr-1-(cardCount-i)];
										pile6[pile6cntr-1-(cardCount-i)].setType('0');
										pile6[pile6cntr-1-(cardCount-i)].setNumber(00);
										pile6[pile6cntr-1-(cardCount-i)].setStatus("unknown");
									}
									pile5cntr = pile5cntr + (cardCount+1);
									pile6cntr = pile6cntr - (cardCount+1);
								} else {
									output << endl;
									output << "Invalid Move!" << endl;
								}
								break;

							}
							case 6: {
								break;
							}
							}
						} else {
							output << endl;
							output << "Invalid Move!" << endl;
						}
						break;
					}
					}
				}
			} else if (comma2 == "waste") { // MOVE WASTE NUMBER
				ReadFileCommands >> comma3;
				output << comma1 << " " << comma2 << " " << comma3 << endl;
				int pileNum = stoi(comma3);

				int wasteNum = waste[wasteCntr-1].getNumber();
				char wasteColour = waste[wasteCntr-1].getColour();

				switch (pileNum) {
				case 0: {
					if (wasteCntr == 0) {
						output << "Invalid Move!" << endl;
						break;
					}
					string koyCardStatus = pile0[pile0cntr-1].getStatus();
					int koyCardNum = pile0[pile0cntr-1].getNumber();
					char koyCardColour = pile0[pile0cntr-1].getColour();

					if (wasteColour != koyCardColour && koyCardNum == wasteNum+1 && koyCardStatus == "open") {
						pile0[pile0cntr] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						wasteCntr--;
						pile0cntr++;
					} else if (pile0cntr == 0 && wasteNum == 13) {
						pile0[0] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						pile0cntr++;
						wasteCntr--;
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 1: {
					if (wasteCntr == 0) {
						output << "Invalid Move!" << endl;
						break;
					}
					string koyCardStatus = pile1[pile1cntr-1].getStatus();
					int koyCardNum = pile1[pile1cntr-1].getNumber();
					char koyCardColour = pile1[pile1cntr-1].getColour();

					if (wasteColour != koyCardColour && koyCardNum == wasteNum+1 && koyCardStatus == "open") {
						pile1[pile1cntr] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						wasteCntr--;
						pile1cntr++;
					} else if (pile1cntr == 0 && wasteNum == 13) {
						pile1[0] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						pile1cntr++;
						wasteCntr--;
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 2: {
					if (wasteCntr == 0) {
						output << "Invalid Move!" << endl;
						break;
					}
					string koyCardStatus = pile2[pile2cntr-1].getStatus();
					int koyCardNum = pile2[pile2cntr-1].getNumber();
					char koyCardColour = pile2[pile2cntr-1].getColour();

					if (wasteColour != koyCardColour && koyCardNum == wasteNum+1 && koyCardStatus == "open") {
						pile2[pile2cntr] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						wasteCntr--;
						pile2cntr++;
					} else if (pile2cntr == 0 && wasteNum == 13) {
						pile2[0] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						pile2cntr++;
						wasteCntr--;
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 3: {
					if (wasteCntr == 0) {
						output << "Invalid Move!" << endl;
						break;
					}
					string koyCardStatus = pile3[pile3cntr-1].getStatus();
					int koyCardNum = pile3[pile3cntr-1].getNumber();
					char koyCardColour = pile3[pile3cntr-1].getColour();

					if (wasteColour != koyCardColour && koyCardNum == wasteNum+1 && koyCardStatus == "open") {
						pile3[pile3cntr] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						wasteCntr--;
						pile3cntr++;
					} else if (pile3cntr == 0 && wasteNum == 13) {
						pile3[0] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						pile3cntr++;
						wasteCntr--;
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 4: {
					if (wasteCntr == 0) {
						output << "Invalid Move!" << endl;
						break;
					}
					string koyCardStatus = pile4[pile4cntr-1].getStatus();
					int koyCardNum = pile4[pile4cntr-1].getNumber();
					char koyCardColour = pile4[pile4cntr-1].getColour();

					if (wasteColour != koyCardColour && koyCardNum == wasteNum+1 && koyCardStatus == "open") {
						pile4[pile4cntr] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						wasteCntr--;
						pile4cntr++;
					} else if (pile4cntr == 0 && wasteNum == 13) {
						pile4[0] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						pile4cntr++;
						wasteCntr--;
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 5: {
					if (wasteCntr == 0) {
						output << "Invalid Move!" << endl;
						break;
					}
					string koyCardStatus = pile5[pile5cntr-1].getStatus();
					int koyCardNum = pile5[pile5cntr-1].getNumber();
					char koyCardColour = pile5[pile5cntr-1].getColour();

					if (wasteColour != koyCardColour && koyCardNum == wasteNum+1 && koyCardStatus == "open") {
						pile5[pile5cntr] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						wasteCntr--;
						pile5cntr++;
					} else if (pile5cntr == 0 && wasteNum == 13) {
						pile5[0] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						pile5cntr++;
						wasteCntr--;
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				case 6: {
					if (wasteCntr == 0) {
						output << "Invalid Move!" << endl;
						break;
					}
					string koyCardStatus = pile6[pile6cntr-1].getStatus();
					int koyCardNum = pile6[pile6cntr-1].getNumber();
					char koyCardColour = pile6[pile6cntr-1].getColour();

					if (wasteColour != koyCardColour && koyCardNum == wasteNum+1 && koyCardStatus == "open") {
						pile6[pile6cntr] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						wasteCntr--;
						pile6cntr++;
					} else if (pile6cntr == 0 && wasteNum == 13) {
						pile6[0] = waste[wasteCntr-1];
						waste[wasteCntr-1].setType('0');
						waste[wasteCntr-1].setNumber(00);
						waste[wasteCntr-1].setStatus("unknown");
						pile6cntr++;
						wasteCntr--;
					} else {
						output << endl;
						output << "Invalid Move!" << endl;
					}
					break;
				}
				}

			}
		}
		output << endl;
		output << "****************************************" << endl;
		output << endl;

	}
		output.close();
}
