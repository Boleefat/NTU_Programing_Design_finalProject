#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <fstream>
#include "BlackJack.h"
using namespace std;

// Card struct -----------------------------------

// 建構函式
Card::Card(const string &s, const string &r, int v) : suit(s), rank(r), value(v), isInDeck(true) {}




// SkillCounter struct ----------------------------

// 建構函式
SkillCounter::SkillCounter() : isSkillOneAvailable(true), isSkillTwoAvailable(true), isSkillThreeAvailable(true) {}




/* Game class --------------------------------------
隨著遊戲進行，會陸續新增敵人、物品，玩家也會跟這些敵人和物品互動 */


// Size of players
int Game::getNumPlayers() const
{
    return players.size();
}


// 將 Player 加入 players 向量
void Game::addPlayer(Player *player)
{
    players.push_back(player);
}


// 建立players的vector
void Game::addPlayers()
{
    // 詢問玩家數量
    int numPlayers;
    while (true)
    {
        try
        {
            cout << "Enter the number of players (Only 2 currently): ";
            cin >> numPlayers;
            
            if (cin.fail() || numPlayers < 2 || numPlayers > 2)
            {
                throw out_of_range("Invalid input. Please enter 2.");
            }
            
            break;
        }
        catch (const out_of_range &e)
        {
            cout << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    this->characterIntro();
    
    // 動態建立玩家，加入遊戲
    for (int i = 0; i < numPlayers; ++i)
    {
        string playerName;
        string playerCharacter;
        
        cout << "Enter name for Player " << i + 1 << ": ";
        cin >> playerName;
        
        while (true)
        {
            cout << "Choose character for Player " << i + 1 << endl
            << " (Enter S to choose Seeker. Enter T to choose Targetor): ";
            cin >> playerCharacter;
            
            try
            {
                Player *newPlayer = nullptr;
                
                if (playerCharacter == "S")
                {
                    newPlayer = new Seeker(playerName);
                }
                else if (playerCharacter == "T")
                {
                    newPlayer = new Targetor(playerName);
                }
                else
                {
                    throw invalid_argument("Invalid player class");
                }
                
                // 將玩家加入遊戲
                players.push_back(newPlayer);
                
                break; // 跳出無窮迴圈，因為輸入正確
            }
            catch (const invalid_argument &e)
            {
                cout << e.what() << " Please try again." << endl;
            }
        }
    }
}


// 印出角色介紹
void Game::characterIntro() const
{
    ifstream intro("./characters/intro.txt");
    if(intro)
    {
        cout << "Here's the introduction of the characters:" << endl
        << "------------------------------";
        char line[100];
        while(!intro.eof())
        {
            intro.getline(line,100);
            cout << endl << line;
        }
    }
    intro.close();
    cout << "**************************************************" << endl;
}


// 進行初始發牌，每位玩家隨機抽兩張卡
void Game::initialDeal()
{
    cout << "Game Begins!" << endl;
    for (Player *player : players)
    {
        // 每位玩家抽兩張卡
        Card *drawnCard1 = gameDeck.drawOneCard();
        Card *drawnCard2 = gameDeck.drawOneCard();
        // 將抽到的卡片加入玩家手牌
        player->addSpecificCard(drawnCard1);
        player->addSpecificCard(drawnCard2);
    }
}


// 顯示每位玩家的名稱和角色類型
void Game::showPlayersNameAndChr() const
{
    cout << "Players Information:" << endl;
    for (const Player *player : players)
    {
        cout << "Name: " << player->getName() << ", Character: ";

        // 使用 dynamic_cast 來檢查實際型別
        const Seeker *seeker = dynamic_cast<const Seeker *>(player);
        const Targetor *targetor = dynamic_cast<const Targetor *>(player);

        if (seeker != nullptr)
        {
            cout << "Seeker";
        }
        else if (targetor != nullptr)
        {
            cout << "Targetor";
        }
        else
        {
            cout << "Unknown Character";
        }
        cout << endl;
    }
    this->enterYtoContinue();
}


// 顯示每位玩家的手牌
void Game::showPlayersHands() const
{
    for (const Player *player : players)
    {
        cout << "Name: " << player->getName() << "'s hand:" << endl;
        player->showHand();
        cout << endl;
    }
}


// 判斷player的type，呼叫該type的move
void Game::playerMove(Player *player)
{
    if (player == nullptr)
    {
        cout << "Invalid player." << endl;
        return;
    }

    // Check the player's type using dynamic_cast
    Seeker *seeker = dynamic_cast<Seeker *>(player);
    Targetor *targetor = dynamic_cast<Targetor *>(player);
    Enemy *enemy = dynamic_cast<Enemy *>(player);

    if (seeker != nullptr)
    {
        seekerMove(seeker);
    }
    else if (targetor != nullptr)
    {
        targetorMove(targetor);
    }
    else if (enemy != nullptr)
    {
        enemyMove(enemy);
    }
    else
    {
        cout << "Unknown Character" << endl;
    }
}


// Enemy move
void Game::enemyMove(Enemy *enemy)
{
    enemy->randomlyAddOneCard(gameDeck);
}


// Seeker的move
void Game::seekerMove(Seeker *seeker)
{
    cout << endl;
    cout << "Player's turn: " << seeker->getName() << ", Character: Seeker" << endl;
    cout << "Current hand: " << endl;
    seeker->showHand();

    char move;
    while (true)
    {
        cout << "Enter your move (A: Add a card, S: Use skill, N: Do nothing): ";
        cin >> move;

        if (move == 'A' || move == 'S' || move == 'N')
        {
            break; // 輸入正確，跳出迴圈
        }
        else
        {
            cout << "Invalid move. Please enter A, S, or N." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    switch (move)
    {
    case 'A':
        seeker->randomlyAddOneCard(gameDeck);
        cout << "Adding a card..." << endl;
        break;
    case 'S':
        int skillChoice;
        while (true)
        {
            cout << "Choose a skill (1: Skill One, 2: Skill Two): ";
            cin >> skillChoice;

            if (skillChoice == 1 || skillChoice == 2)
            {
                break; // 輸入正確，跳出迴圈
            }
            else
            {
                cout << "Invalid skill choice. Please enter 1 or 2." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        // 使用技能
        switch (skillChoice)
        {
        case 1:
        {
            // 展示玩家資訊
            showPlayersNameAndChr();

            // 讓使用者輸入要尋找的玩家名稱
            cout << "Enter the name of the player to seek: ";
            string playerNameToSeek;
            cin >> playerNameToSeek;

            // 在 players 中尋找玩家指標
            Player *targetPlayer = nullptr;
            for (Player *p : players)
            {
                if (p->getName() == playerNameToSeek)
                {
                    targetPlayer = p;
                    break;
                }
            }

            // 如果找到玩家，執行技能
            if (targetPlayer != nullptr)
            {
                seeker->seekAnotherPlayer(targetPlayer);
            }
            else
            {
                cout << "Player not found." << endl;
            }
            break; // 這裡加上 break
        }
        case 2:
            seeker->seekDeck(gameDeck);
            break;
        }
        break;
    case 'N':
        // 不做任何事
        cout << "Doing nothing..." << endl;
        break;
    }

    seeker->showHand();
    cout << "\n";
}


// Targetor的move
void Game::targetorMove(Targetor *Targetor)
{
    // Implement Targetor's move logic here
}


// 回傳player的pointer
Player *Game::getPlayerAtIndex(int index)
{
    if (index >= 0 && index < players.size())
    {
        return players[index];
    }
    else
    {
        // 处理索引越界的情况，这里返回nullptr，你可以根据需要修改
        return nullptr;
    }
}


// 讓玩家輸入Y以繼續遊戲
void Game::enterYtoContinue() const
{
    char enter = 'N';
    while(enter != 'Y')
    {
        cout << "If you're ready, enter Y to continue: ";
        cin >> enter;
    }
    cout << "------------------------------" << endl;
}


// destructor
Game::~Game()
{
    for (Player *player : players)
    {
        delete player;
    }
}





/* Player class, 是一個pure abstract class --------------------------
 是遊戲中所有玩家（包含敵我）的基礎類別，裡面定義了所有玩家都需要有的屬性和行為 */

// 建構函式
Player::Player(const string &playerName) : name(playerName), skillCounter{}, record(nullptr)
{
    
}

// 比較運算子 <
bool Player::operator<(const Player &other) const
{
    if (calculateHandValue() == other.calculateHandValue())
    {
        // 如果手牌值相同，比較牌數
        return hand.size() < other.hand.size();
    }
    // 手牌值不同，直接比較手牌值
    return calculateHandValue() < other.calculateHandValue();
}


// 比較運算子 >
bool Player::operator>(const Player &other) const
{
    if (calculateHandValue() == other.calculateHandValue())
    {
        // 如果手牌值相同，比較牌數
        return hand.size() > other.hand.size();
    }
    // 手牌值不同，直接比較手牌值
    return calculateHandValue() > other.calculateHandValue();
}


// 計算手牌總值
int Player::calculateHandValue() const
{
    int totalValue = 0;
    int numAces = 0;

    for (const auto &card : hand)
    {
        totalValue += card.value;

        // 如果是 A，累計 A 的數量
        if (card.rank == "A" && card.suit == "Spades")
        {
            numAces++;
        }
    }

    // 處理 A 的情況，使其適應不同的情境
    while (numAces > 0 && totalValue > 21)
    {
        totalValue -= 10;
        numAces--;
    }

    return totalValue;
}


// 顯示手牌
void Player::showHand() const
{
    cout << "------------------------------" << endl
    << name << "'s hands:" << endl;
    
    for (const auto &card : hand)
    {
        cout << card.suit << "-" << card.rank << ", value : " << card.value << endl;
    }
    
    cout << "Total card value: " << calculateHandValue() << endl
    << "------------------------------" << endl;
}


// 隨機添加一張牌到手牌
void Player::randomlyAddOneCard(CardDeck &deck)
{
    Card *drawnCard = deck.drawOneCard(); // 從卡組中抽一張牌的指標
    if (drawnCard != nullptr)
    {
        hand.push_back(*drawnCard); // 添加該牌到玩家手牌中 ////注意這邊是shallow copy
        cout << name << " get one card: " << drawnCard->suit << "-" << drawnCard->rank << ", value: " << drawnCard->value << endl;
    }
    else
    {
        cout << "CardDeck is empty." << endl;
    }
}


// 在 Player 類別中新增函式，將指定的牌添加到手牌
void Player::addSpecificCard(Card *card)
{
    if (card != nullptr)
    {
        hand.push_back(*card);
    }
}


// 取得玩家名稱
string Player::getName() const
{
    return this->name;
}


// 取得手牌
vector<Card> &Player::getHand()
{
    return hand;
}





/* Seeker class, 繼承 Player 這個類別 ------------------
是玩遊戲的人可以選擇的我方角色，定義所有Seeker的共同屬性 */


// Constructor for Seeker
Seeker::Seeker(const string &playerName) : Player(playerName)
{
    this->record = new Record(playerName);
}

// skillOne: Seeker uses a special ability to seek another player's hand
void Seeker::seekAnotherPlayer(Player *targetPlayer)
{
    // Check if skillOne is available
    if (skillCounter.isSkillOneAvailable)
    {
        // Set skillOne to false after using the special ability
        skillCounter.isSkillOneAvailable = false;
        cout << getName() << " seeks the hand of " << targetPlayer->getName() << ":" << endl;
        targetPlayer->showHand();
        cout << endl;
    }
    else
    {
        // Print a message if the skill is not available
        cout << "You have used this skill." << endl;
    }
}

// skillTwo: Seeker uses a special ability to seek the deck
void Seeker::seekDeck(CardDeck &deck)
{
    // Check if skillOne is available
    if (skillCounter.isSkillTwoAvailable)
    {
        // Set skillOne to false after using the special ability
        skillCounter.isSkillTwoAvailable = false;
        cout << this->name << " seeks the Deck " << endl;
        deck.displayDeck();
    }
    else
    {
        // Print a message if the skill is not available
        cout << "You have used this skill." << endl;
    }
}




/* Targetor class, 繼承 Player 這個類別 ------------------
是玩遊戲的人可以選擇的我方角色，定義所有Targetor的共同屬性 */

// Constructor for Targetor
Targetor::Targetor(const string &playerName) : Player(playerName)
{
    this->record = new Record(playerName);
}

// skillOne
void Targetor::takeSpecificCard(CardDeck &deck, const string &suit, const string &rank)
{
    if (this->skillCounter.isSkillOneAvailable)
    {
        // 使用 Deck 的 specificCard 方法尋找特定卡片
        Card *specificCard = deck.specificCard(suit, rank);

        // 如果找到卡片
        if (specificCard != nullptr)
        {
            // 將卡片的 isInDeck 設為 false
            specificCard->isInDeck = false;

            // 將卡片加入 Targetor 的手牌中
            hand.push_back(*specificCard);

            // 輸出訊息
            cout << "Found and added card: " << specificCard->suit << "-" << specificCard->rank << ", value: " << specificCard->value << " to " << getName() << "'s hand." << endl;
        }
        else
        {
            // 如果卡片為 nullptr，表示卡片在其他玩家手中
            cout << "The card is in someone else's hand." << endl;
        }
    }
    else
    {
        cout << "You have used this skill." << endl;
    }
}

// skillTwo
void Targetor::discardCard(Player *targetPlayer, int value)
{
    if (this->skillCounter.isSkillTwoAvailable)
    {
        // 使用 getHand 方法獲取目標玩家的手牌
        vector<Card> &targetHand = targetPlayer->getHand();

        // 使用標準函式 find_if 找到符合條件的卡片
        auto it = std::find_if(targetHand.begin(), targetHand.end(), [&](const Card &card)
                               { return card.value == value; });

        // 檢查是否找到卡片
        if (it != targetHand.end())
        {
            // 從玩家手牌中刪除該卡片
            targetHand.erase(it);

            // 輸出訊息
            cout << "Discarded card with value " << value << " from " << targetPlayer->getName() << "'s hand." << endl;

            this->skillCounter.isSkillTwoAvailable = false;
        }
        else
        {
            // 如果卡片不在手中，輸出相應訊息
            cout << "Discard fail, you can use it again." << endl;
        }
    }
    else
    {
        cout << "You have used this skill." << endl;
    }
}





/* Enemy class, 繼承 Player 這個類別 --------------------------------
 是由電腦自動操作的敵方角色，定義所有Enemy的共同屬性 */

// Constructor for Enemy
Enemy::Enemy(const string &enemyName) : Player(enemyName) {}



/* Item class 道具卡 ------------------------------------------------
道具卡可以被玩家獲得、儲存、使用，定義所有道具卡的共同屬性 */

// 建構函式
Item::Item(const string &itemName, Player*& owner)
{
    this->name = itemName;
    this->owner = owner;
}

// 取得道具卡名稱
string Item::getName () const { return this->name; }


// 道具卡 1：和另一位玩家交換一張牌
void SwitchCard::useItem(Player*& target, CardDeck &deck)
{
    
}


// 道具卡 2：重新抽一張牌, 並選擇是否拿來換掉自己的一張牌
void ReDrawCard::useItem(Player*& target, CardDeck &deck)
{
    owner->randomlyAddOneCard(deck);
    cout << "Do you want to switch this card with one of your cards on hand?" << endl;
    while(true)
    {
        cout << "(Enter Y for Yes. Enter N for No.): ";
        char enter = 'N';
        cin >> enter;
        if(enter == 'Y')
        {
            string rank = "NULL", suit = "NULL";
            cout << "Enter the RANK of the card that you DON'T want: ";
            cin >> rank;
            cout << "Enter the SUIT of the card that you DON'T want: ";
            cin >> suit;

            break;
        }
        else if(enter == 'N')
        {
            cout << owner->getName() << " did not switch his cards.";
            break;
        }
    }
}


// 道具卡 3：捨棄自己的一張牌
void DisCard::useItem(Player*& target, CardDeck &deck)
{
    
}


// CardDeck class ------------------------------------

// 建構函式
CardDeck::CardDeck()
{
    initializeCards();
    // Use the current time as a seed for the random number generator
    randomGenerator = std::mt19937(static_cast<unsigned>(std::time(0)));
}


// 取得卡片數量
int CardDeck::getDeckSize() const
{
    return cards.size();
}


// 顯示卡組內容
void CardDeck::displayDeck() const
{
    for (const auto &card : cards)
    {
        if (card.isInDeck)
        {
            std::cout << "Card: " << card.suit << "-" << card.rank << ", Value: " << card.value << std::endl;
        }
        else
        {
            std::cout << "Card: " << card.suit << "-" << card.rank << ", Value: " << card.value << " (out)" << std::endl;
        }
    }
}


// 從卡組中抽一張牌
Card *CardDeck::drawOneCard()
{
    if (!isEmpty())
    {
        std::uniform_int_distribution<int> dis(0, cards.size() - 1);

        // 使用迴圈確保一直抽牌，直到抽到一張在卡組中的牌
        while (true)
        {
            int randomIndex = dis(randomGenerator);
            Card *drawnCard = &cards[randomIndex];

            if (drawnCard->isInDeck)
            {
                drawnCard->isInDeck = false;
                return drawnCard;
            }
        }
    }
    else
    {
        cout << "CardDeck is empty" << endl;
        // 返回空指標表示卡組已空
        return nullptr;
    }
}


// 根據 suit 和 rank 尋找特定的 Card 並返回指標，如果 isInDeck 為 false 則輸出訊息
Card *CardDeck::specificCard(const string &suit, const string &rank)
{
    // 使用標準函式 find_if 找到符合條件的卡片
    auto it = std::find_if(cards.begin(), cards.end(), [&](const Card &card)
                           { return (card.suit == suit) && (card.rank == rank); });

    // 檢查是否找到卡片
    if (it != cards.end())
    {
        // 如果 isInDeck 為 false，輸出相應訊息
        if (!it->isInDeck)
        {
            return nullptr;
        }

        // 返回卡片的指標
        return &(*it);
    }
    else
    {
        // 若未找到，返回空指標
        return nullptr;
    }
}


// 檢查卡組是否為空
bool CardDeck::isEmpty() const
{
    return cards.empty();
}


// 初始化卡片
void CardDeck::initializeCards()
{
    const vector<string> suits = {"Spades", "Hearts", "Diamonds", "Clubs"};
    const vector<string> ranks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    // 清空原有的卡片向量
    cards.clear();

    for (const auto &suit : suits)
    {
        for (size_t i = 0; i < ranks.size(); ++i)
        {
            int value = (i >= 10) ? 10 : i + 1; // J, Q, K 的 value 都是 10
            cards.push_back(Card(suit, ranks[i], value));
        }
    }
    
    // 重新將所有卡片設為在卡組中
    for (auto &card : cards)
    {
        card.isInDeck = true;
    }
}






// Record class -------------------------------------

// 建構函式
Record::Record(string playerName)
{
    // 注意：檔名不分大小寫
    this->playerName = playerName;
    this->filePath = "./records/" + playerName + ".txt";
    
    cout
    << "Searching for "+ playerName +"'s game record..." << endl;
    
    ifstream searchFile(filePath);
    if (searchFile)
    {
        cout << "- We found "+ playerName +"'s game record file!" << endl;
        searchFile >> maxCards >> maxPoints >> maxWinStreak;
    }
    else
    {
        cout << "- "+ playerName + "'s game record file does NOT exist." << endl
        << "- Creating new game record file for "+ playerName +"..." << endl;
        maxCards = 0;
        maxPoints = 0;
        maxWinStreak = 0;
        ofstream newFile(filePath);
        if (!newFile)
            cout << "ERROR! Failed to create file!" << endl;
        else
        {
            newFile << maxCards << " " << maxPoints << " " << maxWinStreak;
        }
        newFile.close();
    }
    searchFile.close();
    cout << "**************************************************" << endl;
}


// 更新玩家檔案
void Record::updateRecord(int cardCnt, int pointCnt, int winStreak)
{
    if (cardCnt > maxCards || pointCnt > maxPoints || winStreak > maxWinStreak)
    {
        ofstream recordFile(filePath, ios::trunc);
        if (!recordFile)
            cout << "ERROR! Failed to update record!" << endl;
        else
        {
            if (cardCnt > maxCards)
                this->maxCards = cardCnt;
            if (pointCnt > maxPoints)
                this->maxPoints = pointCnt;
            if (winStreak > maxWinStreak)
                this->maxWinStreak = winStreak;
            cout << "Concrats!" << playerName << "break his/her record!" << endl;
            this->print();
            recordFile << cardCnt << " "
                       << pointCnt << " "
                       << winStreak;
        }
        recordFile.close();
    }
    else
        cout << playerName << "did not break his/her record." << endl;
}


// 列印玩家檔案
void Record::print() const
{
    cout
    << "Max cards per round: " << maxCards << endl
    << "Max points per round: " << maxPoints << endl
    << "Max win streaks: " << maxWinStreak << endl;
         
}
