#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "../Logger/Logger.h"
#include "./Event.h"
#include <map>
#include <typeindex>
#include <list>
#include <memory>

class IEventCallback {
    private:
        virtual void Call(Event& e) = 0;

    public:
        virtual ~IEventCallback() = default;

        void Execute(Event& e) {
            Call(e);
        }
};

template <typename TOwner, typename TEvent>
class EventCallback: public IEventCallback {
    private:
        typedef void (TOwner::*CallbackFuntion)(TEvent&);

        TOwner* m_ownerInstance;
        CallbackFuntion m_callbackFunction;

        virtual void Call(Event& e) override {
            std::invoke(m_callbackFunction, m_ownerInstance, static_cast<TEvent&>(e));
        }

    public:
        EventCallback(TOwner* ownerInstance, CallbackFuntion callbackFunction) {
            m_ownerInstance = ownerInstance;
            m_callbackFunction = callbackFunction;
        }

        virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
    private:
        std::map<std::type_index, std::unique_ptr<HandlerList>> m_subscribers;

    public:
        EventBus() {
            Logger::Info("EventBus contructor Called!");
        }

        ~EventBus() {
            Logger::Info("EventBus destructor Called!");
        }

        void Reset() {
            m_subscribers.clear();
        }

        template <typename TEvent, typename TOwner>
        void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)) {
            if (!m_subscribers[typeid(TEvent)].get()) {
                m_subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
            }
            auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
            m_subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
        }

        template <typename TEvent, typename ...TArgs>
        void EmitEvent(TArgs&& ...args) {
            auto handlers = m_subscribers[typeid(TEvent)].get();
            if (handlers) {
                for (auto i = handlers->begin(); i != handlers->end(); i++) {
                    auto handler = i->get();
                    TEvent event(std::forward<TArgs>(args)...);
                    handler->Execute(event);
                }
            }
        }
};

#endif
