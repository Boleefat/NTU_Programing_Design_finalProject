// structs /////////////////////
struct Card;
struct SkillCounter;



// classes /////////////////////
class CardDeck;
class Record;
class Game;
class Player;

// Characters（繼承Player)
class Seeker;
class Targetor;

// 還沒寫到的
class Enemy;  // 要繼承Player
class Item;   // 道具卡




// Card //////////////////////////////////////////
struct Card
{
    string suit;
    string rank;
    int value;
    bool isInDeck;

    // 建構函式
    Card(const string &s, const string &r, int v);
};


// SkillCounter /////////////////////////////////////
struct SkillCounter
{
    bool isSkillOneAvailable;
    bool isSkillTwoAvailable;
    bool isSkillThreeAvailable;

    // 建構函式
    SkillCounter();
};
