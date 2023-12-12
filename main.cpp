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
    cout << "Welcome to Black Jack!" << endl;
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
