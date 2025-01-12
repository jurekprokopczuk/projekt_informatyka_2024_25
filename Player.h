#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"

// Klasa Player reprezentuje statek gracza.
// Odpowiada za:
// 1) Ruch w poziomie (lew/prawa strza�ka),
// 2) Strzelanie (pociski gracza),
// 3) Przechowywanie wyniku i liczby �y�.
class Player {
public:
    // Konstruktor wczytuje tekstur� statku (ship.png)
    // i ustawia domy�lne warto�ci (pr�dko��, score, lives).
    Player();

    // update(float deltaTime):
    // Aktualizuje po�o�enie statku,
    // przetwarza klawisze (Left/Right) i cooldown strza�u.
    void update(float deltaTime);

    // render(sf::RenderWindow& window):
    // Rysuje sprite statku gracza w oknie.
    void render(sf::RenderWindow& window);

    // shoot(std::vector<Bullet>& bullets):
    // Gdy cooldown <= 0, dodaje nowy pocisk do wektora bullets,
    // ustawia cooldown na domy�ln� warto�� (np. 0.5 sek).
    void shoot(std::vector<Bullet>& bullets);

    // getBounds():
    // Zwraca prostok�t kolizji (bounding box) sprite'a.
    // Wykorzystywane np. do wykrycia trafienia przez pocisk wroga.
    sf::FloatRect getBounds() const;

    // getPosition():
    // Zwraca aktualn� pozycj� (x,y) sprite'a.
    sf::Vector2f getPosition() const;

    // setPosition(x, y):
    // Ustawia pozycj� statku w oknie.
    void setPosition(float x, float y);

    // increaseScore(amount):
    // Dodaje podan� liczb� punkt�w do aktualnego wyniku.
    void increaseScore(int amount);

    // loseLife():
    // Zmniejsza liczb� �y� o 1.
    void loseLife();

    // getScore():
    // Zwraca bie��cy wynik (punkty).
    int getScore() const;

    // getLives():
    // Zwraca liczb� pozosta�ych �y�.
    int getLives() const;

    // setScore(newScore):
    // Ustawia wynik na podan� warto��.
    void setScore(int newScore);

    // setLives(newLives):
    // Ustawia liczb� �y� na newLives.
    void setLives(int newLives);

    // setDeltaTime(dt):
    // (Opcjonalnie) ustawia zmienn� deltaTime, je�li chcemy j� przechowywa�
    // w klasie Player.
    void setDeltaTime(float dt);

private:
    // sprite - obrazek statku,
    // wczytywany z pliku "ship.png".
    sf::Sprite sprite;

    // texture - tekstura statku gracza (po kt�rej sprite jest tworzony).
    sf::Texture texture;

    // speed - pr�dko�� statku w osi X.
    float speed;

    // shootCooldown - czas do kolejnego strza�u (po wystrzale ustawiany np. na 0.5 sek).
    float shootCooldown;

    // deltaTime - przechowuje czas mi�dzy klatkami (opcjonalne u�ycie).
    float deltaTime;

    // score - bie��cy wynik gracza (trafienia).
    int score;

    // lives - liczba �y� gracza.
    int lives;
};
