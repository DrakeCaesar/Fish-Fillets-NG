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

#include "KeyStroke.h"
#include "OptionAgent.h"
#include "MenuOptions.h"

//-----------------------------------------------------------------
LevelInput::LevelInput(Level *level)
    : GameInput(level)
{
    //TODO: help on F1
    m_keymap->registerKey(KeyStroke(SDLK_SPACE, KMOD_NONE),
            KeyDesc(KEY_SWITCH, "switch"));
    m_keymap->registerKey(KeyStroke(SDLK_F2, KMOD_NONE),
            KeyDesc(KEY_SAVE, "save"));
    m_keymap->registerKey(KeyStroke(SDLK_F3, KMOD_NONE),
            KeyDesc(KEY_LOAD, "load"));
    m_keymap->registerKey(KeyStroke(SDLK_BACKSPACE, KMOD_NONE),
            KeyDesc(KEY_RESTART, "restart"));
    m_keymap->registerKey(KeyStroke(SDLK_F5, KMOD_NONE),
            KeyDesc(KEY_SHOW_STEPS, "show number of steps"));
}
//-----------------------------------------------------------------
Level *
LevelInput::getLevel()
{
    return dynamic_cast<Level*>(m_state);
}
//-----------------------------------------------------------------
void
LevelInput::specKey(int keyIndex)
{
    switch (keyIndex) {
        case KEY_SWITCH:
            if (!getLevel()->isShowing() && !getLevel()->isLoading()) {
                getLevel()->switchFish();
            }
            break;
        case KEY_SAVE:
            if (!getLevel()->isShowing() && !getLevel()->isLoading()) {
                getLevel()->action_save();
            }
            break;
        case KEY_LOAD:
            getLevel()->interruptShow();
            getLevel()->action_load();
            break;
        case KEY_RESTART:
            getLevel()->interruptShow();
            getLevel()->action_restart();
            break;
        case KEY_SHOW_STEPS:
            toggleShowSteps();
            break;
        default:
            GameInput::specKey(keyIndex);
    }
}
//-----------------------------------------------------------------
void
LevelInput::specStroke(const KeyStroke &stroke)
{
    if (!getLevel()->isShowing() && !getLevel()->isLoading()) {
        getLevel()->controlEvent(stroke);
    }
}
//-----------------------------------------------------------------
    void
LevelInput::toggleShowSteps()
{
    OptionAgent *option = OptionAgent::agent();
    bool show = option->getAsInt("show_steps");

    option->setPersistent("show_steps", !show);
}
