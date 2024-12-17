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
    // Tutaj dodasz logikê gry w przysz³oœci
}

void Game::render() {
    window.clear();
    // Tutaj dodasz rysowanie obiektów
    window.display();
}
