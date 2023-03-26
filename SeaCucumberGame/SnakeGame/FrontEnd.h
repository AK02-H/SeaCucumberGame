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
#include <SFML/Graphics.hpp>
#include <vector>
#include "Game.h"
#include "UiButton.h"
#include "GLOBALS.h"
#include "COLPALETTE.h"


using namespace std;
using namespace sf;


class UiButton;
enum PlayerType;

class OptionList    //was previously a struct;
{
public:

    int maxSize;
    int id;
    vector<string> options;


    OptionList(vector<string> optionList) {
        for (string s : optionList) {
            options.push_back(s);
        }
        maxSize = optionList.size();
        id = 0;
    }

    string current() {
        return options[id];
    }

    void next() {
        id++;
        if (id >= maxSize) {
            id = 0;
        }
    }

    void previous() {
        id--;
        if (id < 0) {
            id = maxSize - 1;
        }
    }

    void shift(int value) {
        id += value;
        if (id >= maxSize) {
            id = 0;
        }
        else if (id < 0) {
            id = maxSize - 1;
        }
    }

    
};


class FrontEnd
{
public:

    FrontEnd()
    {
        if (!sbuffer_error.loadFromFile("Resources/Sounds/sfx_error.wav")) {
            cout << "WARNING: COULD NOT LOAD sfx_error.wav" << endl;
        }

        sfx_error.setBuffer(sbuffer_error);
    }

    void Run();

    void ButtonPressEvent(int funcId, int additionalParam1 = 0) {

        switch (funcId)
        {
        case(0):
            StartGame();
            break;

        case(1):
            cout << "QUIT" << endl;
            QuitGame();
            break;

        case(2):
            ShiftOptionListLeft(additionalParam1);
            break;

        case(3):
            ShiftOptionListRight(additionalParam1);
            break;

        case(4):
            ToggleEndlessGame();
            break;

        case(5):
            cout << "RESTART" << endl;
            RestartGame();
            break;

        case(6):
            cout << "TO MENU" << endl;
            ToMenu();
            break;

        case(7):
            cout << "QUIT FROM GAMEOVER" << endl;
            QuitGameFromGameOver();
            break;

        case(8):
            cout << "TOGGLE VICIOUS AI" << endl;
            ToggleViciousAI();
            break;

        default:
            break;
        }

    };

    void TestFunc() {
        cout << "TEST FUNCTION" << endl;
    }

private:

    sf::RenderWindow* windowRef;
    Game* gamePtr;

    vector<vector<UiButton>> allButtonGroups;

    vector<UiButton> buttonGroup_1;
    vector<UiButton> buttonGroup_2;
    vector<UiButton> buttonGroup_3;


    vector<vector<Text>> allTextGroups;

    Text testString;

    vector<Text> textGroup_1;
    vector<Text> textGroup_2;

    vector<OptionList> allOptionLists;

    sf::Font font_1;
    sf::SoundBuffer sbuffer_error;
    sf::Sound sfx_error;
    
    PlayerType playerSetup[4];
    bool endlessGameOption = true;
    bool viciousAIOption = false;

    //BUTTON FUNCTIONS

    void StartGame();

    void RestartGame();

    void ToMenu();

    void QuitGame();

    void QuitGameFromGameOver();

    void ShiftOptionListLeft(int listId);

    void ShiftOptionListRight(int listId);

    void ToggleEndlessGame() {
        endlessGameOption = !endlessGameOption;

        cout << "Endless game is now set to " << endlessGameOption << endl;
    }

    void ToggleViciousAI() {
        viciousAIOption = !viciousAIOption;

        cout << "Vicious AI is now set to " << viciousAIOption << endl;
    }
};

