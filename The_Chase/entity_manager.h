#pragma once
#include "iostream"
#include "SFML/Graphics.hpp"
#include "entity.h"
#include <algorithm>


typedef std::vector < std::shared_ptr<Entity>> entity_vec;
typedef std::map<std::string, entity_vec>entity_map_vec;
class Entity_manager
{
private:
	entity_vec m_entities;
	entity_vec m_to_add;
	entity_map_vec m_entity_map;
	size_t m_total_entities = 0;
public:
	Entity_manager() {};
	void update();
	std::shared_ptr<Entity> add_entity(const std::string& tag);
	entity_vec& get_entities();
	entity_vec& get_entities(const std::string& tag);
	void remove_dead_entities(entity_vec& m_entities);
	~Entity_manager() {}
};

