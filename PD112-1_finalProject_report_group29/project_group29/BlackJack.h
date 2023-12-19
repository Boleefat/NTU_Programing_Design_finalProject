#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <fstream>
using namespace std;

// All structs ---------------------------------------------------------

struct Card; // 撲克牌
struct SkillCounter;  // 玩家技能使用狀態

// All classes ---------------------------------------------------------

class Player;    // 是一個純粹抽象類別
class Seeker;    // 由玩家操作的Character, 繼承自Player
class Targetor;  // 由玩家操作的Character, 繼承自Player
class Enemy;     // 由電腦操作的莊家, 繼承自Player

class Item;  // 道具卡 目前有3種
class RandomSwitch;
class DrawOneFoldOne;
class BothFoldOne;

class Game;      // 牌局
class CardDeck;  // 牌組
class Record;    // 玩家記錄檔





// Card struct ---------------------------------------------------------------
// 單張的撲克牌
struct Card
{
    string suit;    // 牌面花色
    string rank;    // 牌面數字
    int value;      // 換算點數
    bool isInDeck;  // 是否在牌組裡

    // 建構函式
    Card(const string &s, const string &r, int v);
    
    // 印出Card資訊
    void print() const;
};


// SkillCounter struct --------------------------------------------------------
// 玩家技能使用狀態
struct SkillCounter
{
    bool isSkillOneAvailable;
    bool isSkillTwoAvailable;
    bool isSkillThreeAvailable;

    // 建構函式
    SkillCounter();
};




// Player class, 是一個 pure abstract class ---------------------------------------------------------------------------------------------
// 是遊戲中所有玩家（包含敵我）的基礎類別，裡面定義了所有玩家都需要有的屬性和行為
class Player
{
protected:
    string name;        // 玩家名稱
    vector<Card> hand;  // 玩家手牌
    Record* record;     // 玩家記錄檔
    SkillCounter skillCounter;   // 技能狀態
    
public:
    // constructors and destructors
    Player(const string &playerName);
    virtual ~Player() = default;

    // 取得玩家姓名
    string getName() const;

    // 取得玩家記錄檔
    Record*& getRecord();
    
    // 取得玩家手牌
    vector<Card> &getHand();
    vector<Card> getHand() const;
    
    // 印出玩家手牌
    void showHand() const;

    // 計算手牌點數
    int calculateHandValue() const;
    
    // 隨機加一張牌
    void randomlyAddOneCard(CardDeck &deck);

    // 加一張指定的牌
    void addSpecificCard(Card *card);
    
    // 運算子多載
    bool operator<(const Player &other) const;
    bool operator>(const Player &other) const;
    bool operator>=(const Player& other) const;
    bool operator<=(const Player& other) const;
    bool operator==(const Player& other) const;
    
    // 玩家行動
    virtual void playerMove(CardDeck& gameDeck, Game& game) = 0;
    
    // 玩家抽牌
    virtual void playerDraw(int &temp, CardDeck& gameDeck) = 0;
};


// Seeker class, 繼承 Player 這個類別 --------------------------------------------------------------------------------------
// 是玩遊戲的人可以選擇的我方角色，定義所有 Seeker 的共同屬性

class Seeker : public Player
{
public:
    // inherited functions
    Seeker(const string &playerName);
    virtual ~Seeker() = default;
    void playerDraw(int &temp, CardDeck& gameDeck);
    void playerMove(CardDeck& gameDeck, Game& game);

    // skillOne: Seeker uses a special ability to seek another player's hand
    void seekAnotherPlayer(Player *targetPlayer);

    // skillTwo: Seeker uses a special ability to seek the deck
    void seekDeck(CardDeck &deck);
    
};


// Targetor class, 繼承 Player 這個類別 ------------------------------------------------------------------------------------
// 是玩遊戲的人可以選擇的我方角色，定義所有Targetor的共同屬性

class Targetor : public Player
{
public:
    // inherited functions
    Targetor(const string &playerName);
    virtual ~Targetor() = default;
    void playerDraw(int &temp, CardDeck& gameDeck);
    void playerMove(CardDeck& gameDeck, Game& game);

    // skillOne
    void takeSpecificCard(CardDeck &deck, const string &suit, const string &rank);

    // skillTwo
    void discardCard(Player *targetPlayer, int value);

};

// Enemy class, 繼承 Player 這個類別 ----------------------------------------------------------------------------------------
// 是由電腦自動操作的敵方角色，定義所有 Enemy 的共同屬性

class Enemy : public Player
{
public:
    // inherited functions
    Enemy(const string &enemyName);
    virtual ~Enemy() = default;
    void playerDraw(int &temp, CardDeck& gameDeck);
    void playerMove(CardDeck& gameDeck, Game& game);
};







// CardDeck class, 牌組 ------------------------------------------------------------------------------------------------
class CardDeck
{
private:
    vector<Card> cards;           // 存放卡片的vector
    std::mt19937 randomGenerator; // 隨機數生成器
    
public:
    // 建構函式並同時初始化 52 張卡片
    CardDeck();
    
    // 初始化卡片
    void initializeCards();

    // 取得卡片數量
    int getDeckSize() const;

    // 顯示卡組內容
    void displayDeck() const;

    // 從卡組中抽一張牌
    Card *drawOneCard();

    // 根據 suit 和 rank 尋找特定的 Card 並返回指標，如果 isInDeck 為 false 則輸出訊息
    Card *specificCard(const string &suit, const string &rank, bool globalSearch);

    // 檢查卡組是否為空
    bool isEmpty() const;
};





// Item class 道具卡 ------------------------------------------------------------------------------------------------------------
// 道具卡可以被玩家獲得、儲存、使用，定義所有道具卡的共同屬性
class Item
{
protected:
    string name;    // 道具卡名稱
    Player* owner;  // 擁有者
    CardDeck deck;  // 使用的牌組
    
public:
    // 建構函式
    Item(Player*& owner, CardDeck& deck);
    
    // 取得道具卡名稱
    string getName () const;
    
    // 使用道具（設定為純虛擬函式）
    virtual void useItem(Player*& theOtherPlayer, Game& game) = 0;
    
    // 詢問玩家要捨棄哪張牌
    void foldOneCard(Game& game, Player*& target);
};


// 道具卡 1：和另一位玩家隨機交換一張牌
class RandomSwitch: public Item
{
public:
    // inherited functions
    RandomSwitch(Player*& owner, CardDeck& deck);
    void useItem(Player*& theOtherPlayer, Game& game);
    
    // 隨機亂數產生器
    int generateRandomNumber(int small, int large);
};


// 道具卡 2：重新抽一張牌, 棄掉自己一張牌
class DrawOneFoldOne: public Item
{
public:
    // inherited functions
    DrawOneFoldOne(Player*& owner, CardDeck& deck);
    void useItem(Player*& theOtherPlayer, Game& game);
};


// 道具卡 3：雙方各棄掉一張牌
class BothFoldOne: public Item
{
public:
    // inherited functions
    BothFoldOne(Player*& owner, CardDeck& deck);
    void useItem(Player*& theOtherPlayer, Game& game);
};



// Game class, 牌局 ------------------------------------------------------------------------------------------------------------
// 隨著遊戲進行，會陸續新增敵人、物品，玩家也會跟這些敵人和物品互動

class Game
{
private:
    CardDeck gameDeck;        // 牌局的牌組
    vector<Player*> players;  // 儲存 Player 的ptr
    vector<Item*> items ;     // 儲存 Item 的ptr
    
public:
    
    // 將 Item 加入遊戲給Player
    void addItem(Player*& player);
    
    // 將 Player 加入players的vector
    void addPlayer(Player* player);

    // 建立 players 的vector
    void addPlayers();
    vector<Player*>& getPlayers();
    
    // Size of players
    int getNumPlayers() const;
    
    // 讓玩家輸入Y以繼續遊戲
    void enterYtoContinue() const;
    
    // 印出介紹文
    void printIntro(const string intro) const;
    
    // 印出分隔線
    void printLong() const;
    
    // 印出大標題
    void printStage(string stage) const;
    
    // 進行初始發牌，每位玩家隨機抽兩張卡
    void initialDeal();

    // 顯示每位玩家的名稱和角色類型
    void showPlayersNameAndChr() const;
    
    // 回傳 Player 的pointer
    Player*& getPlayerAtIndex(int index);
    
    // 判斷勝負
    void result(bool whoWins[]);
    
    // 21點牌局
    void drawRound();
    
    // 道具卡回合
    void itemRound(bool whoWins[], bool& play);
    
    // 詢問是否再玩一次遊戲
    bool playAgain();

};





// Record class 記錄玩家戰績 ----------------------------------------------------------------------------------------
class Record
{
private:
    string playerName;  // 玩家名稱
    string filePath;    // 檔案路徑
    int maxCards;       // 最高手牌數
    int maxPoints;      // 最高點數
    int currentWins;    // 連勝局數
    int totalWins;      // 總勝場
public:
    Record(string playerName);  // 建構函式
    void print() const;  // 列印玩家檔案
    void updateRecord(bool winsToday, Player*& player);  // 更新玩家檔案
    
};
