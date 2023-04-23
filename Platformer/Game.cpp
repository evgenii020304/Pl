#include "Game.h"

void Game::initWindow()
{
	this->window.create(sf::VideoMode(1240, 960), "Platformer", sf::Style::Close | sf::Style::Titlebar);
	this->window.setFramerateLimit(60);
}

void Game::initPlayer()
{
	this->player = new Player();
}

Game::Game()
{
	this->initWindow();
	this->initPlayer();
}

Game::~Game()
{
	delete this->player;
}

void Game::updatePlayer()
{
	this->player->update();
}

void Game::update()
{
	// Pooling window events
	while (this->window.pollEvent(this->ev)) {
		if (this->ev.type == sf::Event::Closed) {
			this->window.close();
		}
		else if (this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Escape) {
			this->window.close();
		}

		if (this->ev.type == sf::Event::KeyReleased &&
			(   this->ev.key.code == sf::Keyboard::A ||
				this->ev.key.code == sf::Keyboard::D ||
				this->ev.key.code == sf::Keyboard::W ||
				this->ev.key.code == sf::Keyboard::S
				)
			) {
			this->player->resetAnimationTimer();
		}
	}
	this->updatePlayer();
}

void Game::renderPlayer()
{
	this->player->render(this->window);
}

void Game::render()
{
	this->window.clear(sf::Color::Black);

	//Render game
	this->renderPlayer();

	this->window.display();
}

const sf::RenderWindow& Game::getWindow() const
{
	// TODO: вставьте здесь оператор return
	return this->window;
}
