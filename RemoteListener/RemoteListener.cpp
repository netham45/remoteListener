
#include "RemoteListener.h"
#include "Network.h"
#include "Configs.h"
#include "Actions.h"

int main()
{
#ifdef __linux__
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
#endif
	printf("RemoteListener running on %s\n", HOSTOS);
	registerActionCallbacks();
	printf("Connecting To %s:%s\n", getServer(), getPort());
	start(getServer(), getPort());
	return 0;
}

