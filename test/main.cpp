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
    cout << "Black Jack start!" << endl;
    game.addPlayers();            // 加入玩家
    game.showPlayersNameAndChr(); // 確認玩家資訊
    game.initialDeal();           // 進行初始發牌

    Enemy enemy("Banker");
    game.addPlayer(&enemy);

	
	int temp1 = 0;
	int temp2 = 0;
	
	do 
    {
    	game.enemyDraw(&enemy);
    	
    	for (int i = 0; i < game.getNumPlayers(); i++)
    	{
    		game.playerMove(game.getPlayerAtIndex(i));
		}	
		for (int i = 0; i < game.getNumPlayers(); i++)
    	{
    		if (i == 0)
    		{
    			game.playerDraw(game.getPlayerAtIndex(i), temp1);
			}
    		else if (i == 1)
    		{
    			game.playerDraw(game.getPlayerAtIndex(i), temp2);
			}
		}
		
	}while(temp1 != 1 && temp2 != 1);
    
    cout << "\nGame End\n";
    
    Player* player1 = game.getPlayerAtIndex(0);
	Player* player2 = game.getPlayerAtIndex(1);
	
	bool player1Under21 = game.getPlayerAtIndex(0)->calculateHandValue() <= 21;
    bool player2Under21 = game.getPlayerAtIndex(1)->calculateHandValue() <= 21;
    bool enemyUnder21 = enemy.calculateHandValue() <= 21;


	if (*player1 > *player2 && *player1 > enemy && player1Under21) {
    	// player1 win
    	cout << "\n" << player1->getName() << " wins!" << endl;
	}
	else if (*player2 > *player1 && *player2 > enemy && player2Under21) {
    	// player2 win
    	cout << "\n" << player2->getName() << " wins!" << endl;
	}
	else if (*player2 > *player1 && enemy.calculateHandValue() > 21 && player2Under21)
	{
		cout << "\n" << player2->getName() << " wins!" << endl;
	}
	else if (*player1 > *player2 && enemy.calculateHandValue() > 21 && player2Under21)
	{
		cout << "\n" << player1->getName() << " wins!" << endl;
	}
	else if (*player1 > *player2 && *player1 > enemy && !player1Under21) 
	{
    	// player1 > player2 > enemy, but player1 is over 21
    	if (!player2Under21) {
        	// player2 is also over 21, compare with enemy
        	if (enemy >= *player1 && enemy >= *player2) {
            	// enemy win
            	cout << "Enemy wins!" << endl;
        	}
        	else {
            	// player2 win
            	cout << "\n" << player2->getName() << " wins!" << endl;
       	 	}
    	}
    	else {
        	// player2 is under 21, compare with enemy
        	if (enemyUnder21) {
            	// player2 win
            	cout << "\n" << player2->getName() << " wins!" << endl;
       	 	}
        	else {
            	// enemy win
            	cout << "Enemy wins!" << endl;
        	}
    	}
	}
	// Similar logic for the case where player2 is over 21 and player1 is under 21
	else if (*player2 > *player1 && *player2 > enemy && !player2Under21) 
	{
    	// player2 > player1 > enemy, but player2 is over 21
    	if (!player1Under21) {
        	// player2 is also over 21, compare with enemy
        	if (enemy >= *player1 && enemy >= *player2) {
            	// enemy win
            	cout << "Enemy wins!" << endl;
        	}
        	else {
            	// player2 win
            	cout << "\n" << player2->getName() << " wins!" << endl;
       	 	}
    	}
    	else {
        	// player2 is under 21, compare with enemy
        	if (enemyUnder21) {
            	// player2 win
            	cout << "\n" << player1->getName() << " wins!" << endl;
       	 	}
        	else {
            	// enemy win
            	cout << "Enemy wins!" << endl;
        	}
    	}
	}
	else if (*player1 == *player2) {
    	// players are tied
    	if (player1->calculateHandValue() > 21 && player2->calculateHandValue() > 21) {
        	// both players are over 21, compare with enemy
        	if (enemy.calculateHandValue() > 21) {
            	// all lose
            	cout << "All lose!" << endl;
        	}
        	else {
            	// enemy win
            	cout << "Enemy wins!" << endl;
        	}
    	}
    	else if (player1Under21 && player2Under21) {
        	// both players are under 21, compare with enemy
        	if (!enemyUnder21) {
        	    // player1 and player2 win, enemy lose
            	cout << "\n" << player1->getName() << " and " << player2->getName() << " win!" << endl;
       	 	}
        	else {
            	// compare with enemy
            	if (enemy >= *player1 && enemy >= *player2) {
                	// enemy win
                	cout << "Enemy wins!" << endl;
            	}
            	else {
                	// player1 and player2 win, enemy lose
                	cout << "\n" << player1->getName() << " and " << player2->getName() << " win!" << endl;
            	}
        	}
    	}
	}
	
    return 0;
}
