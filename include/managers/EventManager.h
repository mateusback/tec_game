#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>

class EventManager {
public:
    template <typename T>
    static void On(std::function<void(const T&)> callback) {
        get().getListeners<T>().push_back(std::move(callback));
    }

    template <typename T>
    static void Emit(const T& event) {
        auto& funcs = get().getListeners<T>();
        for (const auto& f : funcs) {
            f(event);
        }
    }

    static void ClearAll() {
        get().listeners.clear();
    }

private:
    EventManager() = default;

    static EventManager& get() {
        static EventManager instance;
        return instance;
    }

    std::unordered_map<std::type_index, std::shared_ptr<void>> listeners;

    template <typename T>
    std::vector<std::function<void(const T&)>>& getListeners() {
        const std::type_index type = std::type_index(typeid(T));
        if (!listeners.count(type)) {
            listeners[type] = std::make_shared<std::vector<std::function<void(const T&)>>>();
        }
        return *std::static_pointer_cast<std::vector<std::function<void(const T&)>>>(listeners[type]);
    }
};

#endif
