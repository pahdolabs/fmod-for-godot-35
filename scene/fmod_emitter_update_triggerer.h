#ifndef FMOD_EMITTER_UPDATE_TRIGGERER_H
#define FMOD_EMITTER_UPDATE_TRIGGERER_H

#include "core/reference.h"

class FMODEmitterUpdateTriggerer : public Reference {
	GDCLASS(FMODEmitterUpdateTriggerer, Reference);

protected:
	static void _bind_methods();

public:
	void trigger_update();
};

#endif //FMOD_EMITTER_UPDATE_TRIGGERER_H
