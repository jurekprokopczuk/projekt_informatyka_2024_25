#include "Player.h"
#include <iostream>

// Konstruktor Player()
// - £aduje teksturê statku (ship.png)
// - Ustawia pocz¹tkow¹ pozycjê i skalê sprite'a
// - Ustawia domyœlne wartoœci: speed, cooldown, lives, score
Player::Player() {
    if (!texture.loadFromFile("assets/ship.png")) {
        std::cerr << "Nie mo¿na za³adowaæ obrazu statku!" << std::endl;
    }
    else {
        std::cout << "Obraz statku za³adowany pomyœlnie!" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(375.f, 550.f);
    sprite.setScale(0.1f, 0.1f);

    speed = 0.2f;       // prêdkoœæ ruchu statku lewo/prawo
    shootCooldown = 0.f;
    deltaTime = 0.f;

    lives = 3;          // domyœlna liczba ¿yæ
    score = 0;          // domyœlny wynik
}

// update(float deltaTime)
// - Obs³uga klawiszy Left/Right (ruch statku)
// - Ograniczenie do obszaru ekranu (szerokoœæ 800)
// - Aktualizacja cooldownu strza³u
void Player::update(float deltaTime) {
    // Ruch w lewo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (sprite.getPosition().x > 0) {
            sprite.move(-speed, 0.f);
        }
    }
    // Ruch w prawo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        float rightEdge = sprite.getPosition().x +
            sprite.getLocalBounds().width * sprite.getScale().x;
        if (rightEdge < 800) {
            sprite.move(speed, 0.f);
        }
    }
    // Zmniejszanie cooldownu strza³u
    if (shootCooldown > 0) {
        shootCooldown -= deltaTime;
    }
}

// render()
// - Rysuje sprite statku w podanym oknie
void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

// shoot(std::vector<Bullet>& bullets)
// - Dodaje nowy pocisk, jeœli shootCooldown <= 0
// - Ustawia cooldown na 0.5 s (przyk³adowo)
void Player::shoot(std::vector<Bullet>& bullets) {
    if (shootCooldown <= 0) {
        bullets.push_back(Bullet(sprite.getPosition().x + 20.f, sprite.getPosition().y));
        shootCooldown = 0.5f;
    }
}

// getBounds()
// - Zwraca bounding box statku (kolizje z pociskami wroga)
sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

// getPosition()
// - Zwraca aktualn¹ pozycjê sprite'a
sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

// setPosition(float x, float y)
// - Ustawia sprite w pozycji (x, y)
void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

// increaseScore(int amount)
// - Zwiêksza wynik o podan¹ liczbê punktów
void Player::increaseScore(int amount) {
    score += amount;
}

// loseLife()
// - Zmniejsza liczbê ¿yæ o 1
void Player::loseLife() {
    lives--;
}

// getScore()
// - Zwraca bie¿¹cy wynik gracza
int Player::getScore() const {
    return score;
}

// getLives()
// - Zwraca ile ¿yæ pozosta³o
int Player::getLives() const {
    return lives;
}

// setScore(int newScore)
// - Ustawia wynik na newScore
void Player::setScore(int newScore) {
    score = newScore;
}

// setLives(int newLives)
// - Ustawia liczbê ¿yæ na newLives
void Player::setLives(int newLives) {
    lives = newLives;
}

// setDeltaTime(float dt)
// - Przechowuje dt w klasie Player (opcjonalne u¿ycie w update())
void Player::setDeltaTime(float dt) {
    deltaTime = dt;
}
