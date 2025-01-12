#include "Enemy.h"

// Konstruktor Enemy(float x, float y)
// - Ustawia rozmiar (50×50), kolor (zielony)
// - Pozycjê (x, y)
// - Domyœlne wartoœci: speedMultiplier=1.f, enemyBulletCooldown=2.f, currentCooldown=0.f
Enemy::Enemy(float x, float y)
    : speedMultiplier(1.f), enemyBulletCooldown(2.f), currentCooldown(0.f)
{
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(x, y);
}

// update(int direction, float deltaTime)
// - Przesuwa wroga w lewo/prawo zale¿nie od direction i speedMultiplier
// - Zmniejsza currentCooldown o deltaTime
void Enemy::update(int direction, float deltaTime) {
    // Ruch poziomy (direction * speedMultiplier * 0.1f)
    shape.move(direction * speedMultiplier * 0.1f, 0.f);

    // Zmniejszamy cooldown strza³u
    if (currentCooldown > 0.f) {
        currentCooldown -= deltaTime;
    }
}

// render()
// - Rysuje kszta³t wroga w oknie
void Enemy::render(sf::RenderWindow& window) {
    window.draw(shape);
}

// getBounds()
// - Zwraca bounding box wroga (kolizje z pociskami gracza)
sf::FloatRect Enemy::getBounds() const {
    return shape.getGlobalBounds();
}

// getShape()
// - Zwraca referencjê do kszta³tu (u¿ywane np. w moveEnemiesDown())
sf::RectangleShape& Enemy::getShape() {
    return shape;
}

// getPosition()
// - Zwraca (x, y) górny-lewy róg wroga
sf::Vector2f Enemy::getPosition() const {
    return shape.getPosition();
}

// setCooldown(time)
// - Ustawia aktualny cooldown (czas do nastêpnego strza³u)
void Enemy::setCooldown(float time) {
    currentCooldown = time;
}

// canShoot()
// - True, jeœli currentCooldown <= 0 (wróg mo¿e strzeliæ)
bool Enemy::canShoot() const {
    return currentCooldown <= 0.f;
}

// setSpeedMultiplier(multiplier)
// - Ustawia prêdkoœæ wroga (np. na wy¿szym poziomie szybciej)
void Enemy::setSpeedMultiplier(float multiplier) {
    speedMultiplier = multiplier;
}

// setBulletCooldown(cooldown)
// - Ustawia domyœlny cooldown strza³u
void Enemy::setBulletCooldown(float cooldown) {
    enemyBulletCooldown = cooldown;
}
