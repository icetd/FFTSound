#include <iostream>
#include "Application.h"
#include "log.h"
#include "MainLayer.h"
#include "ControlLayer.h"


int main(int argc, char **argv)
{
    initLogger(INFO);
    Application *app = new Application("FFTSound", 1220, 660);
    app->PushLayer<MainLayer>();
    app->PushLayer<ControlLayer>();
    app->Run();
    delete app;
    return 0;
}