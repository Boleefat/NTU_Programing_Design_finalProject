#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <fstream>
#include "BlackJack.h"
using namespace std;


int main()
{
    // initial setting
    Game game; // 創建遊戲物件
    CardDeck deck;
    game.printStage("Ｗelcome to Black Jack!");
    game.addPlayers();            // 加入玩家
    game.showPlayersNameAndChr(); // 確認玩家資訊
    game.enterYtoContinue();
    game.initialDeal();           // 進行初始發牌
    Player* enemy = new Enemy("Banker");
    game.addPlayer(enemy);  // 加入敵人（莊家）
    

    game.drawRound();     // 進行21點
    bool whoWins[2] = {0,0};
    game.result(whoWins);  // 判斷勝負
    
    game.itemRound(whoWins);  // Bonus, 道具卡回合
    game.printStage("GAME ENDS");  //遊戲結束
    
    return 0;
}

