//
// Created by danii on 18.09.2023.
//

#ifndef DDNET_API_TEE_H
#define DDNET_API_TEE_H

#include "Python.h"
#include "api_vector2.h"

using namespace std;

typedef struct {
	PyObject_HEAD;

	Vector2 pos;
	Vector2 vel;

	Vector2 hook_pos;
	Vector2 hook_dir;
	Vector2 hook_tele_base;
	int hook_tick;
	int hook_state;
	int hooked_player;

	int active_weapon;

	bool is_new_hook;

	int jumped;
	// m_JumpedTotal counts the jumps performed in the air
	int jumped_total;
	int jumps;

	int direction;
	int angle;

	int triggered_events;

	// DDRace
	int id;
	bool is_reset;

	Vector2 last_vel;
	int colliding;
	bool is_left_wall;

	// DDNet Character
	bool is_solo;
	bool is_jetpack;
	bool is_collision_disabled;
	bool is_endless_hook;
	bool is_endless_jump;
	bool is_hammer_hit_disabled;
	bool is_grenade_hit_disabled;
	bool is_laser_hit_disabled;
	bool is_shotgun_hit_disabled;
	bool is_hook_hit_disabled;
	bool is_super;
	bool has_telegun_gun;
	bool has_telegun_grenade;
	bool has_telegun_laser;
	int freeze_start;
	int freeze_end;
	bool is_in_freeze;
	bool is_deep_frozen;
	bool is_live_frozen;

	// Input
	int input_direction;
	Vector2 input_target;
	int input_jump;
	int input_fire;
	int input_hook;
	int input_player_flags;
	int input_wanted_weapon;
	int input_next_weapon;
	int input_prev_weapon;
} Tee;

extern PyTypeObject TeeType;

static PyObject* Tee_get_size(Tee* self, void* closure)
{
	return PyFloat_FromDouble(28.0);
}

static Tee *Tee_predict(Tee *self, PyObject *args)
{
	int ticksOffset;

	if (args == nullptr || !PyArg_ParseTuple(args, "i", &ticksOffset)) {
		return NULL;
	}

	CCharacterCore original = PythonAPI_GameClient->m_aClients[self->id].m_Predicted;

	CCharacterCore clone;

	clone.m_Pos = self->pos.toVec2();
	clone.m_Vel = self->vel.toVec2();
	clone.m_HookPos = self->hook_pos.toVec2();
	clone.m_HookDir = self->hook_dir.toVec2();
	clone.m_HookTeleBase = self->hook_tele_base.toVec2();
	clone.m_HookTick = self->hook_tick;
	clone.m_HookState = self->hook_state;
	clone.m_AttachedPlayers = original.m_AttachedPlayers;
	clone.m_ActiveWeapon = self->active_weapon;
	clone.m_aWeapons[0] = original.m_aWeapons[0];
	clone.m_aWeapons[1] = original.m_aWeapons[1];
	clone.m_aWeapons[2] = original.m_aWeapons[2];
	clone.m_aWeapons[3] = original.m_aWeapons[3];
	clone.m_aWeapons[4] = original.m_aWeapons[4];
	clone.m_aWeapons[5] = original.m_aWeapons[5];
	clone.m_Ninja = original.m_Ninja;
	clone.m_NewHook = self->is_new_hook;
	clone.m_Jumped = self->jumped;
	clone.m_JumpedTotal = self->jumped_total;
	clone.m_Jumps = self->jumps;
	clone.m_Direction = self->direction;
	clone.m_Angle = self->angle;
	clone.m_Input = original.m_Input;
	clone.m_TriggeredEvents = self->triggered_events;
	clone.m_Id = self->id;
	clone.m_Reset = self->is_reset;
	clone.m_Colliding = self->colliding;
	clone.m_LeftWall = self->is_left_wall;
	clone.m_Solo = self->is_solo;
	clone.m_Jetpack = self->is_jetpack;
	clone.m_CollisionDisabled = self->is_collision_disabled;
	clone.m_EndlessHook = self->is_endless_hook;
	clone.m_EndlessJump = self->is_endless_jump;
	clone.m_HammerHitDisabled = self->is_hammer_hit_disabled;
	clone.m_GrenadeHitDisabled = self->is_grenade_hit_disabled;
	clone.m_LaserHitDisabled = self->is_laser_hit_disabled;
	clone.m_ShotgunHitDisabled = self->is_shotgun_hit_disabled;
	clone.m_HookHitDisabled = self->is_hook_hit_disabled;
	clone.m_Super = self->is_super;
	clone.m_HasTelegunGun = self->has_telegun_gun;
	clone.m_HasTelegunGrenade = self->has_telegun_grenade;
	clone.m_HasTelegunLaser = self->has_telegun_laser;
	clone.m_FreezeStart = self->freeze_start;
	clone.m_FreezeEnd = self->freeze_end;
	clone.m_IsInFreeze = self->is_in_freeze;
	clone.m_DeepFrozen = self->is_deep_frozen;
	clone.m_LiveFrozen = self->is_live_frozen;
	clone.m_isClone = true;
	clone.ForceSetHookedPlayer(original.HookedPlayer());
	clone.SetTeamsCore(original.getTeams());
	clone.m_Tuning = original.m_Tuning;
	clone.m_pWorld = original.m_pWorld;
	clone.m_pCollision = original.m_pCollision;

	CCharacterCore predictedCore = clone.PredictTicks(ticksOffset);

	Tee *tee = (Tee *)PyObject_New(Tee, &TeeType);
	tee->pos = Vector2(predictedCore.m_Pos);
	tee->vel = Vector2(predictedCore.m_Vel);
	tee->hook_pos = Vector2(predictedCore.m_HookPos);
	tee->hook_dir = Vector2(predictedCore.m_HookDir);
	tee->hook_tele_base = Vector2(predictedCore.m_HookTeleBase);
	tee->hook_tick = predictedCore.m_HookTick;
	tee->hook_state = predictedCore.m_HookState;
	tee->active_weapon = predictedCore.m_ActiveWeapon;
	tee->is_new_hook = predictedCore.m_NewHook;
	tee->jumped = predictedCore.m_Jumped;
	tee->jumped_total = predictedCore.m_JumpedTotal;
	tee->jumps = predictedCore.m_Jumps;
	tee->direction = predictedCore.m_Direction;
	tee->angle = predictedCore.m_Angle;
	tee->triggered_events = predictedCore.m_TriggeredEvents;
	tee->id = predictedCore.m_Id;
	tee->is_reset = predictedCore.m_Reset;
	tee->colliding = predictedCore.m_Colliding;
	tee->is_left_wall = predictedCore.m_LeftWall;
	tee->is_solo = predictedCore.m_Solo;
	tee->is_jetpack = predictedCore.m_Jetpack;
	tee->is_collision_disabled = predictedCore.m_CollisionDisabled;
	tee->is_endless_hook = predictedCore.m_EndlessHook;
	tee->is_endless_jump = predictedCore.m_EndlessJump;
	tee->is_hammer_hit_disabled = predictedCore.m_HammerHitDisabled;
	tee->is_grenade_hit_disabled = predictedCore.m_GrenadeHitDisabled;
	tee->is_laser_hit_disabled = predictedCore.m_LaserHitDisabled;
	tee->is_shotgun_hit_disabled = predictedCore.m_ShotgunHitDisabled;
	tee->is_hook_hit_disabled = predictedCore.m_HookHitDisabled;
	tee->is_super = predictedCore.m_Super;
	tee->has_telegun_gun = predictedCore.m_HasTelegunGun;
	tee->has_telegun_grenade = predictedCore.m_HasTelegunGrenade;
	tee->has_telegun_laser = predictedCore.m_HasTelegunLaser;
	tee->freeze_start = predictedCore.m_FreezeStart;
	tee->freeze_end = predictedCore.m_FreezeEnd;
	tee->is_in_freeze = predictedCore.m_IsInFreeze;
	tee->is_deep_frozen = predictedCore.m_DeepFrozen;
	tee->is_live_frozen = predictedCore.m_LiveFrozen;

	return tee;
}

static PyMethodDef Tee_methods[] = {
	{"get_size", (PyCFunction)Tee_get_size, METH_NOARGS, "Return the size of the tee"},
	{"predict", (PyCFunction)Tee_predict, METH_VARARGS, "Predict current tee on ticksOffset"},
	{NULL}  /* Sentinel */
};

static int Tee_init(Tee *self, PyObject *args, PyObject *kwds)
{
	return 0;
}

static PyObject* Tee_getpos(Tee* self, void* closure)
{
	Vector2 *vector = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	vector->x = self->pos.x;
	vector->y = self->pos.y;

	return (PyObject *) vector;
}

static PyObject* Tee_getvel(Tee* self, void* closure)
{
	Vector2 *vector = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	vector->x = self->vel.x;
	vector->y = self->vel.y;

	return (PyObject *) vector;
}

static PyObject* Tee_gethook_pos(Tee* self, void* closure)
{
	Vector2 *vector = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	vector->x = self->hook_pos.x;
	vector->y = self->hook_pos.y;

	return (PyObject *) vector;
}

static PyObject* Tee_gethook_dir(Tee* self, void* closure)
{
	Vector2 *vector = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	vector->x = self->hook_dir.x;
	vector->y = self->hook_dir.y;

	return (PyObject *) vector;
}

static PyObject* Tee_gethook_tele_base(Tee* self, void* closure)
{
	Vector2 *vector = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	vector->x = self->hook_tele_base.x;
	vector->y = self->hook_tele_base.y;

	return (PyObject *) vector;
}

static PyObject* Tee_gethook_tick(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->hook_tick);
}

static PyObject* Tee_gethook_state(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->hook_state);
}

static PyObject* Tee_gethooked_player(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->hooked_player);
}

static PyObject* Tee_getactive_weapon(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->active_weapon);
}

static PyObject* Tee_getis_new_hook(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_new_hook);
}

static PyObject* Tee_getjumped(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->jumped);
}

static PyObject* Tee_getjumped_total(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->jumped_total);
}

static PyObject* Tee_getjumps(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->jumps);
}

static PyObject* Tee_getdirection(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->direction);
}

static PyObject* Tee_getangle(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->angle);
}

static PyObject* Tee_gettriggered_events(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->triggered_events);
}

static PyObject* Tee_getid(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->id);
}

static PyObject* Tee_getis_reset(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_reset);
}

static PyObject* Tee_getlast_vel(Tee* self, void* closure)
{
	Vector2 *vector = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	vector->x = self->last_vel.x;
	vector->y = self->last_vel.y;

	return (PyObject *) vector;
}

static PyObject* Tee_getcolliding(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->colliding);
}

static PyObject* Tee_getis_left_wall(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_left_wall);
}

static PyObject* Tee_getis_solo(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_solo);
}

static PyObject* Tee_getis_jetpack(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_jetpack);
}

static PyObject* Tee_getis_collision_disabled(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_collision_disabled);
}

static PyObject* Tee_getis_endless_hook(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_endless_hook);
}

static PyObject* Tee_getis_endless_jump(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_endless_jump);
}

static PyObject* Tee_getis_hammer_hit_disabled(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_hammer_hit_disabled);
}

static PyObject* Tee_getis_grenade_hit_disabled(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_grenade_hit_disabled);
}

static PyObject* Tee_getis_laser_hit_disabled(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_laser_hit_disabled);
}

static PyObject* Tee_getis_shotgun_hit_disabled(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_shotgun_hit_disabled);
}

static PyObject* Tee_getis_hook_hit_disabled(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_hook_hit_disabled);
}

static PyObject* Tee_getis_super(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_super);
}

static PyObject* Tee_gethas_telegun_gun(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->has_telegun_gun);
}

static PyObject* Tee_gethas_telegun_grenade(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->has_telegun_grenade);
}

static PyObject* Tee_gethas_telegun_laser(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->has_telegun_laser);
}

static PyObject* Tee_getfreeze_start(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->freeze_start);
}

static PyObject* Tee_getfreeze_end(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->freeze_end);
}

static PyObject* Tee_getis_in_freeze(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_in_freeze);
}

static PyObject* Tee_getis_deep_frozen(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_deep_frozen);
}

static PyObject* Tee_getis_live_frozen(Tee* self, void* closure)
{
	return Py_BuildValue("b", self->is_live_frozen);
}

static PyObject* Tee_getInputDirection(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->input_direction);
}

static PyObject* Tee_getInputTarget(Tee* self, void* closure)
{
	Vector2 *vector = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	vector->x = self->input_target.x;
	vector->y = self->input_target.y;

	return (PyObject *) vector;
}

static PyObject* Tee_getInputJump(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->input_jump);
}

static PyObject* Tee_getInputFire(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->input_fire);
}

static PyObject* Tee_getInputHook(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->input_hook);
}

static PyObject* Tee_getInputPlayerFlags(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->input_player_flags);
}

static PyObject* Tee_getInputWantedWeapon(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->input_wanted_weapon);
}

static PyObject* Tee_getInputNextWeapon(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->input_next_weapon);
}

static PyObject* Tee_getInputPrevWeapon(Tee* self, void* closure)
{
	return Py_BuildValue("i", self->input_prev_weapon);
}

static void Tee_dealloc(Tee* self)
{
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyGetSetDef Tee_getseters[] = {
	{"pos", (getter) Tee_getpos, NULL, "Position of the player (Vector2)", NULL},
	{"vel", (getter) Tee_getvel, NULL, "Velocity of the player (Vector2)", NULL},
	{"hook_pos", (getter) Tee_gethook_pos, NULL, "Hook position (Vector2)", NULL},
	{"hook_dir", (getter) Tee_gethook_dir, NULL, "Direction of the hook (Vector2)", NULL},
	{"hook_tele_base", (getter) Tee_gethook_tele_base, NULL, "Base position of the tele hook (Vector2)", NULL},
	{"hook_tick", (getter) Tee_gethook_tick, NULL, "Hook tick count (integer)", NULL},
	{"hook_state", (getter) Tee_gethook_state, NULL, "State of the hook (integer)", NULL},
	{"hooked_player", (getter) Tee_gethooked_player, NULL, "Hooked player id (integer)", NULL},
	{"active_weapon", (getter) Tee_getactive_weapon, NULL, "Active weapon id (integer)", NULL},
	{"is_new_hook", (getter) Tee_getis_new_hook, NULL, "Check if it is a new hook or not (boolean)", NULL},
	{"jumped", (getter) Tee_getjumped, NULL, "Check if player jumped (integer)", NULL},
	{"jumped_total", (getter) Tee_getjumped_total, NULL, "Total jumps made by player (integer)", NULL},
	{"jumps", (getter) Tee_getjumps, NULL, "Number of jumps thats left for player (integer)", NULL},
	{"direction", (getter) Tee_getdirection, NULL, "Direction in which player is facing (integer)", NULL},
	{"angle", (getter) Tee_getangle, NULL, "Angle which player is facing (integer)", NULL},
	{"triggered_events", (getter) Tee_gettriggered_events, NULL, "Events triggered by player (integer)", NULL},
	{"id", (getter) Tee_getid, NULL, "Player id (integer)", NULL},
	{"is_reset", (getter) Tee_getis_reset, NULL, "Check if player is resetted or not (boolean)", NULL},
	{"last_vel", (getter) Tee_getlast_vel, NULL, "Last velocity of player (Vector2)", NULL},
	{"colliding", (getter) Tee_getcolliding, NULL, "Check if player is colliding (integer)", NULL},
	{"is_left_wall", (getter) Tee_getis_left_wall, NULL, "Check if player is touching left wall (boolean)", NULL},
	{"is_solo", (getter) Tee_getis_solo, NULL, "Check if player is solo or not (boolean)", NULL},
	{"is_jetpack", (getter) Tee_getis_jetpack, NULL, "Check if jetpack is activated or not (boolean)", NULL},
	{"is_collision_disabled", (getter) Tee_getis_collision_disabled, NULL, "Check if collision is disabled or not (boolean)", NULL},
	{"is_endless_hook", (getter) Tee_getis_endless_hook, NULL, "Check if endless hook is activated or not (boolean)", NULL},
	{"is_endless_jump", (getter) Tee_getis_endless_jump, NULL, "Check if endless jump is activated or not (boolean)", NULL},
	{"is_hammer_hit_disabled", (getter) Tee_getis_hammer_hit_disabled, NULL, "Check if hammer hit is disabled or not (boolean)", NULL},
	{"is_grenade_hit_disabled", (getter) Tee_getis_grenade_hit_disabled, NULL, "Check if grenade hit is disabled or not (boolean)", NULL},
	{"is_laser_hit_disabled", (getter) Tee_getis_laser_hit_disabled, NULL, "Check if laser hit is disabled or not (boolean)", NULL},
	{"is_shotgun_hit_disabled", (getter) Tee_getis_shotgun_hit_disabled, NULL, "Check if shotgun hit is disabled or not (boolean)", NULL},
	{"is_hook_hit_disabled", (getter) Tee_getis_hook_hit_disabled, NULL, "Check if hook hit is disabled or not (boolean)", NULL},
	{"is_super", (getter) Tee_getis_super, NULL, "Check if player is super or not (boolean)", NULL},
	{"has_telegun_gun", (getter) Tee_gethas_telegun_gun, NULL, "Check if player has tele gun or not (boolean)", NULL},
	{"has_telegun_grenade", (getter) Tee_gethas_telegun_grenade, NULL, "Check if player has tele grenade or not (boolean)", NULL},
	{"has_telegun_laser", (getter) Tee_gethas_telegun_laser, NULL, "Check if player has tele laser or not (boolean)", NULL},
	{"freeze_start", (getter) Tee_getfreeze_start, NULL, "Freeze start tick count (integer)", NULL},
	{"freeze_end", (getter) Tee_getfreeze_end, NULL, "Freeze end tick count (integer)", NULL},
	{"is_in_freeze", (getter) Tee_getis_in_freeze, NULL, "Check if player is in freeze or not (boolean)", NULL},
	{"is_deep_frozen", (getter) Tee_getis_deep_frozen, NULL, "Check if player is deeply frozen or not (boolean)", NULL},
	{"is_live_frozen", (getter) Tee_getis_live_frozen, NULL, "Check if player is live frozen or not (boolean)", NULL},

	{"input_direction", (getter) Tee_getInputDirection, NULL, "Get input value of direction", NULL},
	{"input_target", (getter) Tee_getInputTarget, NULL, "Get input value of target", NULL},
	{"input_jump", (getter) Tee_getInputJump, NULL, "Get input value of jump", NULL},
	{"input_fire", (getter) Tee_getInputFire, NULL, "Get input value of fire", NULL},
	{"input_hook", (getter) Tee_getInputHook, NULL, "Get input value of hook", NULL},
	{"input_player_flags", (getter) Tee_getInputPlayerFlags, NULL, "Get input value of player_flags", NULL},
	{"input_wanted_weapon", (getter) Tee_getInputWantedWeapon, NULL, "Get input value of wantedWeapon", NULL},
	{"input_next_weapon", (getter) Tee_getInputNextWeapon, NULL, "Get input value of nextWeapon", NULL},

	{NULL}  /* Sentinel */
};

static PyObject* Tee_str(Tee* self)
{
	// Буфер для строковых представлений полей struct.
	char buf[2048];

	// Создание строковых представлений для обьектов типа Vector2.
	PyObject* pos_str_obj = Vector2_str(&(self->pos));
	PyObject* vel_str_obj = Vector2_str(&(self->vel));
	PyObject* hook_pos_str_obj = Vector2_str(&(self->hook_pos));
	PyObject* hook_dir_str_obj = Vector2_str(&(self->hook_dir));
	PyObject* hook_tele_base_str_obj = Vector2_str(&(self->hook_tele_base));
	PyObject* last_vel_str_obj = Vector2_str(&(self->last_vel));
	PyObject* input_target_str_obj = Vector2_str(&(self->input_target));

	// Получение указателей на C-строки.
	const char *pos_str = PyUnicode_AsUTF8(pos_str_obj);
	const char *vel_str = PyUnicode_AsUTF8(vel_str_obj);
	const char *hook_pos_str = PyUnicode_AsUTF8(hook_pos_str_obj);
	const char *hook_dir_str = PyUnicode_AsUTF8(hook_dir_str_obj);
	const char *hook_tele_base_str = PyUnicode_AsUTF8(hook_tele_base_str_obj);
	const char *last_vel_str = PyUnicode_AsUTF8(last_vel_str_obj);
	const char *input_target_str = PyUnicode_AsUTF8(input_target_str_obj);

	// Проверка получения всех указателей на строки.
	if (!pos_str || !vel_str || !hook_pos_str || !hook_dir_str || !hook_tele_base_str || !last_vel_str) {
		// Освобождение всех PyObject, если не удалось получить строки.
		Py_XDECREF(pos_str_obj);
		Py_XDECREF(vel_str_obj);
		Py_XDECREF(hook_pos_str_obj);
		Py_XDECREF(hook_dir_str_obj);
		Py_XDECREF(hook_tele_base_str_obj);
		Py_XDECREF(last_vel_str_obj);
		Py_XDECREF(input_target_str_obj);
		return NULL;
	}

	sprintf(
		buf,
		"Tee(\n"
			"	pos: %s,\n"
			"	vel: %s,\n"
			"	hook_pos: %s,\n"
			"	hook_dir: %s,\n"
			"	hook_tele_base: %s,\n"
			"	hook_tick: %d,\n"
			"	hook_state: %d,\n"
			"	hooked_player: %d,\n"
			"	active_weapon: %d,\n"
			"	is_new_hook: %s,\n"
			"	jumped: %d,\n"
			"	jumped_total: %d,\n"
			"	jumps: %d,\n"
			"	direction: %d,\n"
			"	angle: %d,\n"
			"	triggered_events: %d,\n"
			"	id: %d,\n"
			"	is_reset: %s,\n"
			"	last_vel: %s,\n"
			"	colliding: %d,\n"
			"	is_left_wall: %s,\n"
			"	is_solo: %s,\n"
			"	is_jetpack: %s,\n"
			"	is_collision_disabled: %s,\n"
			"	is_endless_hook: %s,\n"
			"	is_endless_jump: %s,\n"
			"	is_hammer_hit_disabled: %s,\n"
			"	is_grenade_hit_disabled: %s,\n"
			"	is_laser_hit_disabled: %s,\n"
			"	is_shotgun_hit_disabled: %s,\n"
			"	is_hook_hit_disabled: %s,\n"
			"	is_super: %s,\n"
			"	has_telegun_gun: %s,\n"
			"	has_telegun_grenade: %s,\n"
			"	has_telegun_laser: %s,\n"
			"	freeze_start: %d,\n"
			"	freeze_end: %d,\n"
			"	is_in_freeze: %s,\n"
			"	is_deep_frozen: %s,\n"
			"	is_live_frozen: %s\n"
			"	input_direction: %d\n"
			"	input_target: %s\n"
			"	input_jump: %d\n"
			"	input_fire: %d\n"
			"	input_hook: %d\n"
			"	input_player_flags: %d\n"
			"	input_wanted_weapon: %d\n"
			"	input_next_weapon: %d\n"
			"	input_prev_weapon: %d\n"
		")",
		pos_str, vel_str, hook_pos_str, hook_dir_str, hook_tele_base_str, self->hook_tick, self->hook_state,
		self->hooked_player, self->active_weapon, self->is_new_hook ? "true" : "false", self->jumped, self->jumped_total, self->jumps,
		self->direction, self->angle, self->triggered_events, self->id, self->is_reset ? "true" : "false", last_vel_str,
		self->colliding, self->is_left_wall ? "true" : "false", self->is_solo ? "true" : "false", self->is_jetpack ? "true" : "false",
		self->is_collision_disabled ? "true" : "false", self->is_endless_hook ? "true" : "false", self->is_endless_jump ? "true" : "false",
		self->is_hammer_hit_disabled ? "true" : "false", self->is_grenade_hit_disabled ? "true" : "false",
		self->is_laser_hit_disabled ? "true" : "false", self->is_shotgun_hit_disabled ? "true" : "false",
		self->is_hook_hit_disabled ? "true" : "false", self->is_super ? "true" : "false", self->has_telegun_gun ? "true" : "false",
		self->has_telegun_grenade ? "true" : "false", self->has_telegun_laser ? "true" : "false", self->freeze_start,
		self->freeze_end, self->is_in_freeze ? "true" : "false", self->is_deep_frozen ? "true" : "false", self->is_live_frozen ? "true" : "false",
		self->input_direction,
		input_target_str,
		self->input_jump,
		self->input_fire,
		self->input_hook,
		self->input_player_flags,
		self->input_wanted_weapon,
		self->input_next_weapon,
		self->input_prev_weapon
	);

	// Уменьшение счетчика ссылок на все PyObject.
	Py_DECREF(pos_str_obj);
	Py_DECREF(vel_str_obj);
	Py_DECREF(hook_pos_str_obj);
	Py_DECREF(hook_dir_str_obj);
	Py_DECREF(hook_tele_base_str_obj);
	Py_DECREF(last_vel_str_obj);
	Py_DECREF(input_target_str_obj);

	return PyUnicode_FromString(buf);
}

inline PyTypeObject TeeType = {
	{ PyObject_HEAD_INIT(NULL) 0, },
	"API.Tee",                /* tp_name */
	sizeof(Tee),              /* tp_basicsize */
	0,                            /* tp_itemsize */
	(destructor)Tee_dealloc,  /* tp_dealloc */
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
	(reprfunc)Tee_str,                            /* tp_str */
	0,                            /* tp_getattro */
	0,                            /* tp_setattro */
	0,                            /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,           /* tp_flags */
	"Tee",           /* tp_doc */
	0,                            /* tp_traverse */
	0,                            /* tp_clear */
	0,                            /* tp_richcompare */
	0,                            /* tp_weaklistoffset */
	0,                            /* tp_iter */
	0,                            /* tp_iternext */
	Tee_methods,              /* tp_methods */
	0,                            /* tp_members */
	Tee_getseters,                            /* tp_getset */
	0,                            /* tp_base */
	0,                            /* tp_dict */
	0,                            /* tp_descr_get */
	0,                            /* tp_descr_set */
	0,                            /* tp_dictoffset */
	(initproc)Tee_init,      /* tp_init */
	0,                            /* tp_alloc */
	PyType_GenericNew,            /* tp_new */
};

#endif // DDNET_API_TEE_H

