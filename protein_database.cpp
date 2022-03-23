/*BE2-HPROG2 Assignment02: Bioinformatics V1.01*/
//Written by Franz A. Tapia Chaca, CID 01119357
//Began: 11/11/16 (ABOUT A WEEK AGO!!)
//Finished: 18/11/16

/*Purpose of program*/
//This program loads a protein database onto a vector of structs
//Each struct in the vector holds information about each protein
//The user can then access the database, search for proteins and
//Xtract specific information about each protein in the database

//Optional tasks left:
//  Combine SearchAndSelect functions with template T

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct proteinDet
{
	int itemID;				//number of protein on database in given order
	string giID;			//gi id of protein
	string refID;			//red id of protein
	string name;			//name of protein
	string sequence;		//protein sequence
};

/*Function declarations*/

//menu to handle the information about a single protein found and/or selected by search
void ProteinMenu(proteinDet currProtein);

//load the selected database (abridged or complete) onto a vector of structs
vector<proteinDet> LoadData(string proteinData);

//locate a protein by its item id and return it
proteinDet SearchAndSelectitemID(int itemID, vector<proteinDet> proteinDB);

//locate a protein by its gi id and return it
proteinDet SearchAndSelectrefID(string refID, vector<proteinDet> proteinDB);

//locate a protein by its ref id and return it
proteinDet SearchAndSelectgiID(string giID, vector<proteinDet> proteinDB);

//provide protein statistics (number of each amino acids + total number)
void CountAminoAcids(string sequence);

//store information of selected protein onto .txt file
void WriteProteinToFile(proteinDet currProtein);

//search in the protein database for a keyword and return all matches
vector<proteinDet> SearchAndSelectKeyword(string keyword, vector<proteinDet> proteinDB);

/*There are no longer any global variables, Dr. Choi :D*/

int main()
{
	vector<proteinDet> proteinDB;		//stores all proteins in the database
	int lastItemID;		//size of protein database
	
	int option;							//variable for database select prompt
	int mainMenuOption;					//variable for main menu options prompt
	
	int itemID;
	string giID;
	bool validgiIDSearch;				//used for validation for gi ID search
	string refID;
	bool validrefIDSearch;				//used for validation for ref ID search
	
	proteinDet currProtein;				//selected protein from search
	
	string keyword;
	int keywordMatchNum;
	
	vector<proteinDet> keywordSearch;
	
	do
	{
		cout << "\nWelcome to the Protein Database\n" << endl;
		cout << "Select an option from the menu below:" << endl;
		cout << "1) Load the abridged protein data" << endl;
		cout << "2) Load the complete protein data" << endl;
		cout << "3) Quit database" << endl;
		cout << ">> ";
		cin >> option;
		
		if (option == 1)
		{
			proteinDB = LoadData("protein_a.fa");			//load abridged database with 11 proteins
		}
		else if (option == 2)
		{
			proteinDB = LoadData("protein_c.fa");			//load complete database with 110386 proteins
		}
		else if (option == 3)
		{
			cout << "\n\nExiting the database." << endl;
		}
		else if ((option < 1) || (option > 3) || (cin.fail()))	//validation if user inputs characters instead of integers
		{
			cin.clear();										//Clears the error flag on the cin stream
			cin.ignore(10000, '\n');							//Skips 10000 bytes of characters on cin to next newline. Will not work if user inputs longer than 10000 characters
			cout << "\nInvalid. Please try again." << endl;	
		}
		
		if ((option == 1) || (option == 2)) 
		{
			lastItemID = proteinDB.size();
			do
			{
				cout << "\nSelect an option from the menu below:" << endl;
				cout << "1) Overview of the database" << endl;
				cout << "2) Search by proteinID #" << endl;
				cout << "3) Search by giID #" << endl;
				cout << "4) Search by refID #" << endl;
				cout << "5) Search by keyword" << endl;
				cout << "6) Quit database" << endl;
				cout << ">> ";
				cin >> mainMenuOption;
				
				if (mainMenuOption == 1)
				{
					cout << "\n\nThe proteins in the database are from GenBank(R)" << endl;
					cout << "Total number of proteins in the database: " << lastItemID << endl;
					cout << "Amino acids are represented by the following characters:" << endl;
					cout << "A  alanine		P  proline" << endl;
					cout << "B  aspartate/asparagine	Q  glutamine" << endl;
					cout << "C  cystine		R  arginine" << endl;
					cout << "D  aspartate		S  serine" << endl;
					cout << "E  glutamate		T  threonine" << endl;
					cout << "F  phenylalanine	U  selenocysteine" << endl;
					cout << "G  glycine		V  valine" << endl;
					cout << "H  histidine		W  tryptophan" << endl;
					cout << "I  isoleucine		Y  tyrosine" << endl;
					cout << "K  lysine		Z  glutamate/glutamine" << endl;
					cout << "L  leucine		X  any" << endl;
					cout << "M  methionine		*  translation stop" << endl;
					cout << "N  asparagine		-  gap of indeterminate length\n" << endl;
				}
				else if (mainMenuOption == 2)
				{
					do
					{
						cout << "\n\nEnter an item ID" << endl;
						cout << ">> ";
						cin >> itemID;
						if ((itemID >= 1) && (itemID <= lastItemID))		//if item ID given exists...
						{
							currProtein = SearchAndSelectitemID(itemID, proteinDB);	// select found protein from search
						}
						else if ((itemID < 1) || (itemID > lastItemID) || (cin.fail())) //if it doesn't exist or rubbish is entered...
						{
							cin.clear();
							cin.ignore(10000, '\n');									// give error message
							cout << "\n\nInvalid. Please select an item ID between 1 and " << lastItemID << "." << endl;
						}
					} while((itemID < 1) || (itemID > lastItemID));			//repeat search until valid input is given
					ProteinMenu(currProtein);								//load found protein onto protein menu
				}
				else if (mainMenuOption == 3)
				{
					do
					{
						cout << "\n\nEnter a gi ID" << endl;
						cout << ">> ";
						cin >> giID;
						for (int i = 0; i < lastItemID; i++) /*gi ID validation code*/
						{
							if (proteinDB[i].giID == giID) 	//if it exists...
							{
								validgiIDSearch = true; 	// then the search is valid
								break;
							}
							else 							//if not...
							{
								validgiIDSearch = false;	// then search is not valid
							}
						}
						if (validgiIDSearch == true)					//only if search is valid...
						{
							currProtein = SearchAndSelectgiID(giID, proteinDB); 	// select found protein from search
						}
						else
						{
							cout << "\n\nInvalid. Please try again." << endl;
						}
					} while(validgiIDSearch == false);		//repeat search until it is valid
					ProteinMenu(currProtein);
				}
				else if (mainMenuOption == 4)
				{
					do
					{
						cout << "\n\nEnter a refID" << endl;
						cout << ">> ";
						cin >> refID;
						for (int j = 0; j < lastItemID; j++) /*ref ID validation code*/
						{
							if (proteinDB[j].refID.find(refID) != string::npos) //same structure as above
							{
								validrefIDSearch = true;
								break;
							}
							else 
							{
								validrefIDSearch = false;
							}
						}
						if (validrefIDSearch == true)
						{
							currProtein = SearchAndSelectrefID(refID, proteinDB);
						}
						else
						{
							cout << "\n\nInvalid. Please try again." << endl;
						}
					} while(validrefIDSearch == false);
					ProteinMenu(currProtein);
				}
				else if (mainMenuOption == 5)
				{
					do
					{
						cout << "\n\nEnter a keyword" << endl;
						cout << ">> ";
						cin >> keyword;
						
						keywordSearch = SearchAndSelectKeyword(keyword, proteinDB);		//returns with vector of structs for found matches
						
						cout << "\n\nNumber of matches found: " << keywordSearch.size() << endl;
						for (int i = 0; i < keywordSearch.size(); i++)			//display matches
						{
							cout << i+1 << ")item id: " << keywordSearch[i].itemID << ", gi id: " << keywordSearch[i].giID << ", ref id: " << keywordSearch[i].refID << endl;
							cout << " " << keywordSearch[i].name << endl;
						}
						
						if (keywordSearch.size() > 0)							//only if there were matches
						{
							do 
							{
								cout << "\n\nSelect one of the matches" << endl;
								cout << ">> ";
								cin >> keywordMatchNum;
								if ((keywordMatchNum >= 1) && (keywordMatchNum <= keywordSearch.size()))
								{
									currProtein = keywordSearch[keywordMatchNum-1];
									ProteinMenu(currProtein);
								}
								else if ((keywordMatchNum < 1) || (keywordMatchNum > keywordSearch.size()) || (cin.fail())) 
								{
									cin.clear();
									cin.ignore(10000, '\n');
									cout << "\nInvalid. Please try again." << endl;	
								}
							} while((keywordMatchNum < 1) ||(keywordMatchNum > keywordSearch.size()));
						}
					} while(keywordSearch.size() == 0);
				}
				else if (mainMenuOption == 6)
				{
					cout << "\n\nExiting the database." << endl;
				}
				else if ((mainMenuOption < 1) || (mainMenuOption > 6) || (cin.fail())) 
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "\nInvalid. Please try again." << endl;	
				}
			} while (mainMenuOption != 6);	
		}
		if (mainMenuOption == 6)
		{
			break;
		}	
	} while (option != 3);
	return 0;
}

void ProteinMenu(proteinDet currProtein)
{
	int proteinMenuOption;
	int aminoAcidsNum;
	do
	{
		cout << "\n\nSelect an option from the menu below:" << endl;
		cout << "1) Description of the protein" << endl;
		cout << "2) Protein sequence" << endl;
		cout << "3) Protein statistics" << endl;
		cout << "4) Record protein to file" << endl;
		cout << "5) Return to main menu" << endl;
		cout << ">> ";
		cin >> proteinMenuOption;
		
		if (proteinMenuOption == 1)
		{
			cout << "\n\nDescription of the protein:" << endl;
			cout << "item ID: " << currProtein.itemID << endl;
			cout << "gi ID: " << currProtein.giID << endl;
			cout << "ref ID: " << currProtein.refID << endl;
			cout << "name: " << currProtein.name << endl;
		}
		else if (proteinMenuOption == 2)
		{
			cout << "\n\nProtein sequence:" << endl;
			cout << currProtein.sequence;
		}
		else if (proteinMenuOption == 3)
		{
			CountAminoAcids(currProtein.sequence);
		}
		else if (proteinMenuOption == 4)
		{
			WriteProteinToFile(currProtein);
		}
		else if (proteinMenuOption == 5)
		{
			break; //exit while loop to return to the main menu
		}
		else if ((proteinMenuOption < 1) || (proteinMenuOption > 5) || (cin.fail())) 
		{
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "\nInvalid. Please try again." << endl;	
		}
	} while (proteinMenuOption != 5);
}

vector<proteinDet> LoadData(string proteinData)
{
	vector<proteinDet> proteinDB;
	
	cout << "\n\nLoading the database...";
	
	ifstream ist;
	ist.open(proteinData.c_str(), ios_base::in); //open database file
	
	int itemIDCount = 0;
	string ignoreStr;	//temporary string to remove rubbish string e.g. ">gi|"
	long int i = 0;		//MAX_INT cannot hold 110386
	
	while (!ist.eof()) 							//While program didn't reach End of File
	{
		proteinDB.push_back(proteinDet());		//Initialize empty struct in vector proteinDB
		if (itemIDCount > i)					//If we've already read 1 protein, let's read another
		{
			i++;								// by incrementing i for new struct in vector proteinDB
		}
		proteinDB[i].itemID = ++itemIDCount;	//Store current count of protein (in order on file)
		getline(ist, ignoreStr, '|'); 			//Ignore ">gi|"
		getline(ist, proteinDB[i].giID, '|'); 	//Read and store gi ID
		getline(ist, ignoreStr, '|'); 			//Ignore "|ref|"
		getline(ist, proteinDB[i].refID, '|');	//Read and store ref ID
		getline(ist, ignoreStr, ' '); 			//Ignore "| "
		getline(ist, proteinDB[i].name);		//Read and store protein name
		getline(ist, proteinDB[i].sequence, '>');
	}
	
	ist.close();
	
	cout << "\nDatabase loaded.\n" << endl;
	return proteinDB;
}

proteinDet SearchAndSelectitemID(int itemID, vector<proteinDet> proteinDB)
{
	proteinDet currProtein;
	
	for (int i = 0; i < proteinDB.size(); i++)
	{
		if (itemID == proteinDB[i].itemID)
		{
			currProtein.itemID = proteinDB[i].itemID;
			currProtein.giID = proteinDB[i].giID;
			currProtein.refID = proteinDB[i].refID;
			currProtein.name = proteinDB[i].name;
			currProtein.sequence = proteinDB[i].sequence;
		}
	}
	return currProtein;
}

proteinDet SearchAndSelectgiID(string giID, vector<proteinDet> proteinDB)
{
	proteinDet currProtein;
	
	for (int i = 0; i < proteinDB.size(); i++)
	{
		if (giID == proteinDB[i].giID)
		{
			currProtein.itemID = proteinDB[i].itemID;
			currProtein.giID = proteinDB[i].giID;
			currProtein.refID = proteinDB[i].refID;
			currProtein.name = proteinDB[i].name;
			currProtein.sequence = proteinDB[i].sequence;
		}
	}
	return currProtein;
}

proteinDet SearchAndSelectrefID(string refID, vector<proteinDet> proteinDB)
{
	proteinDet currProtein;
	
	for (int i = 0; i < proteinDB.size(); i++)
	{
		if (refID == proteinDB[i].refID)
		{
			currProtein.itemID = proteinDB[i].itemID;
			currProtein.giID = proteinDB[i].giID;
			currProtein.refID = proteinDB[i].refID;
			currProtein.name = proteinDB[i].name;
			currProtein.sequence = proteinDB[i].sequence;
		}
	}
	return currProtein;
}

void CountAminoAcids(string sequence)
{
	char aminoAcids[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N',
						 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'Y', 'Z', 'X', '*', '-'}; //list of amino acid chars
	int aACount[26] = {0}; //counts of each amino acid to be updated here
	int countChar;
	int countNUL;
	
	for (int j = 0; j < 26; j++) 
	{
		countChar = 0;
		countNUL = 0;
		for (int i = 0; i < sequence.length(); i++) 
		{
			if (sequence[i] == aminoAcids[j])
			{
				aACount[j] = ++countChar;
			}
			else if (sequence[i] == '\n')
			{
				countNUL++; //The number of NUL terminators in each line of Sequence
			}				// These are read onto the protein sequence, and need to
		}					// be removed in order to provide an accurate total number
	}
	cout << countNUL << endl;
	cout << "\n\nReport on the protein statistics:\n" << endl;
	cout << "Total number of amino acids: " << sequence.length() - countNUL << endl;
	cout << "A: " << aACount[0] << "	P:  " << aACount[13] << endl;
	cout << "B: " << aACount[1] << "	Q:  " << aACount[14] << endl;
	cout << "C: " << aACount[2] << "	R:  " << aACount[15] << endl;
	cout << "D: " << aACount[3] << "	S:  " << aACount[16] << endl;
	cout << "E: " << aACount[4] << "	T:  " << aACount[17] << endl;
	cout << "F: " << aACount[5] << "	U:  " << aACount[18] << endl;
	cout << "G: " << aACount[6] << "	V:  " << aACount[19] << endl;
	cout << "H: " << aACount[7] << "	W:  " << aACount[20] << endl;
	cout << "I: " << aACount[8] << "	Y:  " << aACount[21] << endl;
	cout << "K: " << aACount[9] << "	Z:  " << aACount[22] << endl;
	cout << "L: " << aACount[10] << "	X:  " << aACount[23] << endl;
	cout << "M: " << aACount[11] << "	*:  " << aACount[24] << endl;
	cout << "N: " << aACount[12] << "	-:  " << aACount[25] << endl;
}

void WriteProteinToFile(proteinDet currProtein)
{
	string oname = "selected_proteins.txt";
	
	ofstream ost;
	ost.open(oname.c_str(), ios_base::app);	//open output file. If it doesn't exist, create one.
	
	ost << "Protein " << currProtein.itemID << " (item ID)" << endl;
	ost << "gi ID: " << currProtein.giID << endl;
	ost << "ref ID: " << currProtein.refID << endl;
	ost << "name: " << currProtein.name << endl;
	ost << "sequence: \n" << currProtein.sequence << endl;
	
	ost.close();
	
	cout << "\n\nThe protein was written to file." << endl;
}

vector<proteinDet> SearchAndSelectKeyword(string keyword, vector<proteinDet> proteinDB)
{
	vector<proteinDet> listOfMatchedProteins;
	
	/*//int lastItemID;
	//vector<proteinDet>::iterator vit;
	//vit = proteinDB.end();
	//lastItemID = (*--vit).itemID;*/ //Peculiar code. This used to be used to find lastItemID of protein Database
	
	for (int i = 0; i < proteinDB.size(); i++)
	{
		if (proteinDB[i].name.find(keyword) != string::npos) 	//if the keyword is found (NOT an invalid search) in the protein name...
		{
			listOfMatchedProteins.push_back(proteinDB[i]);		// then select protein and add to vector
		}
	}
	return listOfMatchedProteins;
}