/*BE2-HPROG2 Assignment03: Cell Simulator V1.01*/
//Written by Franz A. Tapia Chaca, CID 01119357
//Began:	01/12/16
//Finished: 12/12/16

/*-------------------------------------------------------*/
//					Purpose of the program
/*-------------------------------------------------------*/
//This program simulates the population of cells in a petriDish
//over time. Time is passed as the user presses the 'Enter' key
//which causes the board to update according to the following rules:

//For Normal cells, Death occurs if:
//Cell is overpopulated:	4 or more neighbours (8 max)
//Cell is lonely:			1 or less neighbour
//and Birth occours if:
//Cell has exactly			3 neighbours

//For Cancel cells, Death occurs if:
//Cell is overpopulated:	5 or more neighbours
//Cell is lonely:			1 or less neighbour
//and Birth occours if:
//Cell has exactly			3 neighbours

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

/*-------------------------------------------------------*/
//					   Base class: Cell
/*-------------------------------------------------------*/

class Cell
{
	protected:
	
		//Member data
		bool cellState;			//State of the cell; 1 or TRUE for alive; 0 or FALSE for dead/empty
		char chosenCellType;	//Normal or Cancer cell
		int neighboursCount;	//Number of neighbours for the cell
	
	public:
	
		//Constructors & Destructor
		Cell();
		~Cell();
		
		//Member define functions
		void set_chosenCellType(char typeOfCell) { chosenCellType = typeOfCell; };	//Updates the type of cell selected
		void set_neighboursCount(int count) { neighboursCount = count; };			//Updates the number of neighbours of each cell
		void set_cellState(bool state) { cellState = state; };						//Updates the state of the cell (0: Dead or 1: Alive)

		//Member access functions
		int get_neighboursCount() { return neighboursCount; };						//Retrieves number of neighbours for 1 cell
		char get_chosenCellType() { return chosenCellType; };						//Retrieves the character corresponding to the chosen cell type
		bool get_cellState() { return cellState; };									//Retrives the state of the cell
		
		//Member functions
		virtual bool next_state();													//Determines what the next state of a Normal cell will be

		//Operator overload
		Cell& operator=(const Cell& cell_in);
};

Cell::Cell() //Default constructor
{
	cellState = 0; //0 or FALSE for dead/empty cell
	chosenCellType = 'O'; //Normal cell selected
}

Cell::~Cell()
{
	//Cell destructor
}

bool Cell::next_state()
{
	bool next_cellState;
	
	if (get_cellState() == 1)
	{
		//Cell Death
		if ((get_neighboursCount() >= 4) || (get_neighboursCount() <= 1))
		{
			next_cellState = 0;				
		}	
		else
		{
			//Cell Stasis
			next_cellState = get_cellState(); //the next state will be the current state
		}
	}
		
	else if (get_cellState() == 0)
	{
		//Cell Birth
		if (get_neighboursCount() == 3)
		{
			next_cellState = 1;
		}
		else
		{
			//Cell Stasis
			next_cellState = get_cellState(); //the next state will be the current state
		}
	}
	return next_cellState;
}

Cell& Cell::operator=(const Cell& cell_in) //Copy assignment
{
	cellState = cell_in.cellState;
	chosenCellType = cell_in.chosenCellType;
	
	return *this;
}

ostream& operator<<(ostream& os, Cell cell1)
{
	return os << cell1.get_chosenCellType();
}

/*-------------------------------------------------------*/
//					Derived Class: Cancer
/*-------------------------------------------------------*/

class Cancer: public Cell
{
	public:
		
		//Constructors & Destructor
		Cancer();
		~Cancer();
		
		//Member functions
		bool next_state();		//Determines what the next state of a Cancer cell will be
};

Cancer::Cancer():Cell() //Cancer default constructor
{
	cellState = 0; //0 or FALSE for dead/empty cell
	chosenCellType = 'X'; //Cancer cell selected
}

Cancer::~Cancer()
{
	//Cancer destructor
}

bool Cancer::next_state()
{
	bool next_cellState;
	
	if (get_cellState() == 1)
	{
		//Cell Death
		if ((get_neighboursCount() >= 5) || (get_neighboursCount() <= 1)) //Overpopulation minimum of 5 or Cancer cells
		{
			next_cellState = 0;				
		}	
		else
		{
			//Cell Stasis
			next_cellState = get_cellState(); //the next state will be the current state
		}
	}
		
	else if (get_cellState() == 0)
	{
		//Cell Birth
		if (get_neighboursCount() == 3)
		{
			next_cellState = 1;
		}
		else
		{
			//Cell Stasis
			next_cellState = get_cellState(); //the next state will be the current state
		}
	}
	return next_cellState;
}

/*-------------------------------------------------------*/
//					   Base Class: Board
/*-------------------------------------------------------*/

class Board
{
	private:
		
		//Member data
		vector<Cell> board;		//Stores the board in a vector of length = rows*columns
		int rows;				//Stores the number of rows
		int columns;			//Stores the number of columns
		int time;				//Stores the times passed
		
	public:
		
		//Constructors & Destructor
		Board(int rows, int columns);
		~Board();
		
		//Member define functions
		void set_columns(int columnsNum) { columns = columnsNum; };		//Updates the number of columns of the board
		void set_time(int boardTime) { time = boardTime; };				//Updates the time passed due to updating the board
		void set_rows(int rowsNum) { rows = rowsNum; };					//Updates the number of rows of the board

		//Member functions
		void seed_cells(int selection, int confluence);		//Fills the board randomly according to the selection of cell and confluence
		void count_neighbours();							//Counts the number of neighbours for each cell in the board
		int get_num_cells();								//Counts the number of alive cells on the board
		void next_state();									//Updates the board according to the state of the cell and number of neighbours
		void display();										//Displays the board in rows x columns
		int get_time() { return time; };					//Retrieves the time passed by the board
};

Board::Board(int rows, int columns)
{	
	int boardTime = 0;
	set_rows(rows);
	set_columns(columns);
	set_time(boardTime);
	Cell cell; //default constructor called -> results in dead/empty Normal cell	

	for (int a = 0; a < rows*columns; a++)
	{
		board.push_back(cell);
	}
}

Board::~Board()
{
	//Board destructor
}

void Board::display()
{
	for (int b = 0; b < board.size(); b++)
	{
		if (board[b].get_cellState() == 0)
			cout << ' ';
		else if (board[b].get_cellState() == 1)
			cout << board[b];
		if (((b+1) % columns) == 0) //to space out entire vector by rows of 75 columns each once at end of row
		{							// e.g. 1st row starts w/ 0th vector member and ends w/ 74th.
			cout << endl;			// (74+1) % 75 = 0, display remaining 75 on next row
		}
	}
	
	cout << endl;
}

void Board::seed_cells(int selection, int confluence)
{
	int boardTime = 0;
	set_time(boardTime);
	Cancer cancerCell;
	
	if (selection == 1)
	{
		//Do nothing as there are already normal cells in the board
	}	
	else if (selection == 2)
	{
		for (int g = 0; g < rows*columns; g++)
		{
			board.pop_back();
		}
		for (int f = 0; f < rows*columns; f++)
		{
			board.push_back(cancerCell); //redefine all cells as Cancer cells
		}
	}

	/* Random number generation */
	vector<int> randomNums;
	int random;
	bool existingNum;

	srand(1);
	
	for (int i = 0; i < (confluence*rows*columns/100); i++) // Generating random numbers
	{
		do
		{
			random = rand()%(rows*columns); // will result in numbers in the range 0 - 1499 for a 20 by 75 board
			for (int j = 0; j < i; j++)  			// (total 1500 possibilities)
			{
				if (randomNums[j] == random)
				{
					existingNum = true;
					break;
				}
				else
					existingNum = false;
			}
		} while (existingNum == true);
		randomNums.push_back(random);
	}
	for (int k = 0; k < randomNums.size(); k++) // from 0 to 1499 (total 1500 iterations)
	{
		board[randomNums[k]].set_cellState(1);	//1 or TRUE for alive cell
	}
}

int Board::get_num_cells()
{
	int cellCount = 0;
	for (int l = 0; l < board.size(); l++)
	{
		if (board[l].get_cellState() == 1)  //count the number of cells whose cellState is 1, therefore ALIVE
		{
			cellCount++;
		}
	}
	return cellCount;
}

void Board::count_neighbours()
{
	int count = 0;
	
	for (int m = 0; m < board.size(); m++)
	{
		//Top row, left corner
		if (m == 0)
		{
			if (board[m+1].get_cellState() == 1)
				count++;
			for (int n = m + 75; n <= m + 76;  n++)
			{
				if (board[n].get_cellState() == 1)
					count++;
			}
		}
		
		//Top row, excl. corners
		else if ((m > 0) && (m < (columns-1))) //if there are 75 columns, then top row runs from 0 to 74, or 0 to (75-1)
		{
			for (int o = m - 1; o <= m + 1; o = o + 2)
			{
				if (board[o].get_cellState() == 1)
					count++;
			}
			for (int p = m + 74; p <= m + 76; p++)
			{
				if (board[p].get_cellState() == 1)
					count++;
			}
		}
		
		//Top row, right corner
		else if (m == (columns-1))
		{
			if (board[m-1].get_cellState() == 1)
				count++;
			for (int q = m + 74; q <= m + 75;  q++)
			{
				if (board[q].get_cellState() == 1)
					count++;
			}
		}
		
		//Leftmost column of board; excl. top and bottom corners
		else if ( (m != 0) && ((m%columns) == 0) && (m != columns*(rows-1)) )
		{
			for (int r = m - 75; r <= m - 74; r++)
			{
				if (board[r].get_cellState() == 1)
					count++;
			}
			if (board[m+1].get_cellState() == 1)
				count++;
			for (int s = m + 75; s <= m + 76; s++)
			{
				if (board[s].get_cellState() == 1)
					count++;
			}
		}
		
		//Rightmost column of board; excl. top and bottom corners
		else if ( (m != columns-1) && ((m+1)%columns == 0) && (m != (columns*rows)-1) )
		{
			for (int t = m - 76; t <= m - 75; t++)
			{
				if (board[t].get_cellState() == 1)
					count++;
			}
			if (board[m-1].get_cellState() == 1)
				count++;
			for (int u = m + 74; u <= m + 75; u++)
			{
				if (board[u].get_cellState() == 1)
					count++;
			}
		}
		
		//Bottom row, left corner
		else if (m == columns*(rows-1))
		{
			for (int v = m - 75; v <= m - 74;  v++)
			{
				if (board[v].get_cellState() == 1)
					count++;
			}			
			if (board[m+1].get_cellState() == 1)
				count++;
		}
		
		//Bottom row excl. corners
		else if ((m > columns*(rows-1)) && (m < (rows*columns)-1))
		{
			for (int w = m - 76; w <= m - 74;  w++)
			{
				if (board[w].get_cellState() == 1)
					count++;
			}
			for (int ad = m - 1; ad <= m + 1; ad = ad + 2)
			{
				if (board[ad].get_cellState() == 1)
					count++;
			}
			
		}
		
		//Bottom row, right corner
		else if (m == (rows*columns)-1)
		{
			for (int y = m - 76; y <= m - 75;  y++)
			{
				if (board[y].get_cellState() == 1)
					count++;
			}			
			if (board[m-1].get_cellState() == 1)
				count++;
		}
		
		//All cells excl. those from the edges and corners
		else 
		{
			for (int z = m - 76; z <= m - 74;  z++)
			{
				if (board[z].get_cellState() == 1)
					count++;
			}
			for (int ab = m - 1; ab <= m + 1; ab = ab + 2)
			{
				if (board[ab].get_cellState() == 1)
					count++;
			}
			for (int ac = m + 74; ac <= m + 76; ac++)
			{
				if (board[ac].get_cellState() == 1)
					count++;
			}	
		}
		board[m].set_neighboursCount(count);
		count = 0;
	}
}

void Board::next_state()
{
	count_neighbours();
	
	bool next_cell_state;
	
	for (int ad = 0; ad < board.size(); ad++)
	{
		next_cell_state = board[ad].next_state();
		board[ad].set_cellState(next_cell_state);
	}

	//increasing the time of board
	int boardTime = get_time();
	boardTime++;
	set_time(boardTime);
}

int main() 
{
	int selection = 0;
	int confluence = 0;
	int c;
	
	Board petriDish(20, 75);
	string trash;
	
	system("cls");
	
	cout << "Welcome to the cell simulator\n" << endl;
	cout << "Select your cell type: (1) normal cells or (2) cancer cells" << endl;
	
	do
	{
		cout << ">";
		cin  >> selection;
	} while ( (selection < 1) || (selection > 2) );
	getline(cin, trash); //getline clears the cin stream so that the next input is read and operated on
	
	cout << "Select the confluence percentage (%)" << endl;
	
	do
	{
		cout << ">";
		cin  >> confluence;
	} while ( (confluence <= 0 ) || (confluence > 100) );
	getline(cin, trash);	//if this line was not included, then the rest of the program would not work properly
							// the program freezes after putting giving a confluence value
	
	petriDish.seed_cells(selection, confluence);
	system("cls");
	
	do
	{
		system ("cls");
		cout << "time:			" << petriDish.get_time() << endl;
		cout << "number of cells: 	" << petriDish.get_num_cells() << endl;
		petriDish.display();
		c = cin.get();
		petriDish.next_state();
	} while (c != 'q');
	
	return 0;
}