#include "PlannedDialog.h"

#include "Dialog.h"

//-----------------------------------------------------------------
PlannedDialog::PlannedDialog(int actor, int delay, const Dialog *dialog)
{
    m_actor = actor;
    m_delay = delay;
    m_dialog = dialog;
    m_channel = -1;
}
//-----------------------------------------------------------------
/**
 * Run talk.
 */
void
PlannedDialog::talk()
{
    m_channel = m_dialog->talk();
}
//-----------------------------------------------------------------
/**
 * Stop talking.
 */
void
PlannedDialog::killTalk()
{
    if (isTalking()) {
        Mix_HaltChannel(m_channel);
    }
}
//-----------------------------------------------------------------
/**
 * Return true when our channel is playing and
 * our chunk is the last one on this channel.
 */
bool
PlannedDialog::isTalking()
{
    bool result = false;
    if (m_channel > -1) {
        if (Mix_Playing(m_channel)) {
            result = m_dialog->equalSound(Mix_GetChunk(m_channel));
        }
    }
    return result;
}