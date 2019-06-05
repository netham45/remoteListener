
#include "RemoteListener.h"
#include "Network.h"
#include "Configs.h"
#include "Actions.h"

using namespace std;

int main()
{
	registerActionCallbacks();
	cout << "Connecting To " << getServer() << ":" << getPort() << "\n";
	start(getServer(), getPort());
}

