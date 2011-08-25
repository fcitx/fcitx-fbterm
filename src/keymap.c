/*
 * original from ibus-fbterm
 */

#include <linux/keyboard.h>
#include <fcitx-config/hotkey.h>
#include "keymap.h"

static FcitxKeySym linux_to_x[256] = {
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_BackSpace,   Key_Tab,     Key_Linefeed,    Key_None,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_None,   Key_None,   Key_None,   Key_Escape,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_space,   Key_exclam,  Key_quotedbl,    Key_numbersign,
    Key_dollar,  Key_percent, Key_ampersand,   Key_apostrophe,
    Key_parenleft,   Key_parenright,  Key_asterisk,    Key_plus,
    Key_comma,   Key_minus,   Key_period,  Key_slash,
    Key_0,       Key_1,       Key_2,       Key_3,
    Key_4,       Key_5,       Key_6,       Key_7,
    Key_8,       Key_9,       Key_colon,   Key_semicolon,
    Key_less,    Key_equal,   Key_greater, Key_question,
    Key_at,      Key_A,       Key_B,       Key_C,
    Key_D,       Key_E,       Key_F,       Key_G,
    Key_H,       Key_I,       Key_J,       Key_K,
    Key_L,       Key_M,       Key_N,       Key_O,
    Key_P,       Key_Q,       Key_R,       Key_S,
    Key_T,       Key_U,       Key_V,       Key_W,
    Key_X,       Key_Y,       Key_Z,       Key_bracketleft,
    Key_backslash,   Key_bracketright,Key_asciicircum, Key_underscore,
    Key_grave,   Key_a,       Key_b,       Key_c,
    Key_d,       Key_e,       Key_f,       Key_g,
    Key_h,       Key_i,       Key_j,       Key_k,
    Key_l,       Key_m,       Key_n,       Key_o,
    Key_p,       Key_q,       Key_r,       Key_s,
    Key_t,       Key_u,       Key_v,       Key_w,
    Key_x,       Key_y,       Key_z,       Key_braceleft,
    Key_bar,     Key_braceright,  Key_asciitilde,  Key_BackSpace,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_None,   Key_None,   Key_None,   Key_None,
    Key_nobreakspace,Key_exclamdown,  Key_cent,    Key_sterling,
    Key_currency,    Key_yen,     Key_brokenbar,   Key_section,
    Key_diaeresis,   Key_copyright,   Key_ordfeminine, Key_guillemotleft,
    Key_notsign, Key_hyphen,  Key_registered,  Key_macron,
    Key_degree,  Key_plusminus,   Key_twosuperior, Key_threesuperior,
    Key_acute,   Key_mu,      Key_paragraph,   Key_periodcentered,
    Key_cedilla, Key_onesuperior, Key_masculine,   Key_guillemotright,
    Key_onequarter,  Key_onehalf, Key_threequarters,Key_questiondown,
    Key_Agrave,  Key_Aacute,  Key_Acircumflex, Key_Atilde,
    Key_Adiaeresis,  Key_Aring,   Key_AE,      Key_Ccedilla,
    Key_Egrave,  Key_Eacute,  Key_Ecircumflex, Key_Ediaeresis,
    Key_Igrave,  Key_Iacute,  Key_Icircumflex, Key_Idiaeresis,
    Key_ETH,     Key_Ntilde,  Key_Ograve,  Key_Oacute,
    Key_Ocircumflex, Key_Otilde,  Key_Odiaeresis,  Key_multiply,
    Key_Ooblique,    Key_Ugrave,  Key_Uacute,  Key_Ucircumflex,
    Key_Udiaeresis,  Key_Yacute,  Key_THORN,   Key_ssharp,
    Key_agrave,  Key_aacute,  Key_acircumflex, Key_atilde,
    Key_adiaeresis,  Key_aring,   Key_ae,      Key_ccedilla,
    Key_egrave,  Key_eacute,  Key_ecircumflex, Key_ediaeresis,
    Key_igrave,  Key_iacute,  Key_icircumflex, Key_idiaeresis,
    Key_eth,     Key_ntilde,  Key_ograve,  Key_oacute,
    Key_ocircumflex, Key_otilde,  Key_odiaeresis,  Key_division,
    Key_oslash,  Key_ugrave,  Key_uacute,  Key_ucircumflex,
    Key_udiaeresis,  Key_yacute,  Key_thorn,   Key_ydiaeresis
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
            keyval = Key_F1 + kval;
        else switch (keysym) {
            case K_FIND:
                keyval = Key_Home; /* or Key_Find */
                break;
            case K_INSERT:
                keyval = Key_Insert;
                break;
            case K_REMOVE:
                keyval = Key_Delete;
                break;
            case K_SELECT:
                keyval = Key_End; /* or Key_Select */
                break;
            case K_PGUP:
                keyval = Key_Prior;
                break;
            case K_PGDN:
                keyval = Key_Next;
                break;
            case K_HELP:
                keyval = Key_Help;
                break;
            case K_DO:
                keyval = Key_Execute;
                break;
            case K_PAUSE:
                keyval = Key_Pause;
                break;
            case K_MACRO:
                keyval = Key_Menu;
                break;
            default:
                break;
            }
        break;

    case KT_SPEC:
        switch (keysym) {
        case K_ENTER:
            keyval = Key_Return;
            break;
        case K_BREAK:
            keyval = Key_Break;
            break;
        case K_CAPS:
            keyval = Key_Caps_Lock;
            break;
        case K_NUM:
            keyval = Key_Num_Lock;
            break;
        case K_HOLD:
            keyval = Key_Scroll_Lock;
            break;
        case K_COMPOSE:
            keyval = Key_Multi_key;
            break;
        default:
            break;
        }
        break;

    case KT_PAD:
        switch (keysym) {
        case K_PPLUS:
            keyval = Key_KP_Add;
            break;
        case K_PMINUS:
            keyval = Key_KP_Subtract;
            break;
        case K_PSTAR:
            keyval = Key_KP_Multiply;
            break;
        case K_PSLASH:
            keyval = Key_KP_Divide;
            break;
        case K_PENTER:
            keyval = Key_KP_Enter;
            break;
        case K_PCOMMA:
            keyval = Key_KP_Separator;
            break;
        case K_PDOT:
            keyval = Key_KP_Decimal;
            break;
        case K_PPLUSMINUS:
            keyval = Key_KP_Subtract;
            break;
        default:
            if (kval <= 9)
                keyval = Key_KP_0 + kval;
            break;
        }
        break;

        /*
         * KT_DEAD keys are for accelerated diacritical creation.
         */
    case KT_DEAD:
        switch (keysym) {
        case K_DGRAVE:
            keyval = Key_dead_grave;
            break;
        case K_DACUTE:
            keyval = Key_dead_acute;
            break;
        case K_DCIRCM:
            keyval = Key_dead_circumflex;
            break;
        case K_DTILDE:
            keyval = Key_dead_tilde;
            break;
        case K_DDIERE:
            keyval = Key_dead_diaeresis;
            break;
        }
        break;

    case KT_CUR:
        switch (keysym) {
        case K_DOWN:
            keyval = Key_Down;
            break;
        case K_LEFT:
            keyval = Key_Left;
            break;
        case K_RIGHT:
            keyval = Key_Right;
            break;
        case K_UP:
            keyval = Key_Up;
            break;
        }
        break;

    case KT_SHIFT:
        switch (keysym) {
        case K_ALTGR:
            keyval = Key_Alt_R;
            break;
        case K_ALT:
            keyval = (keycode == 0x64 ?
                      Key_Alt_R : Key_Alt_L);
            break;
        case K_CTRL:
            keyval = (keycode == 0x61 ?
                      Key_Control_R : Key_Control_L);
            break;
        case K_CTRLL:
            keyval = Key_Control_L;
            break;
        case K_CTRLR:
            keyval = Key_Control_R;
            break;
        case K_SHIFT:
            keyval = (keycode == 0x36 ?
                      Key_Shift_R : Key_Shift_L);
            break;
        case K_SHIFTL:
            keyval = Key_Shift_L;
            break;
        case K_SHIFTR:
            keyval = Key_Shift_R;
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
            keyval = Key_Shift_Lock;
        break;

    default:
        break;
    }

    return keyval;
}

KEY_STATE calculate_modifiers(KEY_STATE state, FcitxKeySym keyval, char down)
{
    KEY_STATE mask = 0;
    switch (keyval) {
    case Key_Shift_L:
    case Key_Shift_R:
        mask = KEY_SHIFT_COMP;
        break;

    case Key_Control_L:
    case Key_Control_R:
        mask = KEY_CTRL_COMP;
        break;

    case Key_Alt_L:
    case Key_Alt_R:
    case Key_Meta_L:
        mask = KEY_ALT_COMP;
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
