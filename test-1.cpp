#include <iostream>
#include <vector>
#include <string>
#include <random>
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

// CardDeck//////////////////////////////////////
class CardDeck
{
public:
    // 建構函式並同時初始化 52 張卡片
    CardDeck()
    {
        initializeCards();
        // 初始化隨機數生成器
        randomGenerator = std::mt19937(std::random_device()());
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
            // 使用隨機數生成器取得一個隨機索引
            std::uniform_int_distribution<int> dis(0, cards.size() - 1);
            int randomIndex = dis(randomGenerator);

            // 取得隨機抽中的牌的指標
            Card *drawnCard = &cards[randomIndex];

            // 將 isInDeck 設為 false
            drawnCard->isInDeck = false;

            return drawnCard;
        }
        else
        {
            cout << "CardDeck is empty" << endl;
            // 返回空指標表示卡組已空
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

        for (const auto &suit : suits)
        {
            for (size_t i = 0; i < ranks.size(); ++i)
            {
                int value = (i >= 10) ? 10 : i + 1; // J, Q, K 的 value 都是 10
                cards.push_back(Card(suit, ranks[i], value));
            }
        }
    }
};

// Player////////////////////////////////////////
class Player
{
public:
    Player(const string &playerName) : name(playerName) {}

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
            hand.push_back(*drawnCard); // 添加該牌到玩家手牌中
            cout << name << " get one card: " << drawnCard->suit << "-" << drawnCard->rank << ", value: " << drawnCard->value << endl;
            delete drawnCard; // 在這裡刪除牌，請確保在手牌中有複製牌的副本，以免懸空指標
        }
        else
        {
            cout << "CardDeck is empty." << endl;
        }
    }

private:
    string name;
    vector<Card> hand;
};

int main()
{
    // 創建一個卡組
    CardDeck deck;

    // 顯示卡組內容
    deck.displayDeck();

    // 創建兩位玩家
    Player player1("pp1");
    Player player2("pp2");

    // 玩家1抽一張牌
    player1.randomlyAddOneCard(deck);
    cout << endl;

    // 玩家2抽一張牌
    player2.randomlyAddOneCard(deck);
    cout << endl;

    // 顯示玩家手牌
    player1.showHand();
    cout << endl;
    player2.showHand();

    // 顯示卡組內容
    deck.displayDeck();

    return 0;
}
