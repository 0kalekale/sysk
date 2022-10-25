#include "tkt.h"
#include <stdlib.h>
#include <unistd.h>

int main() {
	tkt_init(400, 400, "TEST TEST");
	sleep(1);
	tkt_close();
}
