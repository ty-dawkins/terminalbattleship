//Goal: Create a battleship game where two players can place ships on a grid
//and take turns guessing the locations of each other's ships until one player wins.
//1 player vs CPU & 2 player mode

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
// Constants

const int SIZE = 5; // ARRAY SIZE
const int SHIPS = 4; // NUMBER OF SHIPS PER PLAYER
const int MAX_TURNS = SIZE * SIZE; // MAX TURNS POSSIBLE

// Row and Column limits for RNG
const int MIN_ROW_VAL = 'A';
const int MAX_ROW_VAL = 'E';
const int RANGE_ROW_VAL = MAX_ROW_VAL - MIN_ROW_VAL + 1;
const int MIN_COL_VAL = '1';
const int MAX_COL_VAL = '5';
const int RANGE_COL_VAL = MAX_COL_VAL - MIN_COL_VAL + 1;


// Function prototypes
static bool checkWinner(int shipsCPU, int shipsP1);

static void printBoard(bool board[SIZE][SIZE], int P1HP, int CPUHP);

static void printBoard(char board[SIZE][SIZE], int P1HP, int CPUHP);

static bool didGuessHit(bool hiddenBoard[SIZE][SIZE], char seenBoard[SIZE][SIZE], char guess[2][MAX_TURNS], int turnCounter);

static bool isValidGuessCPU(char guess[2][MAX_TURNS], int turnCounter);

static bool isValidGuess(char guess[2][MAX_TURNS], int turnCounter);

void placeShipsRandomly(bool board[SIZE][SIZE], string id);

void playerPlacesShips(bool playerSetupBoard[SIZE][SIZE], char shipCoordinates[2][SHIPS]);

void getValidCoordinatesForPlacement(char shipCoordinates[2][SHIPS], bool playerSetupBoard[SIZE][SIZE], int placed);

void playerTurn(bool cpuBoardSetup[SIZE][SIZE], char cpuBoardHits[SIZE][SIZE], char playerGuess[2][MAX_TURNS], int turnCounter, int& shipsCPU, int& shipsP1, string P);

void player2Turn(bool cpuBoardSetup[SIZE][SIZE], char cpuBoardHits[SIZE][SIZE], char playerGuess[2][MAX_TURNS], int turnCounter, int& shipsCPU, int& shipsP1, string P);

void cpuTurn(bool playerBoardSetup[SIZE][SIZE], char playerBoardHits[SIZE][SIZE], char cpuGuess[2][MAX_TURNS], int turnCounter, int& shipsP1, int& shipsCPU);





int main() {
    system("color B0"); // Changes console color scheme

    char replay = 'Y';

    srand(static_cast<unsigned int>(time(NULL)));

    while (toupper(replay) == 'Y') {
        replay = 'Y';
        int shipsP1 = SHIPS;
        int shipsCPU = SHIPS;
        int turnCounter = 0;

        char manualSetupResponse = '-';

        char shipCoordinates[2][SHIPS];
        char playerGuess[2][MAX_TURNS];
        char cpuGuess[2][MAX_TURNS];

        // Initialize guess and ship coordinate arrays
        for (int i = 0; i < SHIPS; i++) {
            shipCoordinates[0][i] = '-';
            shipCoordinates[1][i] = '-';
        }

        for (int i = 0; i < MAX_TURNS; i++) {


            playerGuess[0][i] = '-';
            playerGuess[1][i] = '-';

            cpuGuess[0][i] = '-';
            cpuGuess[1][i] = '-';
        }

        bool validGuess = false;

        bool playerBoardSetup[SIZE][SIZE];

        char playerBoardHits[SIZE][SIZE];


        bool cpuBoardSetup[SIZE][SIZE];

        char cpuBoardHits[SIZE][SIZE];

        // Initialize bool boards
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                playerBoardSetup[i][j] = false;
                cpuBoardSetup[i][j] = false;
            }
        }

        //initialize char boards
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                playerBoardHits[i][j] = '-';
                cpuBoardHits[i][j] = '-';
            }
        }

        //Intro - rules
        cout << "Welcome to Battleship!" << endl;
        cout << "You and the CPU will each place " << SHIPS << " ships on a " << SIZE << "x" << SIZE << " grid" << endl;
        cout << "You will take turns guessing the locations of each other's ships until one player wins!" << endl << endl;


        // Player places ships
        cout << "Player 1 Board Setup: " << endl;
        cout << "Would you like to place your ships manually? (Y/N): ";
        cin >> manualSetupResponse;
        manualSetupResponse = toupper(manualSetupResponse);
        cin.ignore(); // Clear newline character from input buffer

        while (manualSetupResponse != 'Y' && manualSetupResponse != 'N') {
            cout << "Please enter a valid option (Y/N): ";
            cin >> manualSetupResponse;
            manualSetupResponse = toupper(manualSetupResponse);
        }

        if (manualSetupResponse == 'N') {
            placeShipsRandomly(playerBoardSetup, "Player"); // User Defined function
            cout << "Player 1 Board after placing ships: " << endl;
            cout << "0 = empty, 1 = ship" << endl;
            printBoard(playerBoardSetup, shipsP1, shipsCPU); // User Defined function
        }

        else if (manualSetupResponse == 'Y') {
            cout << "You have chosen to place your ships manually." << endl;
            printBoard(playerBoardSetup, shipsP1, shipsCPU); // User Defined function
            playerPlacesShips(playerBoardSetup, shipCoordinates); // User Defined function
            cout << "Player 1 Board after placing ships?" << endl;
            cout << "0 = empty, 1 = ship" << endl;
            printBoard(playerBoardSetup, shipsP1, shipsCPU); // User Defined function
        }

        cout << "Would you like to play against the CPU? (Y/N): ";
        char vsCPU;
        cin >> vsCPU;
        vsCPU = toupper(vsCPU);
        while (vsCPU != 'Y' && vsCPU != 'N') {
            cout << "Please enter a valid option (Y/N): ";
            cin >> vsCPU;
            vsCPU = toupper(vsCPU);
        }
        cin.ignore();// Clear newline character from input buffer

        // P1 vs CPU
        if (vsCPU == 'Y') {


            placeShipsRandomly(cpuBoardSetup, "CPU"); // User Defined function

            //ansewr key below if needed for debugging
            //cout << "CPU Board Setup after placing ships: " << endl;
            //printBoard(cpuBoardSetup, shipsP1, shipsCPU); // User Defined function

            while (shipsP1 > 0 && shipsCPU > 0) {

                // Player 1 turn
                playerTurn(cpuBoardSetup, cpuBoardHits, playerGuess, turnCounter, shipsCPU, shipsP1, "Player 1"); // User Defined function

                //Turn End

                if (checkWinner(shipsCPU, shipsP1)) // User Defined function
                {

                    cout << "Game Over!" << endl;
                    break;
                }
                system("pause");

                // CPU turn

                cpuTurn(playerBoardSetup, playerBoardHits, cpuGuess, turnCounter, shipsP1, shipsCPU); // User Defined function

                //Turn End
                if (checkWinner(shipsCPU, shipsP1)) // User Defined function
                {

                    cout << "Game Over!" << endl;
                    break;
                }

                turnCounter++;
                //cout << "Turn Couynter: " << turnCounter << endl;
                system("pause");
            }
        }

        // P1 V P2
        if (vsCPU == 'N') {

            // P2 places ships
            manualSetupResponse = '-';
            cout << "Player 2 Board Setup: " << endl;
            cout << "Would you like to place your ships manually? (Y/N): ";
            cin >> manualSetupResponse;
            manualSetupResponse = toupper(manualSetupResponse);
            cin.ignore(); // Clear newline character from input buffer

            while (manualSetupResponse != 'Y' && manualSetupResponse != 'N') {
                cout << "Please enter a valid option (Y/N): ";
                cin >> manualSetupResponse;
                manualSetupResponse = toupper(manualSetupResponse);
            }

            if (manualSetupResponse == 'N') {
                placeShipsRandomly(cpuBoardSetup, "Player 2"); // User Defined function
                cout << "Player 2 Board after placing ships: " << endl;
                cout << "0 = empty, 1 = ship" << endl;
                printBoard(cpuBoardSetup, shipsP1, shipsCPU); // User Defined function
            }

            else if (manualSetupResponse == 'Y') {
                cout << "You have chosen to place your ships manually." << endl;
                printBoard(cpuBoardSetup, shipsP1, shipsCPU); // User Defined function
                playerPlacesShips(cpuBoardSetup, shipCoordinates); // User Defined function
                cout << "Player 2 Board after placing ships?" << endl;
                cout << "0 = empty, 1 = ship" << endl;
                printBoard(cpuBoardSetup, shipsP1, shipsCPU); // User Defined function
            }
            while (shipsP1 > 0 && shipsCPU > 0) {

                // Player 1 turn
                playerTurn(cpuBoardSetup, cpuBoardHits, playerGuess, turnCounter, shipsCPU, shipsP1, "Player 1"); // User Defined function

                //Turn End

                if (checkWinner(shipsCPU, shipsP1)) // User Defined function
                {

                    cout << "Game Over!" << endl;
                    break;
                }
                system("pause");

                // P2 turn

                player2Turn(playerBoardSetup, playerBoardHits, cpuGuess, turnCounter, shipsCPU, shipsP1, "Player 2"); // User Defined function

                //Turn End
                if (checkWinner(shipsCPU, shipsP1)) // User Defined function
                {

                    cout << "Game Over!" << endl;
                    break;
                }

                turnCounter++;
                //cout << "Turn Couynter: " << turnCounter << endl;
                system("pause");
            }

        }

        cout << "Would you like to play again? (Y/N): ";

        cin >> replay;


        while (toupper(replay) != 'Y' && toupper(replay) != 'N') {
            cout << "Please enter a valid option (Y/N): ";
            cin >> replay;

        }
    }
    cout << "Thank you for playing!" << endl;
    replay = 'Y';
    return 0;
}

static bool checkWinner(int shipsCPU, int shipsP1) {
    if (shipsCPU != 0 && shipsP1 != 0) {
        return false;
    }
    else if (shipsCPU == 0 || shipsP1 == 0) {
        if (shipsCPU == 0) {
            cout << "Player 1 win!" << endl;
        }
        if (shipsP1 == 0) {
            cout << "Player 2 wins!" << endl;
        }
        return true;
    }
    return false;
}

static void printBoard(bool board[SIZE][SIZE], int P1HP, int CPUHP) {
    cout << "   ";
    for (int i = 0; i < SIZE; i++) {
        cout << "|" << static_cast<char>('A' + i) << "| ";
    }
    cout << endl << "----------------------" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << setw(2) << left << i + 1 << " ";
        for (int j = 0; j < 5; j++) {
            cout << "|" << board[i][j] << "| ";
        }
        cout << endl << "----------------------" << endl;
    }
    cout << "Player 1 HP: " << P1HP << endl;
    cout << setw(13) << "Player 2 HP: " << CPUHP << endl;
}

static void printBoard(char board[SIZE][SIZE], int P1HP, int CPUHP) {
    cout << "   ";
    for (int i = 0; i < SIZE; i++) {
        cout << "|" << static_cast<char>('A' + i) << "| ";
    }
    cout << endl << "----------------------" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << setw(2) << left << i + 1 << " ";
        for (int j = 0; j < 5; j++) {
            cout << "|" << board[i][j] << "| ";
        }
        cout << endl << "----------------------" << endl;
    }
    cout << "Player 1 HP: " << P1HP << endl;
    cout << setw(13) << "Player 2 HP: " << CPUHP << endl;
}

static bool didGuessHit(bool hiddenBoard[SIZE][SIZE], char seenBoard[SIZE][SIZE], char guess[2][MAX_TURNS], int turnCounter) {
    int col = 0;
    int row = guess[1][turnCounter] - '1';

    if (guess[0][turnCounter] == 'A') col = 0;
    if (guess[0][turnCounter] == 'B') col = 1;
    if (guess[0][turnCounter] == 'C') col = 2;
    if (guess[0][turnCounter] == 'D') col = 3;
    if (guess[0][turnCounter] == 'E') col = 4;

    if (hiddenBoard[row][col]) {
        seenBoard[row][col] = 'X';
        return true;
    }
    else {
        seenBoard[row][col] = 'O';
        return false;
    }
}

static bool isValidGuessCPU(char guess[2][MAX_TURNS], int turnCounter) {
    // cout << "Checking validity of guess: " << guess[0][turnCounter] << guess[1][turnCounter] << endl;

    if (turnCounter > 0) {
        for (int i = 0; i < turnCounter; i++) {
            //cout << "entering for loop iteration " << i << endl;

            if (guess[0][i] == guess[0][turnCounter] && guess[1][i] == guess[1][turnCounter]) {

                return false;
            }
        }
    }
    return true;
}

static bool isValidGuess(char guess[2][MAX_TURNS], int turnCounter) {
    // cout << "Checking validity of guess: " << guess[0][turnCounter] << guess[1][turnCounter] << endl;

    if (turnCounter > 0) {
        for (int i = 0; i < turnCounter; i++) {
            //cout << "entering for loop iteration " << i << endl;

            if (guess[0][i] == guess[0][turnCounter] &&
                guess[1][i] == guess[1][turnCounter]) {

                cout << "You have already guessed that location. Please try again." << endl;
                return false;
            }
        }
    }
    return true;
}

void placeShipsRandomly(bool board[SIZE][SIZE], string id) {
    int placed = 0;
    while (placed < SHIPS) {

        int r = rand() % SIZE;
        int c = rand() % SIZE;

        if (board[r][c] == false) {
            board[r][c] = true;
            placed++;
        }
    }
    cout << SHIPS << " ships have randomly been placed on the " << id << " board..." << endl;
    return;
}

void playerPlacesShips(bool playerSetupBoard[SIZE][SIZE], char shipCoordinates[2][SHIPS]) {
    int r = 0, c = 0;
    int placed = 0;
    int shipsLeft = SHIPS - placed;

    while (placed < SHIPS) {

        cout << "You have " << SHIPS - placed << " ships left" << endl;
        cout << "Where would you like to place your remaining ship(s)?" << endl;

        getValidCoordinatesForPlacement(shipCoordinates, playerSetupBoard, placed); // User Defined function
        r = shipCoordinates[1][placed] - '1';
        if (shipCoordinates[0][placed] == 'A') c = 0;
        if (shipCoordinates[0][placed] == 'B') c = 1;
        if (shipCoordinates[0][placed] == 'C') c = 2;
        if (shipCoordinates[0][placed] == 'D') c = 3;
        if (shipCoordinates[0][placed] == 'E') c = 4;
        if (playerSetupBoard[r][c] == false) {
            playerSetupBoard[r][c] = true;
            cout << "Ship placed at " << shipCoordinates[0][placed] << shipCoordinates[1][placed] << endl;
            printBoard(playerSetupBoard, SHIPS, SHIPS); // User Defined function
            placed++;

        }
        else if (playerSetupBoard[r][c] == true) {
            cout << "You already have a ship there. Please choose different coordinates." << endl;
        }


    }

    return;
}

void getValidCoordinatesForPlacement(char shipCoordinates[2][SHIPS], bool playerSetupBoard[SIZE][SIZE], int placed) {
    bool validPlace = false;
    bool validRow = false;
    bool validCol = false;

    string response1;
    string response2;

    while (validPlace == false) {
        cout << "Please enter the row you would like to place your next ship (e.g., A - E): ";
        getline(cin, response1);
        while (validRow == false) {

            if (response1.length() == 1 && ((response1 >= "A" && response1 <= "E") || (response1 >= "a" && response1 <= "e"))) {
                validRow = true;
                break;
            }

            if (response1.length() != 1) {
                cout << "Please enter a single character for the row (A - E): ";
                getline(cin, response1);
            }







            if ((response1 < "A" || response1 > "E") && (response1 < "a" || response1 > "e")) {
                cout << "Please enter a valid row (A - E): ";
                getline(cin, response1);
            }



        }
        shipCoordinates[0][placed] = toupper(response1[0]);

        cout << "Please enter the column you would like to place your ship (e.g., 1 - 5): ";
        getline(cin, response2);

        while (validCol == false) {

            if (response2.length() == 1 && ((response2 >= "1" && response2 <= "5"))) {
                validCol = true;
                break;
            }

            if (response1.length() != 1) {
                cout << "Please enter a single character for the column (1 - 5): ";
                getline(cin, response2);
            }







            else
            {
                cout << "Please enter a valid column (1 - 5): ";
                getline(cin, response2);
            }
        }
        shipCoordinates[1][placed] = response2[0];
        validPlace = true;
    }


}

void playerTurn(bool cpuBoardSetup[SIZE][SIZE], char cpuBoardHits[SIZE][SIZE], char playerGuess[2][MAX_TURNS], int turnCounter, int& shipsCPU, int& shipsP1, string P) {


    bool validGuess = false;
    bool validRow = false;
    bool validCol = false;

    string response1;
    string response2;

    cout << endl << P << " Your turn!" << endl;
    printBoard(cpuBoardHits, shipsP1, shipsCPU); // User Defined function

    while (validGuess == false) {
        cout << "Please enter the row you would like to attack (e.g., A - E): ";
        getline(cin, response1);
        while (validRow == false) {

            if (response1.length() == 1 && ((response1 >= "A" && response1 <= "E") || (response1 >= "a" && response1 <= "e"))) {
                validRow = true;
                break;
            }

            else if (response1.length() != 1) {
                cout << "Please enter a single character for the row (A - E): ";
                getline(cin, response1);
            }







            else if ((response1 < "A" || response1 > "E") && (response1 < "a" || response1 > "e")) {
                cout << "Please enter a valid row (A - E): ";
                getline(cin, response1);
            }



        }
        playerGuess[0][turnCounter] = toupper(response1[0]);
        validRow = false; // Reset for next input

        cout << "Please enter the column you would like to attack (e.g., 1 - 5): ";
        getline(cin, response2);

        while (validCol == false) {

            if (response2.length() == 1 && ((response2 >= "1" && response2 <= "5"))) {
                validCol = true;
                break;
            }

            if (response2.length() != 1) {
                cout << "Please enter a single character for the column (1 - 5): ";
                getline(cin, response2);
            }






            if ((response2 < "1" || response2 > "5")) {
                cout << "Please enter a valid column (1 - 5) : ";
                getline(cin, response2);
            }
        }
        playerGuess[1][turnCounter] = response2[0];
        validCol = false; // Reset for next input

        validGuess = isValidGuess(playerGuess, turnCounter); // User Defined function
    }
    // For next turn
    validGuess = false;
    validCol = false;
    validRow = false;

    cout << "You guessed: " << playerGuess[0][turnCounter] << playerGuess[1][turnCounter] << endl << endl;

    cout << "Result" << endl;

    if (didGuessHit(cpuBoardSetup, cpuBoardHits, playerGuess, turnCounter)) // User Defined function
    {
        cout << "It's a hit!" << endl;
        shipsCPU--;
    }
    else {
        cout << "You missed!" << endl;
    }
    printBoard(cpuBoardHits, shipsP1, shipsCPU); // User Defined function

    return;
}

void player2Turn(bool cpuBoardSetup[SIZE][SIZE], char cpuBoardHits[SIZE][SIZE], char playerGuess[2][MAX_TURNS], int turnCounter, int& shipsCPU, int& shipsP1, string P) {


    bool validGuess = false;
    bool validRow = false;
    bool validCol = false;

    string response1;
    string response2;

    cout << endl << P << " Your turn!" << endl;
    printBoard(cpuBoardHits, shipsP1, shipsCPU); // User Defined function

    while (validGuess == false) {
        cout << "Please enter the row you would like to attack (e.g., A - E): ";
        getline(cin, response1);
        while (validRow == false) {

            if (response1.length() == 1 && ((response1 >= "A" && response1 <= "E") || (response1 >= "a" && response1 <= "e"))) {
                validRow = true;
                break;
            }

            else if (response1.length() != 1) {
                cout << "Please enter a single character for the row (A - E): ";
                getline(cin, response1);
            }







            else if ((response1 < "A" || response1 > "E") && (response1 < "a" || response1 > "e")) {
                cout << "Please enter a valid row (A - E): ";
                getline(cin, response1);
            }



        }
        playerGuess[0][turnCounter] = toupper(response1[0]);
        validRow = false; // Reset for next input

        cout << "Please enter the column you would like to attack (e.g., 1 - 5): ";
        getline(cin, response2);

        while (validCol == false) {

            if (response2.length() == 1 && ((response2 >= "1" && response2 <= "5"))) {
                validCol = true;
                break;
            }

            if (response2.length() != 1) {
                cout << "Please enter a single character for the column (1 - 5): ";
                getline(cin, response2);
            }






            if ((response2 < "1" || response2 > "5")) {
                cout << "Please enter a valid column (1 - 5) : ";
                getline(cin, response2);
            }
        }
        playerGuess[1][turnCounter] = response2[0];
        validCol = false; // Reset for next input

        validGuess = isValidGuess(playerGuess, turnCounter); // User Defined function
    }
    // For next turn
    validGuess = false;
    validCol = false;
    validRow = false;

    cout << "You guessed: " << playerGuess[0][turnCounter] << playerGuess[1][turnCounter] << endl << endl;

    cout << "Result" << endl;

    if (didGuessHit(cpuBoardSetup, cpuBoardHits, playerGuess, turnCounter)) // User Defined function
    {
        cout << "It's a hit!" << endl;
        shipsP1--;
    }
    else {
        cout << "You missed!" << endl;
    }
    printBoard(cpuBoardHits, shipsP1, shipsCPU); // User Defined function

    return;
}

void cpuTurn(bool playerBoardSetup[SIZE][SIZE], char playerBoardHits[SIZE][SIZE], char cpuGuess[2][MAX_TURNS], int turnCounter, int& shipsP1, int& shipsCPU) {

    bool validGuess = false;
    bool validRow = false;
    bool validCol = false;

    string response1;
    string response2;

    cout << "\nCPU's turn!" << endl;

    while (validGuess == false) {
        cpuGuess[0][turnCounter] = static_cast<char>(rand() % RANGE_ROW_VAL + MIN_ROW_VAL);
        cpuGuess[1][turnCounter] = static_cast<char>(rand() % RANGE_COL_VAL + MIN_COL_VAL);
        validGuess = isValidGuessCPU(cpuGuess, turnCounter); // User Defined function
    }
    validGuess = false; // Reset for next turn
    cout << "CPU guessed: " << cpuGuess[0][turnCounter] << cpuGuess[1][turnCounter] << endl << endl;
    cout << "Result" << endl;

    if (didGuessHit(playerBoardSetup, playerBoardHits, cpuGuess, turnCounter)) // User Defined function
    {
        cout << "It's a hit!" << endl;
        shipsP1--;
    }
    else {
        cout << "CPU missed!" << endl;
    }
    printBoard(playerBoardHits, shipsP1, shipsCPU); // User Defined function

}
