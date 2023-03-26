/************************************************/
/*	Sea Cucumber Game	    	                */
/*	Programmer : Niblet, Amin Khan		        */
/*	Date : 11th May, 2022	                    */
/*	Email : akhan2019@elam.co.uk                */
/* 	Webpages : https://niblet.itch.io/		    */
/*             https://github.com/Niblet-0		*/
/* 	Discord : Niblet#7617		                */
/* 	                    		                */
/* 	Made for Teesside university		        */
/* 	Uhh, don't redistribute source code         */
/* 	and other copyright stuff                   */
/* 	                                            */
/************************************************/

#pragma once

#include <iostream>
#include "Snake.h"
#include "AISnake.h"
#include <vector>
#include <map>
#include "GLOBALS.h"
#include "COLPALETTE.h"
#include "Food.h"
#include "Water.h"
#include "TextManager.h"
#include "AudioManager.h"
#include "UiButton.h"
#include <SFML/Graphics.hpp>

enum PlayerType{
    human,
    computer,
    none
};

class FrontEnd;
class UiButton;

class Game
{
public:

    Game(FrontEnd* fe, PlayerType setupTypes[], bool isEndlessGame, bool viciousAI) {
        if (!font.loadFromFile("Resources/Fonts/upheavtt.ttf")) {
            _ASSERTE("FONT NOT FOUND");
        }
        else {
            cout << "UPHEAVAL Font loaded" << endl;
        }

        for (size_t i = 0; i < kMaxPlayerCount; i++)
        {
            playerTypes[i] = setupTypes[i];
        }

        fePtr = fe;

        endlessGame = isEndlessGame;
        usingViciousAI = viciousAI;
   
    }



    void Run();



    bool SpaceIsAvailable(Vector2i coord) {

        //check space in snakes
        //check space in food manager
        //check space in walls

        for (auto snake : allSnakes) {

            if (!snake->CoordWithinSnake(coord) && !foodManager->CoordOverlapsWithSpawnedFood(coord)) {
                return true;
            }
        }
        
        return false;

    }

    Water* waterRef() {
        return water;
    }

    int getAlivePlayerCount() {
        
        int count = 0;
        for (size_t i = 0; i < kMaxPlayerCount; i++)
        {
            if (allSnakes[i]->getIsAlive()) count++;
        }

        return count;
    }

    void WinCheck() {

        if (!gameIsRunning) return;

        if (!endlessGame) {
            if (getAlivePlayerCount() <= 1) {
                EndGame();

            }

        }
        else {
            if (getAlivePlayerCount() <= 0) {
                EndGame();
            }
        }

    }

    void EndGame();

    void CloseGame() {
        if (windowPtr != nullptr) {
            windowPtr->close();
        }
    }

    void ButtonPressEvent(int funcId) {
        switch (funcId) {
        case(0):

            break;
        }
    }

private:

    const static int kMaxPlayerCount = 4;

    FrontEnd* fePtr;
    sf::RenderWindow* windowPtr;

    Font font;
    Text* gameOverText;
    Text* winnerText;

    bool gameIsRunning = true;
    bool endlessGame = false;
    bool usingViciousAI = false;

    PlayerType playerTypes[kMaxPlayerCount];

    Snake* allSnakes[kMaxPlayerCount]; //use star symbol for array?
    TextManager* txtManager;
    FoodManager* foodManager;
    AudioManager* audioManager;
    Water* water;
    Snake* lastSnakeStanding = nullptr;

    vector<SnakeHUD> allSnakeHUDS;

    UiButton* restartButton;
    UiButton* quitButton;
    UiButton* menuButton;

    map<int, Vector2i> startPositions = {
        {0, {5, 4}},
        {1, {SCREEN_W / GRIDSIZE - 5, SCREEN_H / GRIDSIZE - 4}},
        {2, {5, SCREEN_H / GRIDSIZE - 4}},
        {3, {SCREEN_W / GRIDSIZE - 5, 4}},
    };
    

};





