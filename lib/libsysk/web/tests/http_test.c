#include <stdio.h>
#include <sysk/web/http.h>

int main() {
	if (!get(80, "kalekale.ga")) {
		return 1;
	}
	else {
		return 0;
	}
}
