/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "PlannedDialog.h"

#include "Dialog.h"
#include "TimerAgent.h"

//-----------------------------------------------------------------
/**
 * Structure to store planned dialog.
 * @param actor who will talk
 * @param dialog what will talk, shared resource
 */
PlannedDialog::PlannedDialog(int actor, Dialog *dialog)
{
    m_actor = actor;
    m_dialog = dialog;
    m_channel = -1;
    m_endtime = 0;
}
//-----------------------------------------------------------------
/**
 * Run talk.
 * @param volume sound volume
 * @param loops numer of loops. 0=play once, 1=play twice, -1=play infinite
 */
void
PlannedDialog::talk(int volume, int loops)
{
    m_channel = m_dialog->talk(volume, loops);
    m_endtime = m_dialog->getMinTime() + TimerAgent::agent()->getCycles();
}

//-----------------------------------------------------------------
bool
PlannedDialog::equalsActor(int other) const
{
    return m_actor == other;
}
//-----------------------------------------------------------------
/**
 * Stop talking.
 */
void
PlannedDialog::killTalk()
{
    if (isPlaying()) {
        Mix_HaltChannel(m_channel);
    }
}
//-----------------------------------------------------------------
/**
 * Return true when our channel is playing and
 * our chunk is the last one on this channel.
 */
bool
PlannedDialog::isPlaying()
{
    bool result = false;
    if (m_channel > -1) {
        if (Mix_Playing(m_channel)) {
            result = m_dialog->equalSound(Mix_GetChunk(m_channel));
        }
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Return true when is playing or 
 * return true for minimal time according subtitle length.
 */
bool
PlannedDialog::isTalking()
{
    bool result = false;
    if (m_channel > -1) {
        result = isPlaying();
    }
    else {
        result = m_endtime > TimerAgent::agent()->getCycles();
    }

    return result;
}

