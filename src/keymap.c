/*
 * original from ibus-fbterm
 */

#include <linux/keyboard.h>
#include <fcitx-config/hotkey.h>
#include "keymap.h"

static FcitxKeySym linux_to_x[256] = {
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_BackSpace,   FcitxKey_Tab,     FcitxKey_Linefeed,    FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_Escape,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_space,   FcitxKey_exclam,  FcitxKey_quotedbl,    FcitxKey_numbersign,
    FcitxKey_dollar,  FcitxKey_percent, FcitxKey_ampersand,   FcitxKey_apostrophe,
    FcitxKey_parenleft,   FcitxKey_parenright,  FcitxKey_asterisk,    FcitxKey_plus,
    FcitxKey_comma,   FcitxKey_minus,   FcitxKey_period,  FcitxKey_slash,
    FcitxKey_0,       FcitxKey_1,       FcitxKey_2,       FcitxKey_3,
    FcitxKey_4,       FcitxKey_5,       FcitxKey_6,       FcitxKey_7,
    FcitxKey_8,       FcitxKey_9,       FcitxKey_colon,   FcitxKey_semicolon,
    FcitxKey_less,    FcitxKey_equal,   FcitxKey_greater, FcitxKey_question,
    FcitxKey_at,      FcitxKey_A,       FcitxKey_B,       FcitxKey_C,
    FcitxKey_D,       FcitxKey_E,       FcitxKey_F,       FcitxKey_G,
    FcitxKey_H,       FcitxKey_I,       FcitxKey_J,       FcitxKey_K,
    FcitxKey_L,       FcitxKey_M,       FcitxKey_N,       FcitxKey_O,
    FcitxKey_P,       FcitxKey_Q,       FcitxKey_R,       FcitxKey_S,
    FcitxKey_T,       FcitxKey_U,       FcitxKey_V,       FcitxKey_W,
    FcitxKey_X,       FcitxKey_Y,       FcitxKey_Z,       FcitxKey_bracketleft,
    FcitxKey_backslash,   FcitxKey_bracketright,FcitxKey_asciicircum, FcitxKey_underscore,
    FcitxKey_grave,   FcitxKey_a,       FcitxKey_b,       FcitxKey_c,
    FcitxKey_d,       FcitxKey_e,       FcitxKey_f,       FcitxKey_g,
    FcitxKey_h,       FcitxKey_i,       FcitxKey_j,       FcitxKey_k,
    FcitxKey_l,       FcitxKey_m,       FcitxKey_n,       FcitxKey_o,
    FcitxKey_p,       FcitxKey_q,       FcitxKey_r,       FcitxKey_s,
    FcitxKey_t,       FcitxKey_u,       FcitxKey_v,       FcitxKey_w,
    FcitxKey_x,       FcitxKey_y,       FcitxKey_z,       FcitxKey_braceleft,
    FcitxKey_bar,     FcitxKey_braceright,  FcitxKey_asciitilde,  FcitxKey_BackSpace,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_None,   FcitxKey_None,   FcitxKey_None,   FcitxKey_None,
    FcitxKey_nobreakspace,FcitxKey_exclamdown,  FcitxKey_cent,    FcitxKey_sterling,
    FcitxKey_currency,    FcitxKey_yen,     FcitxKey_brokenbar,   FcitxKey_section,
    FcitxKey_diaeresis,   FcitxKey_copyright,   FcitxKey_ordfeminine, FcitxKey_guillemotleft,
    FcitxKey_notsign, FcitxKey_hyphen,  FcitxKey_registered,  FcitxKey_macron,
    FcitxKey_degree,  FcitxKey_plusminus,   FcitxKey_twosuperior, FcitxKey_threesuperior,
    FcitxKey_acute,   FcitxKey_mu,      FcitxKey_paragraph,   FcitxKey_periodcentered,
    FcitxKey_cedilla, FcitxKey_onesuperior, FcitxKey_masculine,   FcitxKey_guillemotright,
    FcitxKey_onequarter,  FcitxKey_onehalf, FcitxKey_threequarters,FcitxKey_questiondown,
    FcitxKey_Agrave,  FcitxKey_Aacute,  FcitxKey_Acircumflex, FcitxKey_Atilde,
    FcitxKey_Adiaeresis,  FcitxKey_Aring,   FcitxKey_AE,      FcitxKey_Ccedilla,
    FcitxKey_Egrave,  FcitxKey_Eacute,  FcitxKey_Ecircumflex, FcitxKey_Ediaeresis,
    FcitxKey_Igrave,  FcitxKey_Iacute,  FcitxKey_Icircumflex, FcitxKey_Idiaeresis,
    FcitxKey_ETH,     FcitxKey_Ntilde,  FcitxKey_Ograve,  FcitxKey_Oacute,
    FcitxKey_Ocircumflex, FcitxKey_Otilde,  FcitxKey_Odiaeresis,  FcitxKey_multiply,
    FcitxKey_Ooblique,    FcitxKey_Ugrave,  FcitxKey_Uacute,  FcitxKey_Ucircumflex,
    FcitxKey_Udiaeresis,  FcitxKey_Yacute,  FcitxKey_THORN,   FcitxKey_ssharp,
    FcitxKey_agrave,  FcitxKey_aacute,  FcitxKey_acircumflex, FcitxKey_atilde,
    FcitxKey_adiaeresis,  FcitxKey_aring,   FcitxKey_ae,      FcitxKey_ccedilla,
    FcitxKey_egrave,  FcitxKey_eacute,  FcitxKey_ecircumflex, FcitxKey_ediaeresis,
    FcitxKey_igrave,  FcitxKey_iacute,  FcitxKey_icircumflex, FcitxKey_idiaeresis,
    FcitxKey_eth,     FcitxKey_ntilde,  FcitxKey_ograve,  FcitxKey_oacute,
    FcitxKey_ocircumflex, FcitxKey_otilde,  FcitxKey_odiaeresis,  FcitxKey_division,
    FcitxKey_oslash,  FcitxKey_ugrave,  FcitxKey_uacute,  FcitxKey_ucircumflex,
    FcitxKey_udiaeresis,  FcitxKey_yacute,  FcitxKey_thorn,   FcitxKey_ydiaeresis
};

FcitxKeySym linux_keysym_to_fcitx_keysym(unsigned short keysym, unsigned short keycode)
{
    unsigned kval = KVAL(keysym),  keyval = 0;

    switch (KTYP(keysym)) {
    case KT_LATIN:
    case KT_LETTER:
        keyval = linux_to_x[kval];
        break;

    case KT_FN:
        if (kval <= 19)
            keyval = FcitxKey_F1 + kval;
        else switch (keysym) {
            case K_FIND:
                keyval = FcitxKey_Home; /* or FcitxKey_Find */
                break;
            case K_INSERT:
                keyval = FcitxKey_Insert;
                break;
            case K_REMOVE:
                keyval = FcitxKey_Delete;
                break;
            case K_SELECT:
                keyval = FcitxKey_End; /* or FcitxKey_Select */
                break;
            case K_PGUP:
                keyval = FcitxKey_Prior;
                break;
            case K_PGDN:
                keyval = FcitxKey_Next;
                break;
            case K_HELP:
                keyval = FcitxKey_Help;
                break;
            case K_DO:
                keyval = FcitxKey_Execute;
                break;
            case K_PAUSE:
                keyval = FcitxKey_Pause;
                break;
            case K_MACRO:
                keyval = FcitxKey_Menu;
                break;
            default:
                break;
            }
        break;

    case KT_SPEC:
        switch (keysym) {
        case K_ENTER:
            keyval = FcitxKey_Return;
            break;
        case K_BREAK:
            keyval = FcitxKey_Break;
            break;
        case K_CAPS:
            keyval = FcitxKey_Caps_Lock;
            break;
        case K_NUM:
            keyval = FcitxKey_Num_Lock;
            break;
        case K_HOLD:
            keyval = FcitxKey_Scroll_Lock;
            break;
        case K_COMPOSE:
            keyval = FcitxKey_Multi_key;
            break;
        default:
            break;
        }
        break;

    case KT_PAD:
        switch (keysym) {
        case K_PPLUS:
            keyval = FcitxKey_KP_Add;
            break;
        case K_PMINUS:
            keyval = FcitxKey_KP_Subtract;
            break;
        case K_PSTAR:
            keyval = FcitxKey_KP_Multiply;
            break;
        case K_PSLASH:
            keyval = FcitxKey_KP_Divide;
            break;
        case K_PENTER:
            keyval = FcitxKey_KP_Enter;
            break;
        case K_PCOMMA:
            keyval = FcitxKey_KP_Separator;
            break;
        case K_PDOT:
            keyval = FcitxKey_KP_Decimal;
            break;
        case K_PPLUSMINUS:
            keyval = FcitxKey_KP_Subtract;
            break;
        default:
            if (kval <= 9)
                keyval = FcitxKey_KP_0 + kval;
            break;
        }
        break;

        /*
         * KT_DEAD keys are for accelerated diacritical creation.
         */
    case KT_DEAD:
        switch (keysym) {
        case K_DGRAVE:
            keyval = FcitxKey_dead_grave;
            break;
        case K_DACUTE:
            keyval = FcitxKey_dead_acute;
            break;
        case K_DCIRCM:
            keyval = FcitxKey_dead_circumflex;
            break;
        case K_DTILDE:
            keyval = FcitxKey_dead_tilde;
            break;
        case K_DDIERE:
            keyval = FcitxKey_dead_diaeresis;
            break;
        }
        break;

    case KT_CUR:
        switch (keysym) {
        case K_DOWN:
            keyval = FcitxKey_Down;
            break;
        case K_LEFT:
            keyval = FcitxKey_Left;
            break;
        case K_RIGHT:
            keyval = FcitxKey_Right;
            break;
        case K_UP:
            keyval = FcitxKey_Up;
            break;
        }
        break;

    case KT_SHIFT:
        switch (keysym) {
        case K_ALTGR:
            keyval = FcitxKey_Alt_R;
            break;
        case K_ALT:
            keyval = (keycode == 0x64 ?
                      FcitxKey_Alt_R : FcitxKey_Alt_L);
            break;
        case K_CTRL:
            keyval = (keycode == 0x61 ?
                      FcitxKey_Control_R : FcitxKey_Control_L);
            break;
        case K_CTRLL:
            keyval = FcitxKey_Control_L;
            break;
        case K_CTRLR:
            keyval = FcitxKey_Control_R;
            break;
        case K_SHIFT:
            keyval = (keycode == 0x36 ?
                      FcitxKey_Shift_R : FcitxKey_Shift_L);
            break;
        case K_SHIFTL:
            keyval = FcitxKey_Shift_L;
            break;
        case K_SHIFTR:
            keyval = FcitxKey_Shift_R;
            break;
        default:
            break;
        }
        break;

        /*
         * KT_ASCII keys accumulate a 3 digit decimal number that gets
         * emitted when the shift state changes. We can't emulate that.
         */
    case KT_ASCII:
        break;

    case KT_LOCK:
        if (keysym == K_SHIFTLOCK)
            keyval = FcitxKey_Shift_Lock;
        break;

    default:
        break;
    }

    return keyval;
}

FcitxKeyState calculate_modifiers(FcitxKeyState state, FcitxKeySym keyval, char down)
{
    FcitxKeyState mask = 0;
    switch (keyval) {
    case FcitxKey_Shift_L:
    case FcitxKey_Shift_R:
        mask = FcitxKeyState_Shift;
        break;

    case FcitxKey_Control_L:
    case FcitxKey_Control_R:
        mask = FcitxKeyState_Ctrl;
        break;

    case FcitxKey_Alt_L:
    case FcitxKey_Alt_R:
    case FcitxKey_Meta_L:
        mask = FcitxKeyState_Alt;
        break;

    default:
        break;
    }

    if (mask) {
        if (down) state |= mask;
        else state &= ~mask;
    }

    return state;
}

