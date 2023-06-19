#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

#include "core/engine.h"
#include "core/class_db.h"


#include "api/studio_api.h"
#ifdef TOOLS_ENABLED
#include "editor/inspector_browser.h"
#include "editor/project_browser.h"
#include "editor/studio_event_emitter_3d_gizmo_plugin.h"
#endif // TOOLS_ENABLED
#include "fmod_assets.h"
#include "fmod_studio_editor_module.h"
#include "fmod_studio_module.h"
#include "scene/studio_bank_loader.h"
#include "scene/studio_event_emitter.h"
#include "scene/fmod_emitter_update_triggerer.h"
#include "scene/studio_global_parameter_trigger.h"
#include "scene/studio_listener.h"
#include "scene/studio_parameter_trigger.h"
#include "utils/debug_monitors.h"
#include "utils/runtime_utils.h"
#include "utils/tcp_client.h"

void register_fmod_types();
void unregister_fmod_types();

#endif // REGISTER_TYPES_H