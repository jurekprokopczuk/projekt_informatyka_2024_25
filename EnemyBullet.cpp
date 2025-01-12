#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(float x, float y) {
    shape.setSize(sf::Vector2f(5.f, 15.f)); // Ustawiamy rozmiar pocisku
    shape.setFillColor(sf::Color::Yellow);  // Ustawiamy kolor pocisku
    shape.setPosition(x, y);  // Ustawiamy pocz¹tkow¹ pozycjê pocisku
}

void EnemyBullet::update() {
    shape.move(0.f, 0.05f);  // Prêdkoœæ spadania pocisku
}

void EnemyBullet::render(sf::RenderWindow& window) {
    window.draw(shape);  // Rysowanie pocisku
}

sf::FloatRect EnemyBullet::getBounds() const {
    return shape.getGlobalBounds();  // Zwracamy granice pocisku
}
