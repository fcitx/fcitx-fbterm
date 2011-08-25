/*
 * original from ibus-fbterm
 */

#include <fcitx-config/hotkey.h>

FcitxKeySym linux_keysym_to_fcitx_keysym(unsigned short keysym, unsigned short keycode);

KEY_STATE calculate_modifiers(KEY_STATE state, FcitxKeySym keyval, char down);