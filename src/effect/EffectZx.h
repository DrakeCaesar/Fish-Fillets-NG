#ifndef HEADER_EFFECTZX_H
#define HEADER_EFFECTZX_H

#include "ViewEffect.h"

/**
 * Blinking like ZX spectrum loading.
 */
class EffectZx : public ViewEffect {
    private:
        enum eCorner {
            ZX1 = 1,
            ZX2 = 2,
            ZX3 = 3,
            ZX4 = 4
        };
        static const double STRIPE_STANDARD = 38.5;
        static const double STRIPE_NARROW = 3.4;
        int m_zx;
        int m_phase;
        double m_countHeight;
        double m_stripeHeight;
    public:
        EffectZx();
        virtual void updateEffect();
        virtual void blit(SDL_Surface *screen, SDL_Surface *surface,
                int x, int y);
};

#endif
