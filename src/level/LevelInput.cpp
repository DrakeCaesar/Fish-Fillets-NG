/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LevelInput.h"

#include "Level.h"
#include "Keymap.h"

#include "Log.h"
#include "KeyStroke.h"

//-----------------------------------------------------------------
LevelInput::LevelInput(Level *level)
{
    m_level = level;

    //TODO: help on F1
    m_keymap = new Keymap();
    m_keymap->registerKey(KeyStroke(SDLK_ESCAPE, KMOD_NONE),
            KeyDesc(KEY_QUIT, "quit"));
    m_keymap->registerKey(KeyStroke(SDLK_SPACE, KMOD_NONE),
            KeyDesc(KEY_SWITCH, "switch"));
    m_keymap->registerKey(KeyStroke(SDLK_F2, KMOD_NONE),
            KeyDesc(KEY_SAVE, "save"));
    m_keymap->registerKey(KeyStroke(SDLK_F3, KMOD_NONE),
            KeyDesc(KEY_LOAD, "load"));
    m_keymap->registerKey(KeyStroke(SDLK_BACKSPACE, KMOD_NONE),
            KeyDesc(KEY_RESTART, "restart"));
}
//-----------------------------------------------------------------
LevelInput::~LevelInput()
{
    delete m_keymap;
}
//-----------------------------------------------------------------
    void
LevelInput::keyEvent(const KeyStroke &stroke)
{
    switch (m_keymap->indexPressed(stroke)) {
        case KEY_QUIT:
            m_level->quitState();
            break;
        case KEY_SWITCH:
            if (!m_level->isPlanning()) {
                m_level->switchFish();
            }
            break;
        case KEY_SAVE:
            if (!m_level->isPlanning()) {
                m_level->action_save();
            }
            break;
        case KEY_LOAD:
            m_level->interruptPlan();
            m_level->action_load();
            break;
        case KEY_RESTART:
            m_level->interruptPlan();
            m_level->action_restart();
            break;
        case -1:
            break;
        default:
            LOG_WARNING(ExInfo("unknown key")
                    .addInfo("index", m_keymap->indexPressed(stroke))
                    .addInfo("stroke", stroke.toString()));
    }
}
