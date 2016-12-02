#include "stdafx.h"
#include "EntityManager.h"
#include "Enemy.h"
#include "TopLayerObj.h"
#include "Player.h"
#include "GameState.h"

Enemy::Enemy(ResourceManager<sf::Texture, std::string>* resourceManager, EntityManager* entityManager, sfld::Vector2f initial_pos, std::vector<Action> a) {
    actions = a;
    beat_count = 0;
    constructEntity(resourceManager, "enemy", entityManager,  initial_pos, Entity::SHAPE_SQUARE, Entity::DYNAMIC_MOVING, Entity::TYPE_DEFAULT);
}

void Enemy::update(int frame_time) {
	baseUpdate(frame_time);
}

void Enemy::baseUpdate(int frame_time) {
	if (actions[beat_count] == ACTION_SHOOT) {
		prepareShoot();
	}
}

void Enemy::prepareShoot() {
	setSprite("enemy_shoot");
}



void Enemy::shoot() {
	sf::Texture LaserImage;
	LaserImage.loadFromFImage(/*filename*/);
	LaserImage.setRepeated(false);
	setSprite("enemy");
	sf::RectangleShape shape;
	shape.setTexture(&LaserImage);
	shape.setSize(sfld::Vector2f(TILE_SIZE * (2.0f / 3.0f), SCREEN_HEIGHT));
	shape.setPosition(sfld::Vector2f(getPosition().x - shape.getSize().x/2.0f, getPosition().y));
	TopLayerObj* laser = new TopLayerObj(shape, 500);
	entityManager_->addTopLayer(laser);
	Player* player = entityManager_->getPlayer();
	if (abs(player->getPosition().x - (shape.getPosition().x + shape.getSize().x / 2.0f)) < shape.getSize().x) {
		player->takeDamage(10);
	}
}



void Enemy::onBeat() {
    Action action = actions[beat_count];
    if (action == ACTION_LEFT) {
        setPosition(getPosition() + sfld::Vector2f(-TILE_SIZE, 0));
    } else if (action == ACTION_RIGHT) {
        setPosition(getPosition() + sfld::Vector2f(TILE_SIZE, 0));
    } else if (action == ACTION_UP) {
        setPosition(getPosition() + sfld::Vector2f(0, -TILE_SIZE));
    } else if (action == ACTION_DOWN) {
        setPosition(getPosition() + sfld::Vector2f(0, TILE_SIZE));
    } else if (action == ACTION_SHOOT) {
        shoot();
    }
    beat_count++;
	if (beat_count >= actions.size()) {
		beat_count = 0;
	}
}
