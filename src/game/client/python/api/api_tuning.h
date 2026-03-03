#ifndef API_TUNING_H
#define API_TUNING_H

#include "api.h"

static PyObject* API_Tuning_get_ground_control_speed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundControlSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_ground_control_accel(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundControlAccel;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_ground_friction(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundFriction;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_ground_jump_impulse(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundJumpImpulse;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_air_jump_impulse(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_AirJumpImpulse;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_air_control_speed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_AirControlSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_air_control_accel(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_AirControlAccel;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_air_friction(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_AirFriction;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_hook_length(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HookLength;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_hook_fire_speed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HookFireSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_hook_drag_accel(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HookDragAccel;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_hook_drag_speed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HookDragSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_gravity(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_Gravity;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_velramp_start(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_VelrampStart;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_velramp_range(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_VelrampRange;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_velramp_curvature(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_VelrampCurvature;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_gun_curvature(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GunCurvature;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_gun_speed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GunSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_gun_lifetime(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GunLifetime;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_shotgun_curvature(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunCurvature;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_shotgun_speed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_shotgun_speeddiff(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunSpeeddiff;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_shotgun_lifetime(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunLifetime;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_grenade_curvature(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GrenadeCurvature;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_grenade_speed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GrenadeSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_grenade_lifetime(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GrenadeLifetime;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_laser_reach(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserReach;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_laser_bounce_delay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserBounceDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_laser_bounce_num(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserBounceNum;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_laser_bounce_cost(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserBounceCost;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_laser_damage(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserDamage;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_player_collision(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_PlayerCollision;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_player_hooking(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_PlayerHooking;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_jetpack_strength(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_JetpackStrength;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_shotgun_strength(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunStrength;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_explosion_strength(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ExplosionStrength;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_hammer_strength(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HammerStrength;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_hook_duration(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HookDuration;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_hammer_fire_delay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HammerFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_gun_fire_delay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GunFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_shotgun_fire_delay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_grenade_fire_delay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GrenadeFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_laser_fire_delay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_ninja_fire_delay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_NinjaFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_hammer_hit_fire_delay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HammerHitFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_ground_elasticity_x(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundElasticityX;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_get_ground_elasticity_y(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundElasticityY;

	return Py_BuildValue("f", value);
}


static PyMethodDef API_TuningMethods[] = {
	{"get_ground_control_speed", API_Tuning_get_ground_control_speed, METH_VARARGS, "Return tuning result"},
	{"get_ground_control_accel", API_Tuning_get_ground_control_accel, METH_VARARGS, "Return tuning result"},
	{"get_ground_friction", API_Tuning_get_ground_friction, METH_VARARGS, "Return tuning result"},
	{"get_ground_jump_impulse", API_Tuning_get_ground_jump_impulse, METH_VARARGS, "Return tuning result"},
	{"get_air_jump_impulse", API_Tuning_get_air_jump_impulse, METH_VARARGS, "Return tuning result"},
	{"get_air_control_speed", API_Tuning_get_air_control_speed, METH_VARARGS, "Return tuning result"},
	{"get_air_control_accel", API_Tuning_get_air_control_accel, METH_VARARGS, "Return tuning result"},
	{"get_air_friction", API_Tuning_get_air_friction, METH_VARARGS, "Return tuning result"},
	{"get_hook_length", API_Tuning_get_hook_length, METH_VARARGS, "Return tuning result"},
	{"get_hook_fire_speed", API_Tuning_get_hook_fire_speed, METH_VARARGS, "Return tuning result"},
	{"get_hook_drag_accel", API_Tuning_get_hook_drag_accel, METH_VARARGS, "Return tuning result"},
	{"get_hook_drag_speed", API_Tuning_get_hook_drag_speed, METH_VARARGS, "Return tuning result"},
	{"get_gravity", API_Tuning_get_gravity, METH_VARARGS, "Return tuning result"},
	{"get_velramp_start", API_Tuning_get_velramp_start, METH_VARARGS, "Return tuning result"},
	{"get_velramp_range", API_Tuning_get_velramp_range, METH_VARARGS, "Return tuning result"},
	{"get_velramp_curvature", API_Tuning_get_velramp_curvature, METH_VARARGS, "Return tuning result"},
	{"get_gun_curvature", API_Tuning_get_gun_curvature, METH_VARARGS, "Return tuning result"},
	{"get_gun_speed", API_Tuning_get_gun_speed, METH_VARARGS, "Return tuning result"},
	{"get_gun_lifetime", API_Tuning_get_gun_lifetime, METH_VARARGS, "Return tuning result"},
	{"get_shotgun_curvature", API_Tuning_get_shotgun_curvature, METH_VARARGS, "Return tuning result"},
	{"get_shotgun_speed", API_Tuning_get_shotgun_speed, METH_VARARGS, "Return tuning result"},
	{"get_shotgun_speeddiff", API_Tuning_get_shotgun_speeddiff, METH_VARARGS, "Return tuning result"},
	{"get_shotgun_lifetime", API_Tuning_get_shotgun_lifetime, METH_VARARGS, "Return tuning result"},
	{"get_grenade_curvature", API_Tuning_get_grenade_curvature, METH_VARARGS, "Return tuning result"},
	{"get_grenade_speed", API_Tuning_get_grenade_speed, METH_VARARGS, "Return tuning result"},
	{"get_grenade_lifetime", API_Tuning_get_grenade_lifetime, METH_VARARGS, "Return tuning result"},
	{"get_laser_reach", API_Tuning_get_laser_reach, METH_VARARGS, "Return tuning result"},
	{"get_laser_bounce_delay", API_Tuning_get_laser_bounce_delay, METH_VARARGS, "Return tuning result"},
	{"get_laser_bounce_num", API_Tuning_get_laser_bounce_num, METH_VARARGS, "Return tuning result"},
	{"get_laser_bounce_cost", API_Tuning_get_laser_bounce_cost, METH_VARARGS, "Return tuning result"},
	{"get_laser_damage", API_Tuning_get_laser_damage, METH_VARARGS, "Return tuning result"},
	{"get_player_collision", API_Tuning_get_player_collision, METH_VARARGS, "Return tuning result"},
	{"get_player_hooking", API_Tuning_get_player_hooking, METH_VARARGS, "Return tuning result"},
	{"get_jetpack_strength", API_Tuning_get_jetpack_strength, METH_VARARGS, "Return tuning result"},
	{"get_shotgun_strength", API_Tuning_get_shotgun_strength, METH_VARARGS, "Return tuning result"},
	{"get_explosion_strength", API_Tuning_get_explosion_strength, METH_VARARGS, "Return tuning result"},
	{"get_hammer_strength", API_Tuning_get_hammer_strength, METH_VARARGS, "Return tuning result"},
	{"get_hook_duration", API_Tuning_get_hook_duration, METH_VARARGS, "Return tuning result"},
	{"get_hammer_fire_delay", API_Tuning_get_hammer_fire_delay, METH_VARARGS, "Return tuning result"},
	{"get_gun_fire_delay", API_Tuning_get_gun_fire_delay, METH_VARARGS, "Return tuning result"},
	{"get_shotgun_fire_delay", API_Tuning_get_shotgun_fire_delay, METH_VARARGS, "Return tuning result"},
	{"get_grenade_fire_delay", API_Tuning_get_grenade_fire_delay, METH_VARARGS, "Return tuning result"},
	{"get_laser_fire_delay", API_Tuning_get_laser_fire_delay, METH_VARARGS, "Return tuning result"},
	{"get_ninja_fire_delay", API_Tuning_get_ninja_fire_delay, METH_VARARGS, "Return tuning result"},
	{"get_hammer_hit_fire_delay", API_Tuning_get_hammer_hit_fire_delay, METH_VARARGS, "Return tuning result"},
	{"get_ground_elasticity_x", API_Tuning_get_ground_elasticity_x, METH_VARARGS, "Return tuning result"},
	{"get_ground_elasticity_y", API_Tuning_get_ground_elasticity_y, METH_VARARGS, "Return tuning result"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef API_TuningModule = {
	PyModuleDef_HEAD_INIT,
	"API.Tuning",
	NULL,
	-1,
	API_TuningMethods
};

PyMODINIT_FUNC PyInit_API_Tuning(void)
{
	return PyModule_Create(&API_TuningModule);
}

#endif //API_TUNING_H
