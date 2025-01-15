#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
	protected:
		static int nextId;
};

template <typename T>
class Component: public IComponent {
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
		int GetId() const;

		Entity& operator =(const Entity& other) = default;
		bool operator ==(const Entity& other) const { return m_id == other.m_id; }
		bool operator !=(const Entity& other) const { return m_id != other.m_id; }
		bool operator >(const Entity& other) const { return m_id > other.m_id; }
		bool operator <(const Entity& other) const { return m_id < other.m_id; }
};

class System {
	private:
		Signature m_componentSignature;
		std::vector<Entity> m_entities;

	public:
		System() = default;
		~System() = default;

		void AddEntityToSystem(Entity entity);
		void RemoveEntityToSystem(Entity entity);
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
		std::vector<IPool*> m_componentPools;	
		std::vector<Signature> m_entityComponentSignatures;
		std::unordered_map<std::type_index, System*> m_systems;
		std::set<Entity> m_entitiesToBeAdded;
		std::set<Entity> m_entitiesToBeKilled;

	public:
		Registry() = default;

		Entity CreateEntity();
		template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
		template <typename TComponent> void RemoveComponent(Entity entity);
};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	m_componentSignature.set(componentId);
}

template <typename TComponent, typename ...TArgs> 
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	if (componentId >= m_componentPools.size) {
		m_componentPools.resize(componentId + 1, nullptr);
	}

	if (!m_componentPools[componentId]) {
		Pool<TComponent>* newComponentPool = new Pool<TComponent>();
		m_componentPools[componentId] = newComponentPool;
	}

	Pool<TComponent>* componentPool = m_componentPools[componentId];

	if (entityId >= componentPool->GetSize()) {
		componentPool->Resize(m_numOfEntities);
	}

	TComponent newComponent(std::forward<TArgs>(args)...);

	componentPool->Set(entityId, newComponent);

	m_entityComponentSignatures[entityId].set(componentId);
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	m_entityComponentSignatures[entityId].set(componentId, false);
}

#endif 