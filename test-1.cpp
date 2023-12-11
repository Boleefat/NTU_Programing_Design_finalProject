#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
using namespace std;

// Card//////////////////////////////////////////
struct Card
{
    string suit;
    string rank;
    int value;
    bool isInDeck;

    // 建構函式
    Card(const string &s, const string &r, int v) : suit(s), rank(r), value(v), isInDeck(true) {}
};
// SkillCounter/////////////////////////////////////
struct SkillCounter
{
    bool isSkillOneAvailable;
    bool isSkillTwoAvailable;
    bool isSkillThreeAvailable;

    // 建構函式
    SkillCounter() : isSkillOneAvailable(true), isSkillTwoAvailable(true), isSkillThreeAvailable(true) {}
};
// CardDeck//////////////////////////////////////
class CardDeck
{
public:
    // 建構函式並同時初始化 52 張卡片
    CardDeck()
    {
        initializeCards();
        // Use the current time as a seed for the random number generator
        randomGenerator = std::mt19937(static_cast<unsigned>(std::time(0)));
    }

    // 取得卡片數量
    int getDeckSize() const
    {
        return cards.size();
    }

    // 顯示卡組內容
    void displayDeck() const
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
    Card *drawOneCard()
    {
        if (!isEmpty())
        {
            std::uniform_int_distribution<int> dis(0, cards.size() - 1);

            // 使用迴圈確保不斷嘗試抽牌，直到抽到一張在卡組中的牌
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
    Card *specificCard(const string &suit, const string &rank)
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
    bool isEmpty() const
    {
        return cards.empty();
    }

private:
    vector<Card> cards;           // 存放卡片的向量
    std::mt19937 randomGenerator; // 隨機數生成器
    // 初始化卡片
    void initializeCards()
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
};

// Player////////////////////////////////////////
class Player
{
public:
    Player(const string &playerName) : name(playerName), skillCounter{} {}

    void showHand() const
    {
        cout << name << "'s hands:" << endl;
        for (const auto &card : hand)
        {
            cout << card.suit << "-" << card.rank << ", value : " << card.value << endl;
        }
    }

    void randomlyAddOneCard(CardDeck &deck)
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

    string getName()
    {
        return this->name;
    }

    vector<Card> &getHand()
    {
        return hand;
    }

protected:
    string name;
    vector<Card> hand;
    SkillCounter skillCounter;
};

// Seeker class, derived from Player
class Seeker : public Player
{

public:
    // Constructor for Seeker
    Seeker(const string &playerName) : Player(playerName) {}

    // Additional functionalities specific to Seeker can be added here
    void additionalSeekerFunction()
    {
        // Add any specific functionality for the Seeker class here
        cout << "Seeker-specific function called." << endl;
    }

    // skillOne: Seeker uses a special ability to seek another player's hand
    void seekAnotherPlayer(Player *targetPlayer)
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

    // skillTwo:
    void seekDeck(CardDeck &deck)
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
};

class Targetor : public Player
{
public:
    // Constructor for Targetor
    Targetor(const string &playerName) : Player(playerName) {}

    // skillOne
    void takeSpecificCard(CardDeck &deck, const string &suit, const string &rank)
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
    //  把該玩家手中某value的牌從手牌中刪除
    void discardCard(Player *targetPlayer, int value)
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
};

int main()
{
    // 創建一個卡組
    CardDeck deck;

    // 顯示卡組內容
    deck.displayDeck();

    // 創建兩位玩家
    // 創建一位 Seeker 玩家
    Seeker seekerPlayer("Seeker1");
    // 創建一位 Targetor 玩家
    Targetor targetorPlayer("Targetor1");

    // 玩家1抽一張牌
    seekerPlayer.randomlyAddOneCard(deck);
    seekerPlayer.randomlyAddOneCard(deck);
    seekerPlayer.randomlyAddOneCard(deck);
    cout << endl;

    // 玩家2抽一張牌
    targetorPlayer.randomlyAddOneCard(deck);
    targetorPlayer.randomlyAddOneCard(deck);
    targetorPlayer.randomlyAddOneCard(deck);
    cout << endl;

    // 顯示玩家手牌
    seekerPlayer.showHand();
    cout << endl;
    targetorPlayer.showHand();
    cout << endl;

    // seeker發動能力
    seekerPlayer.seekAnotherPlayer(&targetorPlayer);
    seekerPlayer.seekDeck(deck);

    // targetor發動能力
    targetorPlayer.takeSpecificCard(deck, "Hearts", "5");
    cout << endl;
    targetorPlayer.discardCard(&seekerPlayer, 4);
    cout << endl;

    // 顯示玩家手牌
    seekerPlayer.showHand();
    cout << endl;
    targetorPlayer.showHand();
    cout << endl;

    // 顯示卡組內容
    deck.displayDeck();

    return 0;
}