/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ResImageAgent.h"

#include "ImgException.h"
#include "SDLException.h"

#include "SDL_image.h"

//-----------------------------------------------------------------
ResImageAgent::ResImageAgent()
{
    m_pack = new ResImagePack();
}
//-----------------------------------------------------------------
ResImageAgent::~ResImageAgent()
{
    delete m_pack;
}
//-----------------------------------------------------------------
void
ResImageAgent::own_shutdown()
{
    m_pack->removeAll();
}

