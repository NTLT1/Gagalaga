#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include <math.h>
#include <cstdlib>
#include <list>
#include <iostream>
#include <vector>
#include <fstream>
using namespace sf;
class EnemyBullet
{
public:
	Sprite shape;
	EnemyBullet(Texture* texture, Vector2f pos)
	{
		this->shape.setTexture(*texture);
		this->shape.setScale(0.06f, 0.06f);
		this->shape.setPosition(pos);
	}
};
class Bullet
{
public:
	Sprite shape;
	Bullet(Texture* texture, Vector2f pos)
	{
		this->shape.setTexture(*texture);
		this->shape.setScale(0.2f, 0.2f);
		this->shape.setPosition(pos);
	}
};
class Player
{
public:
	int HP = 0, HPMax = 100;
	Sprite shape;
	Texture* texture;
	std::vector<Bullet>bullets;
	Player(Texture* texture)
	{
		this->HP = 100;
		this->HP = this->HPMax;
		this->texture = texture;
		this->shape.setTexture(*texture);
		this->shape.setScale(0.3f, 0.3f);
	}
};
class Enemy
{
public:
	Sprite shape;
	Enemy(Texture* texture, Vector2u windowSize)
	{
		this->shape.setTexture(*texture);
		this->shape.setScale(0.1f, 0.1f);
		this->shape.setPosition(rand() % int(windowSize.x - this->shape.getGlobalBounds().width), 0);
	}

};
class ShootEnemy
{
public:
	Sprite shape;
	std::vector<EnemyBullet>EnemyProjectile;
	ShootEnemy(Texture* texture, Vector2u windowSize)
	{
		this->shape.setTexture(*texture);
		this->shape.setPosition(rand() % int(windowSize.x - this->shape.getGlobalBounds().width), 0);
		this->shape.setScale(0.5f, 0.5f);
	}

};


int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 800), "Gagalaga");
	window.setFramerateLimit(60);
	Texture mapTex;
	mapTex.loadFromFile("D:/Downloaded/My projects/Project/Bombarino/Image/sky.jpg");
	Sprite mapSprite;
	mapSprite.setTexture(mapTex);
	Clock clock;
	Font font;
	font.loadFromFile("D:/Downloaded/My projects/Project/Bombarino/Font/ALGER.TTF");

	int hell = 0;
	int berserk = 0;
	int hell1 = 0;
	int shootTimer = 0;
	int enemySpawnTimer = 0;
	int shootEnemySpawnTimer = 0;
	int EnemyShootTimer = 0;
	int Score = 0;


	Texture enemyTex;
	enemyTex.loadFromFile("D:/Downloaded/My projects/Project/Bombarino/Image/wasp.png");
	std::vector<Enemy>enemies;

	Texture shootingEnemyTex;
	shootingEnemyTex.loadFromFile("D:/Downloaded/My projects/Project/Bombarino/Image/enemy.png");
	std::vector<ShootEnemy>shootEnemies;
	
	Texture bulletTex;
	bulletTex.loadFromFile("D:/Downloaded/My projects/Project/Bombarino/Image/missile.png");
	Texture projectileTex;
	projectileTex.loadFromFile("D:/Downloaded/My projects/Project/Bombarino/Image/projectile.png");


	Texture playerTex;
	playerTex.loadFromFile("D:/Downloaded/My projects/Project/Bombarino/Image/ship.png");
	Player player(&playerTex);
	player.shape.setPosition(window.getSize().x / static_cast<float>(2), window.getSize().y);

	Text hpText;
	hpText.setFont(font);
	hpText.setCharacterSize(20);
	hpText.setFillColor(Color::Green);

	Text score;
	score.setFont(font);
	score.setCharacterSize(20);
	score.setFillColor(Color::Red);

	Text gameOver;
	gameOver.setFont(font);
	gameOver.setCharacterSize(40);
	gameOver.setFillColor(Color::Magenta);
	gameOver.setPosition(180, 350);
	gameOver.setString("!GAME OVER!");

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
			}
		}
		if (player.HP > 0)
		{

			hell = 50;
			hell1 = 150;
			berserk = 30;

			//Управление//
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.shape.move(0.f, -6.f);
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.shape.move(0.f, 6.f);
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.shape.move(6.f, 0.f);
			}
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.shape.move(-6.f, 0.f);
			}

			//Отображение здоровья и счета//
			hpText.setPosition(0, 0);
			hpText.setString("Health " + std::to_string(player.HP) + "%");
			score.setPosition(150, 0);
			score.setString("Score " + std::to_string(Score));

			//Взаимодействие с картой//
			if (player.shape.getPosition().x <= 0)
			{
				player.shape.setPosition(0.f, player.shape.getPosition().y);
			}
			if (player.shape.getPosition().x >= window.getSize().x - player.shape.getGlobalBounds().width)
			{
				player.shape.setPosition(window.getSize().x - player.shape.getGlobalBounds().width, player.shape.getPosition().y);
			}
			if (player.shape.getPosition().y <= 0)
			{
				player.shape.setPosition(player.shape.getPosition().x, 0.f);
			}
			if (player.shape.getPosition().y >= window.getSize().y - player.shape.getGlobalBounds().height)
			{
				player.shape.setPosition(player.shape.getPosition().x, window.getSize().y - player.shape.getGlobalBounds().height);
			}

			//Стрельба//
			if (Keyboard::isKeyPressed(Keyboard::Space) && shootTimer >= berserk)
			{
				player.bullets.push_back(Bullet(&bulletTex, player.shape.getPosition()));
				shootTimer = 0;
			}
			if (shootTimer < berserk)
			{
				shootTimer++;
			}

			//Кароч просто че там пули делают на карте//
			for (size_t i = 0; i < player.bullets.size(); i++)
			{
				//Движение пуль//
				player.bullets[i].shape.move(0.f, -8.5f);

				//Удаление пуль//
				if (player.bullets[i].shape.getPosition().y > window.getSize().y)
				{
					player.bullets.erase(player.bullets.begin() + i);
				}

				//Удаление пуль и противников при контакте друг с другом//
				for (size_t j = 0; j < shootEnemies.size(); j++)
				{
					if (player.bullets[i].shape.getGlobalBounds().intersects(shootEnemies[j].shape.getGlobalBounds()))
					{
						shootEnemies.erase(shootEnemies.begin() + j);
						player.bullets.erase(player.bullets.begin() + i);
						Score += 50;
						break;
					}
				}
			}
			//Удаление пуль и противников при контакте друг с другом//
			for (size_t i = 0; i < player.bullets.size(); i++)
			{
				for (size_t j = 0; j < enemies.size(); j++)
				{
					if (player.bullets[i].shape.getGlobalBounds().intersects(enemies[j].shape.getGlobalBounds()))
					{
						enemies.erase(enemies.begin() + j);
						player.bullets.erase(player.bullets.begin() + i);
						Score += 10;
						break;

					}
				}
			}
			if (enemySpawnTimer < hell)
			{
				enemySpawnTimer++;
			}
			if (enemySpawnTimer >= hell)
			{
				enemies.push_back(Enemy(&enemyTex, window.getSize()));
				enemySpawnTimer = 0;
			}
			if (shootEnemySpawnTimer < hell1)
			{
				shootEnemySpawnTimer++;
			}
			if (shootEnemySpawnTimer >= hell1)
			{
				shootEnemies.push_back(ShootEnemy(&shootingEnemyTex, window.getSize()));
				shootEnemySpawnTimer = 0;
			}

		}
		//Движение противников и их удаление(при контакте с игроком и вылете за пределы карты)//
		//Обычные камикадзе//
		for (size_t i = 0; i < enemies.size(); i++)
		{
			enemies[i].shape.move(0.f, 4.5f);
			if (enemies[i].shape.getPosition().y <= 0 - enemies[i].shape.getGlobalBounds().height)
			{
				enemies.erase(enemies.begin() + i);
				break;
			}
			if (enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
			{
				enemies.erase(enemies.begin() + i);
				player.HP-=10;
				Score -= 5;
				break;
			}
		}

		window.draw(mapSprite);
		window.draw(player.shape);
		window.draw(hpText);
		window.draw(score);

		//Отрисока пуль//
		for (size_t i = 0; i < player.bullets.size(); i++)
		{
			window.draw(player.bullets[i].shape);
		}

		//Отрисовка противнков//
		for (size_t i = 0; i < shootEnemies.size(); i++)
		{
			window.draw(shootEnemies[i].shape);
		}
		for (size_t i = 0; i < enemies.size(); i++)
		{
			window.draw(enemies[i].shape);
		}

		//Стреляющие противники//
		for (size_t i = 0; i < shootEnemies.size(); i++)
		{
			shootEnemies[i].shape.move(0.f, 2.5f);
			if (shootEnemies[i].shape.getPosition().y>=150)
			{
				shootEnemies[i].shape.move(0.f, -2.5f);
				//Задаем параметры вектору(пулям противника)//
				if (EnemyShootTimer>=250)
				{
					shootEnemies[i].EnemyProjectile.push_back(EnemyBullet(&projectileTex, shootEnemies[i].shape.getPosition()));
					EnemyShootTimer = 0;
				}
				if (EnemyShootTimer<250)
				{
					EnemyShootTimer++;
				}
				//Отрисовка, удаление, взаимодействие с игроком//
				for (size_t j = 0; j < shootEnemies[i].EnemyProjectile.size(); j++)
				{
					window.draw(shootEnemies[i].EnemyProjectile[j].shape);
					Vector2f direction((player.shape.getPosition().x+player.shape.getGlobalBounds().width-shootEnemies[i].shape.getPosition().x) / 100, (player.shape.getPosition().y + player.shape.getGlobalBounds().width-shootEnemies[i].shape.getPosition().y) / 100);
					shootEnemies[i].EnemyProjectile[j].shape.move(direction);
					if (shootEnemies[i].EnemyProjectile[j].shape.getPosition().y>window.getSize().y)
					{
						shootEnemies[i].EnemyProjectile.erase(shootEnemies[i].EnemyProjectile.begin() + j);
						break;

					}
					if (shootEnemies[i].EnemyProjectile[j].shape.getPosition().x > window.getSize().x)
					{
						shootEnemies[i].EnemyProjectile.erase(shootEnemies[i].EnemyProjectile.begin() + j);
						break;

					}
					if (shootEnemies[i].EnemyProjectile[j].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
					{
						shootEnemies[i].EnemyProjectile.erase(shootEnemies[i].EnemyProjectile.begin() + j);
						player.HP-=5;
						Score -= 10;
						break;
					}
				}
				
			}
			if (shootEnemies[i].shape.getPosition().y<=0-shootEnemies[i].shape.getGlobalBounds().height)
			{
				shootEnemies.erase(shootEnemies.begin() + i);
				break;
			}
			if (shootEnemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
			{
				shootEnemies.erase(shootEnemies.begin() + i);
				player.HP-=10;
				Score -= 10;
				break;
			}
		}

		if (player.HP <= 0)
		{
			window.draw(gameOver);
		}
		window.display();
		window.clear();
	}


	return 0;
}




