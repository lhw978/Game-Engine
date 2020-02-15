/*
    Copyright (c) 2018-2020 Piotr Doan. All rights reserved.
*/

#pragma once

#include "Game/EventRouter.hpp"

// Forward declarations.
namespace Engine
{
    class Root;
}

/*
    Game Framework
*/

namespace Game
{
    // Forward declarations.
    class GameState;

    // Game framework class.
    class GameFramework
    {
    public:
        GameFramework();
        ~GameFramework();

        // Disallow copying.
        GameFramework(const GameFramework& other) = delete;
        GameFramework& operator=(const GameFramework& other) = delete;

        // Move constructor and assignment.
        GameFramework(GameFramework&& other);
        GameFramework& operator=(GameFramework&& other);

        // Initializes game framework.
        bool Initialize(Engine::Root* engine);

        // Updates game framework.
        // Returns true if game state actually updates.
        bool Update();

        // Draws game framework.
        void Draw();

        // Sets the main game state instance.
        void SetGameState(std::shared_ptr<GameState>& gameState);

        // Gets the current game state instance.
        std::shared_ptr<GameState> GetGameState();

    public:
        // Framework events.
        struct Events
        {
            // Called when current game state changes.
            Event::Dispatcher<void(const std::shared_ptr<GameState>&)> gameStateChanged;
        } events;

    private:
        // Engine reference.
        Engine::Root* m_engine;

        // Engine event router.
        EventRouter m_eventRouter;

        // Current game state.
        std::shared_ptr<GameState> m_gameState;

        // Initialization state.
        bool m_initialized;
    };
}
