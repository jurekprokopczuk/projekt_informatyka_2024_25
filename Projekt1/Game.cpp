#include "Game.h"

Game::Game() : window(sf::VideoMode(800, 600), "Space Invaders") {}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Game::update() {
    // Tutaj dodasz logik� gry w przysz�o�ci
}

void Game::render() {
    window.clear();
    // Tutaj dodasz rysowanie obiekt�w
    window.display();
}
