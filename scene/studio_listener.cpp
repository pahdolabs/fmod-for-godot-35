#include "studio_listener.h"

static Vector<ListenerImpl*> listeners;
static const int MAX_LISTENERS = 8;

int ListenerImpl::get_num_listener()
{
	return listeners.find(this);
}

void ListenerImpl::add_listener()
{
	for (size_t i = 0; i < listeners.size(); i++)
	{
		if (listeners[i] == this)
		{
			WARN_PRINT("[FMOD] Listener has already been added.");
			return;
		}
	}

	if (listeners.size() >= MAX_LISTENERS)
	{
		WARN_PRINT("[FMOD] Max number of listeners reached.");
	}

	listeners.push_back(this);

	FMODStudioModule* fmod_module = FMODStudioModule::get_singleton();

	if (fmod_module)
	{
		fmod_module->get_studio_system_ref()
				->set_num_listeners(
						CLAMP(static_cast<int>(listeners.size()), 1, MAX_LISTENERS));
	}
}

void ListenerImpl::remove_listener()
{
	for (size_t i = 0; i < listeners.size(); i++)
	{
		if (listeners[i] == this)
		{
			listeners.remove(i);
			break;
		}
	}

	FMODStudioModule* fmod_module = FMODStudioModule::get_singleton();

	if (fmod_module)
	{
		fmod_module->get_studio_system_ref()
				->set_num_listeners(
						CLAMP(static_cast<int>(listeners.size()), 1, MAX_LISTENERS));
	}
}

int ListenerImpl::get_listener_count()
{
	return listeners.size();
}

void ListenerImpl::set_listener_location()
{
	int listener_index = get_num_listener();

	if (node_2d)
	{
		Node2D* attenuation_node = Object::cast_to<Node2D>(attenuation_object);
		Variant transform = node_2d->get_global_transform();

		if (attenuation_node)
		{
			set_listener_attributes(attenuation_node->get_global_transform().get_origin(), listener_index, transform, rigidbody);
		}
		else
		{
			set_listener_attributes(Variant(), listener_index, transform, rigidbody);
		}
	}
	else if (node_3d)
	{
		Spatial* attenuation_node = Object::cast_to<Spatial>(attenuation_object);
		Variant transform = node_3d->get_global_transform();
		if (attenuation_node)
		{
			set_listener_attributes(attenuation_node->get_global_transform().get_origin(), listener_index, transform, rigidbody);
		}
		else
		{
			set_listener_attributes(Variant(), listener_index, transform, rigidbody);
		}
	}
}

void ListenerImpl::set_listener_attributes(const Variant& attenuation, int num_listener, const Variant& transform, Object* rigidbody)
{
	RuntimeUtils::to_3d_attributes_transform_physicsbody(attributes, transform, rigidbody);

	FMODStudioModule* fmod_module = FMODStudioModule::get_singleton();
	if (fmod_module)
	{
		fmod_module->get_studio_system_ref()->set_listener_attributes(num_listener, attributes, attenuation);
	}
}

float ListenerImpl::distance_to_nearest_listener(const Variant& position)
{
	float result = FLT_MAX;

	for (size_t i = 0; i < listeners.size(); i++)
	{
		ListenerImpl* listener_impl = listeners[i];

		if (listener_impl)
		{
			if (listener_impl->node_2d)
			{
				Transform2D t2d = listener_impl->node_2d->get_global_transform();
				Vector2 origin = t2d.get_origin() / FMODStudioModule::get_singleton()->distance_scale_2d;
				result = MIN(result, origin.distance_to(position));
			}
			else if (listener_impl->node_3d)
			{
				Transform t3d = listener_impl->node_3d->get_global_transform();
				result = MIN(result, t3d.get_origin().distance_to(position));
			}
		}
	}

	return result;
}

void ListenerImpl::_enter_tree()
{
	add_listener();
}

void ListenerImpl::_exit_tree()
{
	remove_listener();
}

void ListenerImpl::_process(double p_delta)
{
	if (get_num_listener() >= 0 && get_num_listener() < MAX_LISTENERS)
	{
		set_listener_location();
	}
}

void StudioListener2D::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_attenuation_object_path", "path"), &StudioListener2D::set_attenuation_object_path);
	ClassDB::bind_method(D_METHOD("get_attenuation_object_path"), &StudioListener2D::get_attenuation_object_path);
	ClassDB::bind_method(D_METHOD("get_attenuation_object"), &StudioListener2D::get_attenuation_object);
	ClassDB::bind_method(D_METHOD("set_rigidbody_path", "path"), &StudioListener2D::set_rigidbody_path);
	ClassDB::bind_method(D_METHOD("get_rigidbody"), &StudioListener2D::get_rigidbody);
	ClassDB::bind_method(D_METHOD("get_rigidbody_path"), &StudioListener2D::get_rigidbody_path);
	ClassDB::bind_method(D_METHOD("set_num_listener", "listener"), &StudioListener2D::set_num_listener);
	ClassDB::bind_method(D_METHOD("get_num_listener"), &StudioListener2D::get_num_listener);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "attenuation_object_path"),
			"set_attenuation_object_path", "get_attenuation_object_path");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "rigidbody"), "set_rigidbody_path",
			"get_rigidbody_path");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_listener", PROPERTY_HINT_NONE, "",
						 PROPERTY_USAGE_EDITOR),
			"set_num_listener", "get_num_listener");
}

void StudioListener2D::_notification(int p_what)
{
	if (p_what == NOTIFICATION_ENTER_TREE)
	{
		if (Engine::get_singleton()->is_editor_hint())
		{
			return;
		}

		implementation.node_2d = this;
		implementation._enter_tree();
	}
	if (p_what == NOTIFICATION_EXIT_TREE)
	{
		if (Engine::get_singleton()->is_editor_hint())
		{
			return;
		}

		implementation._exit_tree();
	}
	if (p_what == NOTIFICATION_INTERNAL_PROCESS)
	{
		if (Engine::get_singleton()->is_editor_hint())
		{
			return;
		}

		implementation._process(get_process_delta_time());
	}
}

float StudioListener2D::distance_to_nearest_listener(const Vector2& position)
{
	return ListenerImpl::distance_to_nearest_listener(position);
}

void StudioListener2D::set_attenuation_object(Object* object)
{
	implementation.attenuation_object = object;
}

void StudioListener2D::set_attenuation_object_path(NodePath path)
{
	implementation.attenuation_object_path = path;
	set_attenuation_object(get_node(path));
}

Object* StudioListener2D::get_attenuation_object() const
{
	return implementation.attenuation_object;
}

NodePath StudioListener2D::get_attenuation_object_path() const
{
	return implementation.attenuation_object_path;
}

void StudioListener2D::set_rigidbody(Object* object)
{
	implementation.rigidbody = object;
}

void StudioListener2D::set_rigidbody_path(NodePath path)
{
	implementation.rigidbody_path = path;
	set_rigidbody(get_node(path));
}

Object* StudioListener2D::get_rigidbody() const
{
	return implementation.rigidbody;
}

NodePath StudioListener2D::get_rigidbody_path() const
{
	return implementation.rigidbody_path;
}

void StudioListener2D::set_num_listener(int num)
{
	_change_notify();
}

int StudioListener2D::get_num_listener()
{
	return implementation.get_num_listener();
}

StudioListener2D::StudioListener2D()
{
	set_process_internal(true);
}

void StudioListener3D::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_listener_count"), &StudioListener3D::local_get_listener_count);
	ClassDB::bind_method(D_METHOD("set_attenuation_object_path", "path"), &StudioListener3D::set_attenuation_object_path);
	ClassDB::bind_method(D_METHOD("get_attenuation_object_path"), &StudioListener3D::get_attenuation_object_path);
	ClassDB::bind_method(D_METHOD("set_rigidbody_path", "path"), &StudioListener3D::set_rigidbody_path);
	ClassDB::bind_method(D_METHOD("get_rigidbody_path"), &StudioListener3D::get_rigidbody_path);
	ClassDB::bind_method(D_METHOD("get_rigidbody"), &StudioListener3D::get_rigidbody);
	ClassDB::bind_method(D_METHOD("set_num_listener", "listener"), &StudioListener3D::set_num_listener);
	ClassDB::bind_method(D_METHOD("get_num_listener"), &StudioListener3D::get_num_listener);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "attenuation_object_path"),
			"set_attenuation_object_path", "get_attenuation_object_path");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "rigidbody_path"), "set_rigidbody_path",
			"get_rigidbody_path");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_listener", PROPERTY_HINT_NONE, "",
						 PROPERTY_USAGE_EDITOR),
			"set_num_listener", "get_num_listener");
}

void StudioListener3D::_notification(int p_what)
{
	if (p_what == NOTIFICATION_ENTER_TREE)
	{
		if (Engine::get_singleton()->is_editor_hint())
		{
			return;
		}

		implementation.node_3d = this;
		implementation._enter_tree();
	}
	if (p_what == NOTIFICATION_EXIT_TREE)
	{
		if (Engine::get_singleton()->is_editor_hint())
		{
			return;
		}

		implementation._exit_tree();
	}
	if (p_what == NOTIFICATION_INTERNAL_PROCESS)
	{
		if (Engine::get_singleton()->is_editor_hint())
		{
			return;
		}

		implementation._process(get_process_delta_time());
	}
}

float StudioListener3D::distance_to_nearest_listener(const Vector3& position)
{
	return ListenerImpl::distance_to_nearest_listener(position);
}

int StudioListener3D::local_get_listener_count()
{
	return get_listener_count();
}

int StudioListener3D::get_listener_count()
{
	return ListenerImpl::get_listener_count();
}

void StudioListener3D::set_attenuation_object_path(NodePath path)
{
	implementation.attenuation_object_path = path;
	set_attenuation_object(get_node(path));
}

void StudioListener3D::set_attenuation_object(Object* object)
{
	implementation.attenuation_object = object;
}

NodePath StudioListener3D::get_attenuation_object_path() const
{
	return implementation.attenuation_object_path;
}

Object* StudioListener3D::get_attenuation_object() const
{
	return implementation.attenuation_object;
}

void StudioListener3D::set_rigidbody_path(NodePath p_path)
{
	implementation.rigidbody_path = p_path;
	set_rigidbody(get_node(p_path));
}

void StudioListener3D::set_rigidbody(Object* object)
{
	implementation.rigidbody = object;
}

Object* StudioListener3D::get_rigidbody() const
{
	return implementation.rigidbody;
}

NodePath StudioListener3D::get_rigidbody_path() const
{
	return implementation.rigidbody_path;
}

void StudioListener3D::set_num_listener(int num)
{
	_change_notify();
}

int StudioListener3D::get_num_listener()
{
	return implementation.get_num_listener();
}

StudioListener3D::StudioListener3D()
{
	set_process_internal(true);
}