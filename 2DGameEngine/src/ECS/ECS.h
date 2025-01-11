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
};

class System {
	private:
		Signature m_componentSignature;
		std::vector<Entity> m_entities;

	public:
		System() = default;
		~System() = default;

		void AddEntityToSystem(Entity m_entity);
		void RemoveEntityToSystem(Entity m_entity);
		std::vector<Entity> GetSystemEntities() const;
		Signature& GetComponentSignature() const;

		template <typename TComponent> void RequireComponent();
};

class Registry {};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	m_componentSignature.set(componentId);
}

#endif 