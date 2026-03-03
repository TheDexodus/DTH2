#ifndef DDNET_API_WORLD_H
#define DDNET_API_WORLD_H

#include "api.h"
#include "game/client/prediction/entities/character.h"

struct World {
	PyObject_HEAD;
	CGameWorld world;
};

extern PyTypeObject WorldType;

static Player* World_getPlayer(World* self, PyObject *args)
{
	int playerId;

	if (args == nullptr || !PyArg_ParseTuple(args, "i", &playerId)) {
		return NULL;
	}

	auto clientData = PythonAPI_GameClient->m_aClients[playerId];
	CCharacter* character = self->world.GetCharacterById(playerId);

	Player *player = (Player *)PyObject_New(Player, &PlayerType);
	player->use_custom_color = clientData.m_UseCustomColor;
	player->color_body = clientData.m_ColorBody;
	player->color_feet = clientData.m_ColorFeet;

	strcpy(player->name, clientData.m_aName);
	strcpy(player->clan, clientData.m_aClan);
	player->country = clientData.m_Country;
	strcpy(player->skin_name, clientData.m_aSkinName);
	player->skin_color = clientData.m_SkinColor;
	player->team = character->Team();
	player->emoticon = clientData.m_Emoticon;
	player->emoticon_start_fraction = clientData.m_EmoticonStartFraction;
	player->emoticon_start_tick = clientData.m_EmoticonStartTick;
	player->is_solo = clientData.m_Solo;
	player->is_jetpack = clientData.m_Jetpack;
	player->is_collision_disabled = clientData.m_CollisionDisabled;
	player->is_endless_hook = clientData.m_EndlessHook;
	player->is_endless_jump = clientData.m_EndlessJump;
	player->is_hammer_hit_disabled = clientData.m_HammerHitDisabled;
	player->is_grenade_hit_disabled = clientData.m_GrenadeHitDisabled;
	player->is_laser_hit_disabled = clientData.m_LaserHitDisabled;
	player->is_shotgun_hit_disabled = clientData.m_ShotgunHitDisabled;
	player->is_hook_hit_disabled = clientData.m_HookHitDisabled;
	player->is_super = clientData.m_Super;
	player->has_telegun_gun = clientData.m_HasTelegunGun;
	player->has_telegun_grenade = clientData.m_HasTelegunGrenade;
	player->has_telegun_laser = clientData.m_HasTelegunLaser;
	player->freeze_end = clientData.m_FreezeEnd;
	player->is_deep_frozen = clientData.m_DeepFrozen;
	player->is_live_frozen = clientData.m_LiveFrozen;
	player->angle = clientData.m_Angle;
	player->is_active = clientData.m_Active;
	player->is_chat_ignore = clientData.m_ChatIgnore;
	player->is_emoticon_ignore = clientData.m_EmoticonIgnore;
	player->is_friend = clientData.m_Friend;
	player->is_foe = clientData.m_Foe;

	player->auth_level = clientData.m_AuthLevel;
	player->is_afk = clientData.m_Afk;
	player->is_paused = clientData.m_Paused;
	player->is_spec = clientData.m_Spec;

	player->render_pos = Vector2(clientData.m_RenderPos);
	player->is_predicted = clientData.m_IsPredicted;
	player->is_predicted_local = clientData.m_IsPredictedLocal;
	player->smooth_start[0] = clientData.m_aSmoothStart[0];
	player->smooth_start[1] = clientData.m_aSmoothStart[1];
	player->smooth_len[0] = clientData.m_aSmoothLen[0];
	player->smooth_len[1] = clientData.m_aSmoothLen[1];
	player->is_spec_char_present = clientData.m_SpecCharPresent;
	player->spec_char = clientData.m_SpecChar;

	player->tee.pos = Vector2(character->Core()->m_Pos);
	player->tee.vel = Vector2(character->Core()->m_Vel);
	player->tee.hook_pos = Vector2(character->Core()->m_HookPos);
	player->tee.hook_dir = Vector2(character->Core()->m_HookDir);
	player->tee.hook_tele_base = Vector2(character->Core()->m_HookTeleBase);
	player->tee.hook_tick = character->Core()->m_HookTick;
	player->tee.hook_state = character->Core()->m_HookState;
	player->tee.active_weapon = character->Core()->m_ActiveWeapon;
	player->tee.is_new_hook = character->Core()->m_NewHook;
	player->tee.jumped = character->Core()->m_Jumped;
	player->tee.jumped_total = character->Core()->m_JumpedTotal;
	player->tee.jumps = character->Core()->m_Jumps;
	player->tee.direction = character->Core()->m_Direction;
	player->tee.angle = character->Core()->m_Angle;
	player->tee.triggered_events = character->Core()->m_TriggeredEvents;
	player->tee.id = character->Core()->m_Id;
	player->tee.is_reset = character->Core()->m_Reset;
	player->tee.colliding = character->Core()->m_Colliding;
	player->tee.is_left_wall = character->Core()->m_LeftWall;
	player->tee.is_solo = character->Core()->m_Solo;
	player->tee.is_jetpack = character->Core()->m_Jetpack;
	player->tee.is_collision_disabled = character->Core()->m_CollisionDisabled;
	player->tee.is_endless_hook = character->Core()->m_EndlessHook;
	player->tee.is_endless_jump = character->Core()->m_EndlessJump;
	player->tee.is_hammer_hit_disabled = character->Core()->m_HammerHitDisabled;
	player->tee.is_grenade_hit_disabled = character->Core()->m_GrenadeHitDisabled;
	player->tee.is_laser_hit_disabled = character->Core()->m_LaserHitDisabled;
	player->tee.is_shotgun_hit_disabled = character->Core()->m_ShotgunHitDisabled;
	player->tee.is_hook_hit_disabled = character->Core()->m_HookHitDisabled;
	player->tee.is_super = character->Core()->m_Super;
	player->tee.has_telegun_gun = character->Core()->m_HasTelegunGun;
	player->tee.has_telegun_grenade = character->Core()->m_HasTelegunGrenade;
	player->tee.has_telegun_laser = character->Core()->m_HasTelegunLaser;
	player->tee.freeze_start = character->Core()->m_FreezeStart;
	player->tee.freeze_end = character->Core()->m_FreezeEnd;
	player->tee.is_in_freeze = character->Core()->m_IsInFreeze;
	player->tee.is_deep_frozen = character->Core()->m_DeepFrozen;
	player->tee.is_live_frozen = character->Core()->m_LiveFrozen;

	player->tee.input_direction = character->Core()->m_Input.m_Direction;
	player->tee.input_target = Vector2(vec2(character->Core()->m_Input.m_TargetX, character->Core()->m_Input.m_TargetY));
	player->tee.input_jump = character->Core()->m_Input.m_Jump;
	player->tee.input_fire = character->Core()->m_Input.m_Fire;
	player->tee.input_hook = character->Core()->m_Input.m_Hook;
	player->tee.input_player_flags = character->Core()->m_Input.m_PlayerFlags;
	player->tee.input_wanted_weapon = character->Core()->m_Input.m_WantedWeapon;
	player->tee.input_next_weapon = character->Core()->m_Input.m_NextWeapon;
	player->tee.input_prev_weapon = character->Core()->m_Input.m_PrevWeapon;

	return player;
}

static World* World_predict(World* self, PyObject *args)
{
	int ticks;

	if (args == nullptr || !PyArg_ParseTuple(args, "i", &ticks)) {
		return NULL;
	}

	World *world = (World *)PyObject_New(World, &WorldType);
	if (!world) {
		return NULL; // Не удалось выделить память
	}
	new (&world->world) CGameWorld();
	world->world.CopyWorld(&self->world);

	for (int i = 0; i < ticks; i++)
	{
		world->world.Tick();
	}

	return world;
}

static PyObject* World_InputMove(World* self, PyObject *args)
{
	int playerId;
	int direction;

	if (!PyArg_ParseTuple(args, "ii", &playerId, &direction))
		return NULL;


	self->world.GetCharacterById(playerId)->m_SavedInput.m_Direction = direction;

	Py_RETURN_NONE;
}

static PyObject* World_InputSetTarget(World* self, PyObject *args)
{
	int playerId;
	Vector2 *target;

	if (!PyArg_ParseTuple(args, "iO!", &playerId, &Vector2Type, &target))
		return NULL;

	self->world.GetCharacterById(playerId)->m_SavedInput.m_TargetX = target->x;
	self->world.GetCharacterById(playerId)->m_SavedInput.m_TargetY = target->y;

	Py_RETURN_NONE;
}

static PyObject* World_InputJump(World* self, PyObject *args)
{
	int playerId;

	if (!PyArg_ParseTuple(args, "i", &playerId))
		return NULL;

	self->world.GetCharacterById(playerId)->m_SavedInput.m_Jump = 1;

	Py_RETURN_NONE;
}

static PyObject* World_InputFire(World* self, PyObject *args)
{
	int playerId;

	if (!PyArg_ParseTuple(args, "i", &playerId))
		return NULL;

	self->world.GetCharacterById(playerId)->m_SavedInput.m_Jump = (self->world.GetCharacterById(playerId)->m_SavedInput.m_Fire + 1) % 64;

	Py_RETURN_NONE;
}

static PyObject* World_InputHook(World* self, PyObject *args)
{
	int playerId;
	bool hook;

	if (!PyArg_ParseTuple(args, "ib", &playerId, &hook))
		return NULL;


	self->world.GetCharacterById(playerId)->m_SavedInput.m_Hook = hook ? 1 : 0;

	Py_RETURN_NONE;
}

static PyObject* World_GetGameTick(World* self, PyObject *args)
{
	return PyLong_FromLong(self->world.GameTick());
}

static PyMethodDef World_methods[] = {
	{"get_game_tick", (PyCFunction)World_GetGameTick, METH_VARARGS, "get_game_tick()"},
	{"get_player", (PyCFunction)World_getPlayer, METH_VARARGS, "get_player(player_id)"},
	{"input_move", (PyCFunction)World_InputMove, METH_VARARGS, "input_move(player_id, direction)"},
	{"input_set_target", (PyCFunction)World_InputSetTarget, METH_VARARGS, "input_set_target(player_id, target_pos)"},
	{"input_jump", (PyCFunction)World_InputJump, METH_VARARGS, "input_jump(player_id)"},
	{"input_fire", (PyCFunction)World_InputFire, METH_VARARGS, "input_fire(player_id)"},
	{"input_hook", (PyCFunction)World_InputHook, METH_VARARGS, "input_hook(player_id, hook)"},
	{"predict", (PyCFunction)World_predict, METH_VARARGS, "Predict world on ticks"},
	{NULL}  /* Sentinel */
};

static PyGetSetDef World_getseters[] = {
	{NULL}  /* Sentinel */
};

static int World_init(World *self, PyObject *args, PyObject *kwds)
{
	self->world.CopyWorld(&PythonAPI_GameClient->m_GameWorld);

	return 0;
}

static void World_dealloc(World* self)
{
	self->world.~CGameWorld();
	Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *API_World_Predict(PyObject *self, PyObject *args)
{
	int ticksOffset;

	if(args == nullptr || !PyArg_ParseTuple(args, "i", &ticksOffset))
	{
		return NULL;
	}

	CGameWorld world;
	world.CopyWorld(&PythonAPI_GameClient->m_GameWorld);

	for(int i = 0; i < ticksOffset; i++)
	{
		world.Tick();
	}

	return Py_None;
}

inline PyTypeObject WorldType = {
	{ PyObject_HEAD_INIT(NULL) 0, },
	"API.World",                /* tp_name */
	sizeof(World),              /* tp_basicsize */
	0,                            /* tp_itemsize */
	(destructor)World_dealloc,  /* tp_dealloc */
	0,                            /* tp_print */
	0,                            /* tp_getattr */
	0,                            /* tp_setattr */
	0,                            /* tp_compare */
	0,                            /* tp_repr */
	0,               /* tp_as_number */
	0,                            /* tp_as_sequence */
	0,                            /* tp_as_mapping */
	0,                            /* tp_hash */
	0,                            /* tp_call */
	0,                            /* tp_str */
	0,                            /* tp_getattro */
	0,                            /* tp_setattro */
	0,                            /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,           /* tp_flags */
	"World",           /* tp_doc */
	0,                            /* tp_traverse */
	0,                            /* tp_clear */
	0,                            /* tp_richcompare */
	0,                            /* tp_weaklistoffset */
	0,                            /* tp_iter */
	0,                            /* tp_iternext */
	World_methods,              /* tp_methods */
	0,                            /* tp_members */
	World_getseters,                            /* tp_getset */
	0,                            /* tp_base */
	0,                            /* tp_dict */
	0,                            /* tp_descr_get */
	0,                            /* tp_descr_set */
	0,                            /* tp_dictoffset */
	(initproc)World_init,      /* tp_init */
	0,                            /* tp_alloc */
	PyType_GenericNew,            /* tp_new */
};

#endif // DDNET_API_WORLD_H

