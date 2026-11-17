#include <iostream>
using namespace std;

void displayBoard(int array[][30]);

int main() {
    int array[30][30] = {};
    displayBoard(array);
}

void displayBoard(int array[][30])
{
    for (int row = 0; row < 30; row++)
    {
        for (int col = 0; col < 30; col++) {
            cout << "| " << array[row][col] << " |";
        }
        cout << endl;
    }
}