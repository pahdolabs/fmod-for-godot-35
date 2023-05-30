// This is slighty modified version of Godot's AudioStreamPlayer3DGizmoPlugin
// in editor/plugins/node_3d_editor_gizmos.cpp and adapted to also display
// the minimum distance of an FMOD Event.
// License: godotengine.org/license

#include "core/vector.h"
#include "editor/plugins/spatial_editor_plugin.h"
#include "scene/resources/material.h"
#include "scene/studio_event_emitter.h"

class StudioEventEmitter3DGizmoPlugin : public EditorSpatialGizmoPlugin
{
	GDCLASS(StudioEventEmitter3DGizmoPlugin, EditorSpatialGizmoPlugin);

protected:
	static void _bind_methods();

private:
	const String emitter_icon_path = "res://addons/FMOD/editor/images/fmod_emitter.png";

public:
	StudioEventEmitter3DGizmoPlugin()
	{
		call_deferred("setup");
	}

	bool has_gizmo(Spatial* for_node_3d) const;
	virtual String get_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo* p_gizmo);
	void setup();
};
