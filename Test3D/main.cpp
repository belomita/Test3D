#include "app.h"

int main(int, char**)
{
	if(App_Init()) {
		App_Run();
	}
	App_UnInit();
	return 0;
}
