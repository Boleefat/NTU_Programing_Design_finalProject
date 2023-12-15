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

// Card struct, 撲克牌 -------------------------------------------------------------------

// 建構函式
Card::Card(const string &s, const string &r, int v) : suit(s), rank(r), value(v), isInDeck(true) {}

// 印出Card資訊
void Card::print() const
{
    cout << suit << "-" << rank << ", 點數: " << value << endl;
}

// SkillCounter struct 技能使用狀態 ------------------------------------------------------------

// 建構函式
SkillCounter::SkillCounter() : isSkillOneAvailable(true), isSkillTwoAvailable(true), isSkillThreeAvailable(true) {}

/* Player class, 是一個 pure abstract class ----------------------------------------------------------
 是遊戲中所有玩家（包含敵我）的基礎類別，裡面定義了所有玩家都需要有的屬性和行為 */

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

    /*有誤
    // 處理 A 的情況，使其適應不同的情境
    while (numAces > 0 && totalValue > 21)
    {
        totalValue -= 10;
        numAces--;
    }
     */

    return totalValue;
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
         << name << "的手牌: " << endl;

    for (const auto &card : hand)
    {
        cout << "- " << card.suit << "-" << card.rank << ", 點數 : " << card.value << endl;
    }

    cout << "點數總和: " << calculateHandValue();
    if (calculateHandValue() > 21)
        cout << " (爆點了!)";

    cout << endl << endl;;
}

// 隨機添加一張牌到手牌
void Player::randomlyAddOneCard(CardDeck &deck)
{
    Card *drawnCard = deck.drawOneCard(); // 從卡組中抽一張牌的指標
    if (drawnCard != nullptr)
    {
        hand.push_back(*drawnCard); // 添加該牌到玩家手牌中 ////注意這邊是shallow copy
        cout << "- " + name + " 得到卡牌: " << drawnCard->suit << "-" << drawnCard->rank << ", 點數: " << drawnCard->value << endl;
    }
    else
    {
        cout << "牌組裡啥都沒有TT" << endl;
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

vector<Card> Player::getHand() const
{
    return hand;
}

Record *&Player::getRecord()
{
    return record;
}

/* Seeker class, 繼承 Player 這個類別 --------------------------------------------------
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
        cout << getName() << " 尋找...的手牌 " << targetPlayer->getName() << "..." << endl;
        targetPlayer->showHand();
        
    }
    else
    {
        // Print a message if the skill is not available
        cout << "- 你成功使用了這項技能!" << endl;
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
        cout << this->name << " 尋找手牌..." << endl;
        deck.displayDeck();
    }
    else
    {
        // Print a message if the skill is not available
        cout << "- 你成功使用了這項技能!" << endl;
    }
}

// Seeker的move
void Seeker::playerMove(CardDeck &gameDeck, Game &game)
{
    cout << "下一位玩家: " << getName() << ", 角色: Seeker" << endl;
    game.enterYtoContinue();
    showHand();

    char move;
    while (true)
    {
        cout << "您想要使用您的技能嗎？(1: 尋找玩家, 2: 尋找牌組, N: 不做任何事): ";
        cin >> move;

        if (move == '1' || move == '2' || move == 'N')
        {
            break; // 輸入正確，跳出迴圈
        }
        else
        {
            cout << "輸入錯誤囉!請輸入 1 或 2 或 N" << endl;
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
            cout << "請輸入你想要尋找的玩家名稱: ";
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
                cout << "你現在的手牌是:" << endl;
                showHand();
            }
            else
            {
                cout << "- 沒有找到該玩家QAQ." << endl
                 << "你現在的手牌是:" << endl;
                showHand();
            }
            break; // 這裡加上 break
        }
    case '2':

        seekDeck(gameDeck);
        break;

    case 'N':
        // 不做任何事
        // cout << "我不想做任何事..." << endl;
        break;
    }

    // seeker->showHand();
    // cout << "\n";
}

// seeker draw card
void Seeker::playerDraw(int &temp, CardDeck &gameDeck)
{
    // cout << seeker->getName() << "'s turn: \n";
    // seeker->showHand();
    if (calculateHandValue() >= 21)
    {
        cout << "- 你不能再抽牌了...\n";
        temp = 1;
    }
    else
    {
        char move;
        while (true)
        {
            cout << "您想要抽一張牌嗎？(Y: 抽一張牌, N: 不做任何事): ";
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
                cout << "輸入錯誤囉!請輸入 Y, N." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }
}

/* Targetor class, 繼承 Player 這個類別 --------------------------------------------------
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
            cout << "- "+ getName() +" 得到一張牌: "<< specificCard->suit << "-" << specificCard->rank << ", 點數: " << specificCard->value << endl;
        }
        else
        {
            // 如果卡片為 nullptr，表示卡片在其他玩家手中
            cout << "-這張牌在其他玩家手中!" << endl;
        }
    }
    else
    {
        cout << "- 你成功使用了這項技能!" << endl;
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
            cout << "- 摧毀卡牌點數為 " << value << "在" << targetPlayer->getName() << "的手中" << endl;
            targetPlayer->showHand();

            this->skillCounter.isSkillTwoAvailable = false;
        }
        else
        {
            // 如果卡片不在手中，輸出相應訊息
            cout << "- 摧毀失敗，你可以再試一次!" << endl;
        }
    }
    else
    {
        cout << "- 你成功使用了這項技能!" << endl;
    }
}

// Targetor的move
void Targetor::playerMove(CardDeck &gameDeck, Game &game)
{
    cout << "下一位玩家: " << getName() << ", Targetor" << endl;
    game.enterYtoContinue();
    showHand();

    char move;
    while (true)
    {
        cout << "您想要使用您的技能嗎？(1: 獲得卡牌, 2: 摧毀卡牌, N: 不做任何事): ";
        cin >> move;

        if (move == '1' || move == '2' || move == 'N')
        {
            break; // 輸入正確，跳出迴圈
        }
        else
        {
            cout << "輸入錯誤囉!請輸入 1 or 2 or N." << endl;
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
                std::cout << "- 輸入花色 (S for Spades, H for Hearts, D for Diamonds, C for Clubs): ";
                std::cin >> suit;
                // 檢查 suit 是否為合法值
                if (suit == 'H' || suit == 'D' || suit == 'C' || suit == 'S')
                {
                    break; // 使用者輸入合法值，跳出迴圈
                }
                else
                {
                    std::cerr << "輸入錯誤囉!請輸入 H, D, C, or S." << std::endl;
                }
            }
            // 詢問rank
            while (true)
            {
                // 詢問 rank
                std::cout << "- 請輸入 AJQK or 2-10: ";
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

                    std::cerr << "錯誤輸入!請輸入在 1 和 13之間的整數" << std::endl;
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

            cout << endl << "你現在的手牌是:" << endl;
            showHand();
            
            break;
        }

    case '2':
    {
        // 讓使用者輸入要尋找的玩家名稱
        cout << "------------------------------" << endl;
        game.showPlayersNameAndChr();
        cout << "請輸入你想要摧毀卡牌的玩家名稱: ";
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
                std::cout << "請輸入你想要摧毀的牌面點數(1-13): ";
                std::cin >> rank;
                // 檢查是否為合法值
                if (rank >= 1 && rank <= 10)
                {
                    break; // 使用者輸入合法值，跳出迴圈
                }
                else
                {
                    std::cerr << "錯誤輸入!請輸入在1和13之間的整數." << std::endl;
                }
            }
            discardCard(targetPlayer, rank);
            cout << endl;
            cout << "你現在的手牌有:" << endl;
            showHand();
            
        }
        else
        {
            cout << "找不到該玩家" << endl;
        }
        break; // 這裡加上 break
    }

    case 'N':
        // 不做任何事
        // cout << "不作任何事..." << endl;
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
        cout << "- 你不能再抽牌了...\n";
        temp = 1;
    }
    else
    {
        char move;
        while (true)
        {
            cout << "您想要抽一張牌嗎？(Y: 抽一張牌, N: 不做任何事): ";
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
                cout << "錯誤輸入，請輸入 Y, N." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }
}

/* Enemy class, 繼承 Player 這個類別 ----------------------------------------------------------------
 是由電腦自動操作的敵方角色，定義所有Enemy的共同屬性 */

// Constructor for Enemy
Enemy::Enemy(const string &enemyName) : Player(enemyName) {}
void Enemy::playerMove(CardDeck &gameDeck, Game &game) {}

// enemy Draw a card
void Enemy::playerDraw(int &temp, CardDeck &gameDeck)
{
    const int threshold = 17;

    //     檢查敵人手牌總點數
    int totalValue = calculateHandValue();

    cout << getName() << "的機會, 角色: Enemy" << endl;

    // 如果手牌總點數小於 17，則繼續抽牌
    if (totalValue < threshold)
    {

        randomlyAddOneCard(gameDeck);

        // 更新手牌總點數
        totalValue = calculateHandValue();
    }
    else
    {
        cout << "Enemy 停止抽牌." << endl;
    }
}

/* Item class 道具卡 ---------------------------------------------------------------------------
道具卡可以被玩家獲得、儲存、使用，定義所有道具卡的共同屬性 */
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
        while (true)
        {

            std::cout << "請輸入您想要棄掉的牌的點數(1-13): ";
            std::cin >> value;
            // 檢查是否為合法值
            if (value >= 1 && value <= 10)
            {
                break; // 使用者輸入合法值，跳出迴圈
            }
            else
            {
                std::cerr << "請輸入在1到13之間的整數." << std::endl;
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
            cout << "- 棄掉卡牌點數 " << value << " 在 " << targetPlayer->getName() << "的手中." << endl;
            targetPlayer->showHand();
        }
        else
        {
            // 如果卡片不在手中，輸出相應訊息
            cout << "- 摧毀失敗，請再試一次." << endl;
        }
        
        }
}

// 道具卡 1：和另一位玩家隨機交換一張牌 --------------------------------------------------------
// 建構函式
RandomSwitch::RandomSwitch(Player *&owner, CardDeck &deck) : Item(owner, deck)
{
    this->name = "隨機交換牌";
}

// 使用
void RandomSwitch::useItem(Player*& theOtherPlayer, Game& game)
{
    cout << endl << "道具使用:隨機交換" << endl;
    if (owner->getHand().empty() || theOtherPlayer->getHand().empty()) {
        cout << "沒有牌能夠交換" << endl;
        return;
    }
// 確保隨機數生成範圍在手牌數量內
    int ownerRandom = generateRandomNumber(0, owner->getHand().size() - 1);
    int theOtherRandom = generateRandomNumber(0, theOtherPlayer->getHand().size() - 1);
    // cout << "Randomly selected indices: " << random_user << " and " << random_target << endl;
    swap(owner->getHand()[ownerRandom], theOtherPlayer->getHand()[theOtherRandom]);
    cout <<  "交換完畢" << endl;
    owner->showHand();
    theOtherPlayer->showHand();
    game.enterYtoContinue();
}
// 亂數產生器
int RandomSwitch::generateRandomNumber(int small, int large)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(small, large);
    return dis(gen);
}

// 道具卡 2：重新抽一張牌, 棄掉一張牌 --------------------------------------------------------
// 建構函式
DrawOneFoldOne::DrawOneFoldOne(Player *&owner, CardDeck &deck) : Item(owner, deck)
{
    this->name = "重新抽一張牌, 棄掉一張牌";
}

// 使用
void DrawOneFoldOne::useItem(Player*& theOtherPlayer, Game& game)
{
    Card *reDraw = deck.drawOneCard();
    cout << endl << "道具使用: 重新抽一張牌, 棄掉一張牌" << endl;
     if (reDraw == nullptr) {
        cout << "沒有牌可以抽." << endl;
        return;
    }
    cout << "- 你新抽到的牌是: ";
    reDraw->print();

    this->foldOneCard(game, owner);

    owner->addSpecificCard(reDraw);
    owner->showHand();
}


// 道具卡 3：雙方各棄掉一張牌, 由對方先棄 --------------------------------------------------------
// 建構函式
BothFoldOne::BothFoldOne(Player *&owner, CardDeck &deck) : Item(owner, deck)
{
    this->name = "雙方各棄一張牌";
}

// 使用
void BothFoldOne::useItem(Player*& theOtherPlayer, Game& game)
{
    cout << endl << "道具使用: 雙方各棄一張牌" << endl;
    if (owner->getHand().empty() || theOtherPlayer->getHand().empty()) {
        cout << "沒有牌可以丟棄." << endl;
        return;
    }
    cout << "- 每個玩家都需要丟棄一張牌." << endl;
    this->foldOneCard(game, theOtherPlayer);
    this->foldOneCard(game, this->owner);
    
    game.enterYtoContinue();
}


// CardDeck class -------------------------------------------------------------------------------

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
            std::cout << "卡牌: " << card.suit << "-" << card.rank << ", 點數: " << card.value << std::endl;
        }
        else
        {
            std::cout << "卡牌: " << card.suit << "-" << card.rank << ", 點數: " << card.value << " (丟棄)" << std::endl;
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
        cout << "卡組已空" << endl;
        // 返回空指標表示卡組已空
        return nullptr;
    }
}

// 根據 suit 和 rank 尋找特定的 Card 並返回指標，如果 isInDeck 為 false 則輸出訊息
Card *CardDeck::specificCard(const string &suit, const string &rank, bool globalSearch)
{
    cout << endl<< "尋找卡牌: " << suit << "-" << rank << endl;

    // 使用標準函式 find_if 找到符合條件的卡片
    auto it = std::find_if(cards.begin(), cards.end(), [&](const Card &card)
                           { return (card.suit == suit) && (card.rank == rank); });

    // 檢查是否找到卡片
    if (it != cards.end())
    {
        // 如果 isInDeck 為 false，輸出相應訊息
        if (!globalSearch && !it->isInDeck)
        {
            cout << "- 找到卡牌，但不在牌組裡." << endl;
            return nullptr;
        }

        // 返回卡片的指標
        cout << "- 找到卡牌了!" << endl;
        return &(*it);
    }
    else
    {
        // 若未找到，返回空指標
        cout << "- 找不到卡牌..." << endl;
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

// Record class --------------------------------------------------------------------------------

// 建構函式
Record::Record(string playerName)
{
    // 注意：檔名不分大小寫
    this->playerName = playerName;
    this->filePath = "./record/" + playerName + ".txt";

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
        if (pointCnt <= 21 && cardCnt > maxCards)
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
             << "玩家名稱: " << playerName << endl
             << "Max cards per round: " << maxCards << endl
             << "Max points that round: " << maxPoints << endl
             << "Current win streaks: " << currentWins << endl
             << "Total wins: " << totalWins << endl << endl;
    }
}

/* Game class ---------------------------------------------------------------------------
隨著遊戲進行，會陸續新增敵人、物品，玩家也會跟這些敵人和物品互動 */

// Size of players
int Game::getNumPlayers() const
{
    return players.size();
}

void Game::printGameRules() const {
    cout << "                  接下來是遊戲規則介紹!                   " << endl;
    cout << "************************************************************" << endl;
    cout << "\n1. 目標：盡可能讓您的手牌總點數接近21點，但不超過21點。" << endl;
    cout << "2. 玩家可以選擇抽牌或不抽牌。" << endl;
    cout << "3. 爆點出局：如果您的手牌總點數超過21點，則立即失去勝利資格。" << endl;
    cout << "4. 最接近21點的玩家獲勝。如果點數相同，則視為平局。" << endl;
    cout << "5. 使用道具牌：輸家可以在回合結束後使用道具牌，嘗試逆轉局勢。" << endl;
    cout << "6. 最終勝者：使用道具牌後，再次比較點數，最接近21點的玩家獲勝。" << endl;
    cout << "\n祝您遊戲愉快！" << endl;
    cout << "************************************************************" << endl << endl;

    // 等待玩家準備好
    char ready;
    cout << "輸入 'Y' 開始遊戲: ";
    cin >> ready;
    while(ready != 'Y' && ready != 'y') {
        cout << "輸入 'Y' 開始遊戲: ";
        cin >> ready;
    }
}

// 將 Player 加入 players的vector
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
            cout << "請輸入玩家數量 (目前只能輸入2 ): ";
            cin >> numPlayers;

            if (cin.fail() || numPlayers < 2 || numPlayers > 2)
            {
                throw out_of_range("請輸入2.");
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
    this->printStage("設定玩家資訊");

    // 動態建立玩家，加入遊戲
    for (int i = 0; i < numPlayers; ++i)
    {
        string playerName;
        string playerCharacter;

        cout << "輸入玩家名稱" << i + 1 << ": ";
        cin >> playerName;

        while (true)
        {
            cout << "選擇玩家角色 " << i + 1 << endl
                 << " (輸入 S 選擇 Seeker, 輸入 T 選擇 Targetor): ";
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
                    throw invalid_argument("錯誤輸入");
                }

                // 將玩家加入遊戲
                players.push_back(newPlayer);

                break; // 跳出無窮迴圈，因為輸入正確
            }
            catch (const invalid_argument &e)
            {
                cout << e.what() << " 請再試一次." << endl;
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
        cout << "輸入 Y 繼續: ";
        cin >> enter;
    }
}

// 印出角色介紹
void Game::printIntro(const string file) const
{
    ifstream intro("./English_intros/" + file + ".txt");
    if (intro)
    {
        /*cout << "Here's the introduction of the "+ file +":" << endl
        << "------------------------------";*/
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

void Game::printLong() const
{
    cout << "************************************************************" << endl;
}

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
    printStage("遊戲開始");

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
    cout << "玩家資訊:" << endl;
    for (const Player *player : players)
    {
        cout << "- 名字: " << player->getName() << ", 角色: ";

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

// 顯示每位玩家的手牌
void Game::showPlayersHands() const
{
    for (const Player *player : players)
    {
        cout << player->getName() << "的手中:" << endl;
        player->showHand();
        
    }
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
        // ?理索引越界的情?，?里返回nullptr，你可以根据需要修改
        return players[0];
    }
}

// destructor
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
    cout << player->getName() + ", 請選一張道具牌..." << endl
         << "- 1: 隨機交換" << endl
         << "- 2: 抽一張，棄一張" << endl
         << "- 3: 雙方各棄一張牌" << endl
         << "輸入 N 取消." << endl;
    while (true)
    {
        char itemType = '0';
        cout << "- 你的選擇是: ";
        cin >> itemType;
        try
        {
            Item *newItem = nullptr;

            if (itemType == '1')
            {
                newItem = new RandomSwitch(player, gameDeck);
                cout << player->getName() + " 得到一張隨機交換牌!" << endl;
            }
            else if (itemType == '2')
            {
                newItem = new DrawOneFoldOne(player, gameDeck);
                cout << player->getName() + " 得到抽一張，棄一張牌!" << endl;
            }
            else if (itemType == '3')
            {
                newItem = new BothFoldOne(player, gameDeck);
                cout << player->getName() + " 得到雙方各棄一張牌!" << endl;
            }
            else if (itemType == 'N')
            {
            }
            else
            {
                throw invalid_argument("錯誤輸入");
            }

            // 將道具加入遊戲
            items.push_back(newItem);

            break; // 跳出無窮迴圈，因為輸入正確
        }
        catch (const invalid_argument &e)
        {
            cout << e.what() << " 請再試一次." << endl;
        }
    }
}

void Game::itemRound(bool whoWins[])
{
    Player *player = nullptr;

    for (int i = 0; i < 2; i++)
    {
        if (whoWins[i] == 0)
        {
            player = getPlayerAtIndex(i);
            printLong();
            cout << player->getName() + ", 你輸了...\n- 你想要進行道具回合嗎? ";
            while (true)
            {
                char move = 'N';
                cout << "(I: 進行道具回合, N: 不要): ";
                cin >> move;
                try
                {
                    if (move == 'I')
                    {
                        printStage("道具回合");
                        addItem(player);
                        char enter = 'N';
                        while (enter != 'Y')
                        {
                            cout << player->getName()+", 輸入Y 使用你的道具卡: ";
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
                    }
                    else if (move == 'N')
                    {
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
                    cout << e.what() << " 請再試一次." << endl;
                }
            }
            
        }
    }
    
    

    bool itemWins[2] = {0, 0};
    result(itemWins); // 判斷勝負
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
        printStage(player1->getName() + " 贏了!");
        p1wins = 1;
    }
    else if (*player2 > *player1 && *player2 > *enemy && player2Under21)
    {
        // player2 win
        printStage(player2->getName() + " 贏了!");
        p2wins = 1;
    }
    else if (*player2 > *player1 && enemy->calculateHandValue() > 21 && player2Under21)
    {
        printStage(player2->getName() + " 贏了!");
        p2wins = 1;
    }
    else if (*player1 > *player2 && enemy->calculateHandValue() > 21 && player2Under21)
    {
        printStage(player1->getName() + " 贏了!");
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
                printStage("Enemy 贏了...");
            }
            else
            {
                // player2 win
                printStage(player2->getName() + " 贏了!");
                p2wins = 1;
            }
        }
        else
        {
            // player2 is under 21, compare with enemy
            if (enemyUnder21)
            {
                // player2 win
                printStage(player2->getName() + " 贏了!");
                p2wins = 1;
            }
            else
            {
                // enemy win
                printStage("Enemy 又贏...");
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
                printStage("Enemy 贏了...");
            }
            else
            {
                // player2 win
                printStage(player2->getName() + " 贏了!");
                p2wins = 1;
            }
        }
        else
        {
            // player2 is under 21, compare with enemy
            if (enemyUnder21)
            {
                // player2 win
                printStage(player1->getName() + " 贏了!");
                p1wins = 1;
            }
            else
            {
                // enemy win
                printStage("Enemy 又贏...");
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
                printStage("沒有人勝利，大家都輸了!");
            }
            else
            {
                // enemy win
                printStage("Enemy 贏了...");
            }
        }
        else if (player1Under21 && player2Under21)
        {
            // both players are under 21, compare with enemy
            if (!enemyUnder21)
            {
                // player1 and player2 win, enemy lose
                printStage(player1->getName() + " 和 " + player2->getName() + " 都贏了!");
                p1wins = 1;
                p2wins = 1;
            }
            else
            {
                // compare with enemy
                if (*enemy >= *player1 && *enemy >= *player2)
                {
                    // enemy win
                    printStage("Enemy 贏了...");
                }
                else
                {
                    // player1 and player2 win, enemy lose
                    printStage(player1->getName() + "和  " + player2->getName() + " 都贏了!");
                    p1wins = 1;
                    p2wins = 1;
                }
            }
        }
    }
    
    cout << "最終的牌組是..." << endl;
    player1->showHand();
    player2->showHand();
    
   
    char enter = 'N';
    while (enter != 'Y')
    {
        cout << "輸入 Y 更新玩家資料: ";
        cin >> enter;
    }
    printStage("Player's Records");
    player1->getRecord()->updateRecord(p1wins, player1);
    player2->getRecord()->updateRecord(p2wins, player2);

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
