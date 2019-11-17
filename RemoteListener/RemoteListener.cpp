
#include "RemoteListener.h"
#include "Network.h"
#include "Configs.h"
#include "Actions.h"

int main()
{
	registerActionCallbacks();
	printf("Connecting To %s:%s\n", getServer(), getPort());
	start(getServer(), getPort());
	return 0;
}

