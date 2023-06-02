#ifndef FMOD_RESOURCES_H
#define FMOD_RESOURCES_H

#include "api/studio_api.h"
#include "fmod_types.h"
#include "misc/fmod_io.h"
#include "misc/fmod_memory.h"
#include "core/engine.h"
#include "scene/main/node.h"
#include "core/os/os.h"

#include "core/class_db.h"

#include <string>
#include <type_traits>
#include <unordered_map>
#ifdef FMOD_OSX
#include <AudioToolbox/AudioToolbox.h>
#endif

class FMODAsset : public Resource
{
	GDCLASS(FMODAsset, Resource);

protected:
	static void _bind_methods();

public:
	void set_guid(const String& guid);
	String get_guid() const;
	void set_fm_path(const String &path);
	String get_fm_path() const;
	void set_fm_name(const String& name);
	String get_fm_name() const;

private:
	String guid{};
	String path{};
	String name{};
};

class BankAsset : public FMODAsset
{
	GDCLASS(BankAsset, FMODAsset);

protected:
	static void _bind_methods();

public:
	void set_bank_file_info(const Dictionary& file_bank_info);
	void set_bank_ref(const FMOD_STUDIO_BANK* bank);
	void set_file_path(const String& file_path);
	String get_file_path() const;
	void set_modified_time(int modified_time);
	int get_modified_time() const;

private:
	String file_path{};
	int modified_time{};
};

class EventAsset : public FMODAsset
{
	GDCLASS(EventAsset, FMODAsset);

protected:
	static void _bind_methods();

public:
	void set_event_ref(FMOD_STUDIO_EVENTDESCRIPTION* event_description);
	FMOD_STUDIO_EVENTDESCRIPTION* get_event_description() const;

	void set_event_ref_from_description_ref(const Ref<StudioApi::EventDescription>& event_description);

	void set_3d(bool is_3d);
	bool get_3d() const;

	void set_oneshot(bool is_oneshot);
	bool get_oneshot() const;

	void set_is_snapshot(bool is_snapshot);
	bool get_is_snapshot() const;

	void set_min_distance(float min_distance);
	float get_min_distance() const;

	void set_max_distance(float max_distance);
	float get_max_distance() const;

	void set_parameters(const Dictionary& parameters);
	Dictionary get_parameters() const;

	bool has_parameter(const String& parameter_guid) const;

private:
	bool is_3d{};
	bool is_oneshot{};
	bool is_snapshot{};
	float min_distance{};
	float max_distance{};
	Dictionary parameters;
};

class BusAsset : public FMODAsset
{
	GDCLASS(BusAsset, FMODAsset);

protected:
	static void _bind_methods();

public:
	void set_bus_ref(const FMOD_STUDIO_BUS* bus);
};

class VCAAsset : public FMODAsset
{
	GDCLASS(VCAAsset, FMODAsset);

protected:
	static void _bind_methods();

public:
	void set_vca_ref(const FMOD_STUDIO_VCA* vca);
};

class ParameterAsset : public FMODAsset
{
	GDCLASS(ParameterAsset, FMODAsset);

protected:
	static void _bind_methods();

private:
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> parameter_description;
	Array labels;

public:
	void set_parameter_ref(const FMOD_STUDIO_PARAMETER_DESCRIPTION& parameter_description);

	void set_parameter_description(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>& parameter_description);
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> get_parameter_description() const;

	void set_labels(const Array& labels);
	Array get_labels() const;
};
#endif // FMOD_RESOURCES_H