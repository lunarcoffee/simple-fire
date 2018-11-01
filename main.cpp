#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <sys/ioctl.h>
#include <unistd.h>

using std::cout;
using std::vector;

void tick(vector<vector<int>> &sc, int rows, int cols) {
    // Display the corresponding color/character for each cell in the fire matrix.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int temp = sc[i][j];
            
            // Yellow for strong fire, red for medium, grey for weak.
            if (temp > 54) {
                cout << "\033[33;1m@";
            } else if (temp > 36) {
                cout << "\033[31m?";
            } else if (temp > 20) {
                cout << "\033[30;1m*";
            } else {
                cout << " ";
            }
        }
        // Flush each row.
        cout << "\n";
    }
}

int main() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    // Get the rows and columns in the terminal window.
    const int ROWS = w.ws_row;
    const int COLS = w.ws_col;
    
    // Holds integers representing the strength of the fire at each cell.
    vector<vector<int>> sc(ROWS);
    for (int i = 0; i < ROWS; ++i) {
        sc[i] = vector<int>(ROWS);
    }
    
    while (true) {
        // Set new values for the base of the fire.
        for (int i = 0; i < COLS; ++i) {
            sc[ROWS - 1][i] = rand() % 2 ? 18 : 99;
        }
        
        // Update the plane with new fire strength values.
        for (int i = 0; i < ROWS - 1; ++i) {
            for (int j = 1; j < COLS - 1; ++j) {
                // Average out the three below the current cell along with the current cell.
                sc[i][j] = (sc[i][j] + sc[i + 1][j - 1] + sc[i + 1][j] + sc[i + 1][j + 1]) / 4 - 1;
            }
        }
        
        // Wait and display the new fire.
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        tick(sc, ROWS, COLS);
    }
}
