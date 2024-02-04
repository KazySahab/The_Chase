// entity
#include "entity.h"

bool Entity::is_active() const
{
	return m_active;
}

const std::string& Entity::tag()const
{
	return m_tag;
}

size_t Entity::id() const
{
	return m_id;
}

void Entity::destroy()
{
	m_active = false;
}
