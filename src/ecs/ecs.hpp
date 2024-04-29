#ifndef ENTITY_H
#define ENTITY_H

#include <array>
#include <bitset>
#include <tuple>
#include <functional>

// todo named entities
template <typename... Components> class ecs {

    static constexpr std::size_t entity_count = 1000; // todo somewhere else?
    static constexpr std::size_t component_count = sizeof...(Components);

    template <typename T>
    using component_array = std::array<T, entity_count>;
    using entity_container = std::tuple<component_array<Components>...>;

    entity_container components;
    std::bitset<entity_count> entity_alive_status;
    std::array<std::bitset<component_count>, entity_count> component_alive_status;

public:
    // helper class for referencing table columns
    class entity {
        std::size_t id_;
        ecs<Components...>* source_table;

    public:

        entity(decltype(source_table) table, std::size_t id): source_table(table), id_(id) {};

        template <typename Component>
        [[nodiscard]] Component* get() {
            return source_table->template get<Component>(*this);
        }

        template <typename... EntityComponents>
        [[nodiscard]] bool has() {
            return source_table->template has<EntityComponents...>(*this);
        }

        template<typename Component>
        void add() {
            source_table->template add<Component>(*this);
        }

        template<typename... EntityComponents>
        void add(EntityComponents&&... comps) {
            source_table->template add(*this, std::forward<EntityComponents>(comps)...);
        }

        template<typename...EntityComponents>
        void remove() {
            source_table->template remove<EntityComponents...>(*this);
        }

        void kill() {
            source_table->kill_entity(*this);
        }

        [[nodiscard]] std::size_t id() const noexcept {
            return this->id_;
        }

        [[nodiscard]] bool is_alive() const {
            return source_table->is_alive(*this);
        }
    }; // class ecs<Components...>::entity

    class iterator {
        ecs<Components...>* source_table;
        std::size_t id_; // i should probably declare a size_type somewhere...
    public:

        using value_type = entity;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        iterator(decltype(source_table) table, std::size_t id) : source_table(table), id_(id) {}

        value_type operator*() const {
            return value_type(source_table, id_);
        }

        iterator& operator++() {
            ++id_;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return id_ == other.id_ && source_table == other.source_table;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        friend difference_type operator-(const iterator& lhs, const iterator& rhs) {
            return static_cast<difference_type>(lhs.id) - static_cast<difference_type>(rhs.id);
        }

    }; // class ecs<Components...>::interator

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, entity_count);
    }

    // todo how many components can this handle at compile time?
    template<typename T, std::size_t Index = 0>
    constexpr std::size_t get_component_index() const {
        if constexpr (Index == std::tuple_size_v<decltype(components)>) {
            static_assert(Index != std::tuple_size_v<decltype(components)>, "Component type is not in the tuple.");
            return Index; // unreachable
        } else if constexpr (std::is_same_v<component_array<std::decay_t<T>>, std::tuple_element_t<Index, decltype(components)>>) { // todo std::decay_t?
            return Index;
        } else {
            return get_component_index<std::decay_t<T>, Index + 1>(); // todo std::decay_t?
        }
    }

    template<typename Component>
    [[nodiscard]] std::decay_t<Component>* const get(entity& ent) {
        std::size_t component_id = get_component_index<Component>();
        bool has_component = this->has<Component>(ent);
        if(has_component) {
            return &std::get<component_array<std::decay_t<Component>>>(components)[ent.id()]; // todo std::decay_t?
        } else {
            return nullptr;
        }
    }

    /*template<typename Component>
      [[nodiscard]] bool has(const entity& ent) const {
      std::size_t component_id = get_component_index<Component>();
      return component_alive_status[ent.id()][component_id] && entity_alive_status[ent.id()];
      }*/

    template<typename Component, typename... EntityComponents>
    [[nodiscard]] bool has(const entity& ent) const {
        std::size_t component_id = get_component_index<Component>();
        if constexpr (sizeof...(EntityComponents) > 0) {
            return component_alive_status[ent.id()][component_id] && has<EntityComponents...>(ent);
        } else {
            return component_alive_status[ent.id()][component_id] && entity_alive_status[ent.id()];
        }
    }

    template<typename Component>
    void add(entity& ent) {
        std::size_t component_id = get_component_index<Component>();
        component_alive_status[ent.id()].set(component_id);
        *(this->get<Component>(ent)) = Component(); // todo requirements default constructible
    }

    template<typename Component, typename... EntityComponents>
    void add(entity& ent, Component&& comp, EntityComponents&&... comps) {
        std::size_t component_id = get_component_index<Component>();
        component_alive_status[ent.id()].set(component_id);
        // todo requirements move/copy assignment
        *(this->get<Component>(ent)) = std::forward<Component>(comp);

        if constexpr (sizeof...(EntityComponents) > 0) {
            this->add(ent, std::forward<EntityComponents>(comps)...);
        }
    }

    template<typename Component, typename...EntityComponents>
    void remove(entity& ent) {
        std::size_t component_id = get_component_index<Component>();
        component_alive_status[ent.id()].reset(component_id);

        if constexpr (sizeof...(EntityComponents) > 0) {
            this->remove<EntityComponents...>(ent);
        }
    }

    entity add_entity() {
        for (std::size_t i = 0; i < entity_count; i++) {
            if (!entity_alive_status[i]) {
                entity_alive_status.set(i);
                return entity(this, i);
            }
        }
        throw "entity overflow";
    }

    void kill_entity(entity& ent) {
        entity_alive_status.reset(ent.id());
        component_alive_status[ent.id()].reset();
    }

    bool is_alive(const entity& ent) {
        return entity_alive_status[ent.id()];
    }
};

#endif /* ENTITY_H */
