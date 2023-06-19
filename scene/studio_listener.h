#ifndef STUDIO_LISTENER_H
#define STUDIO_LISTENER_H

#include "fmod_studio_module.h"
#include "utils/runtime_utils.h"
#include <float.h>
#include <algorithm>
#include "scene/2d/node_2d.h"
#include "scene/3d/spatial.h"

struct ListenerImpl
{
	Node2D* node_2d = nullptr;
	Spatial* node_3d = nullptr;
	Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes = create_ref<FmodTypes::FMOD_3D_ATTRIBUTES>();
	Object* attenuation_object = nullptr;
	NodePath attenuation_object_path = NodePath();
	Object* rigidbody = nullptr;
	NodePath rigidbody_path = NodePath();

	int get_num_listener();

	void add_listener();
	void remove_listener();
	static int get_listener_count();
	void set_listener_location();
	void set_listener_attributes(const Variant& attenuation, int num_listener, const Variant& transform, Object* rigidbody);
	static float distance_to_nearest_listener(const Variant& position);

	void _enter_tree();
	void _exit_tree();
	void _process(double p_delta);
};

class StudioListener2D : public Node2D
{
	GDCLASS(StudioListener2D, Node2D);

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	ListenerImpl implementation;
	void set_attenuation_object(Object* object);
	void set_rigidbody(Object* object);

public:
	static float distance_to_nearest_listener(const Vector2& position);


	NodePath get_attenuation_object_path() const;
	Object* get_attenuation_object() const;

	void set_attenuation_object_path(NodePath p_path);
	void set_rigidbody_path(NodePath p_path);
	Object* get_rigidbody() const;
	NodePath get_rigidbody_path() const;

	void set_num_listener(int num);
	int get_num_listener();

	StudioListener2D();
};

class StudioListener3D : public Spatial
{
	GDCLASS(StudioListener3D, Spatial);

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	ListenerImpl implementation;
	void set_attenuation_object(Object* object);
	void set_rigidbody(Object* object);

public:
	static float distance_to_nearest_listener(const Vector3& position);
	static int get_listener_count();
	int local_get_listener_count();


	void set_attenuation_object_path(NodePath p_path);
	Object* get_attenuation_object() const;
	NodePath get_attenuation_object_path() const;

	void set_rigidbody_path(NodePath p_path);
	Object* get_rigidbody() const;
	NodePath get_rigidbody_path() const;

	void set_num_listener(int num);
	int get_num_listener();

	StudioListener3D();
};

#endif // STUDIO_LISTENER_H