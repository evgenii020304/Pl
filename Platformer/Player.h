#pragma once
#include "stdafx.h"

enum class PLAYER_ANIMATION_STATES 
{
	IDLE = 0, 
	MOVING_LEFT, 
	MOVING_RIGHT, 
	JUMPING, 
	FALLING 
};

const float FRAME_HEIGHT = 32.f;
const float FRAME_WIDTH = 32.f;
const int IDLE_FRAMES = 6;
const int MOVING_FRAMES = 6;

class Player
{
private:
	sf::Sprite sprite;
	sf::Texture textureSheet;
	sf::Clock animationTimer;
	Object player;
	b2Body* playerBody;

	// Animation
	PLAYER_ANIMATION_STATES animState;
	sf::IntRect currentFrame;
	bool animationSwitch;

	//Physics
	sf::Vector2f velocity;
	float velocityMax;
	float velocityMin;
	float acceleration;
	float deceleration;
	float gravity;
	float velocityMaxY;


	void initVariables();
	void initTexture();
	void initSprite();
	void initAnimations();
	void initPhysics();

public:
	Player();
	virtual ~Player();

	const bool& getAnimSwitch();

	void resetAnimationTimer();
	void move(const float dir_x, const float dir_y);
	void updatePhysics();
	void updateMovement();
	void updateAnimations();
	void update();
	void render(sf::RenderTarget& target);
};

