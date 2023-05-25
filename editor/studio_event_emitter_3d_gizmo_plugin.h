// This is slighty modified version of Godot's AudioStreamPlayer3DGizmoPlugin
// in editor/plugins/node_3d_editor_gizmos.cpp and adapted to also display
// the minimum distance of an FMOD Event.
// License: godotengine.org/license

#include "scene/studio_event_emitter.h"
#include "editor/plugins/spatial_editor_plugin.h"
#include "scene/resources/material.h"
#include "core/vector.h"

class StudioEventEmitter3DGizmoPlugin : public EditorNode3DGizmoPlugin
{
	GDCLASS(StudioEventEmitter3DGizmoPlugin, EditorNode3DGizmoPlugin);

protected:
	static void _bind_methods();

private:
	const String emitter_icon_path = "res://addons/FMOD/editor/images/fmod_emitter.png";

public:
	StudioEventEmitter3DGizmoPlugin()
	{
		Ref<Texture2D> icon = ResourceLoader::get_singleton()->load(emitter_icon_path);
		create_icon_material("studio_event_emitter_3d_icon", icon);
		create_material("studio_event_emitter_3d_material_billboard", Color(0.4, 0.8, 1), true, false, true);
	}

	virtual bool _has_gizmo(Node3D* for_node_3d) const override;
	virtual String _get_gizmo_name() const override;
	virtual int32_t _get_priority() const override;
	virtual void _redraw(const Ref<EditorNode3DGizmo>& gizmo) override;
};
