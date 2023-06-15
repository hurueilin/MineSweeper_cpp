#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <random>
using namespace std;

int BOARD_SIZE = 5;
int NUM_BOMBS = 3;
vector<vector<char> > board(BOARD_SIZE, vector<char>(BOARD_SIZE, '@'));
unordered_set<int> bombs;
vector<pair<int,int> > dirs = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};

bool isInBound(int x, int y){
    if(x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) return false;
    return true;
}

void genBombs(int numBombs=NUM_BOMBS){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrubution(0, BOARD_SIZE-1); // [0, BOARD_SIZE-1]
    
    while(numBombs){
        int r = distrubution(gen);
        int c = distrubution(gen);
        if(bombs.count(r*BOARD_SIZE + c)) continue; // if this bomb already exist
        bombs.insert(r*BOARD_SIZE + c);
        numBombs--;
    }
}

void showBoard(bool reveal=false){
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if(reveal && bombs.count(i*BOARD_SIZE + j)){
                board[i][j] = '*';
            }
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

/* Count the number of bombs of (r,c)'s 8 neighbors. */
int countNeighboringBombs(int r, int c){
    int count = 0;
    for(auto dir: dirs){
        int x = r + dir.first;
        int y = c + dir.second;
        if(!isInBound(x, y)) continue;
        if(bombs.count(x*BOARD_SIZE + y)) count++;
    }
    return count;
}

/** 
 * Update board after clicking (r,c). 
 *
 * @return (r,c) is a bomb or not.
 */
bool updateBoard(int r, int c, int flag=0){
    // flag mode
    if(flag){
        if(board[r][c] == '@') board[r][c] = '!';
        else if(board[r][c] == '!') board[r][c] = '@';
        return false;
    }

    // Clicked on a Bomb!
    if(bombs.count(r*BOARD_SIZE + c)) return true;
    
    // Update current pos: (r,c)
    board[r][c] = countNeighboringBombs(r, c) + '0';
    
    // Check 8 neighbors
    for(auto dir: dirs){
        int x = r + dir.first;
        int y = c + dir.second;
        if(!isInBound(x, y)) continue;
        if(board[x][y] != '@' || bombs.count(x*BOARD_SIZE + y)) continue; // if (x,y) is visited or is a bomb, skip it
        
        board[x][y] = countNeighboringBombs(x, y) + '0';
        if(board[x][y] == '0') updateBoard(x, y); // ‘0'的八個鄰居都要打開
    }

    return false;
}

/* Check whether the game is cleared or not. */
bool checkGameCleared(){
    int count = 0;
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if(board[i][j] == '@' || board[i][j] == '!'){
                count++;
            }
        }
    }
    if(count == bombs.size()) return true;
    return false;
}

int main(){
    genBombs();
    // for(int x: bombs){
    //     cout << x / BOARD_SIZE << ", " << x % BOARD_SIZE << endl;
    // }

    bool hitBomb = false;
    bool isGameCleared = false;
    while(!hitBomb && !isGameCleared){
        showBoard();

        int flag = 0;
        int r, c;
        printf("Enter position (1 ~ %d)\n", BOARD_SIZE);
        printf("Flag? (1/0): "); cin >> flag;
        if(cin.fail() || flag > 1 || flag < 0){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }
        printf("row: "); cin >> r;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }
        printf("column: "); cin >> c;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }
        r--; c--;
        if(!isInBound(r, c)) continue; // Invalid position
        cout << "=================" << endl;
        
        hitBomb = updateBoard(r, c, flag);
        isGameCleared = checkGameCleared();
    }

    showBoard(true);
    if(hitBomb) cout << "YOU HIT BOMB!" << endl;
    else if(isGameCleared) cout << "YOU WIN!" << endl;
    
    return 0; 
}
