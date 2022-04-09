#include <poll.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <libinput.h>
#include <xkbcommon/xkbcommon.h>

#define EXIT(msg) { fputs(msg, stderr); exit(EXIT_FAILURE); }

static struct xkb_state  *xkb_state;
static struct xkb_keymap *xkb_keymap;
static struct xkb_context *xkb_context;

static void
process_event(struct libinput_event *event)
{
    int type = libinput_event_get_type(event);

    if (type == LIBINPUT_EVENT_KEYBOARD_KEY)
    {
        struct libinput_event_keyboard *keyboard_event = libinput_event_get_keyboard_event(event);

        uint8_t key = libinput_event_keyboard_get_key(keyboard_event);
        int state = libinput_event_keyboard_get_key_state(keyboard_event);
        xkb_state_update_key(xkb_state, key, state);

        if (state == LIBINPUT_KEY_STATE_PRESSED)
        {
            uint8_t utf8 = xkb_state_key_get_utf32(xkb_state, key);

            if (utf8)
            {
                if (utf8 >= 0x21 && utf8 <= 0x7E)
                {
                    printf("Key %c was pressed\n", (char)utf8);
                }
                else {
                    printf("Key U+%04X was pressed\n", utf8);
                }
            }
        }
    }
    libinput_event_destroy(event);
}

static inline int
open_restricted(const char *path, int flags, void *user_data)
{
    return open(path, flags);
}

static inline void
close_restricted(int fd, void *user_data)
{
    close(fd);
}

static struct
libinput_interface iface = {
    &open_restricted,
    &close_restricted
};

int main()
{
    struct udev *udev = udev_new();
    struct libinput *libinput = libinput_udev_create_context(&iface, NULL, udev);

    if (!udev) EXIT("Udev error\n");
    if (!libinput) EXIT("Libinput error\n");

    void* libinput_udev_assign(void*, const char*);
    if (libinput_udev_assign_seat(libinput, "seat0") == -1)
    {
        EXIT("Cannot assign seat\n");
    }

    xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

    xkb_keymap = xkb_keymap_new_from_names(xkb_context, NULL, XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (!xkb_keymap) EXIT("Keymap error\n");

    xkb_state = xkb_state_new(xkb_keymap);

    while (1) {
        struct pollfd fd = {
            libinput_get_fd(libinput),
            POLLIN,	0
        };

        poll(&fd, 1, -1);
        libinput_dispatch(libinput);

        struct libinput_event *event;
        while ((event = libinput_get_event(libinput)))
            process_event(event);
    }
}
