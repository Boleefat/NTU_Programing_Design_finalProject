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

class Player;    // 必須是一個純粹抽象類別（pure abstract class）
class Seeker;    // 由玩家操作的Character, 繼承自Player
class Targetor;  // 由玩家操作的Character, 繼承自Player
class Enemy;     // 由電腦操作的莊家, 繼承自Player

class Item;  // 道具卡 目前有3種
class RandomSwitch;
class DrawOneFoldOne;
class BothFoldOne;

class Game;
class CardDeck;
class Record;








// Card struct -------------------------------------------
struct Card
{
    string suit;  // 花色
    string rank;  // 牌面數字
    int value;    // 換算點數
    bool isInDeck;

    // 建構函式
    Card(const string &s, const string &r, int v);
    
    // 印出Card資訊
    void print() const;
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




/* Player class, 是一個pure abstract class ------------------------------
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

    string getName() const;

    vector<Card> &getHand();

    void showHand() const;

    int calculateHandValue() const;
    
    bool operator<(const Player &other) const;

    bool operator>(const Player &other) const;

    void randomlyAddOneCard(CardDeck &deck);

    void addSpecificCard(Card *card);
};





/* Seeker class, 繼承 Player 這個類別 ----------------------------
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




/* Targetor class, 繼承 Player 這個類別 -----------------------
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




/* Enemy class, 繼承 Player 這個類別 ---------------------------
 是由電腦自動操作的敵方角色，定義所有Enemy的共同屬性 */

class Enemy : public Player
{
public:
    // Constructor for Enemy
    Enemy(const string &enemyName);
};




// CardDeck class -------------------------------------------
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





/* Item class 道具卡 ---------------------------------------------
道具卡可以被玩家獲得、儲存、使用，定義所有道具卡的共同屬性 */

class Item
{
protected:
    string name;    // 道具卡名稱
    Player* owner;  // 擁有者
    CardDeck deck;  // 所屬牌組
    
public:
    Item(Player*& owner, CardDeck& deck);
    string getName () const;
    virtual void useItem(Player*& target) = 0;
    void foldOneCard(Player*& target);
    int chooseHandCard(Player*& target);
};


// 道具卡 1：和另一位玩家隨機交換一張牌
class RandomSwitch: public Item
{
public:
    RandomSwitch(Player*& owner, CardDeck& deck);
    void useItem(Player*& target);
    int generateRandomNumber(int small, int large);
};


// 道具卡 2：重新抽一張牌, 棄掉自己一張牌
class DrawOneFoldOne: public Item
{
public:
    DrawOneFoldOne(Player*& owner, CardDeck& deck);
    void useItem(Player*& target);
};


// 道具卡 3：雙方各棄掉一張牌
class BothFoldOne: public Item
{
public:
    BothFoldOne(Player*& owner, CardDeck& deck);
    void useItem(Player*& target);
};



/* Game class --------------------------------------------------
隨著遊戲進行，會陸續新增敵人、物品，玩家也會跟這些敵人和物品互動 */

class Game
{
private:
    CardDeck gameDeck;        // 遊戲的卡片牌組
    vector<Player*> players; // 儲存Player的ptr
    vector<Item*> items ;    // 儲存Item的ptr
    
public:
    
    // 將 Item 加入遊戲給玩家
    void addItem(Player*& player);
    
    // 將 Player 加入 players的vector
    void addPlayer(Player* player);

    // 建立players的vector
    void addPlayers();
    
    // Size of players
    int getNumPlayers() const;
    
    // 讓玩家輸入Y以繼續遊戲
    void enterYtoContinue() const;
    
    // 印出介紹
    void intro(const string) const;

    // 進行初始發牌，每位玩家隨機抽兩張卡
    void initialDeal();

    // 顯示每位玩家的名稱和角色類型
    void showPlayersNameAndChr() const;

    // 顯示每位玩家的手牌
    void showPlayersHands() const;

    // 判斷player的type，呼叫該type的move
    void playerMove(Player *player);
    
    // Seeker的move
    void seekerMove(Seeker *seeker);
    
    // Targetor的move
    void targetorMove(Targetor *Targetor);

    //判斷player的type，呼叫該type的抽卡行動
    void playerDraw(Player *player, int &temp)

    //Seeker抽卡
    void seekerDraw(Seeker *seeker, int &temp)
    
    // Targetor抽卡
    void targetorDraw(Targetor *targetor, int &temp)

    //enemy 抽卡
 	void enemyDraw(Enemy *enemy)
    
    // 回傳player的pointer
    Player *getPlayerAtIndex(int index);

    // destructor
    ~Game();
};





// Record class --------------------------------------------
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
