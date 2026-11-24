#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <cmath>
using namespace std;

const int BOARD_SIZE = 30;
const int MAX_YEARS = 30;
const int DEAD_CELL = 0;
const int LIVE_CELL = 1;
const int MINIMUM_POSITIVE_NUMBER = 1;
const int NO_BEST_GAME = -1;
const double MIN_PROBABILITY = 0.0;
const double MAX_PROBABILITY = 1.0;

// Alan - Game records
struct GameRecord
{
    int startingBoard[BOARD_SIZE][BOARD_SIZE] = {};
    int livingCells = 0;
    int deadCells = 0;
    int yearsPlayed = 0;
};

vector<GameRecord> gameHistory;
int bestInitialBoard[BOARD_SIZE][BOARD_SIZE] = {};
int bestLivingCells = NO_BEST_GAME;
bool hasBestGame = false;

void displayMenu();
bool validateChoice(int userChoice);
void displayBoard(const int board[][BOARD_SIZE], int rows, int cols, bool csvFormat = false);
bool validateCSV(ifstream &file, int rows, int cols, int board[][BOARD_SIZE]);
bool loadCSV(string filename, int rows, int cols, int board[][BOARD_SIZE]);
void getCSVFILE(int board[][BOARD_SIZE], int rows, int cols);
void seedRandomCells(int board[][BOARD_SIZE], int rows, int cols);
int countLivingCells(const int board[][BOARD_SIZE]);
int checkAdjacentCells(const int board[][BOARD_SIZE], int row, int col);
void calculateNextYear(const int currentBoard[][BOARD_SIZE], int nextBoard[][BOARD_SIZE]);
bool boardsAreEqual(const int board1[][BOARD_SIZE], const int board2[][BOARD_SIZE]);
void copyBoard(const int source[][BOARD_SIZE], int destination[][BOARD_SIZE]);
int simulateGame(const int startingBoard[][BOARD_SIZE], int &yearsPlayed, bool displayEachYear);
GameRecord runAndRecordGame(const int startingBoard[][BOARD_SIZE], bool displayEachYear);
int getPositiveInteger(string message);
double getProbability();
void displayGameStatistics();
void runRandomExperiments();
void displayBestInitialBoard();

// Arian - Main menu
int main()
{
    const int NUMBER_OF_ROWS = BOARD_SIZE;
    const int NUMBER_OF_COLUMNS = BOARD_SIZE;
    int initialBoard[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS] = {};

    int userInput;
    string line, extra;
    while (true)
    {
        displayMenu();
        if (!getline(cin, line))
        {
            break;
        }

        stringstream input(line);
        if (!(input >> userInput) || (input >> extra))
        {
            cout << "Invalid choice! Please enter a number." << endl;
            continue;
        }

        if (!validateChoice(userInput))
        {
            cout << "Invalid choice! Please enter a number between 1 and 8." << endl;
            continue;
        }

        if (userInput == 8)
        {
            break;
        }

        switch (userInput)
        {
        case 1:
            displayBoard(initialBoard, NUMBER_OF_ROWS, NUMBER_OF_COLUMNS);
            break;
        case 2:
            getCSVFILE(initialBoard, NUMBER_OF_ROWS, NUMBER_OF_COLUMNS);
            break;
        case 3:
            runAndRecordGame(initialBoard, true);
            break;
        case 4:
            seedRandomCells(initialBoard, NUMBER_OF_ROWS, NUMBER_OF_COLUMNS);
            break;
        case 5:
            displayGameStatistics();
            break;
        case 6:
            runRandomExperiments();
            break;
        case 7:
            displayBestInitialBoard();
            break;
        }

        if (!cin)
        {
            break;
        }
    }
    return 0;
}

// Arian - Menu, CSV validation, board management and random seeding
void displayMenu()
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
}

bool validateChoice(int userChoice)
{
    return userChoice >= 1 && userChoice <= 8;
}

void displayBoard(const int board[][BOARD_SIZE], int rows, int cols, bool csvFormat)
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (csvFormat)
            {
                cout << board[row][col];
                if (col < cols - 1)
                {
                    cout << ",";
                }
            }
            // else
            // {
            //     if (board[row][col] == 1)
            //     {
            //         cout << " ❤️ ";
            //     }
            //     else
            //     {
            //         cout << " 0 ";
            //     }
            // }
            else
            {
                cout << "| " << board[row][col] << " |";
            }
        }
        cout << endl;
    }
}

bool validateCSV(ifstream &file, int rows, int cols, int board[][BOARD_SIZE])
{
    string line, cell;
    int rowCount = 0;

    while (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }
        if (rowCount >= rows || line.empty() || line.back() == ',')
        {
            return false;
        }

        stringstream sline(line);
        int colCount = 0;
        while (getline(sline, cell, ','))
        {
            if (colCount >= cols || (cell != "0" && cell != "1"))
            {
                return false;
            }
            board[rowCount][colCount] = stoi(cell);
            colCount++;
        }

        if (colCount != cols)
        {
            return false;
        }
        rowCount++;
    }
    return rowCount == rows && file.eof() && !file.bad();
}

bool loadCSV(string filename, int rows, int cols, int board[][BOARD_SIZE])
{
    ifstream userFile(filename);
    if (!userFile.is_open())
    {
        cout << "Error opening file: " << filename << endl;
        return false;
    }

    // Only replace the initial board after the entire CSV is valid.
    int tempBoard[BOARD_SIZE][BOARD_SIZE] = {};
    if (!validateCSV(userFile, rows, cols, tempBoard))
    {
        cout << "Invalid CSV. Expected " << rows << " rows and " << cols
             << " columns containing only 0 or 1." << endl;
        return false;
    }

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            board[row][col] = tempBoard[row][col];
        }
    }
    return true;
}

void getCSVFILE(int board[][BOARD_SIZE], int rows, int cols)
{
    string fileName;
    while (true)
    {
        cout << "What is the name of the CSV file? ";
        if (!getline(cin, fileName))
        {
            return;
        }
        if (loadCSV(fileName, rows, cols, board))
        {
            cout << "Initial board loaded successfully!" << endl;
            return;
        }
    }
}

void seedRandomCells(int board[][BOARD_SIZE], int rows, int cols)
{
    int availableCells = 0;
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (board[row][col] == DEAD_CELL)
            {
                availableCells++;
            }
        }
    }

    if (availableCells == 0)
    {
        cout << "The initial board is already full." << endl;
        return;
    }

    int numberOfCellsToAdd;
    while (true)
    {
        numberOfCellsToAdd = getPositiveInteger("How many cells would you like the scientist to add? ");
        if (numberOfCellsToAdd == 0)
        {
            return;
        }
        if (numberOfCellsToAdd <= availableCells)
        {
            break;
        }
        cout << "Invalid input! Only " << availableCells << " dead cells are available." << endl;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> randomRow(0, rows - 1);
    uniform_int_distribution<> randomCol(0, cols - 1);

    int cellsAdded = 0;
    while (cellsAdded < numberOfCellsToAdd)
    {
        int row = randomRow(gen);
        int col = randomCol(gen);
        if (board[row][col] == DEAD_CELL)
        {
            board[row][col] = LIVE_CELL;
            cellsAdded++;
        }
    }
    cout << "The scientist added " << numberOfCellsToAdd << " cells to the board!" << endl;
}

// Adom - Game simulation engine, neighbour counting and game rules
int countLivingCells(const int board[][BOARD_SIZE])
{
    int livingCells = 0;
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (board[row][col] == LIVE_CELL)
            {
                livingCells++;
            }
        }
    }
    return livingCells;
}

int checkAdjacentCells(const int board[][BOARD_SIZE], int row, int col)
{
    int numberOfLiveNeighbours = 0;
    int currentRow;
    int maxRow;
    int currentCol;
    int maxCol;

    switch (row)
    {
    case 0:
        currentRow = row;
        maxRow = row + 2;
        break;
    case BOARD_SIZE - 1:
        currentRow = row - 1;
        maxRow = row + 1;
        break;
    default:
        currentRow = row - 1;
        maxRow = row + 2;
        break;
    }
    switch (col)
    {
    case 0:
        currentCol = col;
        maxCol = col + 2;
        break;
    case BOARD_SIZE - 1:
        currentCol = col - 1;
        maxCol = col + 1;
        break;
    default:
        currentCol = col - 1;
        maxCol = col + 2;
        break;
    }

    for (int i = currentRow; i < maxRow; i++)
    {
        for (int j = currentCol; j < maxCol; j++)
        {
            if (i == row && j == col)
            {
                continue;
            }
            if (board[i][j] == LIVE_CELL)
            {
                numberOfLiveNeighbours++;
            }
        }
    }
    return numberOfLiveNeighbours;
}

void calculateNextYear(const int currentBoard[][BOARD_SIZE], int nextBoard[][BOARD_SIZE])
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            int neighbours = checkAdjacentCells(currentBoard, row, col);
            if (currentBoard[row][col] == LIVE_CELL)
            {
                if (neighbours == 2 || neighbours == 3)
                {
                    nextBoard[row][col] = LIVE_CELL;
                }
                else
                {
                    nextBoard[row][col] = DEAD_CELL;
                }
            }
            else
            {
                if (neighbours == 3)
                {
                    nextBoard[row][col] = LIVE_CELL;
                }
                else
                {
                    nextBoard[row][col] = DEAD_CELL;
                }
            }
        }
    }
}

bool boardsAreEqual(const int board1[][BOARD_SIZE], const int board2[][BOARD_SIZE])
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (board1[row][col] != board2[row][col])
            {
                return false;
            }
        }
    }
    return true;
}

void copyBoard(const int source[][BOARD_SIZE], int destination[][BOARD_SIZE])
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            destination[row][col] = source[row][col];
        }
    }
}

int simulateGame(const int startingBoard[][BOARD_SIZE], int &yearsPlayed, bool displayEachYear)
{
    int currentBoard[BOARD_SIZE][BOARD_SIZE];
    int nextBoard[BOARD_SIZE][BOARD_SIZE];
    copyBoard(startingBoard, currentBoard);

    yearsPlayed = 0;
    int livingCells = countLivingCells(currentBoard);

    if (displayEachYear)
    {
        cout << "Year " << yearsPlayed << endl;
        displayBoard(currentBoard, BOARD_SIZE, BOARD_SIZE);
    }

    while (livingCells > 0 && yearsPlayed < MAX_YEARS)
    {
        calculateNextYear(currentBoard, nextBoard);
        bool unchanged = boardsAreEqual(currentBoard, nextBoard);
        copyBoard(nextBoard, currentBoard);
        yearsPlayed++;
        livingCells = countLivingCells(currentBoard);

        if (displayEachYear)
        {
            cout << "Year " << yearsPlayed << endl;
            displayBoard(currentBoard, BOARD_SIZE, BOARD_SIZE);
        }
        if (unchanged)
        {
            break;
        }
    }
    return livingCells;
}

// Alan - Game statistics
GameRecord runAndRecordGame(const int startingBoard[][BOARD_SIZE], bool displayEachYear)
{
    GameRecord newRecord;
    copyBoard(startingBoard, newRecord.startingBoard);
    newRecord.livingCells = simulateGame(startingBoard, newRecord.yearsPlayed, displayEachYear);
    newRecord.deadCells = (BOARD_SIZE * BOARD_SIZE) - newRecord.livingCells;

    gameHistory.push_back(newRecord);
    if (!hasBestGame || newRecord.livingCells > bestLivingCells)
    {
        bestLivingCells = newRecord.livingCells;
        copyBoard(startingBoard, bestInitialBoard);
        hasBestGame = true;
    }
    return newRecord;
}

int getPositiveInteger(string message)
{
    int number;
    string line, extra;
    while (true)
    {
        cout << message;
        if (!getline(cin, line))
        {
            return 0;
        }
        stringstream input(line);
        if ((input >> number) && number >= MINIMUM_POSITIVE_NUMBER && !(input >> extra))
        {
            return number;
        }
        cout << "Invalid input. Please enter a positive number." << endl;
    }
}

double getProbability()
{
    double probability;
    string line, extra;
    while (true)
    {
        cout << "Enter the probability of a cell being alive (0-1): ";
        if (!getline(cin, line))
        {
            return -1.0;
        }
        stringstream input(line);
        if ((input >> probability) && isfinite(probability) &&
            probability >= MIN_PROBABILITY && probability <= MAX_PROBABILITY && !(input >> extra))
        {
            return probability;
        }
        cout << "Invalid probability. Please enter a value from 0 to 1." << endl;
    }
}

void displayGameStatistics()
{
    if (gameHistory.empty())
    {
        cout << "No games have been played yet." << endl;
        return;
    }

    cout << endl;
    cout << "========== GAME STATISTICS ==========" << endl;
    for (size_t i = 0; i < gameHistory.size(); i++)
    {
        cout << endl;
        cout << "Game " << i + 1 << endl;
        cout << "Living cells: " << gameHistory[i].livingCells << endl;
        cout << "Dead cells: " << gameHistory[i].deadCells << endl;
        cout << "Years played: " << gameHistory[i].yearsPlayed << endl;
    }
    cout << endl;
    cout << "Total games played: " << gameHistory.size() << endl;
    cout << "=====================================" << endl;
}

void runRandomExperiments()
{
    int numberOfGames = getPositiveInteger("How many games would you like to play? ");
    if (numberOfGames == 0)
    {
        return;
    }
    double probability = getProbability();
    if (probability < MIN_PROBABILITY)
    {
        return;
    }

    random_device rd;
    mt19937 generator(rd());
    bernoulli_distribution aliveDistribution(probability);

    cout << endl;
    cout << "Starting random experiments..." << endl;
    for (int game = 0; game < numberOfGames; game++)
    {
        int randomBoard[BOARD_SIZE][BOARD_SIZE] = {};
        for (int row = 0; row < BOARD_SIZE; row++)
        {
            for (int col = 0; col < BOARD_SIZE; col++)
            {
                if (aliveDistribution(generator))
                {
                    randomBoard[row][col] = LIVE_CELL;
                }
                else
                {
                    randomBoard[row][col] = DEAD_CELL;
                }
            }
        }

        GameRecord result = runAndRecordGame(randomBoard, false);
        cout << "Completed game " << game + 1 << " / " << numberOfGames
             << " | Living cells at end: " << result.livingCells << endl;
    }
    cout << endl;
    cout << "All random experiments completed!" << endl;
}

void displayBestInitialBoard()
{
    if (!hasBestGame)
    {
        cout << "No completed games are available yet." << endl;
        return;
    }
    cout << endl;
    cout << "========== BEST GAME ==========" << endl;
    cout << "Most living cells at the end: " << bestLivingCells << endl;
    cout << "Best initial board:" << endl;
    displayBoard(bestInitialBoard, BOARD_SIZE, BOARD_SIZE, true);
    cout << "===============================" << endl;
}
