/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "dialog-script.h"

class Actor;

#include "Log.h"
#include "Path.h"
#include "FishDialog.h"
#include "DialogAgent.h"
#include "SubTitleAgent.h"
#include "SoundAgent.h"
#include "Planner.h"
#include "Color.h"

#include "def-script.h"

//-----------------------------------------------------------------
    inline Planner *
getPlanner(lua_State *L)
{
    return static_cast<Planner*>(script_getLeader(L));
}


//-----------------------------------------------------------------
/**
 * void file_include(filename)
 *
 * Do file in usedir or systemdir.
 */
    int
script_file_include(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *filename = luaL_checkstring(L, 1);

    getPlanner(L)->scriptInclude(Path::dataReadPath(filename));
    END_NOEXCEPTION;
    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * bool file_exists(filename)
 *
 * Returns true when such file exists in userdir or systemdir.
 */
    int
script_file_exists(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *filename = luaL_checkstring(L, 1);

    bool exists = Path::dataReadPath(filename).exists();
    lua_pushboolean(L, exists);
    END_NOEXCEPTION;
    //NOTE: return exists
    return 1;
}


//-----------------------------------------------------------------
/**
 * void game_planAction(func)
 */
    int
script_game_planAction(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    luaL_checktype(L, 1, LUA_TFUNCTION);
    int funcRef = luaL_ref(L, LUA_REGISTRYINDEX);

    getPlanner(L)->planAction(funcRef);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * bool game_isPlanning()
 */
    int
script_game_isPlanning(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    bool planning = getPlanner(L)->isPlanning();
    lua_pushboolean(L, planning);
    END_NOEXCEPTION;
    //NOTE: return planning
    return 1;
}
//-----------------------------------------------------------------
/**
 * bool game_killPlan()
 */
    int
script_game_killPlan(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    getPlanner(L)->interruptPlan();
    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
/**
 * bool dialog_empty()
 */
    int
script_dialog_empty(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    bool empty = DialogAgent::agent()->empty();
    lua_pushboolean(L, empty);
    END_NOEXCEPTION;
    //NOTE: return empty
    return 1;
}
//-----------------------------------------------------------------
/**
 * void dialog_addFont(fontname, red, green, blue)
 */
    int
script_dialog_addFont(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *name = luaL_checkstring(L, 1);
    int red = luaL_checkint(L, 2);
    int green = luaL_checkint(L, 3);
    int blue = luaL_checkint(L, 4);

    SubTitleAgent::agent()->addFont(name, new Color(red, green, blue));

    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void dialog_addDialog(name, lang, soundfile, fontname, subtitle)
 */
    int
script_dialog_addDialog(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *name = luaL_checkstring(L, 1);
    const char *lang = luaL_checkstring(L, 2);
    const char *soundfile = luaL_checkstring(L, 3);
    const char *fontname = luaL_optstring(L, 4, "");
    const char *subtitle = luaL_optstring(L, 5, "");

    FishDialog *dialog =
        new FishDialog(lang, soundfile, subtitle, fontname);
    DialogAgent::agent()->addDialog(name, dialog);

    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * bool model_isTalking(model_index)
 */
    int
script_model_isTalking(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);

    bool talking = DialogAgent::agent()->isTalking(model_index);
    lua_pushboolean(L, talking);
    END_NOEXCEPTION;
    //NOTE: return talking
    return 1;
}
//-----------------------------------------------------------------
/**
 * void model_talk(model_index, name, volume, loops)
 */
    int
script_model_talk(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *name = luaL_checkstring(L, 2);
    int volume = luaL_optint(L, 3, 75);
    int loops = luaL_optint(L, 4, 0);

    DialogAgent::agent()->actorTalk(model_index, name, volume, loops);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_killSound(model_index)
 */
    int
script_model_killSound(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);

    DialogAgent::agent()->killSound(model_index);
    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
/**
 * void sound_playMusic(music_name)
 */
    int
script_sound_playMusic(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *music_name = luaL_checkstring(L, 1);

    SoundAgent::agent()->playMusic(Path::dataReadPath(music_name), NULL);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void sound_stopMusic()
 */
    int
script_sound_stopMusic(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    SoundAgent::agent()->stopMusic();
    END_NOEXCEPTION;
    return 0;
}

