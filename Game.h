#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "EnemyBullet.h"

// Nowa struktura do przechowywania pozycji w tabeli wyników
struct ScoreEntry {
    std::string name;
    int score;

    // Konstruktor domyœlny
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

    void run();             // G³ówna pêtla aplikacji

private:
    // Obs³uga zdarzeñ, logika i rysowanie
    void processEvents();
    void update(float deltaTime);
    void render();

    // Ruch wrogów w dó³ (gdy dojd¹ do krawêdzi)
    void moveEnemiesDown();

    // Zapisywanie / wczytywanie stanu gry
    void saveGameState(const std::string& filename);
    void loadGameState(const std::string& filename);

    // Zapisywanie / wczytywanie listy wyników
    void loadHighscoresFromFile(const std::string& filename);
    void saveHighscoresToFile(const std::string& filename);
    void updateHighscores();

    // NOWA metoda tworz¹ca drugie okno SFML z list¹ wyników
    void showHighscoresWindow();

    // Resetuje stan gry do wartoœci pocz¹tkowych
    void resetGame();

    // Przygotowuje wrogów i ustawienia dla danego poziomu
    void setupLevel();

    //funkcja pomocnicza do zmiany lvl
    void skipLevel();


    // Okno
    sf::RenderWindow window;

    // Tekstury i sprity t³a
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

    // Kierunek ruchu wrogów (1 - w prawo, -1 - w lewo)
    int direction;

    // Indeks wroga, który ma strzeliæ
    int enemyToShoot;

    // Aktualny poziom
    int currentLevel;

    // Aktualny stan gry
    GameState currentState;

    // Czy gra jest wstrzymana (np. gdy wyœwietlamy pomoc)
    bool isPaused;

    // (opcjonalnie) czas od ostatniego pocisku wroga
    float lastEnemyBulletTime;

    // Szybkoœæ wrogów
    float enemySpeed;

    // Nick gracza
    std::string playerName;

    // ------------------------
    // Lista wyników graczy
    // ------------------------
    std::vector<ScoreEntry> highscores;
    //dzwieki
    sf::SoundBuffer shootBuffer;
    sf::Sound shootSound;
};
