#include "Bullet.h"

Bullet::Bullet(float x, float y) {
    shape.setSize(sf::Vector2f(5.f, 10.f));  // Rozmiar pocisku
    shape.setFillColor(sf::Color::Red);      // Kolor pocisku
    shape.setPosition(x, y);                 // Pocz¹tkowa pozycja
    speed = 0.25f;                            // Prêdkoœæ pocisku
}

void Bullet::update() {
    shape.move(0.f, -speed);  // Pocisk porusza siê w górê
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();  // Zwracanie granicy pocisku
}
