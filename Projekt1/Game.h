#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

class Game {
public:
    Game();       // Konstruktor
    void run();   // G³ówna pêtla gry

private:
    sf::RenderWindow window;  // Okno gry

    void processEvents();  // Obs³uga zdarzeñ
    void update();         // Aktualizacja logiki gry
    void render();         // Renderowanie obiektów
};

#endif
