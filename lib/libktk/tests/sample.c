#include "ktk.h"
#include <stdlib.h>
#include <unistd.h>

int main() {
	ktk_init(400, 400, "TEST TEST");
	sleep(1);
	ktk_close();
}
