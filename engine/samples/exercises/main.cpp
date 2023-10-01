#include <cubos/engine/cubos.hpp>
#include "/home/manita_vm/cubos/cubos/core/include/cubos/core/ecs/accessors.hpp"
#include "/home/manita_vm/cubos/cubos/engine/include/cubos/engine/transform/local_to_world.hpp"
#include "/home/manita_vm/cubos/cubos/engine/include/cubos/engine/transform/position.hpp"
#include "/home/manita_vm/cubos/cubos/engine/include/cubos/engine/transform/rotation.hpp"
#include <cubos/engine/renderer/plugin.hpp>
#include <cubos/engine/voxels/palette.hpp>
#include <cubos/engine/voxels/grid.hpp>
#include <cubos/engine/renderer/point_light.hpp>
#include <cubos/engine/renderer/environment.hpp>


using namespace cubos::engine;
using namespace cubos::core::ecs;


static void setPaletteSystem(Write<Renderer> renderer){
    (*renderer)->setPalette(VoxelPalette{{
        {{1, 0, 0, 1}},
        {{0, 1, 0, 1}},
        {{0, 0, 1, 1}},
    }});
}

static void spawnVoxelGridSystem(Commands commands, Write<Assets> assets){
    auto gridAsset = assets->create(VoxelGrid{{2,2,2}, {1,2,3,1,2,3,1,2}});

    commands.create(RenderableGrid{gridAsset, {-1.0f, 0.0f, -1.0f}}, LocalToWorld{});
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

static void spawnCamerasSystem(Commands commands, Write<ActiveCameras> camera){
    camera->entities[0] =
        commands.create()
            .add(Camera{.fovY = 60.0f, .zNear = 0.1f, .zFar = 100.0f})
            .add(Position{{-3.0f, 1.0f, -3.0f}})
            .add(Rotation{glm::quatLookAt(glm::normalize(glm::vec3{1.0f, 0.0f, 1.0f}), glm::vec3{0.0f, 1.0f, 0.0f})})
            .entity();
    
    camera->entities[1] = camera->entities[0];
    camera->entities[2] = camera->entities[0];
}

int main(){
    Cubos cubos{};
    cubos.addPlugin(rendererPlugin);
    cubos.startupSystem(setPaletteSystem).after("cubos.renderer.init");
    cubos.startupSystem(spawnVoxelGridSystem);
    cubos.startupSystem(spawnLightSystem);
    cubos.startupSystem(setEnvironmentSystem);
    cubos.startupSystem(spawnCamerasSystem);

    cubos.run();
}