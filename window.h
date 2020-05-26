#ifdef __cplusplus
extern "C" {
#endif

#define WINDOW_REGULAR    0
#define WINDOW_FLOATING   1
#define WINDOW_FULLSCREEN 2

#define SHIFT 65505

int window_init();

int window_create(int width, int height, int mode);

void window_set_cursor(int val);
void window_set_warp(int val);

void window_poll_events();

void window_swap_buffers();

void window_get_cursor_delta(int *x, int *y);
void window_get_cursor_pos(int *x, int *y);

void window_register_key_cb(void (*cb)(int, int));

int window_alive();

#ifdef __cplusplus
}
#endif
