/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "MenuOptions.h"

#include "Path.h"
#include "HBox.h"
#include "VBox.h"
#include "WiPicture.h"
#include "WiSpace.h"
#include "WiButton.h"
#include "Slider.h"
#include "SelectLang.h"
#include "RadioBox.h"

#include "SimpleMsg.h"
#include "OptionsInput.h"
#include "OptionAgent.h"
#include "SurfaceTool.h"

//-----------------------------------------------------------------
MenuOptions::MenuOptions()
{
    IWidget *soundBox = createSoundPanel();
    IWidget *musicBox = createMusicPanel();

    VBox *vbox = new VBox();
    vbox->addWidget(soundBox);
    vbox->addWidget(new WiSpace(0, 10));
    vbox->addWidget(musicBox);
    vbox->addWidget(new WiSpace(0, 10));
    vbox->addWidget(new SelectLang(
                Path::dataReadPath("script/select_lang.lua")));
    vbox->addWidget(new WiSpace(0, 10));
    vbox->addWidget(createSubtitlesPanel());

    IWidget *backButton = createBackButton();
    HBox *backBox = new HBox();
    backBox->addWidget(new WiSpace(musicBox->getW() - backButton->getW(), 0));
    backBox->addWidget(backButton);

    vbox->addWidget(backBox);
    m_container = vbox;

    takeHandler(new OptionsInput(this));
    registerDrawable(this);
    registerDrawable(m_container);
}
//-----------------------------------------------------------------
MenuOptions::~MenuOptions()
{
    delete m_container;
}
//-----------------------------------------------------------------
/**
 * Display menu.
 */
    void
MenuOptions::own_initState()
{
    own_resumeState();
}
//-----------------------------------------------------------------
    void
MenuOptions::own_updateState()
{
    //TODO: implement volume controls
}
//-----------------------------------------------------------------
/**
 * Display options menu.
 * Menu will be centred on screen.
 */
    void
MenuOptions::own_resumeState()
{
    int contentW = m_container->getW();
    int contentH = m_container->getH();
    OptionAgent *options = OptionAgent::agent();
    int screenW = options->getAsInt("screen_width");
    int screenH = options->getAsInt("screen_height");

    m_container->setShift(
            V2((screenW - contentW) / 2, (screenH - contentH) / 2));
}
//-----------------------------------------------------------------
IWidget *
MenuOptions::createSoundPanel()
{
    HBox *soundBox = new HBox();
    soundBox->addWidget(new WiPicture(
                Path::dataReadPath("images/menu/volume_sound.png")));
    soundBox->addWidget(new WiSpace(10, 0));
    soundBox->addWidget(new Slider("volume_sound", 0, 100));
    return soundBox;
}
//-----------------------------------------------------------------
IWidget *
MenuOptions::createMusicPanel()
{
    HBox *musicBox = new HBox();
    musicBox->addWidget(new WiPicture(
                Path::dataReadPath("images/menu/volume_music.png")));
    musicBox->addWidget(new WiSpace(10, 0));
    musicBox->addWidget(new Slider("volume_music", 0, 100));
    return musicBox;
}
//-----------------------------------------------------------------
IWidget *
MenuOptions::createSubtitlesPanel()
{
    HBox *chooseBox = new HBox();
    chooseBox->addWidget(new RadioBox("subtitles", "1",
                Path::dataReadPath("images/menu/subtitles/yes.png")));
    chooseBox->addWidget(new RadioBox("subtitles", "0",
                Path::dataReadPath("images/menu/subtitles/no.png")));
    return chooseBox;
}
//-----------------------------------------------------------------
IWidget *
MenuOptions::createBackButton()
{
    return new WiButton(
            new WiPicture(Path::dataReadPath("images/menu/back.png")),
            new SimpleMsg(this, "quit"));
}
//-----------------------------------------------------------------
/**
 * Start level under pressed button.
 * Start pedometer when level is solved already.
 */
    void
MenuOptions::mouseButton(const MouseStroke &stroke)
{
    m_container->mouseButton(stroke);
}
//-----------------------------------------------------------------
void
MenuOptions::drawOn(SDL_Surface *screen)
{
    SDL_Color gray = {0xf0, 0xf0, 0xf0, 128};
    SurfaceTool::alphaFill(screen, NULL, gray);
}

