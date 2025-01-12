#pragma once

#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(float x, float y);  // Konstruktor

    void update(int direction, float deltaTime);  // Funkcja aktualizuj¹ca pozycjê wroga
    void render(sf::RenderWindow& window);       // Funkcja rysuj¹ca wroga
    sf::FloatRect getBounds() const;             // Funkcja zwracaj¹ca granice wroga
    sf::RectangleShape& getShape();              // Zwracamy kszta³t wroga
    sf::Vector2f getPosition() const;            // Zwracamy pozycjê wroga
    void setCooldown(float time);                // Ustawiamy cooldown
    bool canShoot() const;                       // Sprawdzamy, czy wróg mo¿e strzelaæ

    // Nowe metody
    void setSpeedMultiplier(float multiplier);   // Ustawia mno¿nik prêdkoœci wroga
    void setBulletCooldown(float cooldown);      // Ustawia cooldown pocisków

private:
    sf::RectangleShape shape;  // Kszta³t wroga
    float speedMultiplier;     // Mno¿nik prêdkoœci wroga
    float enemyBulletCooldown; // Czas oczekiwania na kolejny strza³
    float currentCooldown;     // Aktualny cooldown na strza³
};
