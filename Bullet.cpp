#include "Bullet.h"

Bullet::Bullet(float x, float y) {
    shape.setSize(sf::Vector2f(5.f, 10.f));  // Rozmiar pocisku
    shape.setFillColor(sf::Color::Red);      // Kolor pocisku
    shape.setPosition(x, y);                 // Pocz�tkowa pozycja
    speed = 0.25f;                            // Pr�dko�� pocisku
}

void Bullet::update() {
    shape.move(0.f, -speed);  // Pocisk porusza si� w g�r�
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();  // Zwracanie granicy pocisku
}
