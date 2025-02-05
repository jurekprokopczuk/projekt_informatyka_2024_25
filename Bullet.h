#pragma once

#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(float x, float y);  // Konstruktor

    void update();  // Funkcja aktualizująca pozycję pocisku
    void render(sf::RenderWindow& window);  // Funkcja renderująca pocisk
    sf::FloatRect getBounds() const;  // Funkcja zwracająca granice pocisku

private:
    sf::RectangleShape shape;  // Kształt pocisku
    float speed;  // Prędkość pocisku
};
