#include <GL/glx.h>

extern void loadgl_loader(void *load);

void loadgl(void) {
	loadgl_loader(glXGetProcAddress);
}
