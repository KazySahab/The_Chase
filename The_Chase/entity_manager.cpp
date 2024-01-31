#include "entity_manager.h"

std::shared_ptr<Entity> Entity_manager::add_entity(const std::string& tag)
{
	auto e = std::make_shared<Entity>(tag, m_total_entities++);
	m_to_add.push_back(e);
	return e;
}

entity_vec& Entity_manager::get_entities()
{
	return m_entities;
}

entity_vec& Entity_manager::get_entities(const std::string& tag)
{
	return m_entity_map[tag];
}

void Entity_manager::update()
{
	for (auto& e : m_to_add)
	{
		m_entities.push_back(e);
		m_entity_map[e->tag()].push_back(e);
	}
	m_to_add.clear();

	remove_dead_entities(m_entities);

	for (auto& vec : m_entity_map)
	{
		remove_dead_entities(vec.second);
	}
}

void ::Entity_manager::remove_dead_entities(entity_vec& m_entities)
{
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](const auto& entity) {
		return !entity->is_active();
		}), m_entities.end());
}

