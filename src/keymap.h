/*
 * original from ibus-fbterm
 */

#include <fcitx-config/hotkey.h>

FcitxKeySym linux_keysym_to_fcitx_keysym(unsigned short keysym, unsigned short keycode);

FcitxKeyState calculate_modifiers(FcitxKeyState state, FcitxKeySym keyval, char down);