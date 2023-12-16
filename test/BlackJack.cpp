#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <stdexcept>
#include "BlackJack.h"
using namespace std;

// Card struct ------------------------------------------------------------------------------------------------------------

// 建構函式
Card::Card(const string &s, const string &r, int v) : suit(s), rank(r), value(v), isInDeck(true) {}

// 印出Card資訊
void Card::print() const
{
    cout << suit << "-" << rank << ", value: " << value << endl;
}




// SkillCounter struct ------------------------------------------------------------------------------------------------------------

// 建構函式
SkillCounter::SkillCounter() : isSkillOneAvailable(true), isSkillTwoAvailable(true), isSkillThreeAvailable(true) {}



// Player class, 是一個 pure abstract class ----------------------------------------------------------------------------------------
// 是遊戲中所有玩家（包含敵我）的基礎類別，裡面定義了所有玩家都需要有的屬性和行為
// 建構函式
Player::Player(const string &playerName) : name(playerName), skillCounter{}, record(nullptr) {}


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

    /*
    while (numAces > 0 && totalValue > 21)
    {
        totalValue -= 10;
        numAces--;
    }
     */

    return totalValue;
}

// 運算子多載
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
// 比較運算子 >=
bool Player::operator>=(const Player &other) const
{
    if (calculateHandValue() == other.calculateHandValue())
    {
        // 如果手牌值相同，比較牌數
        return hand.size() >= other.hand.size();
    }
    // 手牌值不同，直接比較手牌值
    return calculateHandValue() >= other.calculateHandValue();
}
// 比較運算子 <=
bool Player::operator<=(const Player &other) const
{
    if (calculateHandValue() == other.calculateHandValue())
    {
        // 如果手牌值相同，比較牌數
        return hand.size() <= other.hand.size();
    }
    // 手牌值不同，直接比較手牌值
    return calculateHandValue() <= other.calculateHandValue();
}
// 比較運算子 ==
bool Player::operator==(const Player &other) const
{
    // 同時比較手牌值和牌數
    return (calculateHandValue() == other.calculateHandValue()) && (hand.size() == other.hand.size());
}


// 顯示手牌
void Player::showHand() const
{
    cout << "------------------------------" << endl
         << name << "'s hands: " << endl;

    for (const auto &card : hand)
    {
        cout << "- " << card.suit << "-" << card.rank << ", value : " << card.value << endl;
    }

    cout << "Total card value: " << calculateHandValue();
    if (calculateHandValue() > 21)
        cout << " (Busted!)";

    cout << endl << endl;;
}


// 隨機添加一張牌到手牌
void Player::randomlyAddOneCard(CardDeck &deck)
{
    Card *drawnCard = deck.drawOneCard(); // 從卡組中抽一張牌的指標
    if (drawnCard != nullptr)
    {
        hand.push_back(*drawnCard); // 添加該牌到玩家手牌中 //// 注意這邊是shallow copy
        cout << "- " + name + " got ONE card: " << drawnCard->suit << "-" << drawnCard->rank << ", value: " << drawnCard->value << endl;
    }
    else
    {
        cout << "CardDeck is empty." << endl;
    }
}

// 將指定的牌添加到手牌
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
vector<Card> Player::getHand() const
{
    return hand;
}

// 取得玩家的記錄檔
Record *&Player::getRecord()
{
    return record;
}


// Seeker class, 繼承 Player 這個類別 -------------------------------------------------------------------------------------
// 是玩遊戲的人可以選擇的我方角色，定義所有Seeker的共同屬性

// Constructor for Seeker
Seeker::Seeker(const string &playerName) : Player(playerName)
{
    // 建立玩家記錄檔
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
        cout << getName() << " seeks the hands of " << targetPlayer->getName() << "..." << endl;
        targetPlayer->showHand();
        
    }
    else
    {
        // Print a message if the skill is not available
        cout << "- You have used this skill." << endl;
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
        cout << this->name << " seeks the Deck..." << endl;
        deck.displayDeck();
    }
    else
    {
        // Print a message if the skill is not available
        cout << "- You have used this skill." << endl;
    }
}

// Seeker的move
void Seeker::playerMove(CardDeck &gameDeck, Game &game)
{
    cout << "Next Player: " << getName() << ", Character: Seeker" << endl;
    game.enterYtoContinue();
    showHand();

    char move;
    while (true)
    {
        cout << "Do you want to use your skill? (1: SeekPlayer, 2: SeekDeck, N: Do Nothing): ";
        cin >> move;

        if (move == '1' || move == '2' || move == 'N')
        {
            break; // 輸入正確，跳出迴圈
        }
        else
        {
            cout << "Invalid move. Please enter 1 or 2 or N." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    switch (move)
    {
        {
        case '1':

            // 展示玩家資訊
            cout << "------------------------------" << endl;
            game.showPlayersNameAndChr();

            // 讓使用者輸入要尋找的玩家名稱
            cout << "Enter the name of the player to seek: ";
            string playerNameToSeek;
            cin >> playerNameToSeek;

            // 在 players 中尋找玩家指標
            Player *targetPlayer = nullptr;
            for (Player *p : game.getPlayers())
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
                seekAnotherPlayer(targetPlayer);
                cout << "Your hands now:" << endl;
                showHand();
            }
            else
            {
                cout << "- Player not found." << endl
                 << "Your hands now:" << endl;
                showHand();
            }
            break; // 這裡加上 break
        }
    case '2':

        seekDeck(gameDeck);
        break;

    case 'N':
        // 不做任何事
        // cout << "Doing nothing..." << endl;
        break;
    }

    // seeker->showHand();
    // cout << "\n";
}

// seeker draw card
void Seeker::playerDraw(int &temp, CardDeck &gameDeck)
{

    if (calculateHandValue() >= 21)
    {
        cout << "- You cannot draw anymore...\n";
        temp = 1;
    }
    else
    {
        char move;
        while (true)
        {
            cout << "Do you want to draw a card? (Y: Draw a card, N: Do nothing): ";
            cin >> move;

            if (move == 'Y')
            {
                randomlyAddOneCard(gameDeck);
                temp = 0;
                if (calculateHandValue() >= 21)
                    temp = 1;
                showHand();
                break; // 輸入正確，跳出迴圈
            }
            else if (move == 'N')
            {
                // cout << "Do nothing\n";
                temp = 1;
                break;
            }
            else
            {
                temp = 0;
                cout << "Invalid move. Please enter Y, N." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }
}


// Targetor class, 繼承 Player 這個類別 -----------------------------------------------------------------------------
// 是玩遊戲的人可以選擇的我方角色，定義所有Targetor的共同屬性
// Constructor for Targetor
Targetor::Targetor(const string &playerName) : Player(playerName)
{
    // 建立玩家記錄檔
    this->record = new Record(playerName);
}

// skillOne
void Targetor::takeSpecificCard(CardDeck &deck, const string &suit, const string &rank)
{
    if (this->skillCounter.isSkillOneAvailable)
    {
        // 使用 Deck 的 specificCard 方法尋找特定卡片
        Card *specificCard = deck.specificCard(suit, rank, false);
        // // 添加調試輸出
        // cout << "After specificCard function call" << endl;
        // // 如果找到卡片
        if (specificCard != nullptr)
        {
            // 將卡片的 isInDeck 設為 false
            specificCard->isInDeck = false;

            // 將卡片加入 Targetor 的手牌中
            hand.push_back(*specificCard);

            // 輸出訊息
            cout << "- "+ getName() +" got ONE card: "<< specificCard->suit << "-" << specificCard->rank << ", value: " << specificCard->value << endl;
        }
        else
        {
            // 如果卡片為 nullptr，表示卡片在其他玩家手中
            cout << "- The card is in someone else's hand." << endl;
        }
    }
    else
    {
        cout << "- You have used this skill." << endl;
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
            cout << "- Destroyed a card with value " << value << " on " << targetPlayer->getName() << "'s hand." << endl;
            targetPlayer->showHand();

            this->skillCounter.isSkillTwoAvailable = false;
        }
        else
        {
            // 如果卡片不在手中，輸出相應訊息
            cout << "- Destroy failed, you can use it again." << endl;
        }
    }
    else
    {
        cout << "- You have used this skill." << endl;
    }
}

// Targetor的move
void Targetor::playerMove(CardDeck &gameDeck, Game &game)
{
    cout << "Next Player: " << getName() << ", Targetor" << endl;
    game.enterYtoContinue();
    showHand();

    char move;
    while (true)
    {
        cout << "Do you want to use your skill? (1: GetCard, 2: DestroyCard, N: Do Nothing): ";
        cin >> move;

        if (move == '1' || move == '2' || move == 'N')
        {
            break; // 輸入正確，跳出迴圈
        }
        else
        {
            cout << "Invalid move. Please enter 1 or 2 or N." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    switch (move)
    {
        {
        case '1':
            char suit;
            string rank;

            // 詢問花色
            while (true)
            {
                // 詢問花色
                std::cout << "- Enter suit (S for Spades, H for Hearts, D for Diamonds, C for Clubs): ";
                std::cin >> suit;
                // 檢查 suit 是否為合法值
                if (suit == 'H' || suit == 'D' || suit == 'C' || suit == 'S')
                {
                    break; // 使用者輸入合法值，跳出迴圈
                }
                else
                {
                    std::cerr << "Invalid suit. Please enter H, D, C, or S." << std::endl;
                }
            }
            // 詢問rank
            while (true)
            {
                // 詢問 rank
                std::cout << "- Enter rank AJQK or 2-10: ";
                std::cin >> rank;
                // 檢查 rank 是否為合法值
                if ((rank == "A" || rank == "J" || rank == "Q" || rank == "K") ||
                    (rank == "2" || rank == "3" || rank == "4" || rank == "5" ||
                     rank == "6" || rank == "7" || rank == "8" || rank == "9" || rank == "10"))
                {
                    break; // 使用者輸入合法值，跳出迴圈
                }
                else
                {

                    std::cerr << "Invalid rank. Please enter AJQK or 2~10." << std::endl;
                }
            }

            // 在這裡處理使用者輸入合法的情況

            switch (suit)
            {
            case 'H':
                takeSpecificCard(gameDeck, "Hearts", (rank));
                break;
            case 'D':

                takeSpecificCard(gameDeck, "Diamonds", (rank));
                break;
            case 'C':

                takeSpecificCard(gameDeck, "Clubs", (rank));
                break;
            case 'S':

                takeSpecificCard(gameDeck, "Spades", (rank));
                break;
            }

            //cout << "You used skill 1." << endl;

            cout << endl << "Your hands now:" << endl;
            showHand();
            
            break;
        }

    case '2':
    {
        // 讓使用者輸入要尋找的玩家名稱
        cout << "------------------------------" << endl;
        game.showPlayersNameAndChr();
        cout << "Enter the Name of the player to Destroy his/her Card: ";
        string playerNameToDiscard;
        cin >> playerNameToDiscard;

        // 在 players 中尋找玩家指標
        Player *targetPlayer = nullptr;
        for (Player *p : game.getPlayers())
        {
            if (p->getName() == playerNameToDiscard)
            {
                targetPlayer = p;
                break;
            }
        }

        // 如果找到玩家，執行技能
        if (targetPlayer != nullptr)
        {
            targetPlayer->showHand();
            int rank;
            while (true)
            {
                std::cout << "Enter the Value of card you want to Destroy(1~13): ";
                std::cin >> rank;
                // 檢查是否為合法值
                if (rank >= 1 && rank <= 10)
                {
                    break; // 使用者輸入合法值，跳出迴圈
                }
                else
                {
                    std::cerr << "Invalid Value. Please enter a Value between 1 and 13." << std::endl;
                }
            }
            discardCard(targetPlayer, rank);
            cout << endl;
            cout << "Your hands now:" << endl;
            showHand();
            
        }
        else
        {
            cout << "Player not found." << endl;
        }
        break; // 這裡加上 break
    }

    case 'N':
        // 不做任何事
        // cout << "Doing nothing..." << endl;
        break;
    }
}

// targetor draw card
void Targetor::playerDraw(int &temp, CardDeck &gameDeck)
{
    // targetor->showHand();
    // cout << targetor->getName() << "'s turn: \n";
    if (calculateHandValue() >= 21)
    {
        cout << "- You cannot draw anymore...\n";
        temp = 1;
    }
    else
    {
        char move;
        while (true)
        {
            cout << "Do you want to draw a card? (Y: Draw a card, N: Do nothing): ";
            cin >> move;

            if (move == 'Y')
            {
                randomlyAddOneCard(gameDeck);
                temp = 0;
                if (calculateHandValue() >= 21)
                    temp = 1;
                showHand();
                break; // 輸入正確，跳出迴圈
            }
            else if (move == 'N')
            {
                // cout << "Do nothing\n";
                temp = 1;
                break;
            }
            else
            {
                temp = 0;
                cout << "Invalid move. Please enter Y, N." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }
}


// Enemy class, 繼承 Player 這個類別 ----------------------------------------------------------------------------------------
//是由電腦自動操作的敵方角色，定義所有Enemy的共同屬性
// Constructor for Enemy
Enemy::Enemy(const string &enemyName) : Player(enemyName) {}
void Enemy::playerMove(CardDeck &gameDeck, Game &game) {}

// enemy Draw a card
void Enemy::playerDraw(int &temp, CardDeck &gameDeck)
{
    const int threshold = 17;

    //  檢查敵人手牌總點數
    int totalValue = calculateHandValue();
    
    cout << getName() << "'s turn, Character: Enemy" << endl;

    // 如果手牌總點數小於 17，則繼續抽牌
    if (totalValue < threshold)
    {

        randomlyAddOneCard(gameDeck);

        // 更新手牌總點數
        totalValue = calculateHandValue();
    }
    else
    {
        cout << "Enemy stopped drawing cards." << endl;
    }
}





// Item class 道具卡 ----------------------------------------------------------------------------------------------
// 道具卡可以被玩家獲得、儲存、使用，定義所有道具卡的共同屬性
// 建構函式
Item::Item(Player *&owner, CardDeck &deck)
{
    this->name = "NULL";
    this->owner = owner;
    this->deck = deck;
}

// 取得道具卡名稱
string Item::getName() const { return this->name; }

// 叫玩家選一張牌捨棄
void Item::foldOneCard(Game& game, Player*& targetPlayer)
{
    if (targetPlayer != nullptr)
    {
        targetPlayer->showHand();
        int value;
        while(true){
            while (true)
            {
                
                std::cout << targetPlayer->getName()+", enter the Value of card you want to Fold(1~13): ";
                std::cin >> value;
                // 檢查是否為合法值
                if (value >= 1 && value <= 10)
                {
                    break; // 使用者輸入合法值，跳出迴圈
                }
                else
                {
                    std::cerr << "Invalid value. Please enter a value between 1 and 13." << std::endl;
                }
            }
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
                cout << "- Fold a card with value " << value << " on " << targetPlayer->getName() << "'s hand." << endl;
                targetPlayer->showHand();
                break;
            }
            else
            {
                // 如果卡片不在手中，輸出相應訊息
                cout << "- Card not found." << endl;
            }
        }
    }
}

// 道具卡 1：和另一位玩家隨機交換一張牌 ---------------------------------------------------------------------------------
// 建構函式
RandomSwitch::RandomSwitch(Player *&owner, CardDeck &deck) : Item(owner, deck)
{
    this->name = "RandomSwitch";
}

// 使用
void RandomSwitch::useItem(Player*& theOtherPlayer, Game& game)
{
    cout << endl << "Before Switch:" << endl;
    owner->showHand();
    theOtherPlayer->showHand();
    game.enterYtoContinue();
    cout << "Item used: Random Switch" << endl;
    if (owner->getHand().empty() || theOtherPlayer->getHand().empty()) {
        cout << "No cards to switch." << endl;
        return;
    }
// 確保隨機數生成範圍在手牌數量內
    int ownerRandom = generateRandomNumber(0, owner->getHand().size() - 1);
    int theOtherRandom = generateRandomNumber(0, theOtherPlayer->getHand().size() - 1);
    // cout << "Randomly selected indices: " << random_user << " and " << random_target << endl;
    swap(owner->getHand()[ownerRandom], theOtherPlayer->getHand()[theOtherRandom]);
    cout << "Cards were switched successfully." << endl;

}
// 亂數產生器
int RandomSwitch::generateRandomNumber(int small, int large)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(small, large);
    return dis(gen);
}

// 道具卡 2：重新抽一張牌, 棄掉一張牌 --------------------------------------------------------------------------------
// 建構函式
DrawOneFoldOne::DrawOneFoldOne(Player *&owner, CardDeck &deck) : Item(owner, deck)
{
    this->name = "DrawOneFoldOne";
}

// 使用
void DrawOneFoldOne::useItem(Player*& theOtherPlayer, Game& game)
{
    Card *reDraw = deck.drawOneCard();
    cout << endl << "Item used: Draw One, Fold One Card" << endl;
     if (reDraw == nullptr) {
        cout << "No more cards to draw." << endl;
        return;
    }
    cout << "The new card you've drawn:\n- ";
    reDraw->print();

    this->foldOneCard(game, owner);

    owner->addSpecificCard(reDraw);
    cout << "- "+ owner->getName() +" got ONE card: ";
    reDraw->print();
    game.enterYtoContinue();
}


// 道具卡 3：雙方各棄掉一張牌, 由對方先棄 -------------------------------------------------------------------------------
// 建構函式
BothFoldOne::BothFoldOne(Player *&owner, CardDeck &deck) : Item(owner, deck)
{
    this->name = "BothFoldOne";
}

// 使用
void BothFoldOne::useItem(Player*& theOtherPlayer, Game& game)
{
    cout << endl << "Item used: Both Fold One" << endl;
    if (owner->getHand().empty() || theOtherPlayer->getHand().empty()) {
        cout << "No cards to fold." << endl;
        return;
    }
    cout << "Both players needs to fold one card each." << endl;
    this->foldOneCard(game, theOtherPlayer);
    this->foldOneCard(game, this->owner);
    
}


// CardDeck class -------------------------------------------------------------------------------------------

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
    cout << "------------------------------" << endl;
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
    cout << "------------------------------" << endl
         << endl;
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
Card *CardDeck::specificCard(const string &suit, const string &rank, bool globalSearch)
{
    cout << endl<< "Searching for card: " << suit << "-" << rank << endl;

    // 使用標準函式 find_if 找到符合條件的卡片
    auto it = std::find_if(cards.begin(), cards.end(), [&](const Card &card)
                           { return (card.suit == suit) && (card.rank == rank); });

    // 檢查是否找到卡片
    if (it != cards.end())
    {
        // 如果 isInDeck 為 false，輸出相應訊息
        if (!globalSearch && !it->isInDeck)
        {
            cout << "- Card found, but it's not in deck." << endl;
            return nullptr;
        }

        // 返回卡片的指標
        cout << "- Card found!" << endl;
        return &(*it);
    }
    else
    {
        // 若未找到，返回空指標
        cout << "- Card not found..." << endl;
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

// Record class ----------------------------------------------------------------------------------------------------

// 建構函式
Record::Record(string playerName)
{
    // 注意：檔名不分大小寫
    this->playerName = playerName;
    this->filePath = "./records/" + playerName + ".txt";

    cout
        << "Searching for " + playerName + "'s game record..." << endl;

    ifstream searchFile(filePath);
    if (searchFile)
    {
        cout << "- We found " + playerName + "'s game record file!" << endl;
        searchFile >> maxCards >> maxPoints >> currentWins >> totalWins;
    }
    else
    {
        cout << "- " + playerName + "'s game record file does NOT exist." << endl
             << "- Creating new game record file for " + playerName + "..." << endl;

        maxCards = 0;
        maxPoints = 0;
        totalWins = 0;
        currentWins = 0;

        ofstream newFile(filePath);
        if (!newFile)
            cout << "ERROR! Failed to create file!" << endl;
        else
        {
            newFile << maxCards << " " << maxPoints << " " << currentWins << " " << totalWins;
        }
        newFile.close();
    }
    searchFile.close();
    cout << "------------------------------------------------------------" << endl;
}

// 更新玩家檔案
void Record::updateRecord(bool winsToday, Player *&player)
{
    ofstream recordFile(filePath, ios::trunc);
    if (!recordFile)
        cout << "ERROR! Failed to update record!" << endl;
    else
    {
        int cardCnt = player->getHand().size();
        int pointCnt = player->calculateHandValue();
        if (winsToday)
        {
            currentWins++;
            totalWins++;
        }
        else
        {
            currentWins = 0;
        }
        if (pointCnt <= 21 &&(cardCnt > maxCards || (cardCnt == maxCards && pointCnt > maxPoints)))
        {
            cout << playerName << " just break his/her own record! " << endl;
            this->maxCards = cardCnt;
            this->maxPoints = pointCnt;
        }
        else
        {
            cout << playerName << " did not break his/her record..." << endl;
        }

        recordFile << maxCards << " " << maxPoints << " " << currentWins << " " << totalWins;
    }
    recordFile.close();

    this->print();
}

// 列印玩家檔案
void Record::print() const
{
    ifstream searchFile(filePath);
    if (searchFile)
    {
        cout << "------------------------------" << endl
             << "Player name: " << playerName << endl
             << "Max cards per round: " << maxCards << endl
             << "Max points that round: " << maxPoints << endl
             << "Current win streaks: " << currentWins << endl
             << "Total wins: " << totalWins << endl << endl;
    }
}









// Game class -----------------------------------------------------------------------------------------------------
// 隨著遊戲進行，會陸續新增敵人、物品，玩家也會跟這些敵人和物品互動

// Size of players
int Game::getNumPlayers() const
{
    return players.size();
}

// 將 Player 加入players的vector
void Game::addPlayer(Player *player)
{
    players.push_back(player);
}

vector<Player *> &Game::getPlayers()
{
    return this->players;
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

    this->printIntro("characters");
    this->printStage("Setting up Players");

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
                 << " (Enter S to choose Seeker, Enter T to choose Targetor): ";
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

// 讓玩家輸入Y以繼續遊戲
void Game::enterYtoContinue() const
{
    char enter = 'N';
    while (enter != 'Y')
    {
        cout << "Enter Y to continue: ";
        cin >> enter;
    }
}

// 印出角色介紹
void Game::printIntro(const string file) const
{
    // 讀取txt檔
    ifstream intro("./English_intros/" + file + ".txt");
    if (intro)
    {
        //cout << "Here's the introduction of the "+ file +":" << endl;
        printLong();
        char line[100];
        while (!intro.eof())
        {
            intro.getline(line, 100);
            cout << line << endl;
        }
    }
    intro.close();
}


// 印出分隔線
void Game::printLong() const
{
    cout << "************************************************************" << endl;
}

// 印出大標題
void Game::printStage(string stage) const
{
    printLong();
    for (int i = 0; i < (60 - stage.size()) / 2; i++)
    {
        cout << " ";
    }
    cout << stage << endl;
    printLong();
}

// 進行初始發牌，每位玩家隨機抽兩張卡
void Game::initialDeal()
{
    printStage("Black Jack Begins");
    for (Player *player : players)  // 清空手牌
    {
        player->getHand().clear();
    }
    this->gameDeck.initializeCards();  // 洗牌
    
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
        cout << "- Name: " << player->getName() << ", Character: ";

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
            cout << "Enemy";
        }
        cout << endl;
    }
    cout << endl;
}



// 回傳player的pointer
Player*& Game::getPlayerAtIndex(int index)
{
    if (index >= 0 && index < players.size())
    {
        return players[index];
    }
    else
    {
        // 处理索引越界的情况，可以根据需要修改
        return players[0];
    }
}



// 顯示每位玩家的手牌  // 注意一下這個function沒用到********************************************************
void Game::showPlayersHands() const
{
    for (const Player *player : players)
    {
        cout << player->getName() << "'s hand:" << endl;
        player->showHand();
        
    }
}



// destructor  // 這邊的delete還沒處理**************************************************************
Game::~Game()
{
    /*for (Player *player : players)
    {
        delete player;
    }*/
}




// 新增item到遊戲
void Game::addItem(Player *&player)
{
    cout << player->getName() + ", please choose ONE Item Card..." << endl
         << "- 1: Random Switch" << endl
         << "- 2: Draw One, Fold One" << endl
    << "- 3: Both Fold One" << endl;
    
    while (true)
    {
        char itemType = '0';
        cout << "Your Choice: ";
        cin >> itemType;
        try
        {
            Item *newItem = nullptr;

            if (itemType == '1')
            {
                newItem = new RandomSwitch(player, gameDeck);
                cout << endl << player->getName() + " earned an Random Switch Card!" << endl;
            }
            else if (itemType == '2')
            {
                newItem = new DrawOneFoldOne(player, gameDeck);
                cout << endl << player->getName() + " earned an Draw And Fold Card!" << endl;
            }
            else if (itemType == '3')
            {
                newItem = new BothFoldOne(player, gameDeck);
                cout << endl << player->getName() + " earned an Both Fold One Card!" << endl;
            }
            else
            {
                throw invalid_argument("Invalid item.");
            }

            // 將道具加入遊戲
            items.push_back(newItem);

            break; // 跳出無窮迴圈，因為輸入正確
        }
        catch (const invalid_argument &e)
        {
            cout << e.what() << " Please try again." << endl;
        }
    }
}

void Game::itemRound(bool whoWins[], bool& play)
{
    Player *player = nullptr;
    for (int i = 0; i < 2; i++)
    {
        
        if (whoWins[i] == 0)
        {
            player = getPlayerAtIndex(i);
            printLong();
            cout << player->getName() + ", you LOST this round...\n\nDo you want to play BONUS Item Round?"<< endl;
            while (true)
            {
                char move = 'N';
                cout  << "(Y: Play bonus Round, N: Exit and update player's record): ";
                cin >> move;
                try
                {
                    if (move == 'Y')
                    {
                        cout << endl;
                        printStage("Bonus: Item Round");
                        addItem(player);
                        char enter = 'N';
                        while (enter != 'Y')
                        {
                            cout << "- Enter Y to use your Item Card: ";
                            cin >> enter;
                        }
                        if(i == 0)
                        {
                            items[0]->useItem(getPlayerAtIndex(1), *this);
                        }
                        else if(i == 1)
                        {
                            items[0]->useItem(getPlayerAtIndex(0), *this);
                        }
                        this->items.clear();
                        break;
                    }
                    else if (move == 'N')
                    {
                        cout << endl;
                        play = false;
                        break;
                    }
                    else
                    {
                        throw invalid_argument("Invalid");
                    }
                    
                    break; // 跳出無窮迴圈，因為輸入正確
                }
                catch (const invalid_argument &e)
                {
                    cout << e.what() << " Please try again." << endl;
                }
            }
            
        }
    }
    
    
   
}

void Game::result(bool whoWins[])
{
    Player *player1 = getPlayerAtIndex(0);
    Player *player2 = getPlayerAtIndex(1);
    Player *enemy = getPlayerAtIndex(2);

    bool player1Under21 = getPlayerAtIndex(0)->calculateHandValue() <= 21;
    bool player2Under21 = getPlayerAtIndex(1)->calculateHandValue() <= 21;
    bool enemyUnder21 = getPlayerAtIndex(2)->calculateHandValue() <= 21;

    bool p1wins = 0;
    bool p2wins = 0;

    if (*player1 > *player2 && *player1 > *enemy && player1Under21)
    {
        // player1 win
        printStage(player1->getName() + " Won!");
        p1wins = 1;
    }
    else if (*player2 > *player1 && *player2 > *enemy && player2Under21)
    {
        // player2 win
        printStage(player2->getName() + " Won!");
        p2wins = 1;
    }
    else if (*player2 > *player1 && enemy->calculateHandValue() > 21 && player2Under21)
    {
        printStage(player2->getName() + " Won!");
        p2wins = 1;
    }
    else if (*player1 > *player2 && enemy->calculateHandValue() > 21 && player2Under21)
    {
        printStage(player1->getName() + " Won!");
        p1wins = 1;
    }
    else if (*player1 > *player2 && *player1 > *enemy && !player1Under21)
    {
        // player1 > player2 > enemy, but player1 is over 21
        if (!player2Under21)
        {
            // player2 is also over 21, compare with enemy
            if (*enemy >= *player1 && *enemy >= *player2)
            {
                // enemy win
                printStage("Enemy Won...");
            }
            else
            {
                // player2 win
                printStage(player2->getName() + " Won!");
                p2wins = 1;
            }
        }
        else
        {
            // player2 is under 21, compare with enemy
            if (enemyUnder21)
            {
                // player2 win
                printStage(player2->getName() + " Won!");
                p2wins = 1;
            }
            else
            {
                // enemy win
                printStage("Enemy Won...");
            }
        }
    }
    // Similar logic for the case where player2 is over 21 and player1 is under 21
    else if (*player2 > *player1 && *player2 > *enemy && !player2Under21)
    {
        // player2 > player1 > enemy, but player2 is over 21
        if (!player1Under21)
        {
            // player2 is also over 21, compare with enemy
            if (*enemy >= *player1 && *enemy >= *player2)
            {
                // enemy win
                printStage("Enemy Won...");
            }
            else
            {
                // player2 win
                printStage(player2->getName() + " Won!");
                p2wins = 1;
            }
        }
        else
        {
            // player2 is under 21, compare with enemy
            if (enemyUnder21)
            {
                // player2 win
                printStage(player1->getName() + " Won!");
                p1wins = 1;
            }
            else
            {
                // enemy win
                printStage("Enemy Won...");
            }
        }
    }
    else if (*player1 == *player2)
    {
        // players are tied
        if (player1->calculateHandValue() > 21 && player2->calculateHandValue() > 21)
        {
            // both players are over 21, compare with enemy
            if (enemy->calculateHandValue() > 21)
            {
                // all lose
                printStage("No one won. Everyone LOST!");
            }
            else
            {
                // enemy win
                printStage("Enemy Won...");
            }
        }
        else if (player1Under21 && player2Under21)
        {
            // both players are under 21, compare with enemy
            if (!enemyUnder21)
            {
                // player1 and player2 win, enemy lose
                printStage(player1->getName() + " and " + player2->getName() + " both Won!");
                p1wins = 1;
                p2wins = 1;
            }
            else
            {
                // compare with enemy
                if (*enemy >= *player1 && *enemy >= *player2)
                {
                    // enemy win
                    printStage("Enemy Won...");
                }
                else
                {
                    // player1 and player2 win, enemy lose
                    printStage(player1->getName() + " and " + player2->getName() + " both Won!");
                    p1wins = 1;
                    p2wins = 1;
                }
            }
        }
    }
    
    // 顯示玩家的最後牌面
    cout << endl << "Here are the final hands..." << endl;
    player1->showHand();
    player2->showHand();
    
    
    // 記錄玩家勝負
    whoWins[0] = p1wins;
    whoWins[1] = p2wins;
    
}

void Game::drawRound()
{
    int tempEnemy = 0;
    int temp1 = 0;
    int temp2 = 0;

    do
    {
        getPlayerAtIndex(2)->playerDraw(tempEnemy, gameDeck);
        enterYtoContinue();
        getPlayerAtIndex(2)->showHand();
        printLong();

        for (int i = 0; i < getNumPlayers(); i++)
        {

            cout << endl;

            getPlayerAtIndex(i)->playerMove(gameDeck, *this);
            /*}
            for (int i = 0; i < game.getNumPlayers(); i++)
            {*/
            if (i == 0)
            {
                getPlayerAtIndex(i)->playerDraw(temp1, gameDeck);
            }
            else if (i == 1)
            {
                getPlayerAtIndex(i)->playerDraw(temp2, gameDeck);
            }

            if (temp1 == 0 || temp2 == 0)
                printLong();
        }

    } while ((temp1 != 1 && temp2 != 1) || (temp1 == 1 && temp2 != 1) || (temp1 != 1 && temp2 == 1));
}


bool Game::playAgain()
{
    
    while (true)
    {
        char move = 'N';
        cout << endl << "Black Jack Ended. Do you want to play again? (Y: Yes, N: No): ";
        cin >> move;
        cout << endl;
        try
        {
            if (move == 'Y')
            {
                return true;
            }
            else if (move == 'N')
            {
                return false;
            }
            else
            {
                throw invalid_argument("Invalid.");
            }

            break; // 跳出無窮迴圈，因為輸入正確
        }
        catch (const invalid_argument &e)
        {
            cout << e.what() << " Please enter Y or N." << endl;
        }
    }
}
