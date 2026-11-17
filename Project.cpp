#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

void displayBoard(int array[][5], int rows, int cols);
bool loadCSV(string filename, int rows, int cols, int array[][5]);
void getCSVFILE(int array[][5], int rows, int cols);

int main()
{
    const int NUMBER_OF_ROWS = 5;
    const int NUMBER_OF_COLUMNS = 5;
    int initialBoard[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS] = {};
    int currentBoard[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS] = {};

    // loadCSV("test.csv", NUMBER_OF_ROWS, NUMBER_OF_COLUMNS, currentBoard);
    // displayBoard(currentBoard, NUMBER_OF_ROWS, NUMBER_OF_COLUMNS);

    int userInput;
    while (true)
    {
        cout << "Hello! Welcome to the game of life in CMPT 1109. What would you like to do?\n\t"
             << "1. Display the initial state for the game of life \n\t"
             << "2. Load a new initial state from a CSV file into memory \n\t"
             << "3. Play the game of life with the current initial state \n\t"
             << "4. Seed more cells into the initial state \n\t"
             << "5. Report summary statistics on all games played\n\t"
             << "6. Randomly play the game multiple times\n\t"
             << "7. Output the best initial board\n\t"
             << "8. quit the program" << endl;
        cin >> userInput;

        if (userInput == 8) {
            break;
        }

        switch (userInput)
        {
        case (1):
            displayBoard(initialBoard, NUMBER_OF_ROWS, NUMBER_OF_COLUMNS);
            break;
        case (2):
            getCSVFILE(initialBoard, NUMBER_OF_ROWS, NUMBER_OF_COLUMNS);
            break;
        case (3):
            break;
        case (4):
            break;
        case (5):
            break;
        case (6):
            break;
        case (7):
            break;
        }
    }
}

void displayBoard(int array[][5], int rows, int cols)
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            cout << "| " << array[row][col] << " |";
        }
        cout << endl;
    }
}

bool loadCSV(string filename, int rows, int cols, int array[][5])
{
    ifstream userFile(filename);
    if (!userFile.is_open())
    {
        cout << "Error opening file: " << filename << endl;
        return 0;
    }

    string line, cell;
    for (int row = 0; row < rows; row++)
    {
        getline(userFile, line);
        stringstream sline(line);
        for (int col = 0; col < cols; col++)
        {
            if (getline(sline, cell, ','))
            {
                array[row][col] = stoi(cell);
            }
        }
    }
    userFile.close();
    return 1;
}

void getCSVFILE(int array[][5], int rows, int cols)
{
    string fileName;
    cout << "What is the name of the CSV file? ";
    cin >> fileName;
    while (!loadCSV(fileName, rows, cols, array))
    {
        cout << "What is the name of the CSV file? ";
        cin >> fileName;
    }
}