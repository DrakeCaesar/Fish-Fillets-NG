/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LevelScript.h"

#include "Room.h"
#include "ScriptState.h"

#include "ScriptCmd.h"
#include "LogicException.h"
#include "Cube.h"
#include "Unit.h"

#include "game-script.h"
#include "level-script.h"

//-----------------------------------------------------------------
/**
 * Create new plan holder.
 */
LevelScript::LevelScript(Level *aLevel)
{
    m_room = NULL;
    m_level = aLevel;
    registerGameFuncs();
}
//-----------------------------------------------------------------
LevelScript::~LevelScript()
{
    cleanRoom();
}
//-----------------------------------------------------------------
void
LevelScript::takeRoom(Room *new_room)
{
    cleanRoom();
    m_room = new_room;
}
//-----------------------------------------------------------------
/**
 * Remove old room.
 */
void
LevelScript::cleanRoom()
{
    if (m_room) {
        delete m_room;
        m_room = NULL;
    }
}
//-----------------------------------------------------------------
Room *
LevelScript::room()
{
    checkRoom();
    return m_room;
}
//-----------------------------------------------------------------
/**
 * Create ScriptCmd for show.
 * Command will be executed in this script context.
 * @param funcRef index of lua function
 * @return new command
 */
Command *
LevelScript::createCommand(int funcRef)
{
    return new ScriptCmd(m_script, funcRef);
}

//-----------------------------------------------------------------
/**
 * Update level (plan dialogs, do anim, ...).
 */
    void
LevelScript::updateScript()
{
    m_script->doString("script_update()");
    satisfyPlan();
}
//-----------------------------------------------------------------
/**
 * Check whether room is ready.
 * @throws LogicException when room is not ready
 */
    void
LevelScript::checkRoom() const
{
    if (NULL == m_room) {
        throw LogicException(ExInfo("room is not ready"));
    }
}
//-----------------------------------------------------------------
/**
 * Add model at scene.
 * @param model new object
 * @param newUnit driver for the object or NULL
 * @return model index
 * @throws LogicException when room is not created yet
 */
    int
LevelScript::addModel(Cube *new_model, Unit *new_unit)
{
    if (NULL == m_room) {
        delete new_model;
        if (new_unit) {
            delete new_unit;
        }
        checkRoom();
    }
    return m_room->addModel(new_model, new_unit);
}
//-----------------------------------------------------------------
    Cube *
LevelScript::getModel(int model_index)
{
    checkRoom();
    return m_room->getModel(model_index);
}
//-----------------------------------------------------------------
/**
 * Returns model at location.
 */
    Cube *
LevelScript::askField(const V2 &loc)
{
    checkRoom();
    return m_room->askField(loc);
}

//-----------------------------------------------------------------
    int
LevelScript::getCycles() const
{
    checkRoom();
    return m_room->getCycles();
}
//-----------------------------------------------------------------
    void
LevelScript::addSound(const std::string &name, const Path &file)
{
    checkRoom();
    m_room->addSound(name, file);
}
//-----------------------------------------------------------------
    void
LevelScript::playSound(const std::string &name, int priority)
{
    checkRoom();
    m_room->playSound(name, priority);
}
//-----------------------------------------------------------------
/**
 * Register functions usable from script.
 */
    void
LevelScript::registerGameFuncs()
{
    m_script->registerFunc("game_addModel", script_game_addModel);
    m_script->registerFunc("game_getCycles", script_game_getCycles);

    m_script->registerFunc("model_addAnim", script_model_addAnim);
    m_script->registerFunc("model_addDuplexAnim", script_model_addDuplexAnim);
    m_script->registerFunc("model_runAnim", script_model_runAnim);
    m_script->registerFunc("model_setAnim", script_model_setAnim);
    m_script->registerFunc("model_useSpecialAnim", script_model_useSpecialAnim);
    m_script->registerFunc("model_setEffect", script_model_setEffect);
    m_script->registerFunc("model_getLoc", script_model_getLoc);
    m_script->registerFunc("model_getAction", script_model_getAction);
    m_script->registerFunc("model_getState", script_model_getState);
    m_script->registerFunc("model_isAlive", script_model_isAlive);
    m_script->registerFunc("model_isOut", script_model_isOut);
    m_script->registerFunc("model_isLeft", script_model_isLeft);
    m_script->registerFunc("model_getW", script_model_getW);
    m_script->registerFunc("model_getH", script_model_getH);
    m_script->registerFunc("model_setGoal", script_model_setGoal);
    m_script->registerFunc("model_change_turnSide",
            script_model_change_turnSide);
    m_script->registerFunc("model_change_setLocation",
            script_model_change_setLocation);
    m_script->registerFunc("model_setBusy", script_model_setBusy);
    m_script->registerFunc("model_equals", script_model_equals);

    m_script->registerFunc("sound_addSound", script_sound_addSound);
    m_script->registerFunc("sound_playSound", script_sound_playSound);

    registerLevelFuncs();
}
//-----------------------------------------------------------------
void
LevelScript::registerLevelFuncs()
{
    m_script->registerFunc("level_save", script_level_save);
    m_script->registerFunc("level_load", script_level_load);

    m_script->registerFunc("level_action_move", script_level_action_move);
    m_script->registerFunc("level_action_save", script_level_action_save);
    m_script->registerFunc("level_action_load", script_level_action_load);
    m_script->registerFunc("level_action_restart", script_level_action_restart);

    m_script->registerFunc("level_createRoom", script_level_createRoom);
    m_script->registerFunc("level_getRestartCounter",
            script_level_getRestartCounter);
    m_script->registerFunc("level_getDepth", script_level_getDepth);
    m_script->registerFunc("level_newDemo", script_level_newDemo);
    m_script->registerFunc("level_planShow", script_level_planShow);
    m_script->registerFunc("level_isShowing", script_level_isShowing);
}
