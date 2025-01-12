#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

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

		bool isEmpty() const { return m_data.empty() }
		int GetSize() const { return m_data.size() }
		void Resize(int newSize) { m_data.resize(newSize) }
		void Clear() { m_data.clear() }
		void Add(T object) { m_data.push_back(object) }
		void Set(int index, T object) { m_data[index] = object }
		T& Get(int index) { return static_cast<T&>m_data[index] }
		
		T& operator [](unsigned int index) { return m_data[index] }
};

class Registry {
	private:
		int m_numOfEntities = 0;
		std::vector<IPool*> m_componentPools;	
};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	m_componentSignature.set(componentId);
}

#endif 