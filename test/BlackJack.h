#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <fstream>
using namespace std;

// structs ------------------------

struct Card;
struct SkillCounter;

// classes ------------------------

class Player;  // 必須是一個純粹抽象類別（pure abstract class）

// 兩種Characters, 繼承自Player
class Seeker;
class Targetor;

// 由電腦操作的莊家, 繼承自Player
class Enemy;

class CardDeck;
class Game;


class Item;  // 道具卡 還沒寫
class Record;  // 玩家的記錄檔






// Card struct -------------------------------------------
struct Card
{
    string suit;
    string rank;
    int value;
    bool isInDeck;

    // 建構函式
    Card(const string &s, const string &r, int v);
};


// SkillCounter struct ----------------------------------
struct SkillCounter
{
    bool isSkillOneAvailable;
    bool isSkillTwoAvailable;
    bool isSkillThreeAvailable;

    // 建構函式
    SkillCounter();
};




/* Player class, 是一個pure abstract class -----------------------------
 是遊戲中所有玩家（包含敵我）的基礎類別，裡面定義了所有玩家都需要有的屬性和行為 */

class Player
{
protected:
    string name;
    vector<Card> hand;
    
    SkillCounter skillCounter;
    Record* record;
    
    
public:
    virtual ~Player() = default; // 虛擬析構函數
    
    Player(const string &playerName);

    bool operator<(const Player &other) const;

    bool operator>(const Player &other) const;

    int calculateHandValue() const;

    void showHand() const;

    void randomlyAddOneCard(CardDeck &deck);

    // 在 Player 類別中新增函式
    void addSpecificCard(Card *card);

    string getName() const;

    vector<Card> &getHand();
};





/* Seeker class, 繼承 Player 這個類別 ---------------------------
是玩遊戲的人可以選擇的我方角色，定義所有Seeker的共同屬性 */

class Seeker : public Player
{
public:
    virtual ~Seeker() = default; // 虛擬析構函數

    // Constructor for Seeker
    Seeker(const string &playerName);

    // skillOne: Seeker uses a special ability to seek another player's hand
    void seekAnotherPlayer(Player *targetPlayer);

    // skillTwo: Seeker uses a special ability to seek the deck
    void seekDeck(CardDeck &deck);
};




/* Targetor class, 繼承 Player 這個類別 ----------------------
是玩遊戲的人可以選擇的我方角色，定義所有Targetor的共同屬性 */

class Targetor : public Player
{
public:
    virtual ~Targetor() = default; // 虛擬析構函數

    // Constructor for Targetor
    Targetor(const string &playerName);

    // skillOne
    void takeSpecificCard(CardDeck &deck, const string &suit, const string &rank);

    // skillTwo
    void discardCard(Player *targetPlayer, int value);
};





/* Enemy class, 繼承 Player 這個類別 --------------------------
 是由電腦自動操作的敵方角色，定義所有Enemy的共同屬性 */

class Enemy : public Player
{
public:
    // Constructor for Enemy
    Enemy(const string &enemyName);
};




// CardDeck class ---------------------------------------
class CardDeck
{
private:
    vector<Card> cards;           // 存放卡片的向量
    std::mt19937 randomGenerator; // 隨機數生成器

    // 初始化卡片
    void initializeCards();
public:
    // 建構函式並同時初始化 52 張卡片
    CardDeck();

    // 取得卡片數量
    int getDeckSize() const;

    // 顯示卡組內容
    void displayDeck() const;

    // 從卡組中抽一張牌
    Card *drawOneCard();

    // 根據 suit 和 rank 尋找特定的 Card 並返回指標，如果 isInDeck 為 false 則輸出訊息
    Card *specificCard(const string &suit, const string &rank);

    // 檢查卡組是否為空
    bool isEmpty() const;
};





/* Game class -------------------------------------------
隨著遊戲進行，會陸續新增敵人、物品，玩家也會跟這些敵人和物品互動 */

class Game
{
private:
    CardDeck gameDeck;        // 遊戲的卡片牌組
    vector<Player *> players; // 儲存player的ptr
    
    
public:
    
    // 將 Player 加入 players 向量
    void addPlayer(Player *player);

    // 建立players的vector
    void addPlayers();
    
    // Size of players
    int getNumPlayers() const;
    
    // 讓玩家輸入Y以繼續遊戲
    void enterYtoContinue() const;

    // 進行初始發牌，每位玩家隨機抽兩張卡
    void initialDeal();

    // 顯示每位玩家的名稱和角色類型
    void showPlayersNameAndChr() const;

    // 顯示每位玩家的手牌
    void showPlayersHands() const;

    // 判斷player的type，呼叫該type的move
    void playerMove(Player *player);
    
    // Enemy move
    void enemyMove(Enemy *enemy);
    
    // Seeker的move
    void seekerMove(Seeker *seeker);
    
    // Targetor的move
    void targetorMove(Targetor *Targetor);
    
    // 回傳player的pointer
    Player *getPlayerAtIndex(int index);

    // destructor
    ~Game();
};





// Record class -------------------------------------
class Record
{
private:
    string playerName;  // 玩家名稱
    string filePath;    // 檔案路徑
    int maxCards;       // 單局最高手牌數
    int maxPoints;      // 單局最高點數
    int maxWinStreak;   // 最高連勝局數
public:
    Record(string playerName);  // 建構函式（輸入玩家名稱）
    void updateRecord(int cardCnt, int pointCnt, int winStreak);  // 更新玩家檔案
    void print() const;  // 列印玩家檔案
};

