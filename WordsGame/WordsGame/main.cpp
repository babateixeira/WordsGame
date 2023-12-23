//Group 12:
// - B�rbara Teixeira (UP202209742)
// - Diogo Bastos (UP202202425)
// - Sara Vidal (UP202202427)

// WORD GAMES
//================================================================================
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
//--------------------------------------------------------------------------------
using namespace std;
//================================================================================
// COLOR CODES / ANSI ESCAPE SEQUENCES
//================================================================================
// TEXT COLOR
#define NO_COLOR "\033[0m"
#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define LIGHTRED "\033[1;31m"
#define WHITE "\033[1;37m"
// BACKGROUND COLOR
#define BLACK_B "\033[0;40m"
#define RED_B "\033[0;41m"
#define GREEN_B "\033[0;42m"
#define WHITE_B "\033[1;47m"

//================================================================================
//================================================================================
// USER DEFINED CLASSES
//================================================================================
//================================================================================

//================================================================================
//CLASSE BOARD
//================================================================================

class Board {
public:
    //Board(int numLins, int numCols);
    Board(const string& filename); // constructor
    void show() const; // const method (NOTE: const methods cannot modify the attributes)
    vector<vector<pair<char, bool>>> getBoard() const;
    vector<pair<char, string>> getPlayableLetters() const;
    bool isValidPosition(char row, char col) const;
    bool isValidInsertion(char row, char col, char letter, vector<char> playablePositions) const;
    bool getEnd() const;
private:
    vector<vector<pair<char, bool>>> b; //False --> letter not covered / True --> letter covered
};

//--------------------------------------------------------------------------------
// CONSTRUCTORS

Board::Board(const string& filename) {
    ifstream f(filename); // open an input file stream to read from a file with the name specified in 'filename'
    if (!(f.is_open())) { // 'is_open()' test whether the file is already open, if is not an error message will show
        cerr << "File not found!" << endl;
        exit(1); // stop the program with an exit code != 0 
    }

    string dummy, boardSize; // 'dummy' is a temporary string to discard certain values 
    getline(f, boardSize); // read the first line of the file 'board.txt' an save its content in the string 'boardSize'
    istringstream iss(boardSize); // initialize string stream 'iss' with the string 'boardSize' 
    int numLins, numCols;
    iss >> dummy >> dummy >> dummy >> numLins >> dummy >> numCols; // read values from 'boardSize' and store integers 'numLins' and 'numCols' 
    b = vector<vector<pair<char, bool>>>(numLins, vector<pair<char, bool>>(numCols, make_pair(' ', false))); // create a bidimensional matrix 'b' 
                                                                                        // with dimensions 'numLins' by 'numCols';
                                                                                        // initialize each element as a pair<char, bool> 
                                                                                        // with a space character (' ') and false
// jump to line 5 (where the board starts)
    for (int i = 0; i < 4; ++i)
        getline(f, dummy);

    // read the board content
    for (int i = 0; i < numLins; ++i)
    {
        getline(f, dummy); // read line of the board

        for (int j = 2; j < dummy.size(); j += 2)
        {
            if (isalpha(dummy[j]))
            {
                // place the letter on the board
                b[i][j / 2 - 1] = make_pair(dummy[j], false);
            }
        }
    }
}

//--------------------------------------------------------------------------------
// GETS AND SETS

vector<vector<pair<char, bool>>> Board::getBoard() const {
    return b;
}

//--------------------------------------------------------------------------------
// UNDERSTAND WHEN THE GAME IS OVER

bool Board::getEnd() const {
    bool gameOver = true;
    for (int i = 0; i < b.at(0).size(); i++) {
        gameOver = all_of(b.at(i).begin(), b.at(i).end(), [](const std::pair<char, bool>& p) {
            return p.first != ' ' || (p.first != ' ' && p.second == true);
            });
        if (!gameOver)
            break;
    }
    return gameOver;
}

//--------------------------------------------------------------------------------
// VALIDATE THE POSITION ON BOARD

bool Board::isValidPosition(char row, char col) const {
    char maxRow = 'A' + static_cast<char>(b.size()) - 1;
    char maxCol = 'a' + static_cast<char>(b.at(0).size()) - 1;

    return (row >= 'A' && row <= maxRow && col >= 'a' && col <= maxCol);
}

//--------------------------------------------------------------------------------
// VALIDATE IF LETTER FITS IN A CELL ON BOARD

//bool Board::isValidInsertion(char row, char col, char letter, vector<char> playablePositions) const {
//    if (letter == b.at(i).at(j).first) {
//
//    }
//    char maxCol = 'a' + static_cast<char>(b.at(0).size()) - 1;
//
//    return ;
//}

//--------------------------------------------------------------------------------
// GET A VECTOR WITH ALL LETTERS IN PLAYABLE POSITIONS AND RESPECTIVE POSITIONS

vector<pair<char, string>> Board::getPlayableLetters() const {
    vector<pair<char, string>> playableLetters;
    //vector<pair<int, int>> playablePositions;
    bool playable;
    for (int i = 0; i < b.at(0).size(); i++) { // scroll through each line
        for (int j = 0; j < b.at(1).size(); j++) { // scroll through each column
            playable = true; // playable until proven otherwise
            if (isalpha(b.at(i).at(j).first)) { // true if it is a letter
                if (!b.at(i).at(j).second) { // true if it is a letter that was not played yet
                    if (i == 0) {
                        if (!isalpha(b.at(i + 1).at(j).first))
                            playable = false; // letter has nothing after in the column
                    }
                    else if (i == b.at(0).size() - 1) {
                        if (!isalpha(b.at(i - 1).at(j).first))
                            playable = false; // letter has nothing before in the column
                    }
                    else {
                        if ((isalpha(b.at(i - 1).at(j).first) == 0 && isalpha(b.at(i + 1).at(j).first) == 0))
                            playable = false; // letter has nothing before and after in the column
                    }                                                                   
                    if (playable) {
                        for (int p = i; p > 0; p--) { // scroll up through the column
                            if (isalpha(b.at(i - (i - p) - 1).at(j).first)) { // true if it is a letter
                                if (!b.at(i - (i - p) - 1).at(j).second) { // true if it is a letter that was not played yet
                                    playable = false; // letter not playable once there is a letter before consecutive without being played
                                }
                            }
                            else {
                                break; // break as soon as it find a space
                            }
                        }
                    }
                    if (playable) {
                        playableLetters.push_back(make_pair(b.at(i).at(j).first, (to_string(i) + to_string(j)))); // add letter and position to the vector   
                    }
                    else { // is gonna check in the line if is valid
                        playable = true; // playable until proven otherwise
                        if (j == 0) {
                            if (!isalpha(b.at(i).at(j + 1).first))
                                playable = false; // letter has nothing after in the column
                        }
                        else if (j == b.at(1).size() - 1) {
                            if (!isalpha(b.at(i).at(j - 1).first))
                                playable = false; // letter has nothing before in the column
                        }
                        else {
                            if (isalpha(b.at(i).at(j - 1).first) == 0 && isalpha(b.at(i).at(j + 1).first) == 0) {
                                playable = false; // letter has nothing before and after in the line
                            }
                        }
                        if (playable) {
                            for (int l = j; l > 0; l--) { // scroll backwards through the line
                                if (isalpha(b.at(i).at(j - (j - l) - 1).first)) { // true if it is a letter
                                    if (!b.at(i).at(j - (j - l) - 1).second) { // true if it is a letter that was not played yet
                                        playable = false; // letter not playable once there is a letter before consecutive without being played
                                    }
                                }
                                else {
                                    break; // break as soon as it find a space
                                }
                            }
                        }
                        if (playable) {
                            playableLetters.push_back(make_pair(b.at(i).at(j).first, (to_string(i) + to_string(j)))); // add letter and position to the vector   
                        }
                    }
                }
            }
        }
    }

    return playableLetters;
}

//--------------------------------------------------------------------------------
// SHOW

void Board::show() const {

    cout << endl;
    cout << BLACK_B << WHITE << " ";
    cout << " ";
    for (size_t j = 0; j < b.at(0).size(); j++)
        cout << (char)('a' + j) << ' ';
    cout << endl;

    for (size_t i = 0; i < b.size(); i++)
    {
        cout << BLACK_B << WHITE << (char)('A' + i) << ' ';
        for (size_t j = 0; j < b.at(i).size(); j++)
            cout << BLACK << WHITE_B << b.at(i).at(j).first << ' ';
        cout << endl;
    }
    cout << NO_COLOR << BLACK_B;
    cout << endl;
}

//================================================================================
// CLASS BAG
//================================================================================

class Bag
{
public:
    Bag(const Board& b);
    vector<char> getLetters();
    void setLetters(vector<char> letters);
    void deleteLetters(int i);
    void showLetters() const;
private:
    vector<char> letters;
};

//--------------------------------------------------------------------------------
// CONSTRUCTORS

Bag::Bag(const Board& b)
{
    // read a line
    for (int i = 0; i < b.getBoard().at(0).size(); ++i)
    {
        // read a column
        for (int j = 0; j < b.getBoard().at(1).size(); j += 1)
        {
            // if the cell board contains a letter, adds it to the vector "letters" in the bag
            if (isalpha(b.getBoard().at(i).at(j).first)) { // "isalpha()" returns 0 if the character is not an alphabet letter
                letters.push_back(b.getBoard().at(i).at(j).first); // create a list with each letter on the board
            }
        }
    }

    srand(static_cast<unsigned int>(time(0))); // use current time as seed for random generator
    random_shuffle(letters.begin(), letters.end()); // rearrange the elements in the vector "letters" randomly
}

//--------------------------------------------------------------------------------
// GETS AND SETS

vector<char> Bag::getLetters() {
    return letters;
}

void Bag::setLetters(vector<char> letters) {
    srand(static_cast<unsigned int>(time(0))); // use current time as seed for random generator
    random_shuffle(letters.begin(), letters.end()); // rearrange the elements in the vector "letters" randomly
    this->letters = letters;
}

//--------------------------------------------------------------------------------
// DELETE LETTERS FROM THE BAG

void Bag::deleteLetters(int i) {
    letters.erase(letters.begin() + i);
}

//--------------------------------------------------------------------------------
// SHOW

void Bag::showLetters() const
{
    cout << "List of letters in the bag:" << endl;
    for (char letter : letters) {
        cout << letter << ' ';
    }
    cout << endl << endl;
}

//================================================================================
// CLASS HAND
//================================================================================

class Hand
{
public:
    Hand();
    Hand(vector<char> playerHand);
    Hand(int nLetters, Bag& letterBag);
    vector<char> getHandLetters();
    pair<vector<char>, bool> checkIfCanPlay(Board& b, Bag& letterBag);
    bool validMoveExist(vector<pair<char, string>>& playableLetters);
    string readLetterToChange(int i);
    bool changeHand(string& lettersSelected, Bag& letterbag);
    void showHand() const;
private:
    vector<char> playerHand;
};

//--------------------------------------------------------------------------------
// CONSTRUCTORS

Hand::Hand() {
    // nothing to do here, the vector<char> playerHand will be created empty automatically
}

Hand::Hand(int nLetters, Bag& letterBag) {
    for (int i = 0; i < nLetters; i++) {
        char randomLetter = letterBag.getLetters().at(i);
        playerHand.push_back(randomLetter);
        letterBag.deleteLetters(i);
    }
}

Hand::Hand(vector<char> playerHand) {
    this->playerHand = playerHand;
}

//--------------------------------------------------------------------------------
// GETS AND SETS

vector<char> Hand::getHandLetters() {
    return playerHand;
}

//--------------------------------------------------------------------------------
// CHECK IF THERE IS A VALID MOVE

bool Hand::validMoveExist(vector<pair<char,string>>& playableLetters) {
    for (int i = 0; i < playableLetters.size(); i++) {
        for (char ch2 : playerHand) {
            if (playableLetters.at(i).first == ch2) {
                return true; // common character found between the player's hand and the playable letters on the board
            }
        }
    }
    return false;
}

//--------------------------------------------------------------------------------
// CHANGE LETTERS BETWEEN HAND AND BAG

bool Hand::changeHand(string& lettersSelected, Bag& letterbag) {
    vector <char> lettersSelectedVet;
    if (lettersSelected.size() == 1) //convert string to vector of chars
        lettersSelectedVet = { lettersSelected[0] };
    else
        lettersSelectedVet = { lettersSelected[0], lettersSelected[2] };
    // order the vectors, as includes requires that the ranges are ordered
    sort(lettersSelectedVet.begin(), lettersSelectedVet.end());
    sort(playerHand.begin(), playerHand.end());
    // check if all elements of lettersSelected are contained in playerHand
    if (includes(playerHand.begin(), playerHand.end(), lettersSelectedVet.begin(), lettersSelectedVet.end())) {
        // it will find the first letters from the bag and replace them with the letters in the hand selected in lettersSelected
        for (int t = 0; t < lettersSelectedVet.size(); t++) {
            for (char& ch : playerHand) {
                if (ch == lettersSelectedVet.at(t)) {
                    char old = ch;
                    vector<char> newBag = letterbag.getLetters();
                    ch = newBag.at(t);
                    newBag.at(t) = old;
                    letterbag.setLetters(newBag); //DEVIA-SE FAZER NO FIM DE TUDO
                    break;  // stop after first substitution;
                }
            }
        }
        cout << BLUE << "Your hand as changed!" << endl << NO_COLOR;
        showHand();
        return true;
    }
    else {
        cout << RED << "You can only select letters that are present in your hand!" << endl << NO_COLOR;
        showHand();
        return false;
    }
}

//--------------------------------------------------------------------------------
// READ LETTERS FROM HAND TO CHANGE

string Hand::readLetterToChange(int i) {
    string input;
    do {
        if (i == 0) // can only change one letter;
            cout << "Which hand letter do you want to change (QUIT/PASS)? " << endl;
        else // can change one or two letters;
            cout << "Which hand letter/s do you want to change (QUIT/PASS)? " << endl;
        if (getline(cin, input)) {
            if (input == "QUIT" || input == "PASS") { // check if the person wants to quit or pass
                return input;
            }
            else {
                if (i == 0) { // to the case where there is only one letter in the bag
                    if (input.size() == 1)
                        return input;
                    else
                        cout << RED << "You can only choose one letter from your hand! " << endl << NO_COLOR;
                }
                else { // to the case where there is two or more letters in the bag
                    if ((input.size() == 3 && input[1] == ' ') || input.size() == 1) // if input is two letters separeted by a space or input is only one letter
                        return input;
                    else
                        cout << RED << "You can only choose one or two letter from your hand separated by space! " << endl << NO_COLOR;
                }
            }
        }
    } while (true);
}

//--------------------------------------------------------------------------------
// CHECK IF CAN PLAY

pair<vector<char>, bool> Hand::checkIfCanPlay(Board& b, Bag& letterbag) {
    vector<pair<char,string>> playableLetters = b.getPlayableLetters();
    if (validMoveExist(playableLetters)) { // it is true if there is a valid move so the player can play
        return make_pair(playerHand, true);
    }
    else { // if there is no valid moves it asks for substitutions in the hand                                              
        cout << BLUE << "You have no play options!" << endl << NO_COLOR;
        string selectedLetters;
        int aux = 0;
        if (letterbag.getLetters().size() == 0) { // true if the bag is empty
            cout << BLUE << "The bag is empty, you cannot change letters, your turn has passed!" << endl << NO_COLOR;
            return make_pair(playerHand, false); // the player is not going to play
        }
        else if (letterbag.getLetters().size() == 1 || playerHand.size() == 1) { // true if the bag has only 1 letter
            cout << BLUE << "You can only exchange one letter from your hand with the bag!" << endl << NO_COLOR;
            aux = 1;
        }
        else {
            cout << BLUE << "You can exchange one or two letters from your hand with the bag!" << endl << NO_COLOR;
            aux = 2;
        }
        if (aux != 0) {
            bool isValid = false;
            do {
                if (aux == 1)
                    selectedLetters = readLetterToChange(0); // can return one letter or QUIT or PASS
                else
                    selectedLetters = readLetterToChange(1); // can return one or two letters or QUIT or PASS
                if (selectedLetters == "PASS") {
                    cout << BLUE << "You passed your turn!" << endl << NO_COLOR;
                    return make_pair(playerHand, false);
                }
                else if (selectedLetters == "QUIT") {
                    throw "You quit the game!";
                }
                else
                    isValid = changeHand(selectedLetters, letterbag); // do the substitution and it returns true if the substitution was completed correctely            
            } while (!isValid);
            if (validMoveExist(playableLetters)) { // it is true if there is a valid move so the player can play
                return make_pair(playerHand, true);
            }
            else {
                cout << BLUE << "Still have no play options!" << endl << NO_COLOR;
                return make_pair(playerHand, false); // the player is not going to play
            }
        }
    }
}

//--------------------------------------------------------------------------------
// SHOW

void Hand::showHand() const {
    cout << "List of letters in the hand: ";
    for (char letter : playerHand) {
        cout << letter << ' ';
    }
    cout << endl;
}

//================================================================================
// CLASS PLAYER
//================================================================================

class Player
{
public:
    Player(int id, int points, string name, Hand hand);
    Player(int id, int nLetters, Bag& letterBag);
    int getId() const;
    int getPoints() const;
    string getName() const;
    Hand getHand() const;
    void play(Board& b, Bag& letterBag);
    void showPlayer() const;
    void printWinner() const;
private:
    int id_;
    int points_;
    string name_;
    Hand hand_;
};

//--------------------------------------------------------------------------------
// CONSTRUCTORS

Player::Player(const int id, const int nLetters, Bag& letterBag) {
    bool isValid = false;
    string name;
    do {
        cout << "Enter the name of the Player " << id << ": ";
        if (getline(cin, name)) {
            if (!name.empty()) {
                isValid = true;
                id_ = id;
                points_ = 0;
                name_ = name;
                Hand hand(nLetters, letterBag);
                hand_ = hand;
            }
        }
        if (!isValid) {
            cin.clear();
            // clean input buffer
            cin.ignore(1000, '\n');
            cout << RED << "Invalid name!" << endl;
        }
        cout << NO_COLOR;
    } while (!isValid);
}

Player::Player(int id, int points, string name, Hand hand) {
    id_ = id;
    points_ = points;
    name_ = name;
    hand_ = hand;
}

//--------------------------------------------------------------------------------
// GETS AND SETS

int Player::getId() const {
    return id_;
}

int Player::getPoints() const {
    return points_;
}

string Player::getName() const {
    return name_;
}

Hand Player::getHand() const {
    return hand_;
}

//--------------------------------------------------------------------------------
// PLAY LETTERS

void Player::play(Board& b, Bag& letterBag) {
    bool isValidLetter = false;
    string letterInput;
    do {
        cout << "Letter (QUIT/PASS)? ";
        if (getline(cin, letterInput)) {
            if (letterInput == "QUIT") {
                throw "You quit the game!";
            }
            else if (letterInput == "PASS") {
                cout << BLUE << "You passed your turn!" << endl << NO_COLOR;
                break;
            }
            else if (letterInput.size() == 1) {
                vector<char> playerHand = hand_.getHandLetters();
                char letter = toupper(letterInput[0]);
                if (find(playerHand.begin(), playerHand.end(), letter) != playerHand.end()) {
                    /*vector<pair<char, string>> playableLet_Pos = b.getPlayableLetters();
                    vector<char> playableLetters;
                    vector<string> playablePositions;
                    for (int i = 0; i < playableLet_Pos.size(); i++) {
                        playableLetters.push_back(playableLet_Pos.at(i).first);
                        playablePositions.push_back(playableLet_Pos.at(i).second);
                    }*/
                    //if (find(playableLetters.begin(), playableLetters.end(), letter) != playableLetters.end()) {
                    //    //if (isValidInsertion(char row, char col, char letter, vector<char> playablePositions)) { // Verifify if the position is playable
                    //    //                                                                                         // and the letter fits
                    //      //  isValidLetter = true;
                    //   // }
                    //}
                    //else {
                    //    cout << RED << "The selected letter does not fit in any playbale position!" << endl << NO_COLOR;
                    //}
                }
                else {
                    cout << RED << "You can only select letters that are present in your hand!" << endl << NO_COLOR;
                }
            }
            else {
                cout << RED << "Invalid input!" << endl << NO_COLOR;
            }
        }
    } while (!isValidLetter);

    bool isValidPosition = false;
    string position;

    do {
        cout << "Position (Lc) (QUIT/PASS)? ";
        if (getline(cin, position)) {
            if (position == "QUIT") {
                throw "You quit the game!";
            }
            else if (position == "PASS") {
                cout << BLUE << "You passed your turn!" << endl << NO_COLOR;
                break;
            }
            else if (position.size() == 2 && isalpha(position[0]) && isupper(position[0]) && isalpha(position[1]) && islower(position[1])) {
                if (b.isValidPosition(position[0], position[1])) {
                    
                    isValidPosition = true;
                }
                else {
                    cout << RED << "Invalid postion! Please enter a position that exists on the board!" << endl << NO_COLOR;
                }
            }
            else {
                cout << RED << "Invalid input format. Please enter a valid position in the format (Lc) or QUIT/PASS." << endl << NO_COLOR;
            }
        }

    } while (!isValidPosition);
}

//--------------------------------------------------------------------------------
// SHOW

void Player::showPlayer() const {
    cout << "Player " << id_ << endl;
    cout << " -> Name: " << name_ << endl;
    cout << " -> ";
    hand_.showHand();
    cout << " -> Points: " << points_ << endl;
    cout << endl;
}

//================================================================================
// CLASS LIST PLAYER
//================================================================================

class ListPlayer
{
public:
    ListPlayer(int nLetters, Bag& letterBag);
    vector<Player> getListPlayers() const;
    void setPlayer(int i, Player player);
    void deletePlayer(int i);
    void showPlayers() const;
private:
    vector<Player> playersList;
};

//--------------------------------------------------------------------------------
// CONSTRUCTORS

ListPlayer::ListPlayer(int nLetters, Bag& letterBag) {
    bool isValid = false;
    int num;
    do {
        cout << "How many players are goin to play? ";
        if (cin >> num) {
            if (num >= 2 && num <= 4) {
                isValid = true;
                // clean input buffer
                cin.ignore(1000, '\n');
            }
        }
        if (!isValid) {
            cin.clear();
            // clean input buffer
            cin.ignore(1000, '\n');
            cout << RED << "The number should be betwwen 2 and 4!" << endl;
        }
        cout << NO_COLOR;
    } while (!isValid);

    for (int i = 1; i <= num; i++) {
        Player p(i, nLetters, letterBag);
        playersList.push_back(p);
    }
    cout << endl;
}

//--------------------------------------------------------------------------------
// GETS AND SETS

vector <Player> ListPlayer::getListPlayers() const {
    return playersList;
}

void ListPlayer::setPlayer(int i, Player player) {
    playersList.at(i) = player;
}

//--------------------------------------------------------------------------------
// DELETE PLAYER FROM THE GAME

void ListPlayer::deletePlayer(int i) {
    playersList.erase(playersList.begin() + i);
}

//--------------------------------------------------------------------------------
// SHOW

void ListPlayer::showPlayers() const {
    cout << "List of players in the game:" << endl;
    for (Player player : playersList) {
        player.showPlayer();
    }
    cout << endl;
}
//---------------------------------------------------------------------------------
//PRINT THE WINNER


void printWinner(const vector<Player>& players) {
    if (players.size() == 1) { //If there is only one player playing, this one is going to be the winner of the game
        cout << "The winner is: " << GREEN << players[0].getName() << NO_COLOR << "!";
        cout << endl;
    }

    else { //If there are more players playing... 
        int maxPoints = 0;
        vector<string> winners; //This is a list that will save the names of the winners

        for (const auto& player : players) {
            if (player.getPoints() > maxPoints) //If the number of points is greater than the "maxPoints", "maxPoints" its going to be updated with this new points, until it reaches the maximum
            {
                maxPoints = player.getPoints();
                winners.push_back(player.getName()); //Gets the name of the player with this Maximum points.

            }
            else if (player.getPoints() == maxPoints) //If the players have the same points...
            {
                winners.push_back(player.getName()); //Their names are going to be saved on the winners list.
            }
        }
        if (winners.size() == 1) //If the list of winners has only one name, this player is going to be the winnner
        {
            cout << "The winner is: " << GREEN << winners[0] << NO_COLOR << ", with " << maxPoints << " points!" << endl;
        }
        else if (winners.size() > 1) //If in the winners list there is more than one player name...
        {
            cout << "It's a tie between:";
            for (const auto& winner : winners) {
                cout << " Player " << GREEN << winner << NO_COLOR;
            }
            cout << " with " << maxPoints << " points each!" << endl;  //both of them are going to be the winners.
        }
    }

}

//================================================================================
//================================================================================
// HELP FUNCTIONS
//================================================================================
//================================================================================

// Read the number of letters initially taken by each player
void readNLetters(int& nLetters, Bag& letterBag) {
    bool isValid = false;
    do {
        cout << "How many letters should be initially taken by each player? ";
        if (cin >> nLetters) {
            if (nLetters <= (letterBag.getLetters().size() / 4)) {
                isValid = true;
                // clean input buffer
                cin.ignore(1000, '\n');
            }
        }
        if (!isValid) {
            cin.clear();
            // clean input buffer
            cin.ignore(1000, '\n');
            cout << RED << "The maximum number of letters should be under " << (letterBag.getLetters().size() / 4) << "!" << endl;
        }
        cout << NO_COLOR << endl;
    } while (!isValid);

}

//================================================================================
//================================================================================
// MAIN
//================================================================================
//================================================================================

int main() {
    cout << "WORDS GAME\n\n";

    string filename;
    cout << "File name: ";
    cin >> filename;
    Board b(filename);
    b.show();

    Bag letterBag(b);
    letterBag.showLetters();

    int nLetters;
    readNLetters(nLetters, letterBag);

    ListPlayer listPlayer(nLetters, letterBag);
    listPlayer.showPlayers();


    while (!b.getEnd() && listPlayer.getListPlayers().size() != 1) {
        cout << BLUE << "--------------------------------------NEW ROUND----------------------------------" << endl << NO_COLOR;
        for (int i = 0; i < listPlayer.getListPlayers().size(); i++) {
            cout << BLUE << "---------------------------------------PLAYER " << listPlayer.getListPlayers().at(i).getId() << "----------------------------------" << endl << NO_COLOR;
            listPlayer.getListPlayers().at(i).getHand().showHand();
            try {
                pair<vector<char>, bool> inf = listPlayer.getListPlayers().at(i).getHand().checkIfCanPlay(b, letterBag);
                Hand hand(inf.first);
                Player player(listPlayer.getListPlayers().at(i).getId(), listPlayer.getListPlayers().at(i).getPoints(), listPlayer.getListPlayers().at(i).getName(), hand);
                listPlayer.setPlayer(i, player);

                if (inf.second) { // if the player has valid moves
                    listPlayer.getListPlayers().at(i).play(b, letterBag);
                }
            }
            catch(const char* message){
                cout << BLUE << message << endl << NO_COLOR;
                listPlayer.deletePlayer(i);
                i--; // Decrease i to correct iteration after player removal
            }
            if (b.getEnd() || listPlayer.getListPlayers().size() == 1) { // it ends if you reach the end of the board or if there is only one player left
                break;
            }
        }
    }
    cout << GREEN << "END OF THE GAME" << endl << NO_COLOR;

    return 0;
}