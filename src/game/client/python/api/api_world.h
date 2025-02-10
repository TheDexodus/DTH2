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
	player->useCustomColor = clientData.m_UseCustomColor;
	player->colorBody = clientData.m_ColorBody;
	player->colorFeet = clientData.m_ColorFeet;

	strcpy(player->name, clientData.m_aName);
	strcpy(player->clan, clientData.m_aClan);
	player->country = clientData.m_Country;
	strcpy(player->skinName, clientData.m_aSkinName);
	player->skinColor = clientData.m_SkinColor;
	player->team = character->Team();
	player->emoticon = clientData.m_Emoticon;
	player->emoticonStartFraction = clientData.m_EmoticonStartFraction;
	player->emoticonStartTick = clientData.m_EmoticonStartTick;
	player->isSolo = clientData.m_Solo;
	player->isJetpack = clientData.m_Jetpack;
	player->isCollisionDisabled = clientData.m_CollisionDisabled;
	player->isEndlessHook = clientData.m_EndlessHook;
	player->isEndlessJump = clientData.m_EndlessJump;
	player->isHammerHitDisabled = clientData.m_HammerHitDisabled;
	player->isGrenadeHitDisabled = clientData.m_GrenadeHitDisabled;
	player->isLaserHitDisabled = clientData.m_LaserHitDisabled;
	player->isShotgunHitDisabled = clientData.m_ShotgunHitDisabled;
	player->isHookHitDisabled = clientData.m_HookHitDisabled;
	player->isSuper = clientData.m_Super;
	player->isHasTelegunGun = clientData.m_HasTelegunGun;
	player->isHasTelegunGrenade = clientData.m_HasTelegunGrenade;
	player->isHasTelegunLaser = clientData.m_HasTelegunLaser;
	player->freezeEnd = clientData.m_FreezeEnd;
	player->isDeepFrozen = clientData.m_DeepFrozen;
	player->isLiveFrozen = clientData.m_LiveFrozen;
	player->angle = clientData.m_Angle;
	player->isActive = clientData.m_Active;
	player->isChatIgnore = clientData.m_ChatIgnore;
	player->isEmoticonIgnore = clientData.m_EmoticonIgnore;
	player->isFriend = clientData.m_Friend;
	player->isFoe = clientData.m_Foe;

	player->authLevel = clientData.m_AuthLevel;
	player->isAfk = clientData.m_Afk;
	player->isPaused = clientData.m_Paused;
	player->isSpec = clientData.m_Spec;

	player->renderPos = Vector2(clientData.m_RenderPos);
	player->isPredicted = clientData.m_IsPredicted;
	player->isPredictedLocal = clientData.m_IsPredictedLocal;
	player->smoothStart[0] = clientData.m_aSmoothStart[0];
	player->smoothStart[1] = clientData.m_aSmoothStart[1];
	player->smoothLen[0] = clientData.m_aSmoothLen[0];
	player->smoothLen[1] = clientData.m_aSmoothLen[1];
	player->isSpecCharPresent = clientData.m_SpecCharPresent;
	player->specChar = clientData.m_SpecChar;

	player->tee.pos = Vector2(character->Core()->m_Pos);
	player->tee.vel = Vector2(character->Core()->m_Vel);
	player->tee.hookPos = Vector2(character->Core()->m_HookPos);
	player->tee.hookDir = Vector2(character->Core()->m_HookDir);
	player->tee.hookTeleBase = Vector2(character->Core()->m_HookTeleBase);
	player->tee.hookTick = character->Core()->m_HookTick;
	player->tee.hookState = character->Core()->m_HookState;
	player->tee.activeWeapon = character->Core()->m_ActiveWeapon;
	player->tee.isNewHook = character->Core()->m_NewHook;
	player->tee.jumped = character->Core()->m_Jumped;
	player->tee.jumpedTotal = character->Core()->m_JumpedTotal;
	player->tee.jumps = character->Core()->m_Jumps;
	player->tee.direction = character->Core()->m_Direction;
	player->tee.angle = character->Core()->m_Angle;
	player->tee.triggeredEvents = character->Core()->m_TriggeredEvents;
	player->tee.id = character->Core()->m_Id;
	player->tee.isReset = character->Core()->m_Reset;
	player->tee.colliding = character->Core()->m_Colliding;
	player->tee.isLeftWall = character->Core()->m_LeftWall;
	player->tee.isSolo = character->Core()->m_Solo;
	player->tee.isJetpack = character->Core()->m_Jetpack;
	player->tee.isCollisionDisabled = character->Core()->m_CollisionDisabled;
	player->tee.isEndlessHook = character->Core()->m_EndlessHook;
	player->tee.isEndlessJump = character->Core()->m_EndlessJump;
	player->tee.isHammerHitDisabled = character->Core()->m_HammerHitDisabled;
	player->tee.isGrenadeHitDisabled = character->Core()->m_GrenadeHitDisabled;
	player->tee.isLaserHitDisabled = character->Core()->m_LaserHitDisabled;
	player->tee.isShotgunHitDisabled = character->Core()->m_ShotgunHitDisabled;
	player->tee.isHookHitDisabled = character->Core()->m_HookHitDisabled;
	player->tee.isSuper = character->Core()->m_Super;
	player->tee.hasTelegunGun = character->Core()->m_HasTelegunGun;
	player->tee.hasTelegunGrenade = character->Core()->m_HasTelegunGrenade;
	player->tee.hasTelegunLaser = character->Core()->m_HasTelegunLaser;
	player->tee.freezeStart = character->Core()->m_FreezeStart;
	player->tee.freezeEnd = character->Core()->m_FreezeEnd;
	player->tee.isInFreeze = character->Core()->m_IsInFreeze;
	player->tee.isDeepFrozen = character->Core()->m_DeepFrozen;
	player->tee.isLiveFrozen = character->Core()->m_LiveFrozen;

	player->tee.inputDirection = character->Core()->m_Input.m_Direction;
	player->tee.inputTarget = Vector2(vec2(character->Core()->m_Input.m_TargetX, character->Core()->m_Input.m_TargetY));
	player->tee.inputJump = character->Core()->m_Input.m_Jump;
	player->tee.inputFire = character->Core()->m_Input.m_Fire;
	player->tee.inputHook = character->Core()->m_Input.m_Hook;
	player->tee.inputPlayerFlags = character->Core()->m_Input.m_PlayerFlags;
	player->tee.inputWantedWeapon = character->Core()->m_Input.m_WantedWeapon;
	player->tee.inputNextWeapon = character->Core()->m_Input.m_NextWeapon;
	player->tee.inputPrevWeapon = character->Core()->m_Input.m_PrevWeapon;

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
	{"getGameTick", (PyCFunction)World_GetGameTick, METH_VARARGS, "Get world game tick"},
	{"predict", (PyCFunction)World_predict, METH_VARARGS, "Predict world on ticks"},
	{"getPlayer", (PyCFunction)World_getPlayer, METH_VARARGS, "Get player from this world"},
	{"inputMove", (PyCFunction)World_InputMove, METH_VARARGS, "No doc"},
	{"inputSetTarget", (PyCFunction)World_InputSetTarget, METH_VARARGS, "No doc"},
	{"inputJump", (PyCFunction)World_InputJump, METH_VARARGS, "No doc"},
	{"inputFire", (PyCFunction)World_InputFire, METH_VARARGS, "No doc"},
	{"inputHook", (PyCFunction)World_InputHook, METH_VARARGS, "No doc"},
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
