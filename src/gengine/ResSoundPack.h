#ifndef HEADER_RESSOUNDPACK_H
#define HEADER_RESSOUNDPACK_H

#include "ResourcePack.h"
#include "Path.h"

#include "SDL_mixer.h"

/**
 * Sound resources.
 */
class ResSoundPack : public ResourcePack<Mix_Chunk*> {
    protected:
        virtual void unloadRes(Mix_Chunk *res);
    public:
        virtual const char *getName() const { return "sound_pack"; }

        Mix_Chunk *loadSound(const Path &file);
        void addSound(const std::string &name, const Path &file);
};

#endif

