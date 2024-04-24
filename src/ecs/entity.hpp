#ifndef ENTITY_H
#define ENTITY_H

#include <array>
#include <bitset>
#include <tuple>
#include <functional>

namespace ecs {


template<typename... Components> class entity;

template <typename... Components> class entity_table {

    static constexpr std::size_t entity_count = 1000;
    static constexpr std::size_t component_count = sizeof...(Components);

    template <typename T>
    using component_array = std::array<T, entity_count>;
    using entity_container = std::tuple<component_array<Components>...>;

    entity_container components;
    std::bitset<entity_count> entity_alive_status;
    std::array<std::bitset<component_count>, entity_count> component_alive_status;

public:

    template<typename...> friend class entity; // todo too many friends

    // todo many components can this handle at compile time?
    template<typename T, std::size_t Index = 0>
    constexpr std::size_t get_component_index() const {
	if constexpr (Index == std::tuple_size_v<decltype(components)>) {
	    static_assert(Index != std::tuple_size_v<decltype(components)>, "Component type is not in the tuple.");
	    return Index; // unreachable
	} else if constexpr (std::is_same_v<component_array<T>, std::tuple_element_t<Index, decltype(components)>>) {
	    return Index;
	} else {
	    return get_component_index<T, Index + 1>();
	}
    }

    template<typename Component>
    [[nodiscard]] Component* const get(entity<Components...>& ent) {
	/*todo evaluate possible alternatives to avoid raw pointers:
	  - std::optional<std::reference_wrapper<Component>> (extremely cumbersome)
	  - std::pair<std::reference_wrapper<Component>, bool> (cumbersome)
	  - simply return Component& (error prone from user side... what if the entity does not have the component)*/
	std::size_t component_id = get_component_index<Component>();
	bool has_component = this->has<Component>(ent);
	if(has_component) {
	    return &std::get<component_array<Component>>(components)[ent.id];
	} else {
	    return nullptr;
	}
    }

    template<typename Component>
    [[nodiscard]] bool has(entity<Components...>& ent) const {
	std::size_t component_id = get_component_index<Component>();
        return component_alive_status[ent.id][component_id];
    }


    template<typename Component>
    void add(entity<Components...>& ent) {
	std::size_t component_id = get_component_index<Component>();
	component_alive_status[ent.id].set(component_id);
    }

    template<typename Component, typename... EntityComponents>
    void add(entity<Components...>& ent, Component&& comp, EntityComponents&&... comps) {
	std::size_t component_id = get_component_index<Component>();
	component_alive_status[ent.id].set(component_id);
	*(this->get<Component>(ent)) = std::forward<Component>(comp); // todo requirements todo does forwarding make sense here?

	if constexpr (sizeof...(EntityComponents) > 0) {
	    this->add(ent, std::forward<EntityComponents>(comps)...);
	}
    }


    auto add_entity() {
      for (std::size_t i = 0; i < entity_count; i++) {
	  if (!entity_alive_status[i]) {
	      entity_alive_status.set(i);
	      return entity<Components...>(this, i);
	  }
      }
      throw "entity overflow";
    }

    auto kill_entity() {
      for (std::size_t i = 0; i < entity_count; i++) {
	  if (!entity_alive_status[i]) {
	      entity_alive_status.set(i);
	      return entity<Components...>(this, i);
	  }
      }
      throw "entity overflow";
    }
};

template <typename... Components>
class entity {
    std::size_t id;
    entity_table<Components...>* source_table;
public:

    template<typename...> friend class entity_table; // todo too many friends

    entity(decltype(source_table) table, std::size_t id): source_table(table), id(id) {};

    template <typename Component>
    [[nodiscard]] Component* get() {
	return source_table->template get<Component>(*this);
    }

    template <typename Component>
    [[nodiscard]] bool has() {
	return source_table->template has<Component>(*this);
    }

    template<typename Component>
    void add() {
	source_table->template add<Component>(*this);
    }

    template<typename... EntityComponents>
    void add(EntityComponents&&... comps) {
	source_table->template add(*this, std::forward<EntityComponents>(comps)...);
    }
};

} // namespace ecs
#endif /* ENTITY_H */
