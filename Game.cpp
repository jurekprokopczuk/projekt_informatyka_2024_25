#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm> // std::sort (do highscore)

// ------------------------------------------------------
// Konstruktor Game():
//  - ustawia wymiary okna,
//  - ładuje zasoby (teksty, tekstury, dźwięki),
//  - inicjuje podstawowe zmienne gry (stan, poziom, prędkości, itp.)
// ------------------------------------------------------
Game::Game()
    : window(sf::VideoMode(800, 600), "Space Invaders"),
    direction(1),            // Ruch wrogów: 1 w prawo, -1 w lewo
    lastEnemyBulletTime(0.f),
    enemyToShoot(-1),        // Który wróg ma strzelić?
    currentState(GameState::StartScreen),
    isPaused(false),
    playerName(""),
    currentLevel(1),
    enemySpeed(0.1f)
{
    // Ładowanie tła do gry
    if (!backgroundTexture.loadFromFile("tlo.jpg")) {
        std::cerr << "Błąd: nie udało się załadować tła (tlo.jpg)." << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Ekran startowy
    if (!startScreenTexture.loadFromFile("start.jpg")) {
        std::cerr << "Błąd: nie udało się załadować start.jpg." << std::endl;
    }
    startScreenSprite.setTexture(startScreenTexture);

    // Ekran pomocy
    if (!helpScreenTexture.loadFromFile("help.jpg")) {
        std::cerr << "Błąd: nie udało się załadować help.jpg." << std::endl;
    }
    helpScreenSprite.setTexture(helpScreenTexture);

    // Ekran wyjścia
    if (!exitScreenTexture.loadFromFile("exit.png")) {
        std::cerr << "Błąd: nie udało się załadować exit.png." << std::endl;
    }
    exitScreenSprite.setTexture(exitScreenTexture);

    // Ekran Game Over
    if (!gameOverTexture.loadFromFile("gameover.png")) {
        std::cerr << "Błąd: nie udało się załadować gameover.png." << std::endl;
    }
    gameOverSprite.setTexture(gameOverTexture);

    // Dźwięk wystrzału
    if (!shootBuffer.loadFromFile("shoot.wav")) {
        std::cerr << "Błąd: nie udało się załadować shoot.wav." << std::endl;
    }
    else {
        shootSound.setBuffer(shootBuffer);
    }

    // Ekran You Win
    if (!youWinTexture.loadFromFile("youwin.png")) {
        std::cerr << "Błąd: nie udało się załadować youwin.png." << std::endl;
    }
    youWinSprite.setTexture(youWinTexture);
    // Skalowanie do 800 × 600
    youWinSprite.setScale(
        800.f / youWinSprite.getLocalBounds().width,
        600.f / youWinSprite.getLocalBounds().height
    );

    // Wczytywanie listy rekordów
    loadHighscoresFromFile("highscores.txt");

    // Przygotowanie startowego poziomu
    setupLevel();

    // Inicjalizacja generatora pseudolosowego
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

// ------------------------------------------------------
// skipLevel():
//  - czyści obiekty z obecnego poziomu,
//  - przechodzi do kolejnego (lub ekranu zwycięstwa).
// ------------------------------------------------------
void Game::skipLevel()
{
    enemies.clear();
    bullets.clear();
    enemyBullets.clear();

    currentLevel++;

    if (currentLevel > 3) {
        // Wyświetl ekran zwycięstwa, gdy przekroczymy 3. poziom
        currentState = GameState::YouWin;
    }
    else {
        setupLevel();
    }
}

// ------------------------------------------------------
// resetGame():
//  - wyzerowanie stanu gry (punkty, życie, poziom),
//  - powrót do startowych wartości.
// ------------------------------------------------------
void Game::resetGame()
{
    player.setScore(0);
    player.setLives(3);
    player.setPosition(375.f, 550.f);

    currentLevel = 1;
    setupLevel();

    direction = 1;
    enemyToShoot = -1;
    lastEnemyBulletTime = 0.f;
}

// ------------------------------------------------------
// setupLevel():
//  - czyści poprzednie obiekty (wrogów, pociski),
//  - tworzy układ wrogów zależnie od bieżącego poziomu,
//  - nadaje im odpowiednią szybkość i cooldown na pociski.
// ------------------------------------------------------
void Game::setupLevel()
{
    enemies.clear();
    bullets.clear();
    enemyBullets.clear();

    // Przy każdym poziomie gracz dostaje 3 życia
    player.setLives(3);

    float startX = 100.f;
    float startY = 50.f;

    int rows = 3;
    if (currentLevel == 1) {
        rows = 2;
    }
    else if (currentLevel == 2) {
        rows = 3;
    }
    else if (currentLevel == 3) {
        rows = 4;
    }

    // Ustawianie wrogów w wierszach i kolumnach
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 11; j++) {
            Enemy enemy(startX + j * 60.f, startY + i * 60.f);

            if (currentLevel == 1) {
                enemy.setSpeedMultiplier(0.5f);
                enemy.setBulletCooldown(2.f);
            }
            else if (currentLevel == 2) {
                enemy.setSpeedMultiplier(1.5f);
                enemy.setBulletCooldown(1.f);
            }
            else if (currentLevel == 3) {
                enemy.setSpeedMultiplier(2.5f);
                enemy.setBulletCooldown(0.5f);
            }
            enemies.push_back(enemy);
        }
    }
}

// ------------------------------------------------------
// run():
//  - główna pętla: obsługa zdarzeń, aktualizacja stanu,
//  - renderowanie.
// ------------------------------------------------------
void Game::run()
{
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        processEvents();

        if (!isPaused) {
            update(deltaTime.asSeconds());
        }
        render();
    }
}

// ------------------------------------------------------
// processEvents():
//  - obsługa zdarzeń (klawiatura, zamknięcie okna),
//  - różne reakcje zależnie od stanu gry (start, pomoc, gra, itp.).
// ------------------------------------------------------
void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        switch (currentState) {
        case GameState::StartScreen:
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 32 && event.text.unicode < 127) {
                    playerName += static_cast<char>(event.text.unicode);
                }
                else if (event.text.unicode == 8 && !playerName.empty()) {
                    playerName.pop_back();
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
                    std::cout << "Wybrany nick: " << playerName << std::endl;
                    currentState = GameState::Playing;
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::F3) {
                    loadGameState("save.txt");
                }
                else if (event.key.code == sf::Keyboard::F4) {
                    showHighscoresWindow();
                }
            }
            break;

        case GameState::Playing:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    // Strzał gracza
                    shootSound.play();
                    player.shoot(bullets);
                }
                else if (event.key.code == sf::Keyboard::F1) {
                    currentState = GameState::Help;
                    isPaused = true;
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::ExitConfirmation;
                    isPaused = true;
                }
                else if (event.key.code == sf::Keyboard::F2) {
                    saveGameState("save.txt");
                }
                else if (event.key.code == sf::Keyboard::F3) {
                    loadGameState("save.txt");
                }
                else if (event.key.code == sf::Keyboard::F4) {
                    showHighscoresWindow();
                }
                else if (event.key.code == sf::Keyboard::F9) {
                    skipLevel();
                }
            }
            break;

        case GameState::Help:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::F1) {
                    currentState = GameState::Playing;
                    isPaused = false;
                }
            }
            break;

        case GameState::GameOver:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::R) {
                    resetGame();
                    currentState = GameState::StartScreen;
                }
                else if (event.key.code == sf::Keyboard::F4) {
                    showHighscoresWindow();
                }
            }
            break;

        case GameState::YouWin:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::R) {
                    resetGame();
                    currentState = GameState::StartScreen;
                }
                else if (event.key.code == sf::Keyboard::F4) {
                    showHighscoresWindow();
                }
            }
            break;

        case GameState::ExitConfirmation:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Y) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::N) {
                    currentState = GameState::Playing;
                    isPaused = false;
                }
            }
            break;
        }
    }
}

// ------------------------------------------------------
// update(float deltaTime):
//  - wywoływane w stanie Playing,
//  - zarządza ruchem statku, wrogów, pocisków,
//  - wykrywa kolizje i warunki przegranej/wygranej.
// ------------------------------------------------------
void Game::update(float deltaTime)
{
    if (currentState == GameState::Playing) {
        // Ruch gracza
        player.update(deltaTime);

        // Kontrola poruszania wrogów (prawo/lewo, schodzenie w dół)
        if (!enemies.empty()) {
            if (direction == 1) {
                float rightEdge = enemies.back().getBounds().left + enemies.back().getBounds().width;
                if (rightEdge >= window.getSize().x) {
                    direction = -1;
                    moveEnemiesDown();
                }
            }
            else {
                float leftEdge = enemies.front().getBounds().left;
                if (leftEdge <= 0) {
                    direction = 1;
                    moveEnemiesDown();
                }
            }
        }

        // Aktualizacja pozycji wrogów
        for (auto& enemy : enemies) {
            enemy.update(direction, deltaTime * enemySpeed);

            // Jeśli wróg zszedł na dół ekranu
            float enemyBottom = enemy.getBounds().top + enemy.getBounds().height;
            if (enemyBottom >= window.getSize().y) {
                currentState = GameState::GameOver;
                updateHighscores();
                saveHighscoresToFile("highscores.txt");
                return;
            }
        }

        // Sprawdzamy, czy wszyscy wrogowie zostali pokonani
        if (enemies.empty()) {
            currentLevel++;
            if (currentLevel > 3) {
                currentState = GameState::YouWin;
                updateHighscores();
                saveHighscoresToFile("highscores.txt");
                return;
            }
            setupLevel();
        }

        // Częstotliwość strzelania wroga (zależna od poziomu)
        float enemyCooldown = (currentLevel == 1) ? 2.f :
            (currentLevel == 2) ? 1.5f : 1.f;

        // Losowy wróg oddaje strzał, jeśli jego cooldown się skończył
        if (enemyToShoot == -1 || enemies[enemyToShoot].canShoot()) {
            enemyToShoot = std::rand() % enemies.size();
            enemies[enemyToShoot].setCooldown(enemyCooldown);

            enemyBullets.push_back(
                EnemyBullet(
                    enemies[enemyToShoot].getPosition().x + 20.f,
                    enemies[enemyToShoot].getPosition().y + 20.f
                )
            );
            // Na 2. i 3. poziomie wrogowie strzelają podwójnie
            if (currentLevel >= 2) {
                enemyBullets.push_back(
                    EnemyBullet(
                        enemies[enemyToShoot].getPosition().x - 20.f,
                        enemies[enemyToShoot].getPosition().y + 20.f
                    )
                );
            }
        }

        // Ruch pocisków gracza
        for (auto& bullet : bullets) {
            bullet.update();
        }
        // Ruch pocisków wroga
        for (auto& eb : enemyBullets) {
            eb.update();
        }

        // ----------------------------------
        // Kolizje: pocisk gracza -> wróg
        // ----------------------------------
        for (auto it = bullets.begin(); it != bullets.end();) {
            bool bulletHit = false;
            for (auto jt = enemies.begin(); jt != enemies.end();) {
                if (it->getBounds().intersects(jt->getBounds())) {
                    jt = enemies.erase(jt);
                    player.increaseScore(100);
                    bulletHit = true;
                    break;
                }
                else {
                    ++jt;
                }
            }
            if (bulletHit) {
                it = bullets.erase(it);
            }
            else {
                ++it;
            }
        }

        // ----------------------------------
        // Kolizje: pocisk wroga -> gracz
        // ----------------------------------
        for (auto it = enemyBullets.begin(); it != enemyBullets.end();) {
            if (it->getBounds().intersects(player.getBounds())) {
                it = enemyBullets.erase(it);
                player.loseLife();

                if (player.getLives() <= 0) {
                    currentState = GameState::GameOver;
                    updateHighscores();
                    saveHighscoresToFile("highscores.txt");
                    return;
                }
            }
            else {
                ++it;
            }
        }
    }
}

// ------------------------------------------------------
// render():
//  - rysuje w oknie to, co trzeba pokazać
//    (tło, ekrany, wrogów, gracza, komunikaty).
// ------------------------------------------------------
void Game::render()
{
    window.clear();

    switch (currentState) {

    case GameState::StartScreen: {
        window.draw(startScreenSprite);

        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Błąd ładowania czcionki arial.ttf" << std::endl;
        }

        sf::Text title("SPACE INVADERS", font, 60);
        title.setFillColor(sf::Color::Yellow);
        sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin(tb.width / 2.f, tb.height / 2.f);
        title.setPosition(400.f, 120.f);

        // Cień dla tytułu
        sf::Text shadow = title;
        shadow.setFillColor(sf::Color::Black);
        shadow.move(3.f, 3.f);
        window.draw(shadow);
        window.draw(title);

        // Informacja dla gracza
        sf::Text info("Wpisz swój nick i wciśnij ENTER (lub SPACJĘ)", font, 24);
        info.setFillColor(sf::Color(255, 255, 150));
        sf::FloatRect ib = info.getLocalBounds();
        info.setOrigin(ib.width / 2.f, ib.height / 2.f);
        info.setPosition(400.f, 220.f);
        window.draw(info);

        // Aktualnie wpisany nick
        sf::Text nick(playerName, font, 30);
        nick.setFillColor(sf::Color::Cyan);
        sf::FloatRect nb = nick.getLocalBounds();
        nick.setOrigin(nb.width / 2.f, nb.height / 2.f);
        nick.setPosition(400.f, 280.f);
        window.draw(nick);

        // Dodatkowe opcje
        sf::Text bottom("ESC - wyjście | F3 - wczytaj grę | F4 - Highscores", font, 18);
        sf::FloatRect bb = bottom.getLocalBounds();
        bottom.setOrigin(bb.width / 2.f, bb.height / 2.f);
        bottom.setPosition(400.f, 550.f);
        bottom.setFillColor(sf::Color(220, 220, 220));
        window.draw(bottom);
    } break;

    case GameState::Playing: {
        // Rysowanie tła w rozmiarze okna
        backgroundSprite.setScale(
            800.f / backgroundSprite.getLocalBounds().width,
            600.f / backgroundSprite.getLocalBounds().height
        );
        window.draw(backgroundSprite);

        // Rysuj gracza, wrogów i pociski
        player.render(window);
        for (auto& e : enemies) {
            e.render(window);
        }
        for (auto& b : bullets) {
            b.render(window);
        }
        for (auto& eb : enemyBullets) {
            eb.render(window);
        }

        sf::Font font;
        font.loadFromFile("arial.ttf");

        sf::Text scoreText("Wynik: " + std::to_string(player.getScore()), font, 20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10.f, 10.f);
        window.draw(scoreText);

        sf::Text livesText("Życia: " + std::to_string(player.getLives()), font, 20);
        livesText.setFillColor(sf::Color::White);
        livesText.setPosition(10.f, 40.f);
        window.draw(livesText);

        sf::Text nameOnScreen("Gracz: " + playerName, font, 20);
        nameOnScreen.setFillColor(sf::Color::White);
        nameOnScreen.setPosition(650.f, 10.f);
        window.draw(nameOnScreen);

        sf::Text levelText("Poziom: " + std::to_string(currentLevel), font, 20);
        levelText.setFillColor(sf::Color::White);
        levelText.setPosition(10.f, 70.f);
        window.draw(levelText);
    } break;

    case GameState::Help: {
        helpScreenSprite.setScale(
            800.f / helpScreenSprite.getLocalBounds().width,
            600.f / helpScreenSprite.getLocalBounds().height
        );
        window.draw(helpScreenSprite);

        sf::Font font;
        font.loadFromFile("arial.ttf");

        sf::Text helpTitle("EKRAN POMOCY", font, 40);
        helpTitle.setFillColor(sf::Color::Yellow);
        sf::FloatRect ht = helpTitle.getLocalBounds();
        helpTitle.setOrigin(ht.width / 2.f, ht.height / 2.f);
        helpTitle.setPosition(400.f, 50.f);
        window.draw(helpTitle);

        sf::Text helpInfo(
            "Sterowanie:\n"
            "- Strzałki: ruch statku\n"
            "- Spacja: strzał\n"
            "- F1: powrót do gry\n"
            "- F2: zapis gry\n"
            "- ESC: wyjście (menu)\n\n"
            "Zasady gry:\n"
            "- Zestrzel wszystkich wrogów, unikaj ich pocisków.\n"
            "- Każdy trafiony wróg to +100 punktów.\n"
            "- Masz do dyspozycji 3 życia.",
            font, 24
        );
        helpInfo.setFillColor(sf::Color::White);
        helpInfo.setPosition(50.f, 120.f);
        window.draw(helpInfo);

        sf::Text back("Naciśnij SPACJĘ lub F1, by wrócić do gry", font, 20);
        back.setFillColor(sf::Color(200, 200, 200));
        sf::FloatRect bb = back.getLocalBounds();
        back.setOrigin(bb.width / 2.f, bb.height / 2.f);
        back.setPosition(400.f, 550.f);
        window.draw(back);
    } break;

    case GameState::GameOver: {
        gameOverSprite.setScale(
            800.f / gameOverSprite.getLocalBounds().width,
            600.f / gameOverSprite.getLocalBounds().height
        );
        window.draw(gameOverSprite);

        sf::Font font;
        font.loadFromFile("arial.ttf");

        sf::Text scoreText("Wynik: " + std::to_string(player.getScore()), font, 30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(300.f, 350.f);
        window.draw(scoreText);

        sf::Text nameText("Nick: " + playerName, font, 25);
        nameText.setFillColor(sf::Color::Yellow);
        nameText.setPosition(300.f, 400.f);
        window.draw(nameText);

        sf::Text infoText("Wciśnij R, aby zagrać ponownie\nlub ESC, żeby wyjść\nF4 - Highscores", font, 20);
        infoText.setFillColor(sf::Color::White);
        infoText.setPosition(300.f, 500.f);
        window.draw(infoText);
    } break;

    case GameState::YouWin: {
        youWinSprite.setScale(
            800.f / youWinSprite.getLocalBounds().width,
            600.f / youWinSprite.getLocalBounds().height
        );
        window.draw(youWinSprite);

        sf::Font font;
        font.loadFromFile("arial.ttf");

        sf::Text infoText("Wciśnij R, aby zagrać ponownie\nlub ESC, żeby wyjść\nF4 - Highscores", font, 20);
        infoText.setFillColor(sf::Color::White);
        sf::FloatRect it = infoText.getLocalBounds();
        infoText.setOrigin(it.width / 2.f, it.height / 2.f);
        infoText.setPosition(400.f, 550.f);

        // Cień dla tekstu
        sf::Text shadowText = infoText;
        shadowText.setFillColor(sf::Color::Black);
        shadowText.move(3.f, 3.f);
        window.draw(shadowText);
        window.draw(infoText);
    } break;

    case GameState::ExitConfirmation: {
        exitScreenSprite.setScale(
            800.f / exitScreenSprite.getLocalBounds().width,
            600.f / exitScreenSprite.getLocalBounds().height
        );
        window.draw(exitScreenSprite);

        sf::Font font;
        font.loadFromFile("arial.ttf");

        sf::Text exitText("Czy na pewno chcesz wyjść?\n\n  Wciśnij Y - TAK\n  Wciśnij N - NIE", font, 30);
        exitText.setFillColor(sf::Color::White);
        sf::FloatRect et = exitText.getLocalBounds();
        exitText.setOrigin(et.width / 2.f, et.height / 2.f);
        exitText.setPosition(400.f, 200.f);
        window.draw(exitText);
    } break;
    }

    window.display();
}

// ------------------------------------------------------
// moveEnemiesDown():
//  - kiedy wróg dojdzie do krawędzi, wszyscy schodzą w dół.
// ------------------------------------------------------
void Game::moveEnemiesDown()
{
    for (auto& enemy : enemies) {
        enemy.getShape().move(0.f, 10.f);
    }
}

// ------------------------------------------------------
// saveGameState(const std::string& filename):
//  - zapisuje bieżący stan gry do pliku (nick, wyniki, pozycje).
// ------------------------------------------------------
void Game::saveGameState(const std::string& filename)
{
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Błąd zapisu do pliku: " << filename << std::endl;
        return;
    }

    // Nick
    outFile << playerName << "\n";
    // Wynik + życie
    outFile << player.getScore() << " " << player.getLives() << "\n";
    // Pozycja gracza
    sf::Vector2f pPos = player.getPosition();
    outFile << pPos.x << " " << pPos.y << "\n";
    // Kierunek wrogów i obecny poziom
    outFile << direction << "\n";
    outFile << currentLevel << "\n";

    // Wrogowie
    outFile << enemies.size() << "\n";
    for (auto& en : enemies) {
        sf::Vector2f enPos = en.getPosition();
        outFile << enPos.x << " " << enPos.y << "\n";
    }

    // Pociski gracza
    outFile << bullets.size() << "\n";
    for (auto& b : bullets) {
        sf::FloatRect br = b.getBounds();
        outFile << br.left << " " << br.top << "\n";
    }

    // Pociski wroga
    outFile << enemyBullets.size() << "\n";
    for (auto& eb : enemyBullets) {
        sf::FloatRect ebr = eb.getBounds();
        outFile << ebr.left << " " << ebr.top << "\n";
    }

    outFile.close();
    std::cout << "[saveGameState] Zapisano do pliku: " << filename << std::endl;
}

// ------------------------------------------------------
// loadGameState(const std::string& filename):
//  - odtwarza stan gry z podanego pliku.
// ------------------------------------------------------
void Game::loadGameState(const std::string& filename)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Błąd odczytu z pliku: " << filename << std::endl;
        return;
    }

    std::getline(inFile, playerName);

    int score, lives;
    inFile >> score >> lives;
    player.setScore(score);
    player.setLives(lives);

    float px, py;
    inFile >> px >> py;
    player.setPosition(px, py);

    inFile >> direction;
    inFile >> currentLevel;

    // Wczytanie wrogów
    size_t eCount;
    inFile >> eCount;
    enemies.clear();
    enemies.reserve(eCount);
    for (size_t i = 0; i < eCount; i++) {
        float ex, ey;
        inFile >> ex >> ey;
        Enemy en(ex, ey);
        enemies.push_back(en);
    }

    // Wczytanie pocisków gracza
    size_t bCount;
    inFile >> bCount;
    bullets.clear();
    bullets.reserve(bCount);
    for (size_t i = 0; i < bCount; i++) {
        float bx, by;
        inFile >> bx >> by;
        Bullet bl(bx, by);
        bullets.push_back(bl);
    }

    // Wczytanie pocisków wroga
    size_t ebCount;
    inFile >> ebCount;
    enemyBullets.clear();
    enemyBullets.reserve(ebCount);
    for (size_t i = 0; i < ebCount; i++) {
        float ebx, eby;
        inFile >> ebx >> eby;
        EnemyBullet ebl(ebx, eby);
        enemyBullets.push_back(ebl);
    }

    inFile.close();
    std::cout << "[loadGameState] Odczytano z pliku: " << filename << std::endl;
}

// ------------------------------------------------------
// loadHighscoresFromFile(...):
//  - wczytuje dane z pliku (nick, wynik) do listy wyników.
// ------------------------------------------------------
void Game::loadHighscoresFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[loadHighscoresFromFile] Błąd otwarcia: " << filename << std::endl;
        return;
    }

    highscores.clear();

    std::string name;
    int score;
    while (file >> name >> score) {
        highscores.emplace_back(name, score);
    }
    file.close();
    std::cout << "[loadHighscoresFromFile] Wczytano wyniki." << std::endl;
}

// ------------------------------------------------------
// saveHighscoresToFile(...):
//  - zapisuje listę najlepszych wyników do pliku.
// ------------------------------------------------------
void Game::saveHighscoresToFile(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[saveHighscoresToFile] Błąd otwarcia: " << filename << std::endl;
        return;
    }
    for (auto& entry : highscores) {
        file << entry.name << " " << entry.score << "\n";
    }
    file.close();
    std::cout << "[saveHighscoresToFile] Zapisano wyniki." << std::endl;
}

// ------------------------------------------------------
// updateHighscores():
//  - dodaje obecny wynik gracza do listy,
//  - sortuje malejąco i przycina do Top 5.
// ------------------------------------------------------
void Game::updateHighscores()
{
    highscores.emplace_back(playerName, player.getScore());
    std::sort(highscores.begin(), highscores.end(),
        [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score > b.score;
        }
    );
    if (highscores.size() > 5) {
        highscores.resize(5);
    }
}

// ------------------------------------------------------
// showHighscoresWindow():
//  - otwiera osobne okno i wyświetla najlepsze wyniki.
// ------------------------------------------------------
void Game::showHighscoresWindow()
{
    sf::RenderWindow scoreboardWindow(sf::VideoMode(400, 400), "High Scores");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "[showHighscoresWindow] Błąd wczytywania czcionki.\n";
        return;
    }

    while (scoreboardWindow.isOpen()) {
        sf::Event ev;
        while (scoreboardWindow.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                scoreboardWindow.close();
            }
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
                scoreboardWindow.close();
            }
        }

        scoreboardWindow.clear(sf::Color(50, 50, 50));

        sf::Text title("Najlepsze Wyniki", font, 30);
        title.setFillColor(sf::Color::Yellow);
        title.setPosition(20.f, 20.f);
        scoreboardWindow.draw(title);

        float startY = 80.f;
        for (size_t i = 0; i < highscores.size(); i++) {
            sf::Text entry(
                std::to_string(i + 1) + ". " +
                highscores[i].name + " - " +
                std::to_string(highscores[i].score),
                font, 24
            );
            entry.setFillColor(sf::Color::White);
            entry.setPosition(20.f, startY + i * 30.f);
            scoreboardWindow.draw(entry);
        }

        scoreboardWindow.display();
    }
}

