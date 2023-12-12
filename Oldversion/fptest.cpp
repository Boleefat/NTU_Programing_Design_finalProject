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
    virtual ~Player() = default; // 虛擬析構函數
    Player(const string &playerName) : name(playerName), skillCounter{} {}
    
    bool operator<(const Player& other) const 
	{
        if (calculateHandValue() == other.calculateHandValue()) {
            // 如果手牌值相同，比較牌數
            return hand.size() < other.hand.size();
        }
        // 手牌值不同，直接比較手牌值
        return calculateHandValue() < other.calculateHandValue();
    }
    
     bool operator>(const Player& other) const 
	 {
        if (calculateHandValue() == other.calculateHandValue()) {
            // 如果手牌值相同，比較牌數
            return hand.size() > other.hand.size();
        }
        // 手牌值不同，直接比較手牌值
        return calculateHandValue() > other.calculateHandValue();
    }
    
    
    
    int calculateHandValue() const
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

    void showHand() const
    {
        cout << name << "'s hands:" << endl;
        for (const auto &card : hand)
        {
            cout << card.suit << "-" << card.rank << ", value : " << card.value << endl;
        }
        cout << "Total card value: " << calculateHandValue() << endl;
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

    // 在 Player 類別中新增函式
    void addSpecificCard(Card *card)
    {
        if (card != nullptr)
        {
            hand.push_back(*card);
        }
    }

    string getName() const
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

class Enemy : public Player
{
public:
    // Constructor for Enemy
    Enemy(const string &enemyName) : Player(enemyName) {}
    
};

// Seeker class, derived from Player///////////////
class Seeker : public Player
{

public:
    virtual ~Seeker() = default; // 虛擬析構函數
    // Constructor for Seeker
    Seeker(const string &playerName) : Player(playerName) {}

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

// Targetor class, derived from Player///////////////
class Targetor : public Player
{
public:
    virtual ~Targetor() = default; // 虛擬析構函數
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

// Game////////////////////////////////////////////////
class Game
{
private:
    vector<Player *> players; // 儲存player的ptr
    CardDeck gameDeck;        // 遊戲的卡片牌組

public:
	//Size of players
	int getNumPlayers() const
    {
        return players.size();
    }
    // 將 Player 加入 players 向量
    void addPlayer(Player *player)
    {
        players.push_back(player);
    }

    // 建立players的vector
    void addPlayers()
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

        // 這邊要加入職業介紹，還沒完成

        // 動態建立玩家，加入遊戲
        for (int i = 0; i < numPlayers; ++i)
        {
            string playerName;
            string playerCharacter;

            cout << "Enter name for Player " << i + 1 << ": ";
            cin >> playerName;

            while (true)
            {
                cout << "Choose character for Player " << i + 1 << " (Press S to choose Seek, Press T to choose Targetor): ";
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

    // 進行初始發牌，每位玩家隨機抽兩張卡
    void initialDeal()
    {
        for (Player *player : players)
        { // 每位玩家抽兩張卡
            Card *drawnCard1 = gameDeck.drawOneCard();
            Card *drawnCard2 = gameDeck.drawOneCard();
            // 將抽到的卡片加入玩家手牌
            player->addSpecificCard(drawnCard1);
            player->addSpecificCard(drawnCard2);
        }
    }

    // 顯示每位玩家的名稱和角色類型
    void showPlayersNameAndChr() const
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
        cout << endl;
    }

    // 顯示每位玩家的手牌
    void showPlayersHands() const
    {
        for (const Player *player : players)
        {
            cout << "Name: " << player->getName() << "'s hand:" << endl;
            player->showHand();
            cout << endl;
        }
    }

    // 判斷player的type，呼叫該type的move
    void playerMove(Player *player)
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
    void enemyMove(Enemy *enemy)
	{
		enemy->randomlyAddOneCard(gameDeck);
	}
    

    // Seeker的move
    void seekerMove(Seeker *seeker)
    {
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
    void targetorMove(Targetor *Targetor)
    {
    }

    // 回傳player的pointer
    Player *getPlayerAtIndex(int index)
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

    // destructor
    ~Game()
    {
        for (Player *player : players)
        {
            delete player;
        }
    }
};

int main()
{
    // initial setting
    Game game; // 創建遊戲物件
    CardDeck deck;
    cout << "Black Jack start!" << endl;
    game.addPlayers();            // 加入玩家
    game.showPlayersNameAndChr(); // 確認玩家資訊
    game.initialDeal();           // 進行初始發牌
    int temp1 = 0;
    int temp2 = 0;
    Enemy enemy1("Enemy 1");
    Enemy enemy2("Enemy 2");
    game.addPlayer(&enemy1);
    game.addPlayer(&enemy2);
    for (int i = 0; i < 3; i++)
    {
    	game.playerMove(static_cast<Player*>(&enemy1));
		game.playerMove(static_cast<Player*>(&enemy2));
	}
	
	do 
    {
    	
    	temp1 = game.getPlayerAtIndex(0)->calculateHandValue();
    	temp2 = game.getPlayerAtIndex(1)->calculateHandValue();

    	for (int i = 0; i < game.getNumPlayers(); i++)
    	{
    		game.playerMove(game.getPlayerAtIndex(i));
		}	
		
	}while(((temp1 != game.getPlayerAtIndex(0)->calculateHandValue()) || (temp2 != game.getPlayerAtIndex(1)->calculateHandValue())));
    
    cout << "\nGame End\n";
    // 檢查大家都小於等於 21 點
    bool player1Under21 = game.getPlayerAtIndex(0)->calculateHandValue() <= 21;
    bool player2Under21 = game.getPlayerAtIndex(1)->calculateHandValue() <= 21;
    bool enemy1Under21 = enemy1.calculateHandValue() <= 21;
    bool enemy2Under21 = enemy2.calculateHandValue() <= 21;

    // 比較四個物件的大小
    if (player1Under21 && player2Under21 && enemy1Under21 && enemy2Under21)
    {
        if (*game.getPlayerAtIndex(0) > *game.getPlayerAtIndex(1) && enemy1 > enemy2)
        {
            cout << "\n" << game.getPlayerAtIndex(0)->getName() << " win!" << endl;
        }
        else if (*game.getPlayerAtIndex(1) > *game.getPlayerAtIndex(0) && enemy2 > enemy1)
        {
            cout << "\n" << game.getPlayerAtIndex(1)->getName() << " win!" << endl;
        }
        else
        {
            cout << "tied" << endl;
        }
    }
    else
    {
        cout << "All players busted! No one wins." << endl;
    }
	
	
	/*
	if (*game.getPlayerAtIndex(0) > *game.getPlayerAtIndex(1)) 
	{
    	cout << "\n" << game.getPlayerAtIndex(0)->getName() << " win!" << endl;
	} 
	else if (*game.getPlayerAtIndex(0) < *game.getPlayerAtIndex(1)) 
	{
    	cout << "\n" << game.getPlayerAtIndex(1)->getName() << " win!" << endl;
	} 
	else 
	{
    	cout << "tied" << endl;
	}
   */
 
    return 0;
}
