#include "FrontEnd.h"




void FrontEnd::Run() {

    #pragma region UI Variables

            const float kWindowScale = 1.5;
            const int kCharSize = 22;
            const int kLabelOutlineThickness = 3;
            const int kVertDistribution = 80;
            const int kPlayerLabelDistributionOffset = 30;
            const int kOptionLabelDistributionOffset = 65;
            const int kOptionButtonDistributionOffset = 75;

    #pragma endregion

    #pragma region Setup window



		    sf::RenderWindow window(sf::VideoMode(FE_SCREEN_W, FE_SCREEN_H), "Game Setup");
            windowRef = &window;

            auto icon = sf::Image{};

            if (!icon.loadFromFile("Resources/Sprites/cqs.png")) {
                cout << "WARNING: cqs.png NOT FOUND" << endl;
                return;
            }

            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

            sf::RectangleShape bgRect({ FE_SCREEN_W, FE_SCREEN_H });
            bgRect.setFillColor(COL_DARKPURPLE);

    #pragma endregion

    #pragma region Load textures



            //LOAD TEXTURES
            sf::Texture titleTex;
            sf::Texture startbuttonTex;
            sf::Texture quitbuttonTex;
            sf::Texture arrowbuttonTex;
            sf::Texture checkboxTex;
            sf::Texture checkboxTickTex;

            if (!titleTex.loadFromFile("Resources/Sprites/titleLogo_2.png")) {
                cout << "WARNING: titleLogo.png NOT FOUND" << endl;
                return;
            }

            if (!startbuttonTex.loadFromFile("Resources/Sprites/buttonStart.png")) {
                cout << "WARNING: buttonStart.png NOT FOUND" << endl;
                return;
            }

            if (!quitbuttonTex.loadFromFile("Resources/Sprites/buttonQuit.png")) {
                cout << "WARNING: buttonQuit.png NOT FOUND" << endl;
                return;
            }

            if (!arrowbuttonTex.loadFromFile("Resources/Sprites/buttonArrow.png")) {
                cout << "WARNING: buttonArrow.png NOT FOUND" << endl;
                return;
            }

            if (!checkboxTex.loadFromFile("Resources/Sprites/checkmarkBox.png")) {
                cout << "WARNING: checkmarkBox.png NOT FOUND" << endl;
                return;
            }

            if (!checkboxTickTex.loadFromFile("Resources/Sprites/checkmarkTick.png")) {
                cout << "WARNING: checkmarkTick.png NOT FOUND" << endl;
                return;
            }

    #pragma endregion

    #pragma region Setup title logo and main buttons


            //SETUP TITLE LOGO
            Sprite titleSprite;
            titleSprite.setTexture(titleTex);
            titleSprite.setScale({ kWindowScale, kWindowScale });     
            titleSprite.setPosition({ (FE_SCREEN_W / 2) - (float)(((Vector2i)titleTex.getSize()).x / 2 * kWindowScale), FE_SCREEN_H / 200 });



            //START BUTTON
            UiButton startButton(this, startbuttonTex, 0);
            startButton.setScale({ kWindowScale, kWindowScale });
            startButton.setPosition({ (int)(FE_SCREEN_W / 2), FE_SCREEN_H - (int)((FE_SCREEN_H / 10) * 2) + 30});

            //QUIT BUTTON
            UiButton quitButton(this, quitbuttonTex, 1);
            quitButton.setScale({ kWindowScale, kWindowScale });
            quitButton.setPosition({ (int)(FE_SCREEN_W / 2), FE_SCREEN_H - (int)(FE_SCREEN_H / 10)  + 30});


            buttonGroup_1.push_back(startButton);
            buttonGroup_1.push_back(quitButton);

    #pragma endregion

    #pragma region Setup player text


            //SETUP OPTIONS

            if (!font_1.loadFromFile("Resources/Fonts/f_vcr.ttf")) {
                _ASSERTE("FONT NOT FOUND");
            }
            else {
                cout << "VCR Font loaded" << endl;
            }

            Text p1Label("Player 1", font_1, kCharSize);
            Text p2Label("Player 2", font_1, kCharSize);;
            Text p3Label("Player 3", font_1, kCharSize);;
            Text p4Label("Player 4", font_1, kCharSize);;

            p1Label.setColor(COL_SNAKEBOD1);
            p2Label.setColor(COL_SNAKEBOD2);
            p3Label.setColor(COL_SNAKEBOD3);
            p4Label.setColor(COL_SNAKEBOD4);

            p1Label.setOutlineColor(COL_SNAKEOUTLINE1);
            p2Label.setOutlineColor(COL_SNAKEOUTLINE2);
            p3Label.setOutlineColor(COL_SNAKEOUTLINE3);
            p4Label.setOutlineColor(COL_SNAKEOUTLINE4);

            p1Label.setOutlineThickness(kLabelOutlineThickness);
            p2Label.setOutlineThickness(kLabelOutlineThickness);
            p3Label.setOutlineThickness(kLabelOutlineThickness);
            p4Label.setOutlineThickness(kLabelOutlineThickness);

            p1Label.setStyle(Text::Style::Bold);
            p2Label.setStyle(Text::Style::Bold);
            p3Label.setStyle(Text::Style::Bold);
            p4Label.setStyle(Text::Style::Bold);

            textGroup_1.push_back(p1Label);
            textGroup_1.push_back(p2Label);
            textGroup_1.push_back(p3Label);
            textGroup_1.push_back(p4Label);

            for (size_t i = 0; i < textGroup_1.size(); i++)
            {
                float yPos = (float)((kVertDistribution + kPlayerLabelDistributionOffset + kVertDistribution * i)); //60 is the offset
                textGroup_1[i].setPosition({ FE_SCREEN_W / 2 - textGroup_1[i].getLocalBounds().width / 2,  yPos });
            }

    #pragma endregion

    #pragma region Setup options



            vector<string> snakeOptions;
            snakeOptions.push_back("HUMAN");
            snakeOptions.push_back("COMPUTER");
            snakeOptions.push_back("NONE");

            OptionList p1Options(snakeOptions);
            OptionList p2Options(snakeOptions);
            OptionList p3Options(snakeOptions);
            OptionList p4Options(snakeOptions);

            allOptionLists.push_back(p1Options);
            allOptionLists.push_back(p2Options);
            allOptionLists.push_back(p3Options);
            allOptionLists.push_back(p4Options);

            Text p1OptionText(p1Options.current(), font_1, kCharSize);
            Text p2OptionText(p2Options.current(), font_1, kCharSize);
            Text p3OptionText(p3Options.current(), font_1, kCharSize);
            Text p4OptionText(p4Options.current(), font_1, kCharSize);

            p1OptionText.setColor(COL_SNAKEBOD1);
            p2OptionText.setColor(COL_SNAKEBOD2);
            p3OptionText.setColor(COL_SNAKEBOD3);
            p4OptionText.setColor(COL_SNAKEBOD4);

            textGroup_2.push_back(p1OptionText);
            textGroup_2.push_back(p2OptionText);
            textGroup_2.push_back(p3OptionText);
            textGroup_2.push_back(p4OptionText);

            for (size_t i = 0; i < textGroup_2.size(); i++)
            {
                float yPos = (float)((kVertDistribution + kOptionLabelDistributionOffset + kVertDistribution * i)); //60 is the offset
                textGroup_2[i].setPosition({ FE_SCREEN_W / 2 - textGroup_2[i].getLocalBounds().width / 2,  yPos });
            }
    #pragma endregion

    #pragma region Setup option buttons

            UiButton p1OptionArrowLeft(this, arrowbuttonTex, 2, 0);
            UiButton p1OptionArrowRight(this, arrowbuttonTex, 3, 0);

            UiButton p2OptionArrowLeft(this, arrowbuttonTex, 2, 1);
            UiButton p2OptionArrowRight(this, arrowbuttonTex, 3, 1);

            UiButton p3OptionArrowLeft(this, arrowbuttonTex, 2, 2);
            UiButton p3OptionArrowRight(this, arrowbuttonTex, 3, 2);

            UiButton p4OptionArrowLeft(this, arrowbuttonTex, 2, 3);
            UiButton p4OptionArrowRight(this, arrowbuttonTex, 3, 3);

            //Upscale
            p1OptionArrowLeft.setScale({ kWindowScale, kWindowScale });
            p1OptionArrowRight.setScale({ kWindowScale, kWindowScale });
            p2OptionArrowLeft.setScale({ kWindowScale, kWindowScale });
            p2OptionArrowRight.setScale({ kWindowScale, kWindowScale });
            p3OptionArrowLeft.setScale({ kWindowScale, kWindowScale });
            p3OptionArrowRight.setScale({ kWindowScale, kWindowScale });
            p4OptionArrowLeft.setScale({ kWindowScale, kWindowScale });
            p4OptionArrowRight.setScale({ kWindowScale, kWindowScale });

            //Flip right buttons
            p1OptionArrowRight.flipScaleX();
            p2OptionArrowRight.flipScaleX();
            p3OptionArrowRight.flipScaleX();
            p4OptionArrowRight.flipScaleX();

            buttonGroup_2.push_back(p1OptionArrowLeft);
            buttonGroup_2.push_back(p2OptionArrowLeft);
            buttonGroup_2.push_back(p3OptionArrowLeft);
            buttonGroup_2.push_back(p4OptionArrowLeft);

            buttonGroup_3.push_back(p1OptionArrowRight);
            buttonGroup_3.push_back(p2OptionArrowRight);
            buttonGroup_3.push_back(p3OptionArrowRight);
            buttonGroup_3.push_back(p4OptionArrowRight);

            //Distribute left buttons
            for (size_t i = 0; i < buttonGroup_2.size(); i++)
            {
                float xPos = (float)(FE_SCREEN_W / 4 * 1);
                float yPos = (float)((kVertDistribution + kOptionButtonDistributionOffset + kVertDistribution * i)); 
                buttonGroup_2[i].setPosition({ (int)xPos,  (int)yPos });
            }

            //Distribute right buttons
            for (size_t i = 0; i < buttonGroup_3.size(); i++)
            {
                float xPos = (float)(FE_SCREEN_W / 4 * 3);
                float yPos = (float)((kVertDistribution + kOptionButtonDistributionOffset + kVertDistribution * i));
                buttonGroup_3[i].setPosition({ (int)xPos, (int)yPos });
            }

    #pragma endregion

    #pragma region Setup togglebox options

            //Options toggle box:

            //Endless
            UiButton endlessGameCheckbox(this, checkboxTex, 4);
            Sprite endlessGameTick(checkboxTickTex);
            Text endlessCheckboxLabel("Endless", font_1, kCharSize);

            endlessGameCheckbox.setScale({ kWindowScale, kWindowScale });
            endlessGameCheckbox.setPosition({ FE_SCREEN_W / 9, FE_SCREEN_H - FE_SCREEN_H / 4 });
            endlessGameTick.setPosition({ FE_SCREEN_W / 30, (int)(FE_SCREEN_H - FE_SCREEN_H / 3.2) });
            endlessCheckboxLabel.setPosition({ FE_SCREEN_W / 5, (int)(FE_SCREEN_H - FE_SCREEN_H / 3.7) });

            endlessGameTick.setScale({ kWindowScale, kWindowScale });

            //Vicious AI

            UiButton viciousAICheckbox(this, checkboxTex, 8);
            Sprite viciousAITick(checkboxTickTex);
            Text viciousAICheckboxLabel("Vicious AI", font_1, kCharSize);

            viciousAICheckbox.setScale({ kWindowScale, kWindowScale });
            viciousAICheckbox.setPosition({ (int)(FE_SCREEN_W / 1.8), FE_SCREEN_H - FE_SCREEN_H / 4 });
            viciousAITick.setPosition({ FE_SCREEN_W / 2.1, (int)(FE_SCREEN_H - FE_SCREEN_H / 3.2) });
            viciousAICheckboxLabel.setPosition({ FE_SCREEN_W / 1.6, (int)(FE_SCREEN_H - FE_SCREEN_H / 3.7) });

            viciousAITick.setScale({ kWindowScale, kWindowScale });

            allButtonGroups.push_back(buttonGroup_1);
            allButtonGroups.push_back(buttonGroup_2);
            allButtonGroups.push_back(buttonGroup_3);

            allTextGroups.push_back(textGroup_1);
            allTextGroups.push_back(textGroup_2);

    #pragma endregion

    #pragma region Main menu loop


		    while (window.isOpen()) {

    #pragma region Poll events


                sf::Event event;
                while (window.pollEvent(event))
                {
                    switch (event.type)
                    {
                    case sf::Event::Closed:
                        window.close();
                        break;

                    case sf::Event::MouseButtonPressed:
                    
                        for (auto &buttonGroup : allButtonGroups) {
                            for (auto& button : buttonGroup) {
                                button.ProcessMouseDownEvent(Mouse::getPosition(window));

                            }
                        }

                        endlessGameCheckbox.ProcessMouseDownEvent(Mouse::getPosition(window));
                        viciousAICheckbox.ProcessMouseDownEvent(Mouse::getPosition(window));

                        break;

                    default:
                        break;
                    }
                }

    #pragma endregion

    #pragma region Rendering


                //RENDERING
                window.clear();

                window.draw(bgRect);
                window.draw(titleSprite);

                for (auto& buttonGroup : allButtonGroups) {
                    for (auto& button : buttonGroup) {
                        button.Render(window);
                    }
                }

                //Drawing all textgroups at once does not work for some reason
                /*for (auto& textGroup : allTextGroups) {

                    for (Text& text : textGroup) {
                        window.draw(text);
                    }
                }*/

                for (Text& text : textGroup_1) {
                    window.draw(text);
                }


                for (Text& text : textGroup_2) {
                    window.draw(text);
                }

                //Render Checkbox
                endlessGameCheckbox.Render(window);
                viciousAICheckbox.Render(window);

                if (endlessGameOption) {
                    window.draw(endlessGameTick);
                }
       
                if (viciousAIOption) {

                    window.draw(viciousAITick);
                }

                window.draw(endlessCheckboxLabel);
                window.draw(viciousAICheckboxLabel);

                window.display();

    #pragma endregion

		    }

    #pragma endregion


	}

#pragma region Button events

void FrontEnd::StartGame() {

    int activePlayerCount = 0;

    for (size_t i = 0; i < 4; i++)
    {
        if (playerSetup[i] != PlayerType::none) activePlayerCount++;
    }

    if (activePlayerCount == 0) {
        sfx_error.play();
        return;
    }

    bool shouldBeEndlessGame = false;

    if (activePlayerCount == 1) {
        shouldBeEndlessGame = true;
    }
    else {
        shouldBeEndlessGame = endlessGameOption;
    }



    Game game(this, playerSetup, shouldBeEndlessGame, viciousAIOption);
    gamePtr = &game;

    game.Run();

 }

void FrontEnd::RestartGame() {

    cout << "RESTART GAME CALLED" << endl;
    gamePtr->CloseGame();   //will allowing the game to destroy itself by scope ending cause memory leaks?

    StartGame();
}

void FrontEnd::ToMenu() {
    gamePtr->CloseGame();
    gamePtr = nullptr;
}

void FrontEnd::QuitGame(){

    if (windowRef != nullptr) {
        windowRef->close();
    }

}

void FrontEnd::QuitGameFromGameOver() {

    if (gamePtr != nullptr) {
        gamePtr->CloseGame();
    }

    if (windowRef != nullptr) {
        windowRef->close();
    }

}

void FrontEnd::ShiftOptionListLeft(int listId)
{

    cout << "Shifting options " << listId << " left" << endl;
    allOptionLists[listId].previous();
    textGroup_2[listId].setString(allOptionLists[listId].current());
    textGroup_2[listId].setPosition({ FE_SCREEN_W / 2 - textGroup_2[listId].getLocalBounds().width / 2,  textGroup_2[listId].getPosition().y });

    playerSetup[listId] = (PlayerType)allOptionLists[listId].id;
    cout << "New option is " << playerSetup[listId] << endl;
}

void FrontEnd::ShiftOptionListRight(int listId) {

    cout << "Shifting options " << listId << " right" << endl;
    allOptionLists[listId].next();
    textGroup_2[listId].setString(allOptionLists[listId].current());
    textGroup_2[listId].setPosition({ FE_SCREEN_W / 2 - textGroup_2[listId].getLocalBounds().width / 2,  textGroup_2[listId].getPosition().y });

    playerSetup[listId] = (PlayerType)allOptionLists[listId].id;
    cout << "New option is " << playerSetup[listId] << endl;
}

#pragma endregion
