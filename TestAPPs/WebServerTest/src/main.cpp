
#include <memory>
#include <thread>

#include "ccWebServerAPI/ccWebServerManager.h"
#include "ccMongooseServerAPI/ccMongooseWebServerObjectFactory.h"

int main(int argc, char* argv[])
{
    ccWebServerManager  oManager;

    oManager.AttachFactory(std::make_shared<ccMongooseWebServerObjectFactory>());

    oManager.CreateWebServer("WebServer #1", "8000");
    oManager.CreateWebServer("WebServer #2", "8020");

    oManager.Start();

    while (1)
        std::this_thread::sleep_for(std::chrono::microseconds{ 100 });
}
