#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

void displayBoard(int array[][5], int rows, int cols);
bool loadCSV(string filename, int rows, int cols, int array[][5]);

int main()
{
    const int NUMBER_OF_ROWS = 5;
    const int NUMBER_OF_COLUMNS = 5;
    int initialBoard[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS] = {};
    int currentBoard[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS] = {};
    loadCSV("test.csv", NUMBER_OF_ROWS, NUMBER_OF_COLUMNS, currentBoard);
    displayBoard(currentBoard, NUMBER_OF_ROWS, NUMBER_OF_COLUMNS);
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