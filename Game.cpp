#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm> // std::sort (do sortowania highscore)

// ------------------------------------------------------
// Konstruktor Game() - inicjalizuje wszystkie zasoby gry
// i ustawia wartości początkowe (stan, poziom, prędkości).
// ------------------------------------------------------
Game::Game()
    : window(sf::VideoMode(800, 600), "Space Invaders"),
    direction(1),            // Kierunek poruszania wrogów: 1 = w prawo, -1 = w lewo
    lastEnemyBulletTime(0.f),
    enemyToShoot(-1),        // Indeks wroga do strzału (na starcie: żaden)
    currentState(GameState::StartScreen),
    isPaused(false),
    playerName(""),          // Nick gracza (wpisywany na ekranie startowym)
    currentLevel(1),         // Rozpoczynamy od poziomu 1
    enemySpeed(0.1f)         // Podstawowa wartość do skalowania prędkości wrogów
{
    // --------------------------
    // Wczytywanie plików tekstur tła
    // --------------------------
    if (!backgroundTexture.loadFromFile("tlo.jpg")) {
        std::cerr << "Nie udalo sie zaladowac tla gry (tlo.jpg)!" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    if (!startScreenTexture.loadFromFile("start.jpg")) {
        std::cerr << "Nie udalo sie zaladowac tla ekranu startowego (start.jpg)!" << std::endl;
    }
    startScreenSprite.setTexture(startScreenTexture);

    if (!helpScreenTexture.loadFromFile("help.jpg")) {
        std::cerr << "Nie udalo sie zaladowac tla ekranu pomocy (help.jpg)!" << std::endl;
    }
    helpScreenSprite.setTexture(helpScreenTexture);

    if (!exitScreenTexture.loadFromFile("exit.png")) {
        std::cerr << "Nie udalo sie zaladowac tla ekranu wyjscia (exit.png)!" << std::endl;
    }
    exitScreenSprite.setTexture(exitScreenTexture);

    if (!gameOverTexture.loadFromFile("gameover.png")) {
        std::cerr << "Nie udalo sie zaladowac tla ekranu GameOver (gameover.png)!" << std::endl;
    }
    gameOverSprite.setTexture(gameOverTexture);

    // --------------------------
    // Wczytywanie dźwięku strzału (shoot.wav)
    // --------------------------
    if (!shootBuffer.loadFromFile("shoot.wav")) {
        std::cerr << "Nie udalo sie zaladowac pliku shoot.wav" << std::endl;
    }
    else {
        shootSound.setBuffer(shootBuffer);
    }

    // --------------------------
    // Tło ekranu YOU WIN
    // --------------------------
    if (!youWinTexture.loadFromFile("youwin.png")) {
        std::cerr << "Nie udalo sie zaladowac tla ekranu YOU WIN (youwin.png)!" << std::endl;
    }
    youWinSprite.setTexture(youWinTexture);
    // Skalowanie do rozmiaru 800×600
    youWinSprite.setScale(
        800.f / youWinSprite.getLocalBounds().width,
        600.f / youWinSprite.getLocalBounds().height
    );

    // --------------------------
    // Wczytanie listy najlepszych wyników (highscores.txt)
    // --------------------------
    loadHighscoresFromFile("highscores.txt");

    // Przygotowanie pierwszego poziomu (wrogowie, itp.)
    setupLevel();

    // Ustawienie generatora liczb pseudolosowych (dla strzałów wroga)
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void Game::skipLevel()
{
    // 1. Wyczyszczamy listę wrogów i pocisków,
    //    bo chcemy "przewinąć" ten poziom
    enemies.clear();
    bullets.clear();
    enemyBullets.clear();

    // 2. Zwiększamy aktualny poziom
    currentLevel++;

    // 3. Jeśli wyjdziemy poza ostatni poziom, przechodzimy do ekranu YouWin
    if (currentLevel > 3) {
        currentState = GameState::YouWin;
    }
    else {
        // W przeciwnym razie przygotowujemy nowy układ wrogów
        // i pocisków w setupLevel()
        setupLevel();
    }
}


// ------------------------------------------------------
// resetGame() - przywraca grę do wartości początkowych
// (np. po przegranej) - ustawia gracza, wrogów, poziom.
// ------------------------------------------------------
void Game::resetGame() {
    // Reset punktów i żyć gracza
    player.setScore(0);
    player.setLives(3);
    // Ustaw pozycję statku na środek dolnej krawędzi
    player.setPosition(375.f, 550.f);

    // Wracamy do poziomu 1
    currentLevel = 1;
    // Tworzymy wrogów i czyszczymy pociski
    setupLevel();

    // Przywrócenie kierunku i indeksu strzelającego wroga
    direction = 1;
    enemyToShoot = -1;
    lastEnemyBulletTime = 0.f;
}

// ------------------------------------------------------
// setupLevel() - tworzy wrogów i ustawia ich parametry
//                zależnie od aktualnego poziomu.
// ------------------------------------------------------
void Game::setupLevel() {
    // Czyścimy stare wrogów i pociski (przy przechodzeniu między poziomami)
    enemies.clear();
    bullets.clear();
    enemyBullets.clear();

    // Przy każdym nowym poziomie (w tej grze) dajemy 3 życia
    player.setLives(3);

    // Pozycja startowa pierwszego wroga (lewy górny)
    float startX = 100.f;
    float startY = 50.f;

    // Domyślnie 3 rzędy, ale tu manipulujemy 
    int rows = 3;
    if (currentLevel == 1) {
        rows = 3;
    }
    else if (currentLevel == 2) {
        rows = 4;
    }
    else if (currentLevel == 3) {
        rows = 4;
    }

    // Tworzymy wrogów w wierszach i kolumnach
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 11; j++) {
            Enemy enemy(startX + j * 60.f, startY + i * 60.f);

            // Na podstawie poziomu wrogowie mają różny speedMultiplier i cooldown
            if (currentLevel == 1) {
                enemy.setSpeedMultiplier(0.5f);
                enemy.setBulletCooldown(2.f);
            }
            else if (currentLevel == 2) {
                enemy.setSpeedMultiplier(1.f);
                enemy.setBulletCooldown(1.f);
            }
            else if (currentLevel == 3) {
                enemy.setSpeedMultiplier(1.5f);
                enemy.setBulletCooldown(0.5f);
            }

            enemies.push_back(enemy);
        }
    }
}

// ------------------------------------------------------
// run() - główna pętla gry 
// ------------------------------------------------------
void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        // Delta time (czas pomiędzy klatkami) do ruchu obiektów
        sf::Time deltaTime = clock.restart();

        // (1) Obsługa zdarzeń
        processEvents();
        // (2) Aktualizacja logiki gry (o ile nie jest wstrzymana)
        if (!isPaused) {
            update(deltaTime.asSeconds());
        }
        // (3) Renderowanie (rysowanie) na ekranie
        render();
    }
}

// ------------------------------------------------------
// processEvents() - obsługa zdarzeń (klawisze, myszka,
// zamykanie okna) - różna w zależności od stanu gry.
// ------------------------------------------------------
void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        // Zamknięcie okna (X w prawym górnym rogu)
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Switch po aktualnym stanie gry (StartScreen, Playing, itd.)
        switch (currentState) {

            // -----------------------------
            // 1) EKRAN STARTOWY (StartScreen)
            // -----------------------------
        case GameState::StartScreen:
            if (event.type == sf::Event::TextEntered) {
                // Wpisywanie nicku 
                if (event.text.unicode >= 32 && event.text.unicode < 127) {
                    playerName += static_cast<char>(event.text.unicode);
                }
                else if (event.text.unicode == 8) { // Backspace
                    if (!playerName.empty()) {
                        playerName.pop_back();
                    }
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                // Enter/Spacja = przejście do gry
                if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
                    std::cout << "Wpisany nick: " << playerName << std::endl;
                    currentState = GameState::Playing;
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::F3) {
                    // Wczytanie stanu gry z pliku
                    loadGameState("save.txt");
                }
                else if (event.key.code == sf::Keyboard::F4) {
                    // Otwieramy okno HighScores
                    showHighscoresWindow();
                }
            }
            break;

            // -----------------------------
            // 2) GŁÓWNA GRA (Playing)
            // -----------------------------
        case GameState::Playing:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    // Odtwórz dźwięk strzału i wystrzel pocisk
                    shootSound.play();
                    player.shoot(bullets);
                }
                else if (event.key.code == sf::Keyboard::F1) {
                    // Pomoc
                    currentState = GameState::Help;
                    isPaused = true;
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    // Wyjście (potwierdzenie)
                    currentState = GameState::ExitConfirmation;
                    isPaused = true;
                }
                else if (event.key.code == sf::Keyboard::F2) {
                    // Zapis stanu gry
                    saveGameState("save.txt");
                }
                else if (event.key.code == sf::Keyboard::F3) {
                    // Wczytanie stanu gry
                    loadGameState("save.txt");
                }
                else if (event.key.code == sf::Keyboard::F4) {
                    // Pokazanie highscore w trakcie gry
                    showHighscoresWindow();
                }
                else if (event.key.code == sf::Keyboard::F9) {
                    
                    skipLevel();
                }

            }
            break;

            // -----------------------------
            // 3) EKRAN POMOCY (Help)
            // -----------------------------
        case GameState::Help:
            if (event.type == sf::Event::KeyPressed) {
                // Spacja lub F1 - wyjście z pomocy i powrót do gry
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::F1) {
                    currentState = GameState::Playing;
                    isPaused = false;
                }
            }
            break;

            // -----------------------------
            // 4) EKRAN GAME OVER
            // -----------------------------
        case GameState::GameOver:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::R) {
                    // Restart gry, powrót do ekranu startowego
                    resetGame();
                    currentState = GameState::StartScreen;
                }
                else if (event.key.code == sf::Keyboard::F4) {
                    // Okno HighScores
                    showHighscoresWindow();
                }
            }
            break;

            // -----------------------------
            // 5) EKRAN YOU WIN
            // -----------------------------
        case GameState::YouWin:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::R) {
                    // Zagraj ponownie, wróć do startu
                    resetGame();
                    currentState = GameState::StartScreen;
                }
                else if (event.key.code == sf::Keyboard::F4) {
                    showHighscoresWindow();
                }
            }
            break;

            // -----------------------------
            // 6) EKRAN POTWIERDZENIA WYJŚCIA
            // -----------------------------
        case GameState::ExitConfirmation:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Y) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::N) {
                    // Kontynuacja gry
                    currentState = GameState::Playing;
                    isPaused = false;
                }
            }
            break;
        }
    }
}

// ------------------------------------------------------
// update(float deltaTime) - aktualizacja logiki:
//  - ruch gracza i wrogów
//  - obsługa kolizji
//  - sprawdzanie wygranej / przegranej
// ------------------------------------------------------
void Game::update(float deltaTime) {
    // Aktualizujemy tylko w stanie Playing
    if (currentState == GameState::Playing) {
        // Ruch i logika statku
        player.update(deltaTime);

        // Ruch horyzontalny wrogów (sprawdzanie krawędzi)
        if (direction == 1) {
            // Prawa krawędź
            if (!enemies.empty() &&
                (enemies.back().getBounds().left + enemies.back().getBounds().width >= window.getSize().x)) {
                direction = -1;
                moveEnemiesDown();
            }
        }
        else if (direction == -1) {
            // Lewa krawędź
            if (!enemies.empty() &&
                (enemies.front().getBounds().left <= 0)) {
                direction = 1;
                moveEnemiesDown();
            }
        }

        // Aktualizacja pozycji wrogów
        for (auto& enemy : enemies) {
            enemy.update(direction, deltaTime * enemySpeed);

            // Jeśli wróg dotrze do dołu ekranu -> GameOver
            float enemyBottom = enemy.getBounds().top + enemy.getBounds().height;
            if (enemyBottom >= window.getSize().y) {
                currentState = GameState::GameOver;

                // Aktualizacja highscore i zapis
                updateHighscores();
                saveHighscoresToFile("highscores.txt");

                return;
            }
        }

        // Sprawdzenie, czy wrogowie się skończyli -> następny poziom
        if (enemies.empty()) {
            currentLevel++;
            if (currentLevel > 3) {
                // WYGRANA
                currentState = GameState::YouWin;
                updateHighscores();
                saveHighscoresToFile("highscores.txt");
                return;
            }
            // Inaczej przygotuj kolejny poziom
            setupLevel();
        }

        // Częstotliwość strzału wroga zależna od poziomu
        float enemyCooldown = (currentLevel == 1) ? 2.0f :
            (currentLevel == 2) ? 1.5f : 1.0f;

        // Losowy wróg strzela
        if (enemyToShoot == -1 || enemies[enemyToShoot].canShoot()) {
            enemyToShoot = std::rand() % enemies.size();
            enemies[enemyToShoot].setCooldown(enemyCooldown);

            // Dodaj pocisk wroga
            enemyBullets.push_back(EnemyBullet(
                enemies[enemyToShoot].getPosition().x + 20.f,
                enemies[enemyToShoot].getPosition().y + 20.f
            ));
            // Na poziomach >= 2 - dodatkowy pocisk
            if (currentLevel >= 2) {
                enemyBullets.push_back(EnemyBullet(
                    enemies[enemyToShoot].getPosition().x - 20.f,
                    enemies[enemyToShoot].getPosition().y + 20.f
                ));
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
                    // Trafienie wroga -> usuwamy wroga, dodajemy punkty
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
                // Gracz traci życie
                it = enemyBullets.erase(it);
                player.loseLife();

                if (player.getLives() <= 0) {
                    // Przegrana
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
// render() - rysuje odpowiedni ekran w zależności
//            od stanu gry (StartScreen, Playing, itp.).
// ------------------------------------------------------
void Game::render() {
    // Czyścimy bufor okna
    window.clear();

    // Switch po aktualnym stanie gry
    if (currentState == GameState::StartScreen) {
        // Ekran startowy
        window.draw(startScreenSprite);

        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Blad ladowania czcionki arial.ttf" << std::endl;
        }

        // Tytuł (SPACE INVADERS)
        sf::Text title("SPACE INVADERS", font, 60);
        title.setFillColor(sf::Color::Yellow);
        sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin(tb.width / 2.f, tb.height / 2.f);
        title.setPosition(400.f, 120.f);

        // Cień tytułu
        sf::Text shadow = title;
        shadow.setFillColor(sf::Color::Black);
        shadow.move(3.f, 3.f);
        window.draw(shadow);
        window.draw(title);

        // Instrukcja wpisania nicku
        sf::Text info("Wpisz swoj nick i nacisnij ENTER (lub SPACJE)", font, 24);
        info.setFillColor(sf::Color(255, 255, 150));
        sf::FloatRect ib = info.getLocalBounds();
        info.setOrigin(ib.width / 2.f, ib.height / 2.f);
        info.setPosition(400.f, 220.f);
        window.draw(info);

        // Wyświetlenie aktualnie wpisywanego nicku
        sf::Text nick(playerName, font, 30);
        nick.setFillColor(sf::Color::Cyan);
        sf::FloatRect nb = nick.getLocalBounds();
        nick.setOrigin(nb.width / 2.f, nb.height / 2.f);
        nick.setPosition(400.f, 280.f);
        window.draw(nick);

        // Dodatkowe info (ESC, F3, F4)
        sf::Text bottom("ESC - wyjscie | F3 - wczytaj stan gry | F4 - Highscores", font, 18);
        sf::FloatRect bb = bottom.getLocalBounds();
        bottom.setOrigin(bb.width / 2.f, bb.height / 2.f);
        bottom.setPosition(400.f, 550.f);
        bottom.setFillColor(sf::Color(220, 220, 220));
        window.draw(bottom);
    }
    else if (currentState == GameState::Playing) {
        // Tło gry
        backgroundSprite.setScale(
            800.f / backgroundSprite.getLocalBounds().width,
            600.f / backgroundSprite.getLocalBounds().height
        );
        window.draw(backgroundSprite);

        // Rysowanie statku gracza, wrogów i pocisków
        player.render(window);
        for (auto& e : enemies) e.render(window);
        for (auto& b : bullets) b.render(window);
        for (auto& eb : enemyBullets) eb.render(window);

        // Dodatkowy tekst (wynik, życia, nick, poziom)
        sf::Font font;
        font.loadFromFile("arial.ttf");

        sf::Text scoreText("Wynik: " + std::to_string(player.getScore()), font, 20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10.f, 10.f);
        window.draw(scoreText);

        sf::Text livesText("Zycia: " + std::to_string(player.getLives()), font, 20);
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
    }
    else if (currentState == GameState::Help) {
        // Ekran pomocy
        helpScreenSprite.setScale(
            800.f / helpScreenSprite.getLocalBounds().width,
            600.f / helpScreenSprite.getLocalBounds().height
        );
        window.draw(helpScreenSprite);

        sf::Font font;
        font.loadFromFile("arial.ttf");

        // Tytuł "EKRAN POMOCY"
        sf::Text helpTitle("EKRAN POMOCY", font, 40);
        helpTitle.setFillColor(sf::Color::Yellow);
        sf::FloatRect ht = helpTitle.getLocalBounds();
        helpTitle.setOrigin(ht.width / 2.f, ht.height / 2.f);
        helpTitle.setPosition(400.f, 50.f);
        window.draw(helpTitle);

        // Informacje o sterowaniu
        sf::Text helpInfo(
            "Sterowanie:\n"
            "- Strzalki: ruch statkiem\n"
            "- Spacja: strzal\n"
            "- F1: powrot do gry\n"
            "- F2: zapis stanu gry\n"
            "- ESC: wywolanie ekranu wyjscia\n\n"
            "Zasady gry:\n"
            "- Strzelaj do wrogow i unikaj ich pociskow.\n"
            "- Kazdy trafiony wrog = +100 do wyniku.\n"
            "- Masz ograniczona liczbe zyc (3).",
            font, 24
        );
        helpInfo.setFillColor(sf::Color::White);
        helpInfo.setPosition(50.f, 120.f);
        window.draw(helpInfo);

        // Komunikat powrotu
        sf::Text back("Nacisnij SPACJE lub F1, aby wrocic do gry", font, 20);
        back.setFillColor(sf::Color(200, 200, 200));
        sf::FloatRect bb = back.getLocalBounds();
        back.setOrigin(bb.width / 2.f, bb.height / 2.f);
        back.setPosition(400.f, 550.f);
        window.draw(back);
    }
    else if (currentState == GameState::GameOver) {
        // Ekran "Game Over"
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

        sf::Text infoText("Wcisnij R, aby zagrac ponownie\nlub ESC, aby wyjsc\nF4 - Highscores", font, 20);
        infoText.setFillColor(sf::Color::White);
        infoText.setPosition(300.f, 500.f);
        window.draw(infoText);
    }
    else if (currentState == GameState::YouWin) {
        // Ekran zwycięstwa
        youWinSprite.setScale(
            800.f / youWinSprite.getLocalBounds().width,
            600.f / youWinSprite.getLocalBounds().height
        );
        window.draw(youWinSprite);

        sf::Font font;
        font.loadFromFile("arial.ttf");

        sf::Text infoText("Wcisnij R, aby zagrac ponownie\nlub ESC, aby wyjsc\nF4 - Highscores", font, 20);
        infoText.setFillColor(sf::Color::White);
        sf::FloatRect it = infoText.getLocalBounds();
        infoText.setOrigin(it.width / 2.f, it.height / 2.f);
        infoText.setPosition(400.f, 550.f);

        // Cień tekstu
        sf::Text shadowText = infoText;
        shadowText.setFillColor(sf::Color::Black);
        shadowText.move(3.f, 3.f);
        window.draw(shadowText);
        window.draw(infoText);
    }
    else if (currentState == GameState::ExitConfirmation) {
        // Ekran wyjścia
        exitScreenSprite.setScale(
            800.f / exitScreenSprite.getLocalBounds().width,
            600.f / exitScreenSprite.getLocalBounds().height
        );
        window.draw(exitScreenSprite);

        sf::Font font;
        font.loadFromFile("arial.ttf");

        sf::Text exitText("Czy chcesz wyjsc z gry?\n\n       Wcisnij Y - TAK\n       Wcisnij N - NIE", font, 30);
        exitText.setFillColor(sf::Color::White);
        sf::FloatRect et = exitText.getLocalBounds();
        exitText.setOrigin(et.width / 2.f, et.height / 2.f);
        exitText.setPosition(400.f, 200.f);
        window.draw(exitText);
    }

    // Prezentacja narysowanej zawartości
    window.display();
}

// ------------------------------------------------------
// moveEnemiesDown() - przesuwa wszystkich wrogów w dół
// (po dojściu do krawędzi ekranu).
// ------------------------------------------------------
void Game::moveEnemiesDown() {
    for (auto& enemy : enemies) {
        enemy.getShape().move(0.f, 10.f);
    }
}

// ------------------------------------------------------
// saveGameState(...) - zapis stanu gry do pliku tekstowego
// (nick, wynik, pozycja gracza, wrogów, pocisków).
// ------------------------------------------------------
void Game::saveGameState(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Nie udalo sie otworzyc pliku do zapisu: " << filename << std::endl;
        return;
    }

    // 1) Nick
    outFile << playerName << "\n";
    // 2) Score i życia
    outFile << player.getScore() << " " << player.getLives() << "\n";
    // 3) Pozycja gracza
    sf::Vector2f pPos = player.getPosition();
    outFile << pPos.x << " " << pPos.y << "\n";
    // 4) Kierunek wrogów + poziom
    outFile << direction << "\n";
    outFile << currentLevel << "\n";

    // 5) Liczba wrogów i ich pozycje
    outFile << enemies.size() << "\n";
    for (auto& en : enemies) {
        sf::Vector2f enPos = en.getPosition();
        outFile << enPos.x << " " << enPos.y << "\n";
    }

    // 6) Pociski gracza
    outFile << bullets.size() << "\n";
    for (auto& b : bullets) {
        sf::FloatRect br = b.getBounds();
        outFile << br.left << " " << br.top << "\n";
    }

    // 7) Pociski wroga
    outFile << enemyBullets.size() << "\n";
    for (auto& eb : enemyBullets) {
        sf::FloatRect ebr = eb.getBounds();
        outFile << ebr.left << " " << ebr.top << "\n";
    }

    outFile.close();
    std::cout << "[saveGameState] Zapisano do: " << filename << std::endl;
}

// ------------------------------------------------------
// loadGameState(...) - wczytanie stanu gry z pliku
// (odtwarzamy nick, wynik, poziom, wrogów, pociski).
// ------------------------------------------------------
void Game::loadGameState(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Nie udalo sie otworzyc pliku do odczytu: " << filename << std::endl;
        return;
    }

    // 1) Nick
    std::getline(inFile, playerName);

    // 2) Score i życie
    int score, lives;
    inFile >> score >> lives;
    player.setScore(score);
    player.setLives(lives);

    // 3) Pozycja gracza
    float px, py;
    inFile >> px >> py;
    player.setPosition(px, py);

    // 4) Kierunek i poziom
    inFile >> direction;
    inFile >> currentLevel;

    // 5) Wrogowie
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

    // 6) Pociski gracza
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

    // 7) Pociski wroga
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
    std::cout << "[loadGameState] Odczytano z: " << filename << std::endl;
}

// ------------------------------------------------------
// loadHighscoresFromFile(...) - wczytuje listę rekordów
// (nick + wynik) z pliku np. highscores.txt
// ------------------------------------------------------
void Game::loadHighscoresFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[loadHighscoresFromFile] Nie udalo sie otworzyc pliku: "
            << filename << std::endl;
        return;
    }

    highscores.clear();

    std::string name;
    int score;
    // Odczyt wiersz po wierszu: "nick wynik"
    while (file >> name >> score) {
        highscores.emplace_back(name, score);
    }
    file.close();
    std::cout << "[loadHighscoresFromFile] Wczytano wyniki." << std::endl;
}

// ------------------------------------------------------
// saveHighscoresToFile(...) - zapisuje listę rekordów
// do pliku (format: nick wyniki).
// ------------------------------------------------------
void Game::saveHighscoresToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[saveHighscoresToFile] Nie udalo sie otworzyc pliku: "
            << filename << std::endl;
        return;
    }

    // Zapis Nick Score linia po linii
    for (auto& entry : highscores) {
        file << entry.name << " " << entry.score << "\n";
    }
    file.close();
    std::cout << "[saveHighscoresToFile] Zapisano wyniki." << std::endl;
}

// ------------------------------------------------------
// updateHighscores() - dodaje aktualny wynik gracza
// do tabeli, sortuje i przycina do top 5.
// ------------------------------------------------------
void Game::updateHighscores() {
    highscores.emplace_back(playerName, player.getScore());

    // Sortujemy malejąco po polu score
    std::sort(highscores.begin(), highscores.end(),
        [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score > b.score;
        });

    // Zachowujemy tylko 5 najlepszych wyników
    if (highscores.size() > 5) {
        highscores.resize(5);
    }
}

// ------------------------------------------------------
// showHighscoresWindow() - otwiera osobne okno SFML
// i wyświetla tablicę wyników (highscore).
// ------------------------------------------------------
void Game::showHighscoresWindow() {
    // Nowe okno 400×400
    sf::RenderWindow scoreboardWindow(sf::VideoMode(400, 400), "High Scores");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "[showHighscoresWindow] Blad wczytywania czcionki.\n";
        return;
    }

    // Pętla zdarzeń w drugim oknie
    while (scoreboardWindow.isOpen()) {
        sf::Event ev;
        while (scoreboardWindow.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                scoreboardWindow.close();
            }
            // ESC => zamknięcie okna HighScores
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
                scoreboardWindow.close();
            }
        }

        // Rysowanie listy wyników w oknie
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
