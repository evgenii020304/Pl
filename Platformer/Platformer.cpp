#include "Game.h"
#include "level.h"
#include "stdafx.h"
#include <Box2D\Box2D.h>


//int main()
//{
//	srand(static_cast<unsigned>(time(0)));
//
//	Game game;
//
//	while (game.getWindow().isOpen()) {
//		game.update();
//		game.render();
//	}
//
//	return 0;
//}


Object player;
b2Body* playerBody;

std::vector<Object> coin;
std::vector<b2Body*> coinBody;

std::vector<object> enemy;
std::vector<b2body*> enemybody;

int main()
{
	srand(time(null));

	level lvl;
	lvl.loadfromfile("../images/platformer.tmx");


	b2vec2 gravity(0.0f, 1.0f);
	b2world world(gravity);

	sf::vector2i tilesize = lvl.gettilesize();

	std::vector<object> block = lvl.getobjects("block");
	for (int i = 0; i < block.size(); i++)
	{
		b2bodydef bodydef;
		bodydef.type = b2_staticbody;
		bodydef.position.set(block[i].rect.left + tilesize.x / 2 * (block[i].rect.width / tilesize.x - 1),
			block[i].rect.top + tilesize.y / 2 * (block[i].rect.height / tilesize.y - 1));
		b2body* body = world.createbody(&bodydef);
		b2polygonshape shape;
		shape.setasbox(block[i].rect.width / 2, block[i].rect.height / 2);
		body->createfixture(&shape, 1.0f);
	}

	coin = lvl.getobjects("coin");
	for (int i = 0; i < coin.size(); i++)
	{
		b2bodydef bodydef;
		bodydef.type = b2_dynamicbody;
		bodydef.position.set(coin[i].rect.left + tilesize.x / 2 * (coin[i].rect.width / tilesize.x - 1),
			coin[i].rect.top + tilesize.y / 2 * (coin[i].rect.height / tilesize.y - 1));
		bodydef.fixedrotation = true;
		b2body* body = world.createbody(&bodydef);
		b2polygonshape shape;
		shape.setasbox(coin[i].rect.width / 2, coin[i].rect.height / 2);
		body->createfixture(&shape, 1.0f);
		coinbody.push_back(body);
	}

	enemy = lvl.getobjects("enemy");
	for (int i = 0; i < enemy.size(); i++)
	{
		b2bodydef bodydef;
		bodydef.type = b2_dynamicbody;
		bodydef.position.set(enemy[i].rect.left +
			tilesize.x / 2 * (enemy[i].rect.width / tilesize.x - 1),
			enemy[i].rect.top + tilesize.y / 2 * (enemy[i].rect.height / tilesize.y - 1));
		bodydef.fixedrotation = true;
		b2body* body = world.createbody(&bodydef);
		b2polygonshape shape;
		shape.setasbox(enemy[i].rect.width / 2, enemy[i].rect.height / 2);
		body->createfixture(&shape, 1.0f);
		enemybody.push_back(body);
	}


	player = lvl.getobject("player");
	b2bodydef bodydef;
	bodydef.type = b2_dynamicbody;
	bodydef.position.set(player.rect.left, player.rect.top);
	bodydef.fixedrotation = true;
	playerbody = world.createbody(&bodydef);
	b2polygonshape shape; shape.setasbox(player.rect.width / 2, player.rect.height / 2);
	b2fixturedef fixturedef;
	fixturedef.shape = &shape;
	playerbody->createfixture(&fixturedef);



	sf::vector2i screensize(800, 600);

	sf::renderwindow window;
	window.create(sf::videomode(screensize.x, screensize.y), "game");


	sf::view view;
	view.reset(sf::floatrect(0.0f, 0.0f, screensize.x, screensize.y));
	view.setviewport(sf::floatrect(0.0f, 0.0f, 2.0f, 2.0f));

	while (window.isopen())
	{
		sf::event evt;

		while (window.pollevent(evt))
		{
			switch (evt.type)
			{
			case sf::event::closed:
				window.close();
				break;

			case sf::event::keypressed:
				if (evt.key.code == sf::keyboard::w && playerbody->getlinearvelocity().y == 0)
					playerbody->setlinearvelocity(b2vec2(0.0f, -15.0f));

				if (evt.key.code == sf::keyboard::d)
					playerbody->setlinearvelocity(b2vec2(30.0f, 0.0f));

				if (evt.key.code == sf::keyboard::a)
					playerbody->setlinearvelocity(b2vec2(-30.0f, 0.0f));
				break;
			}
		}

		world.step(1.0f / 60.0f, 1, 1);


		for (b2contactedge* ce = playerbody->getcontactlist(); ce; ce = ce->next)
		{
			b2contact* c = ce->contact;

			for (int i = 0; i < coinbody.size(); i++)
				if (c->getfixturea() == coinbody[i]->getfixturelist())
				{
					coinbody[i]->destroyfixture(coinbody[i]->getfixturelist());
					coin.erase(coin.begin() + i);
					coinbody.erase(coinbody.begin() + i);
				}

			for (int i = 0; i < enemybody.size(); i++)
				if (c->getfixturea() == enemybody[i]->getfixturelist())
				{
					if (playerbody->getposition().y < enemybody[i]->getposition().y)
					{
						playerbody->setlinearvelocity(b2vec2(0.0f, -10.0f));

						enemybody[i]->destroyfixture(enemybody[i]->getfixturelist());
						enemy.erase(enemy.begin() + i);
						enemybody.erase(enemybody.begin() + i);
					}
					else
					{
						int tmp = (playerbody->getposition().x < enemybody[i]->getposition().x)
							? -1 : 1;
						playerbody->setlinearvelocity(b2vec2(10.0f * tmp, 0.0f));
					}
				}
		}

		for (int i = 0; i < enemybody.size(); i++)
		{
			if (enemybody[i]->getlinearvelocity() == b2vec2_zero)
			{
				int tmp = (rand() % 2 == 1) ? 1 : -1;
				enemybody[i]->setlinearvelocity(b2vec2(5.0f * tmp, 0.0f));
			}
		}


		b2vec2 pos = playerbody->getposition();
		view.setcenter(pos.x + screensize.x / 4, pos.y + screensize.y / 4);
		window.setview(view);

		player.sprite.setposition(pos.x, pos.y);

		for (int i = 0; i < coin.size(); i++)
			coin[i].sprite.setposition(coinbody[i]->getposition().x, coinbody[i]->getposition().y);

		for (int i = 0; i < enemy.size(); i++)
			enemy[i].sprite.setposition(enemybody[i]->getposition().x, enemybody[i]->getposition().y);

		window.clear();

		lvl.draw(window);

		window.draw(player.sprite);

		for (int i = 0; i < coin.size(); i++)
			window.draw(coin[i].sprite);

		for (int i = 0; i < enemy.size(); i++)
			window.draw(enemy[i].sprite);

		window.display();
	}
 return 0;
}