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

    Enemy enemy("Banker");
    game.addPlayer(&enemy);  // 加入敵人（莊家）

    
    int temp1 = 0;
    int temp2 = 0;
    
    do
    {
        game.enemyDraw(&enemy);
        game.printLong();

        for (int i = 0; i < game.getNumPlayers(); i++)
        {
            
            cout << endl;
            
            game.playerMove(game.getPlayerAtIndex(i));
        /*}
        for (int i = 0; i < game.getNumPlayers(); i++)
        {*/
            if (i == 0)
            {
                game.playerDraw(game.getPlayerAtIndex(i), temp1);
            }
            else if (i == 1)
            {
                game.playerDraw(game.getPlayerAtIndex(i), temp2);
            }
            
            if(temp1 == 0 || temp2 == 0)
                game.printLong();
        }
        
        
    }while((temp1 != 1 && temp2 != 1) || (temp1 == 1 && temp2 != 1) || (temp1 != 1 && temp2 == 1));
    
    
    bool whoWins[2] = {0,0};
    game.result(whoWins);  // 判斷勝負
    
    game.itemRound(whoWins);  // Bonus 道具回合
    
    game.printStage("GAME ENDS");  
    
    return 0;
}
