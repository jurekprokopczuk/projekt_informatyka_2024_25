#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"

// Klasa Player reprezentuje statek gracza.
// Odpowiada za:
// 1) Ruch w poziomie (lew/prawa strza³ka),
// 2) Strzelanie (pociski gracza),
// 3) Przechowywanie wyniku i liczby ¿yæ.
class Player {
public:
    // Konstruktor wczytuje teksturê statku (ship.png)
    // i ustawia domyœlne wartoœci (prêdkoœæ, score, lives).
    Player();

    // update(float deltaTime):
    // Aktualizuje po³o¿enie statku,
    // przetwarza klawisze (Left/Right) i cooldown strza³u.
    void update(float deltaTime);

    // render(sf::RenderWindow& window):
    // Rysuje sprite statku gracza w oknie.
    void render(sf::RenderWindow& window);

    // shoot(std::vector<Bullet>& bullets):
    // Gdy cooldown <= 0, dodaje nowy pocisk do wektora bullets,
    // ustawia cooldown na domyœln¹ wartoœæ (np. 0.5 sek).
    void shoot(std::vector<Bullet>& bullets);

    // getBounds():
    // Zwraca prostok¹t kolizji (bounding box) sprite'a.
    // Wykorzystywane np. do wykrycia trafienia przez pocisk wroga.
    sf::FloatRect getBounds() const;

    // getPosition():
    // Zwraca aktualn¹ pozycjê (x,y) sprite'a.
    sf::Vector2f getPosition() const;

    // setPosition(x, y):
    // Ustawia pozycjê statku w oknie.
    void setPosition(float x, float y);

    // increaseScore(amount):
    // Dodaje podan¹ liczbê punktów do aktualnego wyniku.
    void increaseScore(int amount);

    // loseLife():
    // Zmniejsza liczbê ¿yæ o 1.
    void loseLife();

    // getScore():
    // Zwraca bie¿¹cy wynik (punkty).
    int getScore() const;

    // getLives():
    // Zwraca liczbê pozosta³ych ¿yæ.
    int getLives() const;

    // setScore(newScore):
    // Ustawia wynik na podan¹ wartoœæ.
    void setScore(int newScore);

    // setLives(newLives):
    // Ustawia liczbê ¿yæ na newLives.
    void setLives(int newLives);

    // setDeltaTime(dt):
    // (Opcjonalnie) ustawia zmienn¹ deltaTime, jeœli chcemy j¹ przechowywaæ
    // w klasie Player.
    void setDeltaTime(float dt);

private:
    // sprite - obrazek statku,
    // wczytywany z pliku "ship.png".
    sf::Sprite sprite;

    // texture - tekstura statku gracza (po której sprite jest tworzony).
    sf::Texture texture;

    // speed - prêdkoœæ statku w osi X.
    float speed;

    // shootCooldown - czas do kolejnego strza³u (po wystrzale ustawiany np. na 0.5 sek).
    float shootCooldown;

    // deltaTime - przechowuje czas miêdzy klatkami (opcjonalne u¿ycie).
    float deltaTime;

    // score - bie¿¹cy wynik gracza (trafienia).
    int score;

    // lives - liczba ¿yæ gracza.
    int lives;
};
