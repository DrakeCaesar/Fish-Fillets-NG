/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WorldMap.h"

#include "LevelNode.h"
#include "NodeDrawer.h"
#include "LevelStatus.h"
#include "ResImagePack.h"
#include "StateManager.h"
#include "WorldInput.h"

#include "worldmap-script.h"
#include "Log.h"
#include "Path.h"
#include "WorldBranch.h"
#include "OptionAgent.h"
#include "VideoAgent.h"
#include "SoundAgent.h"
#include "LogicException.h"
#include "ResDialogPack.h"
#include "LevelDesc.h"
#include "ScriptState.h"
#include "Level.h"
#include "Pedometer.h"
#include "LayeredPicture.h"
#include "MenuOptions.h"
#include "PosterState.h"

//-----------------------------------------------------------------
WorldMap::WorldMap()
{
    m_selected = NULL;
    m_startNode = NULL;
    prepareBg();

    m_drawer = new NodeDrawer();
    m_descPack = new ResDialogPack();
    m_levelStatus = new LevelStatus();
    takeHandler(new WorldInput(this));
    registerDrawable(m_bg);
    registerDrawable(this);
}
//-----------------------------------------------------------------
WorldMap::~WorldMap()
{
    if (m_startNode) {
        delete m_startNode;
    }
    delete m_bg;
    m_descPack->removeAll();
    delete m_descPack;
    delete m_drawer;
    delete m_levelStatus;
}
//-----------------------------------------------------------------
/**
 * Prepare background with buttons.
 */
    void
WorldMap::prepareBg()
{
    //TODO: allow to set differect images
    m_bg = new LayeredPicture(
            Path::dataReadPath("images/menu/map.png"),
            V2(0, 0),
            Path::dataReadPath("images/menu/map_lower.png"),
            Path::dataReadPath("images/menu/map_mask.png"));

    m_maskIntro = m_bg->getMaskAt(V2(0, 0));
    m_maskExit = m_bg->getMaskAt(V2(m_bg->getW() - 1, 0));
    m_maskCredits = m_bg->getMaskAt(V2(0, m_bg->getH() - 1));
    m_maskOptions = m_bg->getMaskAt(V2(m_bg->getW() - 1, m_bg->getH() - 1));
    m_activeMask = m_bg->getNoMask();
}
//-----------------------------------------------------------------
    void
WorldMap::addDesc(const std::string &codename, LevelDesc *desc)
{
    m_descPack->addRes(codename, desc);
}
//-----------------------------------------------------------------
/**
 * Read dots postions and level descriptions.
 * @throws LogicException when cannot parse data file
 */
    void
WorldMap::initMap(const Path &mapfile, const Path &descfile)
{
    WorldBranch parser(NULL);
    m_startNode = parser.parseMap(mapfile);
    if (NULL == m_startNode) {
        throw LogicException(ExInfo("cannot create world map")
                .addInfo("file", mapfile.getNative()));
    }

    ScriptState script;
    script.registerLeader(this);
    script.registerFunc("worldmap_addDesc", script_worldmap_addDesc);
    script.doFile(descfile);
}
//-----------------------------------------------------------------
/**
 * Display menu and play menu music.
 */
    void
WorldMap::own_initState()
{
    m_levelStatus->setRunning(true);
    own_resumeState();
}
//-----------------------------------------------------------------
    void
WorldMap::own_updateState()
{
    watchCursor();
}
//-----------------------------------------------------------------
/**
 * Display menu and play menu music.
 */
    void
WorldMap::own_resumeState()
{
    if (m_levelStatus->wasRunning()) {
        if (m_levelStatus->isComplete()) {
            markSolved();
        }
        m_levelStatus->setRunning(false);
        m_selected = NULL;

        SoundAgent::agent()->playMusic(
                Path::dataReadPath("music/menu.ogg"), NULL);
        OptionAgent *options = OptionAgent::agent();
        options->setParam("caption", findDesc("menu"));
        options->setParam("screen_width", m_bg->getW());
        options->setParam("screen_height", m_bg->getH());
        VideoAgent::agent()->initVideoMode();
    }
}
//-----------------------------------------------------------------
/**
 * Stop music.
 */
    void
WorldMap::own_cleanState()
{
    SoundAgent::agent()->stopMusic();
}
//-----------------------------------------------------------------
/**
 * Mark node or mask under cursor as selected.
 */
    void
WorldMap::watchCursor()
{
    V2 mouseLoc = getInput()->getMouseLoc();
    m_selected = m_startNode->findSelected(mouseLoc);

    m_activeMask = m_bg->getMaskAtWorld(mouseLoc);
    if (m_activeMask == m_maskIntro
            || m_activeMask == m_maskExit
            || m_activeMask == m_maskCredits
            || m_activeMask == m_maskOptions)
    {
        m_bg->setActiveMask(m_activeMask);
    }
    else {
        m_bg->setNoActive();
    }
}
//-----------------------------------------------------------------
/**
 * Start level under pressed button.
 * Start pedometer when level is solved already.
 */
    void
WorldMap::runSelected()
{
    Level *level = createSelected();
    if (level && m_selected) {
        m_levelStatus->prepareRun(m_selected->getCodename(),
                findLevelName(m_selected->getCodename()),
                m_selected->getPoster());
        level->fillStatus(m_levelStatus);

        if (m_selected->getState() == LevelNode::STATE_SOLVED) {
            Pedometer *pedometer = new Pedometer(m_levelStatus, level);
            pushState(pedometer);
        }
        else {
            pushState(level);
        }
    }
    else {
        if (m_activeMask == m_maskIntro) {
            runIntro();
        }
        else if (m_activeMask == m_maskExit) {
            quitState();
        }
        else if (m_activeMask == m_maskCredits) {
            runCredits();
        }
        else if (m_activeMask == m_maskOptions) {
            runOptions();
        }
    }
}
//-----------------------------------------------------------------
/**
 * Return selected level or NULL;
 */
Level *
WorldMap::createSelected() const
{
    Level *result = NULL;
    if (m_selected) {
        result = m_selected->createLevel();
        result->fillDesc(this);
    }
    return result;
}
//-----------------------------------------------------------------
    void
WorldMap::markSolved()
{
    if (m_selected) {
        m_selected->setState(LevelNode::STATE_SOLVED);
    }
}
//-----------------------------------------------------------------
    void
WorldMap::drawOn(SDL_Surface *screen)
{
    m_drawer->setScreen(screen);
    m_startNode->drawPath(m_drawer);
    if (m_selected) {
        m_drawer->drawSelected(findLevelName(m_selected->getCodename()));
    }
}
//-----------------------------------------------------------------
std::string
WorldMap::findLevelName(const std::string &codename) const
{
    std::string result;
    LevelDesc *desc =
        dynamic_cast<LevelDesc*>(m_descPack->findDialogHard(codename));
    if (desc) {
        result = desc->getLevelName();
    }
    else {
        result = codename;
    }
    return result;
}
//-----------------------------------------------------------------
std::string
WorldMap::findDesc(const std::string &codename) const
{
    std::string result;
    LevelDesc *desc =
        dynamic_cast<LevelDesc*>(m_descPack->findDialogHard(codename));
    if (desc) {
        result = desc->getDesc();
    }
    else {
        result = "???";
    }
    return result;
}
//-----------------------------------------------------------------
    void
WorldMap::runIntro()
{
    //TODO: intro as video
    pushState(new PosterState(Path::dataReadPath("images/menu/intro.png")));
}
//-----------------------------------------------------------------
    void
WorldMap::runCredits()
{
    LOG_INFO(ExInfo("credits are not implemented yet"));
    //pushState(new PosterState(Path::dataReadPath("images/menu/intro.png")));
}
//-----------------------------------------------------------------
    void
WorldMap::runOptions()
{
    MenuOptions *options = new MenuOptions();
    pushState(options);
}

