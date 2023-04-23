#include "Player.h"

void Player::initVariables()
{
	this->animState = PLAYER_ANIMATION_STATES::IDLE;
}

void Player::initTexture()
{
	if (!this->textureSheet.loadFromFile("../images/FinnSprite.png")) {
		std::cout << "ERROR::PLAYER::Not found player sheet." << "\n";
	}
}

void Player::initSprite()
{
	this->sprite.setTexture(this->textureSheet);
	this->currentFrame = sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT); //Изменить под размеры спрайта

	this->sprite.setTextureRect(this->currentFrame);
	this->sprite.setScale(3.f, 3.f);
}

void Player::initAnimations()
{
	this->animationTimer.restart();
	this->animationSwitch = true;
}

void Player::initPhysics()
{
	this->velocityMax = 6.f;
	this->velocityMin = 1.f;
	this->acceleration = 3.f;
	this->deceleration = 0.95f;
	this->gravity = 4.f;
	this->velocityMaxY = 1.f; // 10.f

}

Player::Player()
{
	this->initVariables();
	this->initTexture();
	this->initSprite();
	this->initAnimations();
	this->initPhysics();
}

Player::~Player()
{
}

const bool& Player::getAnimSwitch()
{
	bool anim_switch = this->animationSwitch;

	if (this->animationSwitch) {
		this->animationSwitch = false;
	}
	return anim_switch;
}

void Player::resetAnimationTimer()
{
	this->animationTimer.restart();
	this->animationSwitch = true;
}

void Player::move(const float dir_x, const float dir_y)
{
	this->velocity.x += dir_x * this->acceleration;

	if (std::abs(this->velocity.x) > this->velocityMax) {
		this->velocity.x = this->velocityMax * ((this->velocity.x < 0.f) ? -1.f : 1.f);
	}
}

void Player::updatePhysics()
{
	//Гравитация
	this->velocity.y += 1.0 * this->gravity; 

	if (std::abs(this->velocity.y) > this->velocityMaxY) {
		this->velocity.y = this->velocityMaxY * ((this->velocity.y < 0.f) ? -1.f : 1.f);
	}

	//Торможение
	this->velocity *= this->deceleration;

	if (std::abs(this->velocity.x) < this->velocityMin) {
		this->velocity.x = 0.f;
	}
	if (std::abs(this->velocity.y) < this->velocityMin) {
		this->velocity.y = 0.f;
	}

	this->sprite.move(this->velocity);
}

void Player::updateMovement()
{
	this->animState = PLAYER_ANIMATION_STATES::IDLE;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) { //left
		this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
		this->move(-1.f, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) { //right
		this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
		this->move(1.f, 0.f);
	}
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { //top
		this->sprite.move(0.f, -1.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) { //down
		this->sprite.move(0.f, 1.f); 
	}*/

}

void Player::updateAnimations() {
	if (this->animState == PLAYER_ANIMATION_STATES::IDLE)
	{
		if (this->animationTimer.getElapsedTime().asSeconds() >= 0.5f || this->getAnimSwitch()) 
		{
			this->currentFrame.top = 0.f;
			this->currentFrame.left += FRAME_WIDTH;
			if (this->currentFrame.left > FRAME_WIDTH * IDLE_FRAMES)
				this->currentFrame.left = 0;

			this->animationTimer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}
	}
	else if (this->animState == PLAYER_ANIMATION_STATES::MOVING_RIGHT || this->getAnimSwitch())
	{
		if (this->animationTimer.getElapsedTime().asSeconds() >= 0.5f) 
		{
			this->currentFrame.top = FRAME_HEIGHT;
			this->currentFrame.left += FRAME_WIDTH;
			if (this->currentFrame.left > MOVING_FRAMES * FRAME_WIDTH)
				this->currentFrame.left = 0;

			this->animationTimer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}
		this->sprite.setScale(3.f, 3.f);
		this->sprite.setOrigin(0.f, 0.f);
	}
	else if (this->animState == PLAYER_ANIMATION_STATES::MOVING_LEFT || this->getAnimSwitch())
	{
		if (this->animationTimer.getElapsedTime().asSeconds() >= 0.5f)
		{
			this->currentFrame.top = FRAME_HEIGHT;
			this->currentFrame.left += FRAME_WIDTH;
			if (this->currentFrame.left > MOVING_FRAMES * FRAME_WIDTH)
				this->currentFrame.left = 0;

			this->animationTimer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}	
		this->sprite.setScale(-3.f, 3.f);
		this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 3.f, 0.f);
	}
	else
		this->animationTimer.restart();
}

void Player::update()
{
	this->updateMovement();
	this->updateAnimations();
	this->updatePhysics();
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}



