#include "ECS.h"
#include "../Logger/Logger.h"

int IComponent::nextId = 0;

int Entity::GetId() const {
	return m_id;
}

void Entity::Kill() {
	registry->KillEntity(*this);
}

void System::AddEntityToSystem(Entity entity) {
	m_entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [&entity](Entity other) {
		return entity == other;
	}), m_entities.end());
}

std::vector<Entity> System::GetSystemEntities() const {
	return m_entities;
}

const Signature& System::GetComponentSignature() const {
	return m_componentSignature;
}

Entity Registry::CreateEntity() {
	int entityId;

	if (m_freeIds.empty()) {
		entityId = m_numOfEntities++;
		if (entityId >= m_entityComponentSignatures.size()) {
			m_entityComponentSignatures.resize(entityId + 1);
		}
	}
	else {
		entityId = m_freeIds.front();
		m_freeIds.pop_front();
	}

	Entity entity(entityId);
	entity.registry = this;
	m_entitiesToBeAdded.insert(entity);

	Logger::Success("Entity created with id  = " + std::to_string(entityId));

	return entity;
}

void Registry::KillEntity(Entity entity) {
	m_entitiesToBeKilled.insert(entity);
}

void Registry::AddEntityToSystems(Entity entity) {
	const auto entityId = entity.GetId();
	const auto& entityComponentSignature = m_entityComponentSignatures[entityId];

	for (auto& system : m_systems) {
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested) {
			system.second->AddEntityToSystem(entity);
		}
	}
}

void Registry::RemoveEntityFromSystems(Entity entity) {
	for (auto& system : m_systems) {
		system.second->RemoveEntityFromSystem(entity);
	}
}

void Registry::Update() {
	for (auto& entity: m_entitiesToBeAdded) {
		AddEntityToSystems(entity);
	}
	m_entitiesToBeAdded.clear();

	for (auto& entity : m_entitiesToBeKilled) {
		RemoveEntityFromSystems(entity);

		m_entityComponentSignatures[entity.GetId()].reset();

		m_freeIds.push_back(entity.GetId());
	}
	m_entitiesToBeKilled.clear();
}
