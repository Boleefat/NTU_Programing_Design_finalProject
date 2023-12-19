#include <iostream>
#include "BlackJack.h"
using namespace std;


int main()
{
    
    Game game;      // 創建遊戲
    
    bool play = true;  // 用迴圈讓玩家可以重複遊玩
    while(play)
    {
        game.initialDeal();  // 進行初始發牌
        game.drawRound();    // 進行21點牌局
        
        
        bool whoWins[2] = {0,0};   // 用來記錄勝負
        while(play)
        {
            game.result(whoWins);  // 判斷勝負
            game.itemRound(whoWins, play);  // Bonus道具卡回合
        }
        

        // 更新玩家的戰績檔案
        game.printStage("Player's Record");
        cout << endl;
        game.getPlayerAtIndex(0)->getRecord()->updateRecord(whoWins[0], game.getPlayerAtIndex(0));
        game.getPlayerAtIndex(1)->getRecord()->updateRecord(whoWins[1], game.getPlayerAtIndex(1));
        
        
        // 詢問玩家是否要再玩一次
        play = game.playAgain();
    }
    
    
    return 0;
}
