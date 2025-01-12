#pragma once

#include <SFML/Graphics.hpp>

class EnemyBullet {
public:
    EnemyBullet(float x, float y);  // Konstruktor
    void update();  // Funkcja aktualizuj�ca pozycj� pocisku
    void render(sf::RenderWindow& window);  // Funkcja rysuj�ca pocisk
    sf::FloatRect getBounds() const;  // Funkcja zwracaj�ca granice pocisku

private:
    sf::RectangleShape shape;  // Kszta�t pocisku
};
