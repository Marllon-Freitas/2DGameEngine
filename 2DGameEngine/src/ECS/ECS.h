#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>
#include <deque>

#include "../Logger/Logger.h"

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
	protected:
		static int nextId;
};

template <typename T>
class Component: public IComponent {
	public:
		static int GetId() {
			static auto id = nextId++;
			return id;
		}
};

class Entity {
	private:
		int m_id;

	public:
		Entity(int id): m_id(id) {};
		Entity(const Entity& entity) = default;

		int GetId() const;
		void Kill();

		void Tag(const std::string& tag);
		bool HasTag(const std::string& tag) const;
		void Group(const std::string& group);
		bool BelongsToGroup(const std::string& group) const;
		
		Entity& operator =(const Entity& other) = default;
		bool operator ==(const Entity& other) const { return m_id == other.m_id; }
		bool operator !=(const Entity& other) const { return m_id != other.m_id; }
		bool operator >(const Entity& other) const { return m_id > other.m_id; }
		bool operator <(const Entity& other) const { return m_id < other.m_id; }

		template <typename TComponent, typename ...TArgs> void AddComponent(TArgs... args);
		template <typename TComponent> void RemoveComponent();
		template <typename TComponent> bool HasComponent() const;
		template <typename TComponent> TComponent& GetComponent() const;

		class Registry* registry;
};

class System {
	private:
		Signature m_componentSignature;
		std::vector<Entity> m_entities;

	public:
		System() = default;
		~System() = default;

		void AddEntityToSystem(Entity entity);
		void RemoveEntityFromSystem(Entity entity);
		std::vector<Entity> GetSystemEntities() const;
		const Signature& GetComponentSignature() const;

		template <typename TComponent> void RequireComponent();
};

class IPool {
	public:
		virtual ~IPool() {}
};

template <typename T>
class Pool: public IPool {
	private:
		std::vector<T> m_data;

	public:
		Pool(int size = 100) {
			m_data.resize(size);
		}
		virtual ~Pool() = default;

		bool isEmpty() const { return m_data.empty(); }
		int GetSize() const { return m_data.size(); }
		void Resize(int newSize) { m_data.resize(newSize); }
		void Clear() { m_data.clear(); }
		void Add(T object) { m_data.push_back(object); }
		void Set(int index, T object) { m_data[index] = object; }
		T& Get(int index) { return static_cast<T&>(m_data[index]); }
		
		T& operator [](unsigned int index) { return m_data[index]; }
};

class Registry {
	private:
		int m_numOfEntities = 0;
		std::vector<std::shared_ptr<IPool>> m_componentPools;	
		std::vector<Signature> m_entityComponentSignatures;
		std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;

		std::set<Entity> m_entitiesToBeAdded;
		std::set<Entity> m_entitiesToBeKilled;

		std::unordered_map<std::string, Entity> m_entityPerTag;
		std::unordered_map<int, std::string> m_tagPerEntity;

		std::unordered_map<std::string, std::set<Entity>> m_entitiesPerGroup;
		std::unordered_map<int, std::string> m_groupsPerEntity;

		std::deque<int> m_freeIds;

	public:
		Registry() {
			Logger::Info("Game Registry Created");
		};

		~Registry() {
			Logger::Info("Game Registry deleted");
		};

		void Update();

		// Entity management
		Entity CreateEntity();
		void KillEntity(Entity entity);

		// Tag management
		void TagEntity(Entity entity, const std::string& tag);
		bool EntityHasTag(Entity entity, const std::string& tag) const;
		Entity GetEntityByTag(const std::string& tag) const;
		void RemoveEntityTag(Entity entity);

		// Group management 
		void GroupEntity(Entity entity, const std::string& group);
		bool EntityBelongsToGroup(Entity entity, const std::string& group) const;
		std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
		void RemoveEntityGroup(Entity entity);

		// Component management
		template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
		template <typename TComponent> void RemoveComponent(Entity entity);
		template <typename TComponent> bool HasComponent(Entity entity) const;
		template <typename TComponent> TComponent& GetComponent(Entity entity) const;
		
		// System management
		template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
		template <typename TSystem> void RemoveSystem();
		template <typename TSystem> bool HasSystem() const;
		template <typename TSystem> TSystem& GetSystem() const;

		void AddEntityToSystems(Entity entity);
		void RemoveEntityFromSystems(Entity entity);
};

// Component management
template <typename TComponent, typename ...TArgs> 
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	if (componentId >= m_componentPools.size()) {
		m_componentPools.resize(componentId + 1, nullptr);
	}

	if (!m_componentPools[componentId]) {
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		m_componentPools[componentId] = newComponentPool;
	}

	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(m_componentPools[componentId]);

	if (entityId >= componentPool->GetSize()) {
		componentPool->Resize(m_numOfEntities);
	}

	TComponent newComponent(std::forward<TArgs>(args)...);

	componentPool->Set(entityId, newComponent);

	m_entityComponentSignatures[entityId].set(componentId);

	Logger::Info("Component id = " + std::to_string(componentId) + " was added to entity: " + std::to_string(entityId));
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	m_entityComponentSignatures[entityId].set(componentId, false);

	Logger::Info("Component id = " + std::to_string(componentId) + " was removed from entity: " + std::to_string(entityId));
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	return m_entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(m_componentPools[componentId]);

	return componentPool->Get(entityId);
}

// System management
template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	m_componentSignature.set(componentId);
}

template <typename TSystem, typename ...TArgs> 
void Registry::AddSystem(TArgs&& ...args) {
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	m_systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem> 
void Registry::RemoveSystem() {
	auto system = m_systems.find(std::type_index(typeid(TSystem)));
	m_systems.erase(system);
}

template <typename TSystem> 
bool Registry::HasSystem() const {
	return m_systems.find(std::type_index(typeid(TSystem))) != m_systems.end();
}

template <typename TSystem> 
TSystem& Registry::GetSystem() const {
	auto system = m_systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

// Entity
template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs ...args) {
	registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::RemoveComponent() {
	registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const {
	return registry->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const {
	return registry->GetComponent<TComponent>(*this);
}

#endif 