#ifndef HEADER_ROOM_H
#define HEADER_ROOM_H

class Path;
class Field;
class WavyPicture;
class Controls;
class KeyStroke;
class Unit;
class ResSoundPack;
class PhaseLocker;
class Planner;
class View;
class Decor;
class InputProvider;

#include "Drawable.h"
#include "Cube.h"
#include "StepCounter.h"

#include <string>

/**
 * Room with level.
 */
class Room : public StepCounter, public Drawable {
    private:
        WavyPicture *m_bg;
        Field *m_field;
        ResSoundPack *m_soundPack;
        Controls *m_controls;
        PhaseLocker *m_locker;
        Planner *m_levelScript;
        View *m_view;
        Cube::t_models m_models;
        Cube::eWeight m_impact;
        int m_startTime;
        bool m_fresh;
    private:
        void killPlan();
        void prepareRound();
        bool fallout(bool interactive=true);
        bool falldown();
        void playImpact();
        void playDead(Cube *model);
    public:
        Room(int w, int h, const Path &picture,
                PhaseLocker *locker, Planner *levelScript);
        ~Room();
        void setWaves(double amplitude, double periode, double speed);
        void addDecor(Decor *new_decor);

        int addModel(Cube *new_model, Unit *new_unit);
        Cube *getModel(int model_index);
        Cube *askField(const V2 &loc);

        bool beginFall(bool interactive=true);
        void nextRound(const InputProvider *input);
        void finishRound(bool interactive=true);


        void switchFish();
        void controlEvent(const KeyStroke &stroke);
        void loadMove(char move);
        bool makeMove(char move);
        bool cannotMove() const;
        bool isSolvable() const;
        bool isSolved() const;
        int getStepCount() const;
        std::string getMoves() const;
        void unBusyUnits();

        int getW() const;
        int getH() const;
        int getCycles() const;
        void addSound(const std::string &name, const Path &file);
        void playSound(const std::string &name, int volume=100);

        void setScreenShift(const V2 &shift);
        virtual void drawOn(SDL_Surface *screen);
};

#endif
