#include "Player.h"
#include <iostream>

// Konstruktor Player()
// - �aduje tekstur� statku (ship.png)
// - Ustawia pocz�tkow� pozycj� i skal� sprite'a
// - Ustawia domy�lne warto�ci: speed, cooldown, lives, score
Player::Player() {
    if (!texture.loadFromFile("assets/ship.png")) {
        std::cerr << "Nie mo�na za�adowa� obrazu statku!" << std::endl;
    }
    else {
        std::cout << "Obraz statku za�adowany pomy�lnie!" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(375.f, 550.f);
    sprite.setScale(0.1f, 0.1f);

    speed = 0.2f;       // pr�dko�� ruchu statku lewo/prawo
    shootCooldown = 0.f;
    deltaTime = 0.f;

    lives = 3;          // domy�lna liczba �y�
    score = 0;          // domy�lny wynik
}

// update(float deltaTime)
// - Obs�uga klawiszy Left/Right (ruch statku)
// - Ograniczenie do obszaru ekranu (szeroko�� 800)
// - Aktualizacja cooldownu strza�u
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
    // Zmniejszanie cooldownu strza�u
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
// - Dodaje nowy pocisk, je�li shootCooldown <= 0
// - Ustawia cooldown na 0.5 s (przyk�adowo)
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
// - Zwraca aktualn� pozycj� sprite'a
sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

// setPosition(float x, float y)
// - Ustawia sprite w pozycji (x, y)
void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

// increaseScore(int amount)
// - Zwi�ksza wynik o podan� liczb� punkt�w
void Player::increaseScore(int amount) {
    score += amount;
}

// loseLife()
// - Zmniejsza liczb� �y� o 1
void Player::loseLife() {
    lives--;
}

// getScore()
// - Zwraca bie��cy wynik gracza
int Player::getScore() const {
    return score;
}

// getLives()
// - Zwraca ile �y� pozosta�o
int Player::getLives() const {
    return lives;
}

// setScore(int newScore)
// - Ustawia wynik na newScore
void Player::setScore(int newScore) {
    score = newScore;
}

// setLives(int newLives)
// - Ustawia liczb� �y� na newLives
void Player::setLives(int newLives) {
    lives = newLives;
}

// setDeltaTime(float dt)
// - Przechowuje dt w klasie Player (opcjonalne u�ycie w update())
void Player::setDeltaTime(float dt) {
    deltaTime = dt;
}
