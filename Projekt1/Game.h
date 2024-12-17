#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

class Game {
public:
    Game();       // Konstruktor
    void run();   // G��wna p�tla gry

private:
    sf::RenderWindow window;  // Okno gry

    void processEvents();  // Obs�uga zdarze�
    void update();         // Aktualizacja logiki gry
    void render();         // Renderowanie obiekt�w
};

#endif
