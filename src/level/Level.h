#ifndef HEADER_LEVEL_H
#define HEADER_LEVEL_H

class Cube;
class Unit;
class DescFinder;
class PhaseLocker;
class Picture;
class DemoMode;
class KeyStroke;
class MouseStroke;
class LevelScript;
class LevelLoading;
class LevelStatus;
class LevelCountDown;
class CommandQueue;
class Command;
class MultiDrawer;
class StatusDisplay;

#include "Path.h"
#include "GameState.h"
#include "CountAdvisor.h"

#include <string>

/**
 * Game level with room.
 */
class Level : public GameState, public CountAdvisor {
    private:
        static const int SPEED_REPLAY = 1;

        int m_depth;
        const DescFinder *m_desc;
        std::string m_codename;
        Path m_datafile;
        PhaseLocker *m_locker;
        bool m_newRound;
        LevelScript *m_levelScript;
        LevelLoading *m_loading;
        LevelCountDown *m_countdown;
        CommandQueue *m_show;
        int m_restartCounter;
        bool m_insideUndo;
        bool m_wasDangerousMove;
        MultiDrawer *m_background;
        StatusDisplay *m_statusDisplay;
    private:
        void initScreen();
        void nextAction();
        void updateLevel();
        void saveUndo(const std::string &oldMoves);
        void finishLevel();
        void nextLoadAction();
        void nextShowAction();
        void nextPlayerAction();
        void saveSolution();
        void displaySaveStatus();
    protected:
        virtual void own_initState();
        virtual void own_updateState();
        virtual void own_pauseState();
        virtual void own_resumeState();
        virtual void own_cleanState();
        virtual void own_noteBg();
        virtual void own_noteFg();
    public:
        Level(const std::string &codename, const Path &datafile, int depth);
        virtual ~Level();
        virtual const char *getName() const { return "state_level"; };
        void fillDesc(const DescFinder *desc) { m_desc = desc; }
        void fillStatus(LevelStatus *status);

        void saveGame(const std::string &models);
        void loadGame(const std::string &moves);
        void loadReplay(const std::string &moves);

        bool action_restart();
        bool action_move(char symbol);
        bool action_save();
        bool action_load();
        bool action_undo();

        void switchFish();
        void controlEvent(const KeyStroke &stroke);
        void controlMouse(const MouseStroke &button);

        std::string getLevelName() const;
        int getRestartCounter() const { return m_restartCounter; }
        int getDepth() const { return m_depth; }
        bool isNewRound() const { return m_newRound; }

        void createRoom(int w, int h, const Path &picture);
        void newDemo(const Path &demofile);

        bool isLoading() const;
        void togglePause();
        bool isShowing() const;
        void interruptShow();
        void planShow(Command *new_command);

        virtual int getCountForSolved() const;
        virtual int getCountForWrong() const;
};

#endif
