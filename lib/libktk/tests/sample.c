#include <ktk.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	ktk_init();
	ktk_create_window(400, 400);
	ktk_set_window_name("TEST TEST");
	sleep(1);
	ktk_close();
}
