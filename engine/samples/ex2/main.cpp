#include <cubos/engine/cubos.hpp>
#include <cubos/core/ecs/accessors.hpp>
#include <cubos/engine/transform/local_to_world.hpp>
#include <cubos/engine/transform/position.hpp>
#include <cubos/engine/transform/rotation.hpp>
#include <cubos/engine/renderer/plugin.hpp>
#include <cubos/engine/voxels/palette.hpp>
#include <cubos/engine/voxels/grid.hpp>
#include <cubos/engine/renderer/point_light.hpp>
#include <cubos/engine/renderer/environment.hpp>
#include <cubos/engine/settings/settings.hpp>
#include <cubos/engine/assets/bridges/file.hpp>
#include <cubos/engine/assets/assets.hpp>
#include <iostream>
#include <cubos/core/memory/stream.hpp>
#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>

using namespace cubos::engine;
using cubos::core::ecs::Commands;
using cubos::core::ecs::Read;
using cubos::core::ecs::Write;
using cubos::core::memory::Stream;




static const Asset<VoxelGrid> castleAsset = AnyAsset("41973663-7ca6-425e-97c6-458b9b8e89ea");
static const Asset<VoxelPalette> paletteAsset = AnyAsset("6f42ae5a-59d1-5df3-8720-83b8df6dd536");

static void configSystem(Write<Settings> settings){
    settings->setString("assets.io.path", SAMPLE_ASSETS_FOLDER);
}

static void setPaletteSystem(Read<Assets> assets, Write<Renderer> renderer){
    auto palette = assets->read(paletteAsset);
    (*renderer)->setPalette(*palette);
}

static void spawnLightSystem(Commands commands){
    commands.create()
        .add(PointLight{.color={1.0f, 1.0f, 1.0f}, .intensity=1.0f, .range=10.0f})
        .add(Position{{1.0f, 3.0f, -2.0f}});
}

static void setEnvironmentSystem(Write<RendererEnvironment> env){
    env-> ambient = {0.2f, 0.2f, 0.2f};
    env->skyGradient[0] = {0.1f, 0.2f, 0.4f};
    env->skyGradient[1] = {0.6f, 0.6f, 0.8f};
}

/*
static void spawnCamerasSystem(Commands commands, Write<ActiveCameras> camera){
    camera->entities[0] =
        commands.create()
            .add(Camera{.fovY = 60.0f, .zNear = 0.1f, .zFar = 100.0f})
            .add(Position{{-3.0f, 1.0f, -3.0f}})
            .add(Rotation{glm::quatLookAt(glm::normalize(glm::vec3{1.0f, 0.0f, 1.0f}), glm::vec3{0.0f, 1.0f, 0.0f})})
            .entity();
}
*/
static void spawnCamerasSystem(Commands cmds, Write<ActiveCameras> activeCameras)
{
    // Spawn the camera entity.
    activeCameras->entities[0] =
        cmds.create()
            .add(Camera{.fovY = 60.0F, .zNear = 0.1F, .zFar = 1000.0F})
            .add(Position{{50.0F, 50.0F, 50.0F}})
            .add(Rotation{glm::quatLookAt(glm::normalize(glm::vec3{-1.0F, -1.0F, -1.0F}), glm::vec3{0.0F, 1.0F, 0.0F})})
            .entity();
}

static void spawnCastleSystem(Commands cmds, Read<Assets> assets){
    auto castle = assets->read(castleAsset);
    glm::vec3 offset = glm::vec3(castle->size().x, 0.0f, castle->size().z) / 2.0f;

    cmds.create().add(RenderableGrid{castleAsset, offset}). add(LocalToWorld{});
}

int main(){
    Cubos cubos{};
    cubos.addPlugin(rendererPlugin);
    cubos.addPlugin(assetsPlugin);
    cubos.addPlugin(voxelsPlugin);

    cubos.startupSystem(configSystem).tagged("cubos.settings");
    cubos.startupSystem(spawnLightSystem);
    cubos.startupSystem(setEnvironmentSystem);
    cubos.startupSystem(spawnCamerasSystem);
    cubos.startupSystem(setPaletteSystem).after("cubos.renderer.init");
    cubos.system(spawnCastleSystem);

    cubos.run();
}