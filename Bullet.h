#pragma once

#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(float x, float y);  // Konstruktor

    void update();  // Funkcja aktualizuj�ca pozycj� pocisku
    void render(sf::RenderWindow& window);  // Funkcja renderuj�ca pocisk
    sf::FloatRect getBounds() const;  // Funkcja zwracaj�ca granice pocisku

private:
    sf::RectangleShape shape;  // Kszta�t pocisku
    float speed;  // Pr�dko�� pocisku
};
