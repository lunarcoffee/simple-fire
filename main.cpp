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
            // Fire strength values and their corresponding colors/characters.
            int strengths[] = {70, 64, 56, 50, 44, 40, 34, -1};
            std::string fires[] = {"33;1m#", "33;1m&", "31m%", "31m+", "30;1m*", "30;1m:", "30;1m.", "1m "};
            
            // Find the currrent strength value's color/character and display it.
            for (int k = 0; k < 8; ++k) {
                if (sc[i][j] > strengths[k]) {
                    cout << "\033[" + fires[k];
                    break;
                }
            }
        }
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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        tick(sc, ROWS, COLS);
    }
}
