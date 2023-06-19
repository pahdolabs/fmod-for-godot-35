#include "fmod_emitter_update_triggerer.h"
#include "studio_event_emitter.h"

void FMODEmitterUpdateTriggerer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("trigger_update"), &FMODEmitterUpdateTriggerer::trigger_update);
}

void FMODEmitterUpdateTriggerer::trigger_update() {
	StudioEventEmitter3D::update_active_emitters();
}
