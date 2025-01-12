#pragma once

#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(float x, float y);  // Konstruktor

    void update();  // Funkcja aktualizuj¹ca pozycjê pocisku
    void render(sf::RenderWindow& window);  // Funkcja renderuj¹ca pocisk
    sf::FloatRect getBounds() const;  // Funkcja zwracaj¹ca granice pocisku

private:
    sf::RectangleShape shape;  // Kszta³t pocisku
    float speed;  // Prêdkoœæ pocisku
};
