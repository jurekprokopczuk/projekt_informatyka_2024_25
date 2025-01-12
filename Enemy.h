#pragma once

#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(float x, float y);  // Konstruktor

    void update(int direction, float deltaTime);  // Funkcja aktualizuj�ca pozycj� wroga
    void render(sf::RenderWindow& window);       // Funkcja rysuj�ca wroga
    sf::FloatRect getBounds() const;             // Funkcja zwracaj�ca granice wroga
    sf::RectangleShape& getShape();              // Zwracamy kszta�t wroga
    sf::Vector2f getPosition() const;            // Zwracamy pozycj� wroga
    void setCooldown(float time);                // Ustawiamy cooldown
    bool canShoot() const;                       // Sprawdzamy, czy wr�g mo�e strzela�

    // Nowe metody
    void setSpeedMultiplier(float multiplier);   // Ustawia mno�nik pr�dko�ci wroga
    void setBulletCooldown(float cooldown);      // Ustawia cooldown pocisk�w

private:
    sf::RectangleShape shape;  // Kszta�t wroga
    float speedMultiplier;     // Mno�nik pr�dko�ci wroga
    float enemyBulletCooldown; // Czas oczekiwania na kolejny strza�
    float currentCooldown;     // Aktualny cooldown na strza�
};
