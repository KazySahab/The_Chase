#pragma once
#include "components.h"
#include <memory>

class Entity
{
	bool m_active = true;
	size_t m_id = 0;
	std::string m_tag = "Default";

public:

	std::shared_ptr<Ctransform> transform;
	std::shared_ptr<Cshape> shape;
	std::shared_ptr<Cinput> input;
	std::shared_ptr<Clife> life;
	std::shared_ptr<Ccollision> collision_radius;

	Entity(const std::string& tag, const size_t& id)
		:m_id(id),
		m_tag(tag) 
	{}

	bool  is_active() const;
	const std::string& tag() const;
	size_t id() const;
	void destroy();

	~Entity() {}

};

