//
// Created by danii on 18.09.2023.
//

#ifndef DDNET_API_PLAYER_H
#define DDNET_API_PLAYER_H

#include "Python.h"
#include "game/client/gameclient.h"
#include "api_tee.h"
#include "api.h"
#include "api_vector2.h"

using namespace std;

struct Player {
	PyObject_HEAD;
	int use_custom_color;
	int color_body;
	int color_feet;

	char name[16];
	char clan[12];
	int country;
	char skin_name[64];
	int skin_color;
	int team;
	int emoticon;
	float emoticon_start_fraction;
	int emoticon_start_tick;
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
	int freeze_end;
	bool is_deep_frozen;
	bool is_live_frozen;

	float angle;
	bool is_active;
	bool is_chat_ignore;
	bool is_emoticon_ignore;
	bool is_friend;
	bool is_foe;

	int auth_level;
	bool is_afk;
	bool is_paused;
	bool is_spec;

	Vector2 render_pos;
	bool is_predicted;
	bool is_predicted_local;
	int64_t smooth_start[2];
	int64_t smooth_len[2];
	bool is_spec_char_present;
	Vector2 spec_char;

	Tee tee;
};

extern PyTypeObject PlayerType;

static PyMethodDef Player_methods[] = {
	{NULL}  /* Sentinel */
};

static int Player_init(Player *self, PyObject *args, PyObject *kwds)
{
	int id;

	if (!PyArg_ParseTuple(args, "i", &id))
		return -1;

	if(id == -1)
		id = g_Config.m_ClDummy;
	else if (id == -2)
		id = (g_Config.m_ClDummy + 1) % 2;

	if (id < 0 || id > 63) {
		PyErr_SetString(PyExc_TypeError, "Expected int argument, with value between -2 <-> 63");

		return -1;
	}

	CGameClient::CClientData clientData = PythonAPI_GameClient->m_aClients[id];
	// clientData.m_RenderInfo->

	self->use_custom_color = clientData.m_UseCustomColor;
	self->color_body = clientData.m_ColorBody;
	self->color_feet = clientData.m_ColorFeet;

	strcpy(self->name, clientData.m_aName);
	strcpy(self->clan, clientData.m_aClan);
	self->country = clientData.m_Country;
	strcpy(self->skin_name, clientData.m_aSkinName);
	self->skin_color = clientData.m_SkinColor;
	self->team = clientData.m_Team;
	self->emoticon = clientData.m_Emoticon;
	self->emoticon_start_fraction = clientData.m_EmoticonStartFraction;
	self->emoticon_start_tick = clientData.m_EmoticonStartTick;
	self->is_solo = clientData.m_Solo;
	self->is_jetpack = clientData.m_Jetpack;
	self->is_collision_disabled = clientData.m_CollisionDisabled;
	self->is_endless_hook = clientData.m_EndlessHook;
	self->is_endless_jump = clientData.m_EndlessJump;
	self->is_hammer_hit_disabled = clientData.m_HammerHitDisabled;
	self->is_grenade_hit_disabled = clientData.m_GrenadeHitDisabled;
	self->is_laser_hit_disabled = clientData.m_LaserHitDisabled;
	self->is_shotgun_hit_disabled = clientData.m_ShotgunHitDisabled;
	self->is_hook_hit_disabled = clientData.m_HookHitDisabled;
	self->is_super = clientData.m_Super;
	self->has_telegun_gun = clientData.m_HasTelegunGun;
	self->has_telegun_grenade = clientData.m_HasTelegunGrenade;
	self->has_telegun_laser = clientData.m_HasTelegunLaser;
	self->freeze_end = clientData.m_FreezeEnd;
	self->is_deep_frozen = clientData.m_DeepFrozen;
	self->is_live_frozen = clientData.m_LiveFrozen;

	self->angle = clientData.m_Angle;
	self->is_active = clientData.m_Active;
	self->is_chat_ignore = clientData.m_ChatIgnore;
	self->is_emoticon_ignore = clientData.m_EmoticonIgnore;
	self->is_friend = clientData.m_Friend;
	self->is_foe = clientData.m_Foe;

	self->auth_level = clientData.m_AuthLevel;
	self->is_afk = clientData.m_Afk;
	self->is_paused = clientData.m_Paused;
	self->is_spec = clientData.m_Spec;

	self->render_pos = Vector2(clientData.m_RenderPos);
	self->is_predicted = clientData.m_IsPredicted;
	self->is_predicted_local = clientData.m_IsPredictedLocal;
	self->smooth_start[0] = clientData.m_aSmoothStart[0];
	self->smooth_start[1] = clientData.m_aSmoothStart[1];
	self->smooth_len[0] = clientData.m_aSmoothLen[0];
	self->smooth_len[1] = clientData.m_aSmoothLen[1];
	self->is_spec_char_present = clientData.m_SpecCharPresent;
	self->spec_char = clientData.m_SpecChar;

	self->tee.pos = Vector2(clientData.m_Predicted.m_Pos);
	self->tee.vel = Vector2(clientData.m_Predicted.m_Vel);
	self->tee.hook_pos = Vector2(clientData.m_Predicted.m_HookPos);
	self->tee.hook_dir = Vector2(clientData.m_Predicted.m_HookDir);
	self->tee.hook_tele_base = Vector2(clientData.m_Predicted.m_HookTeleBase);
	self->tee.hook_tick = clientData.m_Predicted.m_HookTick;
	self->tee.hook_state = clientData.m_Predicted.m_HookState;
	self->tee.active_weapon = clientData.m_Predicted.m_ActiveWeapon;
	self->tee.is_new_hook = clientData.m_Predicted.m_NewHook;
	self->tee.jumped = clientData.m_Predicted.m_Jumped;
	self->tee.jumped_total = clientData.m_Predicted.m_JumpedTotal;
	self->tee.jumps = clientData.m_Predicted.m_Jumps;
	self->tee.direction = clientData.m_Predicted.m_Direction;
	self->tee.angle = clientData.m_Predicted.m_Angle;
	self->tee.triggered_events = clientData.m_Predicted.m_TriggeredEvents;
	self->tee.id = clientData.m_Predicted.m_Id;
	self->tee.is_reset = clientData.m_Predicted.m_Reset;
	self->tee.colliding = clientData.m_Predicted.m_Colliding;
	self->tee.is_left_wall = clientData.m_Predicted.m_LeftWall;
	self->tee.is_solo = clientData.m_Predicted.m_Solo;
	self->tee.is_jetpack = clientData.m_Predicted.m_Jetpack;
	self->tee.is_collision_disabled = clientData.m_Predicted.m_CollisionDisabled;
	self->tee.is_endless_hook = clientData.m_Predicted.m_EndlessHook;
	self->tee.is_endless_jump = clientData.m_Predicted.m_EndlessJump;
	self->tee.is_hammer_hit_disabled = clientData.m_Predicted.m_HammerHitDisabled;
	self->tee.is_grenade_hit_disabled = clientData.m_Predicted.m_GrenadeHitDisabled;
	self->tee.is_laser_hit_disabled = clientData.m_Predicted.m_LaserHitDisabled;
	self->tee.is_shotgun_hit_disabled = clientData.m_Predicted.m_ShotgunHitDisabled;
	self->tee.is_hook_hit_disabled = clientData.m_Predicted.m_HookHitDisabled;
	self->tee.is_super = clientData.m_Predicted.m_Super;
	self->tee.has_telegun_gun = clientData.m_Predicted.m_HasTelegunGun;
	self->tee.has_telegun_grenade = clientData.m_Predicted.m_HasTelegunGrenade;
	self->tee.has_telegun_laser = clientData.m_Predicted.m_HasTelegunLaser;
	self->tee.freeze_start = clientData.m_Predicted.m_FreezeStart;
	self->tee.freeze_end = clientData.m_Predicted.m_FreezeEnd;
	self->tee.is_in_freeze = clientData.m_Predicted.m_IsInFreeze;
	self->tee.is_deep_frozen = clientData.m_Predicted.m_DeepFrozen;
	self->tee.is_live_frozen = clientData.m_Predicted.m_LiveFrozen;

	self->tee.input_direction = clientData.m_Predicted.m_Input.m_Direction;
	self->tee.input_target = Vector2(vec2(clientData.m_Predicted.m_Input.m_TargetX, clientData.m_Predicted.m_Input.m_TargetY));
	self->tee.input_jump = clientData.m_Predicted.m_Input.m_Jump;
	self->tee.input_fire = clientData.m_Predicted.m_Input.m_Fire;
	self->tee.input_hook = clientData.m_Predicted.m_Input.m_Hook;
	self->tee.input_player_flags = clientData.m_Predicted.m_Input.m_PlayerFlags;
	self->tee.input_wanted_weapon = clientData.m_Predicted.m_Input.m_WantedWeapon;
	self->tee.input_next_weapon = clientData.m_Predicted.m_Input.m_NextWeapon;
	self->tee.input_prev_weapon = clientData.m_Predicted.m_Input.m_PrevWeapon;

	return 0;
}

static void Player_dealloc(Player* self)
{
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* Player_getuse_custom_color(Player* self, void* closure)
{
	return Py_BuildValue("i", self->use_custom_color);
}

static PyObject* Player_getcolor_body(Player* self, void* closure)
{
	return Py_BuildValue("i", self->color_body);
}

static PyObject* Player_getcolor_feet(Player* self, void* closure)
{
	return Py_BuildValue("i", self->color_feet);
}

static PyObject* Player_getname(Player* self, void* closure)
{
	return Py_BuildValue("s", self->name);
}

static PyObject* Player_getclan(Player* self, void* closure)
{
	return Py_BuildValue("s", self->clan);
}

static PyObject* Player_getcountry(Player* self, void* closure)
{
	return Py_BuildValue("i", self->country);
}

static PyObject* Player_getskin_name(Player* self, void* closure)
{
	return Py_BuildValue("s", self->skin_name);
}

static PyObject* Player_getskin_color(Player* self, void* closure)
{
	return Py_BuildValue("i", self->skin_color);
}

static PyObject* Player_getteam(Player* self, void* closure)
{
	return Py_BuildValue("i", self->team);
}

static PyObject* Player_getemoticon(Player* self, void* closure)
{
	return Py_BuildValue("i", self->emoticon);
}

static PyObject* Player_getemoticon_start_fraction(Player* self, void* closure)
{
	return Py_BuildValue("d", (double) self->emoticon_start_fraction);
}

static PyObject* Player_getemoticon_start_tick(Player* self, void* closure)
{
	return Py_BuildValue("i", self->emoticon_start_tick);
}

static PyObject* Player_getis_solo(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_solo);
}

static PyObject* Player_getis_jetpack(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_jetpack);
}

static PyObject* Player_getis_collision_disabled(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_collision_disabled);
}

static PyObject* Player_getis_endless_hook(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_endless_hook);
}

static PyObject* Player_getis_endless_jump(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_endless_jump);
}

static PyObject* Player_getis_hammer_hit_disabled(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_hammer_hit_disabled);
}

static PyObject* Player_getis_grenade_hit_disabled(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_grenade_hit_disabled);
}

static PyObject* Player_getis_laser_hit_disabled(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_laser_hit_disabled);
}

static PyObject* Player_getis_shotgun_hit_disabled(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_shotgun_hit_disabled);
}

static PyObject* Player_getis_hook_hit_disabled(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_hook_hit_disabled);
}

static PyObject* Player_getis_super(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_super);
}

static PyObject* Player_gethas_telegun_gun(Player* self, void* closure)
{
	return Py_BuildValue("b", self->has_telegun_gun);
}

static PyObject* Player_gethas_telegun_grenade(Player* self, void* closure)
{
	return Py_BuildValue("b", self->has_telegun_grenade);
}

static PyObject* Player_gethas_telegun_laser(Player* self, void* closure)
{
	return Py_BuildValue("b", self->has_telegun_laser);
}

static PyObject* Player_getfreeze_end(Player* self, void* closure)
{
	return Py_BuildValue("i", self->freeze_end);
}

static PyObject* Player_getis_deep_frozen(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_deep_frozen);
}

static PyObject* Player_getis_live_frozen(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_live_frozen);
}

static PyObject* Player_getangle(Player* self, void* closure)
{
	return Py_BuildValue("d", (double) self->angle);
}

static PyObject* Player_getis_active(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_active);
}

static PyObject* Player_getis_chat_ignore(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_chat_ignore);
}

static PyObject* Player_getis_emoticon_ignore(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_emoticon_ignore);
}

static PyObject* Player_getis_friend(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_friend);
}

static PyObject* Player_getis_foe(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_foe);
}

static PyObject* Player_getauth_level(Player* self, void* closure)
{
	return Py_BuildValue("i", self->auth_level);
}

static PyObject* Player_getis_afk(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_afk);
}

static PyObject* Player_getis_paused(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_paused);
}

static PyObject* Player_getis_spec(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_spec);
}

static PyObject* Player_getrender_pos(Player* self, void* closure)
{
	Vector2 *vector = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	vector->x = self->render_pos.x;
	vector->y = self->render_pos.y;

	return (PyObject *) vector;
}

static PyObject* Player_getis_predicted(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_predicted);
}

static PyObject* Player_getis_predicted_local(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_predicted_local);
}

static PyObject* Player_getsmooth_start(Player* self, void* closure)
{
	return Py_BuildValue("(LL)", self->smooth_start[0], self->smooth_start[1]);
}

static PyObject* Player_getsmooth_len(Player* self, void* closure)
{
	return Py_BuildValue("(LL)", self->smooth_len[0], self->smooth_len[1]);
}

static PyObject* Player_getis_spec_char_present(Player* self, void* closure)
{
	return Py_BuildValue("b", self->is_spec_char_present);
}

static PyObject* Player_getspec_char(Player* self, void* closure)
{
	Vector2 *vector = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	vector->x = self->spec_char.x;
	vector->y = self->spec_char.y;

	return (PyObject *) vector;
}

static PyObject* Player_gettee(Player* self, void* closure)
{
	Tee *tee = (Tee *)PyObject_New(Tee, &TeeType);

	tee->pos = self->tee.pos;
	tee->vel = self->tee.vel;
	tee->hook_pos = self->tee.hook_pos;
	tee->hook_dir = self->tee.hook_dir;
	tee->hook_tele_base = self->tee.hook_tele_base;
	tee->hook_tick = self->tee.hook_tick;
	tee->hook_state = self->tee.hook_state;
	tee->hooked_player = self->tee.hooked_player;
	tee->active_weapon = self->tee.active_weapon;
	tee->is_new_hook = self->tee.is_new_hook;
	tee->jumped = self->tee.jumped;
	tee->jumped_total = self->tee.jumped_total;
	tee->jumps = self->tee.jumps;
	tee->direction = self->tee.direction;
	tee->angle = self->tee.angle;
	tee->triggered_events = self->tee.triggered_events;
	tee->id = self->tee.id;
	tee->is_reset = self->tee.is_reset;
	tee->last_vel = self->tee.last_vel;
	tee->colliding = self->tee.colliding;
	tee->is_left_wall = self->tee.is_left_wall;
	tee->is_solo = self->tee.is_solo;
	tee->is_jetpack = self->tee.is_jetpack;
	tee->is_collision_disabled = self->tee.is_collision_disabled;
	tee->is_endless_hook = self->tee.is_endless_hook;
	tee->is_endless_jump = self->tee.is_endless_jump;
	tee->is_hammer_hit_disabled = self->tee.is_hammer_hit_disabled;
	tee->is_grenade_hit_disabled = self->tee.is_grenade_hit_disabled;
	tee->is_laser_hit_disabled = self->tee.is_laser_hit_disabled;
	tee->is_shotgun_hit_disabled = self->tee.is_shotgun_hit_disabled;
	tee->is_hook_hit_disabled = self->tee.is_hook_hit_disabled;
	tee->is_super = self->tee.is_super;
	tee->has_telegun_gun = self->tee.has_telegun_gun;
	tee->has_telegun_grenade = self->tee.has_telegun_grenade;
	tee->has_telegun_laser = self->tee.has_telegun_laser;
	tee->freeze_start = self->tee.freeze_start;
	tee->freeze_end = self->tee.freeze_end;
	tee->is_in_freeze = self->tee.is_in_freeze;
	tee->is_deep_frozen = self->tee.is_deep_frozen;
	tee->is_live_frozen = self->tee.is_live_frozen;

	return (PyObject *) tee;
}

static PyGetSetDef Player_getseters[] = {
	{"use_custom_color", (getter) Player_getuse_custom_color, NULL, "use_custom_color (integer)", NULL},
	{"color_body", (getter) Player_getcolor_body, NULL, "color_body (integer)", NULL},
	{"color_feet", (getter) Player_getcolor_feet, NULL, "color_feet (integer)", NULL},
	{"name", (getter) Player_getname, NULL, "name (string)", NULL},
	{"clan", (getter) Player_getclan, NULL, "clan (string)", NULL},
	{"country", (getter) Player_getcountry, NULL, "country (integer)", NULL},
	{"skin_name", (getter) Player_getskin_name, NULL, "skin_name (string)", NULL},
	{"skin_color", (getter) Player_getskin_color, NULL, "skin_color (integer)", NULL},
	{"team", (getter) Player_getteam, NULL, "team (integer)", NULL},
	{"emoticon", (getter) Player_getemoticon, NULL, "emoticon (integer)", NULL},
	{"emoticon_start_fraction", (getter) Player_getemoticon_start_fraction, NULL, "emoticon_start_fraction (double)", NULL},
	{"emoticon_start_tick", (getter) Player_getemoticon_start_tick, NULL, "emoticon_start_tick (integer)", NULL},
	{"is_solo", (getter) Player_getis_solo, NULL, "is_solo (boolean)", NULL},
	{"is_jetpack", (getter) Player_getis_jetpack, NULL, "is_jetpack (boolean)", NULL},
	{"is_collision_disabled", (getter) Player_getis_collision_disabled, NULL, "is_collision_disabled (boolean)", NULL},
	{"is_endless_hook", (getter) Player_getis_endless_hook, NULL, "is_endless_hook (boolean)", NULL},
	{"is_endless_jump", (getter) Player_getis_endless_jump, NULL, "is_endless_jump (boolean)", NULL},
	{"is_hammer_hit_disabled", (getter) Player_getis_hammer_hit_disabled, NULL, "is_hammer_hit_disabled (boolean)", NULL},
	{"is_grenade_hit_disabled", (getter) Player_getis_grenade_hit_disabled, NULL, "is_grenade_hit_disabled (boolean)", NULL},
	{"is_laser_hit_disabled", (getter) Player_getis_laser_hit_disabled, NULL, "is_laser_hit_disabled (boolean)", NULL},
	{"is_shotgun_hit_disabled", (getter) Player_getis_shotgun_hit_disabled, NULL, "is_shotgun_hit_disabled (boolean)", NULL},
	{"is_hook_hit_disabled", (getter) Player_getis_hook_hit_disabled, NULL, "is_hook_hit_disabled (boolean)", NULL},
	{"is_super", (getter) Player_getis_super, NULL, "is_super (boolean)", NULL},
	{"has_telegun_gun", (getter) Player_gethas_telegun_gun, NULL, "has_telegun_gun (boolean)", NULL},
	{"has_telegun_grenade", (getter) Player_gethas_telegun_grenade, NULL, "has_telegun_grenade (boolean)", NULL},
	{"has_telegun_laser", (getter) Player_gethas_telegun_laser, NULL, "has_telegun_laser (boolean)", NULL},
	{"freeze_end", (getter) Player_getfreeze_end, NULL, "freeze_end (integer)", NULL},
	{"is_deep_frozen", (getter) Player_getis_deep_frozen, NULL, "is_deep_frozen (boolean)", NULL},
	{"is_live_frozen", (getter) Player_getis_live_frozen, NULL, "is_live_frozen (boolean)", NULL},
	{"angle", (getter) Player_getangle, NULL, "angle (double)", NULL},
	{"is_active", (getter) Player_getis_active, NULL, "is_active (boolean)", NULL},
	{"is_chat_ignore", (getter) Player_getis_chat_ignore, NULL, "is_chat_ignore (boolean)", NULL},
	{"is_emoticon_ignore", (getter) Player_getis_emoticon_ignore, NULL, "is_emoticon_ignore (boolean)", NULL},
	{"is_friend", (getter) Player_getis_friend, NULL, "is_friend (boolean)", NULL},
	{"is_foe", (getter) Player_getis_foe, NULL, "is_foe (boolean)", NULL},
	{"auth_level", (getter) Player_getauth_level, NULL, "auth_level (integer)", NULL},
	{"is_afk", (getter) Player_getis_afk, NULL, "is_afk (boolean)", NULL},
	{"is_paused", (getter) Player_getis_paused, NULL, "is_paused (boolean)", NULL},
	{"is_spec", (getter) Player_getis_spec, NULL, "is_spec (boolean)", NULL},
	{"render_pos", (getter) Player_getrender_pos, NULL, "render_pos (Vector2)", NULL},
	{"is_predicted", (getter) Player_getis_predicted, NULL, "is_predicted (boolean)", NULL},
	{"is_predicted_local", (getter) Player_getis_predicted_local, NULL, "is_predicted_local (boolean)", NULL},
	{"smooth_start", (getter) Player_getsmooth_start, NULL, "smooth_start (pair of long int)", NULL},
	{"smooth_len", (getter) Player_getsmooth_len, NULL, "smooth_len (pair of long int)", NULL},
	{"is_spec_char_present", (getter) Player_getis_spec_char_present, NULL, "is_spec_char_present (boolean)", NULL},
	{"spec_char", (getter) Player_getspec_char, NULL, "spec_char (Vector2)", NULL},
	{"tee", (getter) Player_gettee, NULL, "tee (Tee)", NULL},
	{NULL}  /* Sentinel */
};

static PyObject* Player_str(Player* self)
{
	char buf[4096];

	PyObject* name_str = PyUnicode_FromString(self->name);
	PyObject* clan_str = PyUnicode_FromString(self->clan);
	PyObject* skin_name_str = PyUnicode_FromString(self->skin_name);
	PyObject* render_pos_str_obj = Vector2_str(&self->render_pos);
	PyObject* spec_char_str_obj = Vector2_str(&self->spec_char);
	PyObject* tee_str_obj = Tee_str(&self->tee);

	const char *render_pos_str = PyUnicode_AsUTF8(render_pos_str_obj);
	const char *tee_str = PyUnicode_AsUTF8(tee_str_obj);
	const char *spec_char_str = PyUnicode_AsUTF8(spec_char_str_obj);

	if (!render_pos_str || !tee_str || !spec_char_str) {
		Py_XDECREF(render_pos_str_obj);
		Py_XDECREF(tee_str_obj);
		Py_XDECREF(spec_char_str_obj);
		return NULL;
	}

	sprintf(
		buf,
		"Player(\n"
		"use_custom_color: %d,\n"
		"color_body: %d,\n"
		"color_feet: %d,\n"
		"name: %s,\n"
		"clan: %s,\n"
		"country: %d,\n"
		"skin_name: %s,\n"
		"skin_color: %d,\n"
		"team: %d,\n"
		"emoticon: %d,\n"
		"emoticon_start_fraction: %.2f,\n"
		"emoticon_start_tick: %d,\n"
		"is_solo: %s,\n"
		"is_jetpack: %s,\n"
		"is_collision_disabled: %s,\n"
		"is_endless_hook: %s,\n"
		"is_endless_jump: %s,\n"
		"is_hammer_hit_disabled: %s,\n"
		"is_grenade_hit_disabled: %s,\n"
		"is_laser_hit_disabled: %s,\n"
		"is_shotgun_hit_disabled: %s,\n"
		"is_hook_hit_disabled: %s,\n"
		"is_super: %s,\n"
		"has_telegun_gun: %s,\n"
		"has_telegun_grenade: %s,\n"
		"has_telegun_laser: %s,\n"
		"freeze_end: %d,\n"
		"is_deep_frozen: %s,\n"
		"is_live_frozen: %s,\n"
		"angle: %.2f,\n"
		"is_active: %s,\n"
		"is_chat_ignore: %s,\n"
		"is_emoticon_ignore: %s,\n"
		"is_friend: %s,\n"
		"is_foe: %s,\n"
		"auth_level: %d,\n"
		"is_afk: %s,\n"
		"is_paused: %s,\n"
		"is_spec: %s,\n"
		"render_pos: %s,\n"
		"is_spec_char_present: %s,\n"
		"is_predicted: %s,\n"
		"is_predicted_local: %s,\n"
		"spec_char: %s,\n"
		"tee: %s\n"
		")",
		self->use_custom_color, self->color_body, self->color_feet, PyUnicode_AsUTF8(name_str), PyUnicode_AsUTF8(clan_str),
		self->country, PyUnicode_AsUTF8(skin_name_str), self->skin_color, self->team, self->emoticon,
		self->emoticon_start_fraction, self->emoticon_start_tick, self->is_solo ? "true" : "false",
		self->is_jetpack ? "true" : "false", self->is_collision_disabled ? "true" : "false",
		self->is_endless_hook ? "true" : "false", self->is_endless_jump ? "true" : "false",
		self->is_hammer_hit_disabled ? "true" : "false", self->is_grenade_hit_disabled ? "true" : "false",
		self->is_laser_hit_disabled ? "true" : "false", self->is_shotgun_hit_disabled ? "true" : "false",
		self->is_hook_hit_disabled ? "true" : "false", self->is_super ? "true" : "false",
		self->has_telegun_gun ? "true" : "false", self->has_telegun_grenade ? "true" : "false",
		self->has_telegun_laser ? "true" : "false", self->freeze_end, self->is_deep_frozen ? "true" : "false",
		self->is_live_frozen ? "true" : "false", self->angle, self->is_active ? "true" : "false",
		self->is_chat_ignore ? "true" : "false", self->is_emoticon_ignore ? "true" : "false",
		self->is_friend ? "true" : "false", self->is_foe ? "true" : "false", self->auth_level,
		self->is_afk ? "true" : "false", self->is_paused ? "true" : "false", self->is_spec ? "true" : "false",
		render_pos_str, self->is_spec_char_present ? "true" : "false", self->is_predicted ? "true" : "false",
		self->is_predicted_local ? "true" : "false", spec_char_str, tee_str
	);

	Py_DECREF(render_pos_str_obj);
	Py_DECREF(tee_str_obj);
	Py_DECREF(name_str);
	Py_DECREF(clan_str);
	Py_DECREF(skin_name_str);
	Py_DECREF(spec_char_str);

	return PyUnicode_FromString(buf);
}

inline PyTypeObject PlayerType = {
	{ PyObject_HEAD_INIT(NULL) 0, },
	"API.Player",                /* tp_name */
	sizeof(Player),              /* tp_basicsize */
	0,                            /* tp_itemsize */
	(destructor)Player_dealloc,  /* tp_dealloc */
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
	(reprfunc)Player_str,                            /* tp_str */
	0,                            /* tp_getattro */
	0,                            /* tp_setattro */
	0,                            /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,           /* tp_flags */
	"Player",           /* tp_doc */
	0,                            /* tp_traverse */
	0,                            /* tp_clear */
	0,                            /* tp_richcompare */
	0,                            /* tp_weaklistoffset */
	0,                            /* tp_iter */
	0,                            /* tp_iternext */
	Player_methods,              /* tp_methods */
	0,                            /* tp_members */
	Player_getseters,                            /* tp_getset */
	0,                            /* tp_base */
	0,                            /* tp_dict */
	0,                            /* tp_descr_get */
	0,                            /* tp_descr_set */
	0,                            /* tp_dictoffset */
	(initproc)Player_init,      /* tp_init */
	0,                            /* tp_alloc */
	PyType_GenericNew,            /* tp_new */
};

#endif // DDNET_API_PLAYER_H

