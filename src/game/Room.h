#ifndef HEADER_ROOM_H
#define HEADER_ROOM_H

class Field;
class Picture;
class Controls;
class Unit;

#include "NoCopy.h"
#include "Path.h"
#include "Field.h"
#include "Cube.h"

#include <string>

/**
 * Room with level.
 */
class Room : public NoCopy {
    private:
        Picture *m_bg;
        Field *m_field;
        Cube::t_models m_models;
        Cube::eWeight m_impact;
        Controls *m_controls;
    private:
        void prepareRound();
        bool falldown();
        void playImpact();
        bool finishRound();
    public:
        Room(int w, int h, const Path &picture);
        ~Room();

        int addModel(Cube *model);
        Cube *getModel(int model_index);
        Cube *askField(const V2 &loc);
        bool nextRound();

        int getW() const { return m_field->getW(); }
        int getH() const { return m_field->getH(); }

        void addUnit(Unit *unit);
        void switchFish();
        std::string getMoves() const;
        bool loadMove(char move);
};

#endif
