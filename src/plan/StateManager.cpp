/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "StateManager.h"

#include "GameState.h"

#include "MessagerAgent.h"
#include "SimpleMsg.h"
#include "LogicException.h"

//-----------------------------------------------------------------
StateManager::~StateManager()
{
    emptyTrash();

    t_states::iterator end = m_states.end();
    for (t_states::iterator i = m_states.begin(); i != end; ++i) {
        delete (*i);
    }
}
//-----------------------------------------------------------------
    void
StateManager::emptyTrash()
{
    t_states::iterator end = m_trash.end();
    for (t_states::iterator i = m_trash.begin(); i != end; ++i) {
        delete (*i);
    }
    m_trash.clear();
}
//-----------------------------------------------------------------
/**
 * Update current state and states on background
 * and empty trash.
 * The states at bottom will be updated as first.
 */
void
StateManager::updateGame()
{
    t_states::iterator end = m_states.end();
    for (t_states::iterator i = m_states.begin(); i != end; /* empty */) {
        //NOTE: state can remove self and thus invalide current iterator
        GameState *cur = *(i++);
        if (cur->isRunning()) {
            cur->updateState();
        }
    }

    emptyTrash();
}

//-----------------------------------------------------------------
/**
 * Throw given state to the trash.
 * NOTE: given state can be still active, cannot be deleted
 */
void
StateManager::removeState(GameState *state)
{
    state->cleanState();
    m_trash.push_back(state);
    m_states.remove(state);
}
//-----------------------------------------------------------------
/**
 * Remove given state and set this one.
 */
void
StateManager::changeState(GameState *who, GameState *new_state)
{
    new_state->initState(this);
    insertAfter(who, new_state);
    removeState(who);
    checkStack();
}
//-----------------------------------------------------------------
/**
 * Pause given state and activate this one.
 */
void
StateManager::pushState(GameState *who, GameState *new_state)
{
    new_state->initState(this);
    insertAfter(who, new_state);
    checkStack();
}

//-----------------------------------------------------------------
/**
 * Remove given state and resume paused states below it.
 */
void
StateManager::popState(GameState *who)
{
    removeState(who);

    if (!m_states.empty()) {
        checkStack();
    }
    else {
        MessagerAgent::agent()->forwardNewMsg(
                new SimpleMsg(Name::APP_NAME, "quit"));
    }
}

//-----------------------------------------------------------------
/**
 * Insert new state after given state.
 * @param who active state or NULL to insert at the beginning
 * @param new_state state to insert
 */
void
StateManager::insertAfter(GameState *who, GameState *new_state)
{
    if (NULL == who) {
        m_states.push_front(new_state);
    }
    else {
        t_states::iterator it = findIter(who);
        m_states.insert(++it, new_state);
    }
}
//-----------------------------------------------------------------
/**
 * Find iterator under given state.
 * @throws LogicException when state is not found
 */
    StateManager::t_states::iterator
StateManager::findIter(GameState *who)
{
    t_states::iterator end = m_states.end();
    for (t_states::iterator i = m_states.begin(); i != end; ++i) {
        if (who == (*i)) {
            return i;
        }
    }
    throw LogicException(ExInfo("game state is not found in stack")
            .addInfo("state", who ? who->getName() : "(null)"));
}
//-----------------------------------------------------------------
/**
 * Preserve stack consistency.
 * - Node at top must be running.
 * - Only running node which allowBg have running states below.
 * @throws LogicException stack is empty
 */
void
StateManager::checkStack()
{
    if (m_states.empty()) {
        throw LogicException(ExInfo("game state stack is empty"));
    }

    t_states::iterator topIt = m_states.end();
    --topIt;
    GameState *top = (*topIt);
    if (top->isOnBg()) {
        top->noteFg();
    }
    pauseBg(topIt);
    resumeBg(topIt);
    installHandlers();
}
//-----------------------------------------------------------------
/**
 * Pause all running states below on stack which all not allowed.
 * The toppers will be paused first but the order should be insignificant
 * @param stateIt states bellow will be check
 */
void
StateManager::pauseBg(t_states::iterator stateIt)
{
    if (stateIt != m_states.begin()) {
        t_states::iterator prev = stateIt;
        --prev;
        if (!(*stateIt)->isRunning() || !(*stateIt)->allowBg()) {
            if ((*prev)->isRunning()) {
                (*prev)->pauseState();
            }
        }
        pauseBg(prev);
    }
}
//-----------------------------------------------------------------
/**
 * Recursive resume given state and all states below on stack which is allowed.
 * The lowers will be resumed first but the order should be insignificant.
 * @param stateIt state to run
 */
void
StateManager::resumeBg(t_states::iterator stateIt)
{
    if ((*stateIt)->allowBg() && stateIt != m_states.begin()) {
        t_states::iterator prev = stateIt;
        --prev;
        resumeBg(prev);
        (*prev)->noteBg();
    }

    if ((*stateIt)->isRunning()) {
        (*stateIt)->pauseState();
    }
    (*stateIt)->resumeState();
}
//-----------------------------------------------------------------
/**
 * Let all running states to install input and draw handler.
 * And all paused states will uninstall their handlers.
 * The lowers will be called first, order is insignificant.
 *
 * NOTE: first the handlers will be uninstalled and then
 * installed back in right order
 */
void
StateManager::installHandlers()
{
    t_states::iterator end = m_states.end();
    for (t_states::iterator i = m_states.begin(); i != end; ++i) {
        if ((*i)->isRunning()) {
            (*i)->unHandlers();
            (*i)->installHandlers();
        }
        else {
            (*i)->unHandlers();
        }
    }
}

