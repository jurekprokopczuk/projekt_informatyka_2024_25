#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "EnemyBullet.h"

// Nowa struktura do przechowywania pozycji w tabeli wynik�w
struct ScoreEntry {
    std::string name;
    int score;

    // Konstruktor domy�lny
    ScoreEntry() : name(""), score(0) {}

    // Konstruktor z parametrami
    ScoreEntry(const std::string& n, int s)
        : name(n), score(s) {
    }
};

enum class GameState {
    StartScreen,
    Playing,
    Help,
    GameOver,
    ExitConfirmation,
    YouWin
};

class Game {
public:
    Game();                  // Konstruktor

    void run();             // G��wna p�tla aplikacji

private:
    // Obs�uga zdarze�, logika i rysowanie
    void processEvents();
    void update(float deltaTime);
    void render();

    // Ruch wrog�w w d� (gdy dojd� do kraw�dzi)
    void moveEnemiesDown();

    // Zapisywanie / wczytywanie stanu gry
    void saveGameState(const std::string& filename);
    void loadGameState(const std::string& filename);

    // Zapisywanie / wczytywanie listy wynik�w
    void loadHighscoresFromFile(const std::string& filename);
    void saveHighscoresToFile(const std::string& filename);
    void updateHighscores();

    // NOWA metoda tworz�ca drugie okno SFML z list� wynik�w
    void showHighscoresWindow();

    // Resetuje stan gry do warto�ci pocz�tkowych
    void resetGame();

    // Przygotowuje wrog�w i ustawienia dla danego poziomu
    void setupLevel();

    //funkcja pomocnicza do zmiany lvl
    void skipLevel();


    // Okno
    sf::RenderWindow window;

    // Tekstury i sprity t�a
    sf::Texture backgroundTexture;
    sf::Texture startScreenTexture;
    sf::Texture helpScreenTexture;
    sf::Texture exitScreenTexture;
    sf::Texture gameOverTexture;
    sf::Texture youWinTexture;

    sf::Sprite gameOverSprite;
    sf::Sprite backgroundSprite;
    sf::Sprite startScreenSprite;
    sf::Sprite helpScreenSprite;
    sf::Sprite exitScreenSprite;
    sf::Sprite youWinSprite;

    // Obiekty gry
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<EnemyBullet> enemyBullets;

    // Kierunek ruchu wrog�w (1 - w prawo, -1 - w lewo)
    int direction;

    // Indeks wroga, kt�ry ma strzeli�
    int enemyToShoot;

    // Aktualny poziom
    int currentLevel;

    // Aktualny stan gry
    GameState currentState;

    // Czy gra jest wstrzymana (np. gdy wy�wietlamy pomoc)
    bool isPaused;

    // (opcjonalnie) czas od ostatniego pocisku wroga
    float lastEnemyBulletTime;

    // Szybko�� wrog�w
    float enemySpeed;

    // Nick gracza
    std::string playerName;

    // ------------------------
    // Lista wynik�w graczy
    // ------------------------
    std::vector<ScoreEntry> highscores;
    //dzwieki
    sf::SoundBuffer shootBuffer;
    sf::Sound shootSound;
};
