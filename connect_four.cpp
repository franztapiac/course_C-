//Connect 4 by Franz A. Tapia Chaca, CID 011119357

#include <iostream>
#include <string>
using namespace std;

// Function declarations:

//Clears the board when starting a new game
void ClearBoard();

//Prints the Connect4 Grid with appropiately placed X's and O's
void PrintBoard(); 

//Updates the Connect4 Grid based on user input of the column number
int UpdateBoard(int ColumnNum, int Player);

//Checks if the last Player Connected 4 along the column, row, and left and right diagonals of the last play position
bool CheckConnect4(int ColumnNum, int Player, int RowNum);

char Grid [6][7] = 	{
					{32, 32, 32, 32, 32, 32, 32},
					{32, 32, 32, 32, 32, 32, 32},
					{32, 32, 32, 32, 32, 32, 32},
					{32, 32, 32, 32, 32, 32, 32},
					{32, 32, 32, 32, 32, 32, 32},
					{32, 32, 32, 32, 32, 32, 32},
					};
int main() {
	char NextGame;
	do {
		int TurnCount = 0;			//The number of valid turns are counted until 42 to determine a Draw
		int ColumnNum = 0;			//Number of column put in
		bool Connect4 = false;		//Initially, no player has connected 4
		bool Draw = false;			//Initially, count of turns is 0 and not a draw game
		char Player = 88;			//ASCII; 88 for X, 79 for O
		int RowNum = 0;				//Row on which the latest move was placed
		
		ClearBoard();
		cout << "\nConnect Four Game\n" << endl;
		PrintBoard();
		while ((Connect4 || Draw) == false) {						//While the game is not over in its 2 possible ways	
			cout << "\nPlayer " << Player << ", enter a column: ";	//Prompt to enter a column number
			cin  >> ColumnNum; 										//Can be overloaded by integer that is too large e.g. any larger than 2147483647 (INT_MAX)
			if ((ColumnNum >= 1) && (ColumnNum <= 7)) {
				if (Grid[0][ColumnNum-1] != 32){ //If a column is full by the top element being taken
					cout << "Invalid. Please try again." << endl;
				}
				else {
					RowNum = UpdateBoard(ColumnNum, Player);
					Connect4 = CheckConnect4(ColumnNum, Player, RowNum); // TRUE if a player connected 4; FALSE if not
					TurnCount++;
					if (Connect4 == 0){			//If player was not changed with this condition, then the wrong played would be announced the winner
						if (Player == 88){ 		//Player X
							Player = 79; 		//Changes to player O
						}
						else {
							Player = 88; 		//And vice versa
						}
					}	
				}
			}
			else if ((ColumnNum < 1) || (ColumnNum > 7) || (cin.fail())) { 	//If user inputs numbers out of 1-7 range or other characters e.g. a,A,!
				cin.clear();												//Clears the error flag on the cin stream
				cin.ignore(10000, '\n');									//Skips 10000 bytes of characters on cin to next newline. Will not work if user inputs longer than 10000 characters
				cout << "\nInvalid. Please try again." << endl;				// Error message
			}
			PrintBoard();
			cout << "The turn count is: " << TurnCount << endl;
			if (TurnCount < 42) {  			//Board has not been filled yet
				if (Connect4 == 1){
					cout << "Player " << Player << " wins! =D" << endl;
				}
			}
			else { //If all 42 turns have been played
				if (Connect4 == 0){ //and nobody connected4
					cout << "Draw game. Nobody wins =(" << endl;
					Draw = true;
				}
				else {
					cout << "Player " << Player << " wins on the last turn! =D" << endl;
				}
			}
		}
		do {
			cout << "\nAnother game? (Y/N): ";
			cin >> NextGame;
		} while ((NextGame != 'y') && (NextGame != 'Y') && (NextGame != 'n') && (NextGame != 'N')); //Validation for Y/N prompt. Keep prompting if other characters are given
	} while ((NextGame == 'y') || (NextGame == 'Y'));
	return 0;
}

void ClearBoard() {
	for (int row = 0; row < 6; row++) {
		for (int column = 0; column < 7; column++) {
			Grid[row][column] = 32;
		}
	}
}

void PrintBoard() {
	cout << " 1 2 3 4 5 6 7" << endl;
	for (int row = 0; row < 6; row++)
	{
		cout << "|";
		for (int column = 0; column < 7; column++)
		{
			cout << Grid[row][column] << "|";
		}
		cout << endl;
		cout << "---------------" << endl;
	}
}

int UpdateBoard(int ColumnNum, int Player) {
	int i = 5;
	int CurrentRow = 5;
	do {
		if (Grid[i][ColumnNum-1] == 32) {
			Grid[i][ColumnNum-1] = Player;
			CurrentRow = i;
			i = -1; //Done simply to exit while loop after single update
		}
		else {
			i--;
		}
	} while (i >= 0);
	return CurrentRow;
}

bool CheckConnect4(int ColumnNum, int Player, int RowNum) {
	
	//Check for Connect4 on Column
	int ColumnCount = 0;
	bool ColumnConnect4;
	int row = 0; //Begin counting from top row
	while (row <= 5) {
		if (Grid[row][ColumnNum-1] == 32) {
			row++;
		}
		else if (Grid[row][ColumnNum-1] == Player){
			ColumnCount++;
			row++;
		}
		else if (Grid[row][ColumnNum-1] != Player){
			row = 6; //Simply to exit loop and stop counting
		}
	}
	if (ColumnCount >= 4) {
		ColumnConnect4 = true;
	}
	else {
		ColumnConnect4 = false;
	}
	
	//Check for Connect 4 on Row
	int RowCount = 0;
	bool RowConnect4;
	int column = 0; //Begin counting from left-most column
	while (column <= 6) {
		if (Grid[RowNum][column] == 32) {
			RowCount = 0; //Considering the following case: O OOO; if not included, this would be a WIN for O
			column++;
		}
		else if (Grid[RowNum][column] == Player){
			RowCount++;
			if (RowCount >= 4) {
				column = 6; //Exits loop once it's counted 4
			}
			column++;
		}
		else if (Grid[RowNum][column] != Player){
			RowCount = 0; //Reset count if continuous pattern in O or X is broken
			column++;
		}
	}
	if (RowCount >= 4) {
		RowConnect4 = true;
	}
	else {
		RowConnect4 = false;
	}
	
	//Check for Connect4 on \ Left Diagonal
	int EdgeRow = RowNum;
	int EdgeColumn = ColumnNum-1;
	int LeftDiagonalCount = 0;
	bool LeftDiagonalConnect4;
	
	while ((EdgeRow > 0) && (EdgeColumn > 0)) {
		EdgeRow--;
		EdgeColumn--;		
	} //Begin counting from highest cell on left \ diagonal of the current cell from the latest input
	
	while ((EdgeRow <= 5) && (EdgeColumn <= 6)) {
		if (Grid[EdgeRow][EdgeColumn] == 32) {
			LeftDiagonalCount = 0;
			EdgeRow++;
			EdgeColumn++;
		}
		else if (Grid[EdgeRow][EdgeColumn] == Player){
			LeftDiagonalCount++;
			if (LeftDiagonalCount >= 4) {
				EdgeColumn = 6; //Exits loop once it's counted 4
			}
			EdgeRow++;
			EdgeColumn++;
		}
		else if (Grid[EdgeRow][EdgeColumn] != Player){
			LeftDiagonalCount = 0; //Reset count if continuous pattern in O or X is broken
			EdgeRow++;
			EdgeColumn++;
		}
	}
	if (LeftDiagonalCount >= 4) {
		LeftDiagonalConnect4 = true;
	}
	else {
		LeftDiagonalConnect4 = false;
	}
		
	//Check for Connect4 on / Right Diagonal
	EdgeRow = RowNum;
	EdgeColumn = ColumnNum-1;
	int RightDiagonalCount = 0;
	bool RightDiagonalConnect4;
	
	while ((EdgeRow > 0) && (EdgeColumn < 6)) {
		EdgeRow--;
		EdgeColumn++;		
	} //Begin counting from highest cell on right / diagonal of the current cell from the latest input

	while ((EdgeRow <= 5) && (EdgeColumn >= 0)) {
		if (Grid[EdgeRow][EdgeColumn] == 32) {
			RightDiagonalCount = 0;
			EdgeRow++;
			EdgeColumn--;
		}
		else if (Grid[EdgeRow][EdgeColumn] == Player){
			RightDiagonalCount++;
			if (RightDiagonalCount >= 4) {
				EdgeColumn = 0; //Exits loop once it's counted 4
			}
			EdgeRow++;
			EdgeColumn--;
		}
		else if (Grid[EdgeRow][EdgeColumn] != Player){
			RightDiagonalCount = 0; //Reset count if continuous pattern in O or X is broken
			EdgeRow++;
			EdgeColumn--;
		}
	}
	if (RightDiagonalCount >= 4) {
		RightDiagonalConnect4 = true;
	}
	else {
		RightDiagonalConnect4 = false;
	}
	
	return (ColumnConnect4 || RowConnect4 || LeftDiagonalConnect4 || RightDiagonalConnect4);
}