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

#include "Game.h"

void Game::Run() {

#pragma region SETUP

#pragma region Create game screen

    //32 x 24
    sf::RenderWindow window(sf::VideoMode(SCREEN_W + SCOREBOARD_W, SCREEN_H), "Sea Cucumber Game");

    windowPtr = &window;

    auto icon = sf::Image{};

    if (!icon.loadFromFile("Resources/Sprites/cqz.png")) {
        cout << "WARNING: cqz.png NOT FOUND" << endl;
        return;
    }

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    sf::Clock clock;
    float lastFrame;
    float dt;


    RectangleShape backgroundRect;
    backgroundRect.setFillColor(COL_TURQOISE);
    backgroundRect.setSize({ SCREEN_W + SCOREBOARD_W, SCREEN_H });


    //Setup scoreboard

    Texture scoreboardTexture;
    Sprite scoreboardSprite;

    if (!scoreboardTexture.loadFromFile("Resources/Sprites/scoreboard.png")) {
        cout << "WARNING: scoreboard.png NOT FOUND" << endl;
        _ASSERTE("WARNING: SCOREBOARD TEXTURE NOT LOADED");
        return;
    }

    scoreboardSprite.setTexture(scoreboardTexture);

    float screenHscale = SCREEN_H / scoreboardTexture.getSize().y;
    scoreboardSprite.setScale({ screenHscale, screenHscale });
    scoreboardSprite.setPosition({ SCREEN_W, 0 });

#pragma endregion

#pragma region Create managers and snakes


    txtManager = new TextManager();
    water = new Water();
    foodManager = new FoodManager(this);

    audioManager = new AudioManager();

    //Create snakes
    for (size_t i = 0; i < kMaxPlayerCount; i++)
    {
        if (playerTypes[i] == PlayerType::computer)
        {
            cout << "AI SNAKE CREATED" << endl;
            allSnakes[i] = new AISnake(startPositions[i].x, startPositions[i].y, i);
            allSnakes[i]->setIsAI(true);
            allSnakes[i]->setSuperRespiration(usingViciousAI);
            allSnakes[i]->setMaxOxygenMultiplier(5);

            continue;

        }

        cout << "PLAYER SNAKE CREATED" << endl;

        allSnakes[i] = new Snake(startPositions[i].x, startPositions[i].y, i);


        if (playerTypes[i] == PlayerType::none) allSnakes[i]->setIsAlive(false);

    }

    //Assign managers to snakes
    for (size_t i = 0; i < kMaxPlayerCount; i++)
    {
        allSnakes[i]->AssignFoodManager(foodManager);

    }
    for (size_t i = 0; i < kMaxPlayerCount; i++)
    {
        if (playerTypes[i] == PlayerType::none) continue;
        allSnakes[i]->AssignTextManager(txtManager);


    }
    for (size_t i = 0; i < kMaxPlayerCount; i++)
    {
        if (playerTypes[i] == PlayerType::none) continue;
        allSnakes[i]->AssignAudioManager(audioManager);

    }

    //Give snakes references to all other snakes
    for (size_t i = 0; i < kMaxPlayerCount; i++)
    {
        Snake* tempSnake = allSnakes[i];

        for (size_t j = 0; j < kMaxPlayerCount; j++) {
            if (i != j) {
                tempSnake->AssignOtherSnakeRef(allSnakes[j]);
            }
        }
    }
    

#pragma endregion
    

#pragma region Create UI

    //Scoreboard
    for (size_t i = 0; i < kMaxPlayerCount; i++)
    {
        if (playerTypes[i] == PlayerType::none) continue;

        SnakeHUD newHud(allSnakes[i], i);
        allSnakeHUDS.push_back(newHud);
    }

    //Game over buttons
    sf::Texture restartButtonTex;
    sf::Texture quitButtonTex;
    sf::Texture menuButtonTex;

    if (!restartButtonTex.loadFromFile("Resources/Sprites/buttonReplay.png")) {
        cout << "WARNING: buttonReplay.png NOT FOUND" << endl;
        return;
    }
    if (!quitButtonTex.loadFromFile("Resources/Sprites/buttonQuit.png")) {
        cout << "WARNING: buttonQuit.png NOT FOUND" << endl;
        return;
    }
    if (!menuButtonTex.loadFromFile("Resources/Sprites/buttonMenu.png")) {
        cout << "WARNING: buttonMenu.png NOT FOUND" << endl;
        return;
    }

    restartButton = new UiButton(fePtr, restartButtonTex, 5);
    restartButton->setScale({ 3.2, 3.2 });
    restartButton->setPosition({ SCREEN_W / 2, (int)(SCREEN_H / 5 * 3.45) });
    restartButton->setActive(false);

    quitButton = new UiButton(fePtr, quitButtonTex, 7);
    quitButton->setScale({ 3.2, 3.2 });
    quitButton->setPosition({ SCREEN_W / 2, (int)(SCREEN_H / 5 * 3.45 + 220) });
    quitButton->setActive(false);

    menuButton = new UiButton(fePtr, menuButtonTex, 6);
    menuButton->setScale({ 3.2, 3.2 });
    menuButton->setPosition({ SCREEN_W / 2, (int)(SCREEN_H / 5 * 3.45 + 110) });
    menuButton->setActive(false);
    

#pragma endregion


#pragma endregion

#pragma region GAME LOOP

    foodManager->SetupFood(MAX_FOOD);


    while (window.isOpen())
    {
        ///POLLING EVENTS
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:

                if (!gameIsRunning) {
                    if (restartButton != nullptr) {
                        restartButton->ProcessMouseDownEvent(Mouse::getPosition(window));
                    }
                    if (quitButton != nullptr) {
                        quitButton->ProcessMouseDownEvent(Mouse::getPosition(window));
                    }
                    if (menuButton != nullptr) {
                        menuButton->ProcessMouseDownEvent(Mouse::getPosition(window));
                    }
                }

                break;

            default:
                break;
            }
        }



        dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        WinCheck();

    #pragma region Updates

        txtManager->Update(dt);

        if (gameIsRunning)
        {

            water->Update(dt);

            for (auto& snake : allSnakes) {
                snake->SetIsSubmerged(water->GetSurfaceLoc_GRD());
            }

            for (auto& snake : allSnakes) {
                snake->Update(dt);
            }

            for (auto& snakeHUD : allSnakeHUDS) {
                snakeHUD.UpdateData();
            }

            foodManager->SetWaterLevel(water->GetSurfaceLoc_GRD());
            foodManager->Update(dt);

            if (getAlivePlayerCount() == 1) {
                for (auto snake : allSnakes) {
                    if (snake->getIsAlive()) {
                        lastSnakeStanding = snake;
                        break;
                    }
                }
            }

        }

#pragma endregion

    #pragma region Rendering

        //RENDERING DURING GAME


        //Clear back buffer or whatever it's called
        window.clear();

        //Render world
        window.draw(backgroundRect);

        //Render entities
        water->Render(window);
        foodManager->RenderAll(window);

        for (auto& snake : allSnakes) {
            snake->Render(window);
        }

        //Render test
        txtManager->Render(window);


        //UI DONE ABOVE EVERYTHING ELSE

        //Scoreboard during game
        window.draw(scoreboardSprite);
        for (auto& snakeHUD : allSnakeHUDS) {
            snakeHUD.Render(window);
        }

        //UI when game over
        if (!gameIsRunning)
        {
            window.draw(*gameOverText);
            window.draw(*winnerText);

            restartButton->Render(window);
            quitButton->Render(window);
            menuButton->Render(window);
        }

        // Get the window to display its contents
        window.display();

#pragma endregion

    }

#pragma endregion

}

void Game::EndGame() {

    gameOverText = new Text("GAME OVER", font, 150);
    gameOverText->setFillColor(COL_SNAKEOUTLINE3);
    gameOverText->setPosition({ SCREEN_W / 2 - gameOverText->getLocalBounds().width / 2, SCREEN_H / 4 });

    winnerText = new Text();
    winnerText->setCharacterSize(50);
    winnerText->setFont(font);
    winnerText->setOutlineThickness(3);



    string winnerName = "Winner is: ";
    int winnerId;
    int maxScore = INT_MIN;

    if (getAlivePlayerCount() != 0) {   //theoretically this situation would only have 1 player alive
        //meaning the winner is determined by last man standing

        /*for (size_t i = 0; i < 4; i++)
        {
            if (playerTypes[i] != PlayerType::none) {

                cout << "PLAYER " << i << " HAD SCORE OF " << allSnakes[i]->getScore() << endl;

                if (allSnakes[i]->getScore() > maxScore && allSnakes[i]->getIsAlive()) {
                    cout << "PLAYER " << i << " HAS NEW MAX SCORE" << endl;
                    cout << allSnakes[i]->getScore() << " IS BIGGER THAN " << maxScore << endl;

                    string s = " Winner is: Player " + to_string(i + 1) + "\n With score of: ";
                    char str[5];
                    snprintf(str, 5, "%04d", allSnakes[i]->getScore());

                    maxScore = allSnakes[i]->getScore();
                    winnerName = s.append(str);
                    winnerId = i;
                }
            }
        }*/

        for (auto snake : allSnakes) {
            if (snake->getIsAlive()) {
                lastSnakeStanding = snake;
            }
        }

        string s = " Winner is: Player " + to_string(lastSnakeStanding->getPlayerId() + 1) + "\n With score of: ";
        char str[5];
        snprintf(str, 5, "%04d", lastSnakeStanding->getScore());

        winnerName = s.append(str);
        winnerId = lastSnakeStanding->getPlayerId();

    }
    else {  //If no players are alive 

        if (endlessGame) { //and if the game was endless
            //The last player alive should win

            if (lastSnakeStanding == nullptr) {
                cout << "WARNING. LAST ALIVE SNAKE WAS NULL." << endl;
                return;
            }
            else {
                string s = " Winner is: Player " + to_string(lastSnakeStanding->getPlayerId() + 1) + "\n With score of: ";
                char str[5];
                snprintf(str, 5, "%04d", lastSnakeStanding->getScore());

                winnerName = s.append(str);
                winnerId = lastSnakeStanding->getPlayerId();
            }

            
        }
        else //and game was not endless
        {
            //then all players have died at once, and the winner should be determined by score;
            for (size_t i = 0; i < 4; i++)
            {
                if (playerTypes[i] != PlayerType::none) {

                    cout << "PLAYER " << i << " HAD SCORE OF " << allSnakes[i]->getScore() << endl;

                    if (allSnakes[i]->getScore() > maxScore) {
                        cout << "PLAYER " << i << " HAS NEW MAX SCORE" << endl;
                        cout << allSnakes[i]->getScore() << " IS BIGGER THAN " << maxScore << endl;

                        string s = " Winner is: Player " + to_string(i + 1) + "\n With score of: ";
                        char str[5];
                        snprintf(str, 5, "%04d", allSnakes[i]->getScore());

                        maxScore = allSnakes[i]->getScore();
                        winnerName = s.append(str);
                        winnerId = i;
                    }
                }
            }

        }
        
    }


    winnerText->setString(winnerName);
    winnerText->setPosition({ SCREEN_W / 2 - winnerText->getLocalBounds().width / 2, SCREEN_H / 2 });


    switch (winnerId)
    {

    case(0):
        winnerText->setFillColor(COL_SNAKEBOD1);
        winnerText->setOutlineColor(COL_SNAKEOUTLINE1);
        break;

    case(1):
        winnerText->setFillColor(COL_SNAKEBOD2);
        winnerText->setOutlineColor(COL_SNAKEOUTLINE2);
        break;

    case(2):
        winnerText->setFillColor(COL_SNAKEBOD3);
        winnerText->setOutlineColor(COL_SNAKEOUTLINE3);
        break;

    case(3):
        winnerText->setFillColor(COL_SNAKEBOD4);
        winnerText->setOutlineColor(COL_SNAKEOUTLINE4);
        break;

    default:
        winnerText->setFillColor(COL_SNAKEBOD1);
        winnerText->setOutlineColor(COL_SNAKEOUTLINE1);
        break;
    }

    gameIsRunning = false;

    restartButton->setActive(true);
    quitButton->setActive(true);
    menuButton->setActive(true);
}