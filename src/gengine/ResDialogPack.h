#ifndef HEADER_RESDIALOGPACK_H
#define HEADER_RESDIALOGPACK_H

class Dialog;

#include "ResourcePack.h"
#include "Name.h"

#include "SDL.h"

/**
 * Multi-language dialogs pack.
 */
class ResDialogPack : public ResourcePack<Dialog*> {
    protected:
        virtual void unloadRes(Dialog *res);
        int matchScore(const std::string &first,
                const std::string &second) const;
    public:
        virtual const char *getName() const { return "dialog_pack"; }
        const Dialog *findDialog(const std::string &name,
                const std::string &lang);
        const Dialog *findDialogHard(const std::string &name,
                const std::string &select="lang");
};

#endif

