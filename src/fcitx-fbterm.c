/***************************************************************************
 *   Copyright (C) 2010~2011 by CSSlayer                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gio/gio.h>
#include <wchar.h>
#include <iconv.h>
#include <endian.h>
#include "imapi.h"
#include "client.h"
#include "keycode.h"
#include "keymap.h"

struct interval {
    unsigned first;
    unsigned last;
};

#define DRAW_NEXT_STRING(STRING, INDEX, VISIBLE_CONDITION) \
do { \
    rect.w = width; \
    rect.h = MARGIN * 2 + currentInfo.fontHeight; \
    \
    if ((VISIBLE_CONDITION)) \
    { \
        set_im_window((INDEX), rect); \
        fill_rect(rect, Gray); \
        draw_text(rect.x + MARGIN, rect.y + MARGIN, Black, Gray, (STRING), strlen(STRING)); \
        rect.y += rect.h; \
    } \
    else \
    { \
        rect.w = 0; \
        set_im_window((INDEX), rect); \
    } \
} while(0)

static void im_active(void);
static void im_deactive(void);
static void im_show(unsigned winid);
static void im_hide(void);
static void process_raw_key(char *buf, unsigned int len);
static void cursor_pos_changed(unsigned x, unsigned y);
static void update_fbterm_info(Info *info);
static gboolean iochannel_fbterm_callback(GIOChannel *source, GIOCondition condition, gpointer data);

static void _fcitx_fbterm_connect_cb(FcitxIMClient* client, void* user_data);
static void _fcitx_fbterm_destroy_cb(FcitxIMClient* client, void* user_data);
static void
_fcitx_fbterm_enable_im_cb(DBusGProxy* proxy, void* user_data);
static void
_fcitx_fbterm_close_im_cb(DBusGProxy* proxy, void* user_data);
static void
_fcitx_fbterm_commit_string_cb(DBusGProxy* proxy, char* str, void* user_data);
static void
_fcitx_fbterm_forward_key_cb(DBusGProxy* proxy, guint keyval, guint state, gint type, void* user_data);
static void
_fcitx_fbterm_update_client_side_ui_cb(DBusGProxy* proxy, char* auxup, char* auxdown, char* preedit, char* candidateword, char* _imname, int cursor_pos, void* user_data);
static unsigned text_width(char* str);
static int bisearch(unsigned ucs, const struct interval *table, unsigned max);
static int is_double_width(unsigned ucs);

static ImCallbacks cbs = {
    im_active, // .active
    im_deactive, // .deactive
    im_show,
    im_hide,
    process_raw_key, // .send_key
    cursor_pos_changed, // .cursor_position
    update_fbterm_info, // .fbterm_info
    update_term_mode // .term_mode
};

#define BUFSIZE 8192
#define MARGIN 3

static char raw_mode = 1;
static int cursor_x;
static int cursor_y;
static GMainLoop *main_loop;
static FcitxIMClient* client;
static KEY_STATE state;
static int active = 0;
static Info currentInfo;
static char textup[BUFSIZE];
static char textdown[BUFSIZE];
static char imname[BUFSIZE];
static iconv_t iconvW;

static void im_active(void)
{
    if (raw_mode) {
        init_keycode_state();
    }
    FcitxIMClientFocusIn(client);
    FcitxIMClientEnableIC(client);
    active = 1;
}

static void im_deactive(void)
{
    Rectangle rect = { 0, 0, 0, 0 };
    set_im_window(0, rect);
    set_im_window(1, rect);
    set_im_window(2, rect);
    FcitxIMClientCloseIC(client);
    state = 0;
    active = 0;
}

static int bisearch(unsigned ucs, const struct interval *table, unsigned max)
{
    unsigned min = 0;
    unsigned mid;

    if (ucs < table[0].first || ucs > table[max].last)
        return 0;
    while (max >= min) {
        mid = (min + max) / 2;
        if (ucs > table[mid].last)
            min = mid + 1;
        else if (ucs < table[mid].first)
            max = mid - 1;
        else
            return 1;
    }
    return 0;
}

static int is_double_width(unsigned ucs)
{
    ucs = be32toh(ucs);
    static const struct interval double_width[] = {
        { 0x1100, 0x115F}, { 0x2329, 0x232A}, { 0x2E80, 0x303E},
        { 0x3040, 0xA4CF}, { 0xAC00, 0xD7A3}, { 0xF900, 0xFAFF},
        { 0xFE10, 0xFE19}, { 0xFE30, 0xFE6F}, { 0xFF00, 0xFF60},
        { 0xFFE0, 0xFFE6}, { 0x20000, 0x2FFFD}, { 0x30000, 0x3FFFD}
    };
    return bisearch(ucs, double_width, sizeof(double_width) / sizeof(struct interval) - 1);
}

static unsigned int text_width(char* str)
{
    if (iconvW == NULL)
        iconvW = iconv_open("ucs-4be", "utf-8");
    if (iconvW == (iconv_t) -1)
    {
        return utf8_strlen(str);
    }
    else
    {
        size_t len = strlen(str);
        size_t charlen = utf8_strlen(str);
        unsigned *wmessage;
        size_t wlen = (len + 1) * sizeof (unsigned);
        wmessage = (unsigned *) fcitx_malloc0 ((len + 1) * sizeof (unsigned));

        char *inp = str;
        char *outp = (char*) wmessage;

        iconv(iconvW, &inp, &len, &outp, &wlen);

        int i = 0;
        int width = 0;
        for (i = 0; i < charlen; i ++)
        {
            if (is_double_width(wmessage[i]))
                width += 2;
            else
                width += 1;
        }


        free(wmessage);
        return width;
    }
}

static void im_show(unsigned winid)
{
    int sizeup = text_width(textup);
    int sizedown = text_width(textdown);
    int width = (sizeup > sizedown ? sizeup : sizedown) * currentInfo.fontWidth + MARGIN * 2;
    int totalheight = 3 * (MARGIN * 2 + currentInfo.fontHeight);

    Rectangle rect;
    int x, y;
    x = cursor_x + currentInfo.fontWidth;
    y = cursor_y + currentInfo.fontHeight;
    if (y + totalheight > currentInfo.screenHeight)
        y = cursor_y - totalheight - currentInfo.fontHeight * 2;
    if (y < 0)
        y = 0;
    if (x + width > currentInfo.screenWidth)
        x = currentInfo.screenWidth - width;
    if (x < 0)
        x = 0;
    rect.x = x; rect.y = y;

    DRAW_NEXT_STRING(textup, 0, textup[0]);
    DRAW_NEXT_STRING(textdown, 1, textdown[0]);
    DRAW_NEXT_STRING(imname, 2, textup[0] || textdown[0]);
}

static void im_hide()
{
}

static void process_raw_key(char *buf, unsigned int len)
{
    if ( len <= 0 ) {
        return;
    }
    else
    {
        unsigned int i;
        for (i = 0; i < len; i++) {
            char down = !(buf[i] & 0x80);
            short code = buf[i] & 0x7f;
            if (!code) {
                if (i + 2 >= len) break;

                code = (buf[++i] & 0x7f) << 7;
                code |= buf[++i] & 0x7f;
                if (!(buf[i] & 0x80) || !(buf[i - 1] & 0x80)) continue;
            }

            unsigned short linux_keysym = keycode_to_keysym(code, down);
            FcitxKeySym keysym = linux_keysym_to_fcitx_keysym(linux_keysym, code);

            if (keysym == Key_None)
                continue;

            FcitxIMClientFocusIn(client);

            if (FcitxIMClientProcessKey(client, keysym, code, state, (down ? FCITX_PRESS_KEY : FCITX_RELEASE_KEY), 0) <= 0) {
                char *str = keysym_to_term_string(linux_keysym, down);
                if (str)
                    put_im_text(str, strlen(str));
            }

            state = calculate_modifiers(state, keysym, down);
        }
    }

}

static void cursor_pos_changed(unsigned x, unsigned y)
{
    cursor_x = x;
    cursor_y = y;
    im_show(-1);
}

static void update_fbterm_info(Info *info)
{
    currentInfo = *info;
}

static gboolean iochannel_fbterm_callback(GIOChannel *source, GIOCondition condition, gpointer data)
{
    if (!check_im_message()) {
        g_main_loop_quit(main_loop);
        return FALSE;
    }

    return TRUE;
}

void _fcitx_fbterm_connect_cb(FcitxIMClient* client, void* user_data)
{
    if (IsFcitxIMClientValid(client))
    {
        FcitxIMClientConnectSignal(client,
                                   G_CALLBACK(_fcitx_fbterm_enable_im_cb),
                                   G_CALLBACK(_fcitx_fbterm_close_im_cb),
                                   G_CALLBACK(_fcitx_fbterm_commit_string_cb),
                                   G_CALLBACK(_fcitx_fbterm_forward_key_cb),
                                   G_CALLBACK(_fcitx_fbterm_update_client_side_ui_cb),
                                   user_data,
                                   NULL);

        CapacityFlags flags = CAPACITY_CLIENT_SIDE_UI;
        FcitxIMClientSetCapacity(client, flags);

        if (active)
        {
            FcitxIMClientFocusIn(client);
            FcitxIMClientEnableIC(client);
        }
    }
}

void _fcitx_fbterm_destroy_cb(FcitxIMClient* client, void* user_data)
{
    state = 0;
}

void _fcitx_fbterm_close_im_cb(DBusGProxy* proxy, void* user_data)
{
    state = 0;
}

void _fcitx_fbterm_commit_string_cb(DBusGProxy* proxy, char* str, void* user_data)
{
    unsigned short result_len = strlen(str);
    put_im_text( str, result_len );
}

void _fcitx_fbterm_enable_im_cb(DBusGProxy* proxy, void* user_data)
{
    state = 0;
}

void _fcitx_fbterm_forward_key_cb(DBusGProxy* proxy, guint keyval, guint state, gint type, void* user_data)
{

}

void _fcitx_fbterm_update_client_side_ui_cb(DBusGProxy* proxy, char* auxup, char* auxdown, char* preedit, char* candidateword, char* _imname, int cursor_pos, void* user_data)
{
    snprintf(textup, BUFSIZE, "%s%s", auxup, preedit);
    snprintf(textdown, BUFSIZE, "%s%s", auxdown, candidateword);
    snprintf(imname, BUFSIZE, "%s", _imname);
    textup[BUFSIZE - 1] = textdown[BUFSIZE - 1] = '\0';
    im_show(-1);
}

int main()
{
    if (get_im_socket() == -1)
        return 1;

    GIOChannel *iochannel_fbterm = g_io_channel_unix_new(get_im_socket());
    g_io_add_watch(iochannel_fbterm, (GIOCondition)(G_IO_IN | G_IO_HUP | G_IO_ERR), (GIOFunc)iochannel_fbterm_callback, NULL);

    g_type_init();

    client = FcitxIMClientOpen(_fcitx_fbterm_connect_cb, _fcitx_fbterm_destroy_cb, NULL);

    register_im_callbacks(cbs);
    connect_fbterm(1);

    main_loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(main_loop);

    FcitxIMClientClose(client);
    g_io_channel_unref(iochannel_fbterm);
    g_main_loop_unref(main_loop);

    return 0;
}

// kate: indent-mode cstyle; space-indent on; indent-width 0;

