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

    Player* enemy = new Enemy("Banker");
    game.addPlayer(enemy);  // 加入敵人（莊家）
    
    bool play = true;
    while(play)
    {
        game.initialDeal();  // 進行初始發牌
        game.drawRound();    // 進行21點
        
        
        bool whoWins[2] = {0,0};  // 用來記錄勝負
        
        while(play)
        {
            game.result(whoWins);     // 判斷勝負
            game.itemRound(whoWins, play);  // Bonus道具卡回合
        }
        
        // 更新玩家戰績
        game.printStage("Player's Records");
        cout << endl;
        game.getPlayerAtIndex(0)->getRecord()->updateRecord(whoWins[0], game.getPlayerAtIndex(0));
        game.getPlayerAtIndex(1)->getRecord()->updateRecord(whoWins[1], game.getPlayerAtIndex(1));
        
        game.printStage("Black Jack ENDS");
        play = game.playAgain();  // 是否要再玩一次
    }
    
    game.printStage("Program ENDS");
    
    return 0;
}
