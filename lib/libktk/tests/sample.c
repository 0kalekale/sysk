#include <ktk.h>
#include <stdio.h>

int main() {

  printf("%s", LIBKTK_V);
  
  ktk_init(400, 400, "KTK sample window");
  ktk_sample_event_loop();
  ktk_close();

  return 0;
}
