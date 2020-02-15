/*
    Copyright (c) 2018-2020 Piotr Doan. All rights reserved.
*/

#include "Precompiled.hpp"
#include "Game/ComponentSystem.hpp"
using namespace Game;

ComponentSystem::ComponentSystem() :
    m_initialized(false),
    m_entitySystem(nullptr)
{
    // Bind event receiver.
    m_entityCreate.Bind<ComponentSystem, &ComponentSystem::OnEntityCreate>(this);
    m_entityDestroy.Bind<ComponentSystem, &ComponentSystem::OnEntityDestroy>(this);
}

ComponentSystem::~ComponentSystem()
{
}

ComponentSystem::ComponentSystem(ComponentSystem&& other) :
    ComponentSystem()
{
    // Call the move assignment.
    *this = std::move(other);
}

ComponentSystem& ComponentSystem::operator=(ComponentSystem&& other)
{
    // Swap class members.
    std::swap(m_initialized, other.m_initialized);
    std::swap(m_entitySystem, other.m_entitySystem);
    std::swap(m_pools, other.m_pools);
    std::swap(m_entityDestroy, other.m_entityDestroy);

    return *this;
}

bool ComponentSystem::Initialize(EntitySystem* entitySystem)
{
    LOG() << "Initializing component system..." << LOG_INDENT();

    // Make sure we are not initializing twice.
    ASSERT(!m_initialized, "Component system instance has already been initialized!");

    // Reset class instance if initialization fails.
    SCOPE_GUARD_IF(!m_initialized, *this = ComponentSystem());

    // Save entity system reference.
    if(entitySystem == nullptr)
    {
        LOG_ERROR() << "Invalid argument - \"entitySystem\" is null!";
        return false;
    }

    m_entitySystem = entitySystem;

    // Receive events from the entity system.
    if(!m_entityCreate.Subscribe(entitySystem->events.entityCreate))
    {
        LOG_ERROR() << "Failed to subscribe to entity system!";
        return false;
    }

    if(!m_entityDestroy.Subscribe(entitySystem->events.entityDestroy))
    {
        LOG_ERROR() << "Failed to subscribe to entity system!";
        return false;
    }

    // Success!
    return m_initialized = true;
}

bool ComponentSystem::OnEntityCreate(EntityHandle handle)
{
    ASSERT(m_initialized, "Component system instance is not initialized!");
    
    // Initialize all components belonging to this entity.
    for(auto& pair : m_pools)
    {
        auto& pool = pair.second;

        if(!pool->InitializeComponent(handle))
            return false;
    }

    return true;
}

void ComponentSystem::OnEntityDestroy(EntityHandle handle)
{
    ASSERT(m_initialized, "Component system instance is not initialized!");

    // Remove all components belonging to the destroyed entity from every pool.
    for(auto& pair : m_pools)
    {
        auto& pool = pair.second;
        pool->DestroyComponent(handle);
    }
}

EntitySystem* ComponentSystem::GetEntitySystem() const
{
    return m_entitySystem;
}
