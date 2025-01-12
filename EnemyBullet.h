#pragma once

#include <SFML/Graphics.hpp>

class EnemyBullet {
public:
    EnemyBullet(float x, float y);  // Konstruktor
    void update();  // Funkcja aktualizuj¹ca pozycjê pocisku
    void render(sf::RenderWindow& window);  // Funkcja rysuj¹ca pocisk
    sf::FloatRect getBounds() const;  // Funkcja zwracaj¹ca granice pocisku

private:
    sf::RectangleShape shape;  // Kszta³t pocisku
};
