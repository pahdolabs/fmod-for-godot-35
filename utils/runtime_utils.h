#ifndef RUNTIME_UTILS_H
#define RUNTIME_UTILS_H

#include "fmod_studio_module.h"
#include "fmod_types.h"
#include "scene/2d/physics_body_2d.h"
#include "scene/3d/physics_body.h"
#include "scene/2d/physics_body_2d.h"
#include "scene/3d/physics_body.h"

#include "core/class_db.h"


class RuntimeUtils : public Reference
{
	GDCLASS(RuntimeUtils, Reference);

protected:
	static void _bind_methods();

public:
	enum GameEvent
	{
		GAMEEVENT_NONE,
		GAMEEVENT_ENTER_TREE,
		GAMEEVENT_READY,
		GAMEEVENT_EXIT_TREE
	};

	static void to_3d_attributes_node(Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes, Object* node, Object* physicsbody);
	static void to_3d_attributes_transform_physicsbody(Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes,
			const Variant& transform, Object* physicsbody);
	static void to_3d_attributes(Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes, const Variant& position);
};

VARIANT_ENUM_CAST(RuntimeUtils::GameEvent);

#endif // RUNTIME_UTILS_H