#include <cubos/engine/cubos.hpp>
#include <cubos/engine/settings/plugin.hpp>

using cubos::core::ecs::Write;

using namespace cubos::engine;

/// [System]
static void checkSettings(Write<Settings> settings)
{
    CUBOS_INFO("{}", settings->getString("greeting", "Hello!"));
}
/// [System]

/// [Run]
int main(int argc, char** argv)
{
    Cubos cubos{argc, argv};
    cubos.addPlugin(settingsPlugin);
    cubos.startupSystem(checkSettings).after("cubos.settings");
    cubos.run();
}
/// [Run]
