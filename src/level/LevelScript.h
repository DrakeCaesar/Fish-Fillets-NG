#ifndef HEADER_LEVELSCRIPT_H
#define HEADER_LEVELSCRIPT_H

class V2;
class Path;
class Cube;
class Unit;
class Room;
class Level;
class Command;

#include "Planner.h"

#include <string>

/**
 * Handle plan for dialogs and planned actions.
 */
class LevelScript : public Planner {
    private:
        Level *m_level;
        Room *m_room;
    private:
        void checkRoom() const;
        void registerGameFuncs();
        void registerLevelFuncs();
    public:
        LevelScript(Level *aLevel);
        ~LevelScript();
        void takeRoom(Room *new_room);

        void updateScript();
        void cleanRoom();
        virtual void interruptPlan();

        bool isRoom() const { return NULL != m_room; }
        Room *room();
        Level *level() { return m_level; }
        Command *createCommand(int funcRef);

        int addModel(Cube *new_model, Unit *new_unit);
        Cube *getModel(int model_index);
        Cube *askField(const V2 &loc);
        int getCycles() const;

        void addSound(const std::string &name, const Path &file);
        void playSound(const std::string &name, int volume);
};

#endif
