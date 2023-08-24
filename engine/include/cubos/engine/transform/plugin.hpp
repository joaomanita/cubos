/// @dir
/// @brief Transform plugin directory.

/// @file
/// @brief Plugin entry point.

#pragma once

#include <cubos/engine/cubos.hpp>
#include <cubos/engine/transform/local_to_world.hpp>
#include <cubos/engine/transform/position.hpp>
#include <cubos/engine/transform/rotation.hpp>
#include <cubos/engine/transform/scale.hpp>

namespace cubos::engine
{
    /// @defgroup transform-plugin Transform
    /// @ingroup plugins
    /// @brief Adds transform components which assign positions, rotations and scaling to entities.
    ///
    /// This plugin operates on entities with a @ref LocalToWorld component and any combination of
    /// the @ref Position, @ref Rotation and @ref Scale components. For example, if you have an
    /// entity which doesn't need rotation, but has a position and a scale, you do not need to add
    /// the @ref Rotation component, and its transform will still be updated.
    ///
    /// ## Components
    /// - @ref LocalToWorld - holds the local to world transform matrix.
    /// - @ref Position - holds the position of an entity.
    /// - @ref Rotation - holds the rotation of an entity.
    /// - @ref Scale - holds the scaling of an entity.
    ///
    /// ## Tags
    /// - `cubos.transform.update` - the @ref LocalToWorld components are updated with the
    ///    information from the @ref Position, @ref Rotation and @ref Scale components.

    /// @brief Plugin entry function.
    /// @param cubos CUBOS. main class
    /// @ingroup transform-plugin
    void transformPlugin(Cubos& cubos);
} // namespace cubos::engine
