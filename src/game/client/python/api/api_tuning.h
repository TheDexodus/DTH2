#ifndef API_TUNING_H
#define API_TUNING_H

#include "api.h"

static PyObject* API_Tuning_getGroundControlSpeed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundControlSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGroundControlAccel(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundControlAccel;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGroundFriction(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundFriction;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGroundJumpImpulse(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundJumpImpulse;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getAirJumpImpulse(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_AirJumpImpulse;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getAirControlSpeed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_AirControlSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getAirControlAccel(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_AirControlAccel;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getAirFriction(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_AirFriction;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getHookLength(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HookLength;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getHookFireSpeed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HookFireSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getHookDragAccel(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HookDragAccel;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getHookDragSpeed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HookDragSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGravity(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_Gravity;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getVelrampStart(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_VelrampStart;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getVelrampRange(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_VelrampRange;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getVelrampCurvature(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_VelrampCurvature;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGunCurvature(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GunCurvature;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGunSpeed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GunSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGunLifetime(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GunLifetime;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getShotgunCurvature(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunCurvature;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getShotgunSpeed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getShotgunSpeeddiff(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunSpeeddiff;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getShotgunLifetime(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunLifetime;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGrenadeCurvature(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GrenadeCurvature;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGrenadeSpeed(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GrenadeSpeed;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGrenadeLifetime(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GrenadeLifetime;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getLaserReach(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserReach;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getLaserBounceDelay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserBounceDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getLaserBounceNum(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserBounceNum;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getLaserBounceCost(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserBounceCost;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getLaserDamage(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserDamage;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getPlayerCollision(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_PlayerCollision;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getPlayerHooking(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_PlayerHooking;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getJetpackStrength(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_JetpackStrength;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getShotgunStrength(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunStrength;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getExplosionStrength(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ExplosionStrength;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getHammerStrength(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HammerStrength;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getHookDuration(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HookDuration;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getHammerFireDelay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HammerFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGunFireDelay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GunFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getShotgunFireDelay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_ShotgunFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGrenadeFireDelay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GrenadeFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getLaserFireDelay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_LaserFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getNinjaFireDelay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_NinjaFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getHammerHitFireDelay(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_HammerHitFireDelay;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGroundElasticityX(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundElasticityX;

	return Py_BuildValue("f", value);
}

static PyObject* API_Tuning_getGroundElasticityY(PyObject* self, PyObject* args)
{
	float value = PythonAPI_GameClient->GetTuning(0)->m_GroundElasticityY;

	return Py_BuildValue("f", value);
}


static PyMethodDef API_TuningMethods[] = {
	{"getGroundControlSpeed", API_Tuning_getGroundControlSpeed, METH_VARARGS, "Return tuning result"},
	{"getGroundControlAccel", API_Tuning_getGroundControlAccel, METH_VARARGS, "Return tuning result"},
	{"getGroundFriction", API_Tuning_getGroundFriction, METH_VARARGS, "Return tuning result"},
	{"getGroundJumpImpulse", API_Tuning_getGroundJumpImpulse, METH_VARARGS, "Return tuning result"},
	{"getAirJumpImpulse", API_Tuning_getAirJumpImpulse, METH_VARARGS, "Return tuning result"},
	{"getAirControlSpeed", API_Tuning_getAirControlSpeed, METH_VARARGS, "Return tuning result"},
	{"getAirControlAccel", API_Tuning_getAirControlAccel, METH_VARARGS, "Return tuning result"},
	{"getAirFriction", API_Tuning_getAirFriction, METH_VARARGS, "Return tuning result"},
	{"getHookLength", API_Tuning_getHookLength, METH_VARARGS, "Return tuning result"},
	{"getHookFireSpeed", API_Tuning_getHookFireSpeed, METH_VARARGS, "Return tuning result"},
	{"getHookDragAccel", API_Tuning_getHookDragAccel, METH_VARARGS, "Return tuning result"},
	{"getHookDragSpeed", API_Tuning_getHookDragSpeed, METH_VARARGS, "Return tuning result"},
	{"getGravity", API_Tuning_getGravity, METH_VARARGS, "Return tuning result"},
	{"getVelrampStart", API_Tuning_getVelrampStart, METH_VARARGS, "Return tuning result"},
	{"getVelrampRange", API_Tuning_getVelrampRange, METH_VARARGS, "Return tuning result"},
	{"getVelrampCurvature", API_Tuning_getVelrampCurvature, METH_VARARGS, "Return tuning result"},
	{"getGunCurvature", API_Tuning_getGunCurvature, METH_VARARGS, "Return tuning result"},
	{"getGunSpeed", API_Tuning_getGunSpeed, METH_VARARGS, "Return tuning result"},
	{"getGunLifetime", API_Tuning_getGunLifetime, METH_VARARGS, "Return tuning result"},
	{"getShotgunCurvature", API_Tuning_getShotgunCurvature, METH_VARARGS, "Return tuning result"},
	{"getShotgunSpeed", API_Tuning_getShotgunSpeed, METH_VARARGS, "Return tuning result"},
	{"getShotgunSpeeddiff", API_Tuning_getShotgunSpeeddiff, METH_VARARGS, "Return tuning result"},
	{"getShotgunLifetime", API_Tuning_getShotgunLifetime, METH_VARARGS, "Return tuning result"},
	{"getGrenadeCurvature", API_Tuning_getGrenadeCurvature, METH_VARARGS, "Return tuning result"},
	{"getGrenadeSpeed", API_Tuning_getGrenadeSpeed, METH_VARARGS, "Return tuning result"},
	{"getGrenadeLifetime", API_Tuning_getGrenadeLifetime, METH_VARARGS, "Return tuning result"},
	{"getLaserReach", API_Tuning_getLaserReach, METH_VARARGS, "Return tuning result"},
	{"getLaserBounceDelay", API_Tuning_getLaserBounceDelay, METH_VARARGS, "Return tuning result"},
	{"getLaserBounceNum", API_Tuning_getLaserBounceNum, METH_VARARGS, "Return tuning result"},
	{"getLaserBounceCost", API_Tuning_getLaserBounceCost, METH_VARARGS, "Return tuning result"},
	{"getLaserDamage", API_Tuning_getLaserDamage, METH_VARARGS, "Return tuning result"},
	{"getPlayerCollision", API_Tuning_getPlayerCollision, METH_VARARGS, "Return tuning result"},
	{"getPlayerHooking", API_Tuning_getPlayerHooking, METH_VARARGS, "Return tuning result"},
	{"getJetpackStrength", API_Tuning_getJetpackStrength, METH_VARARGS, "Return tuning result"},
	{"getShotgunStrength", API_Tuning_getShotgunStrength, METH_VARARGS, "Return tuning result"},
	{"getExplosionStrength", API_Tuning_getExplosionStrength, METH_VARARGS, "Return tuning result"},
	{"getHammerStrength", API_Tuning_getHammerStrength, METH_VARARGS, "Return tuning result"},
	{"getHookDuration", API_Tuning_getHookDuration, METH_VARARGS, "Return tuning result"},
	{"getHammerFireDelay", API_Tuning_getHammerFireDelay, METH_VARARGS, "Return tuning result"},
	{"getGunFireDelay", API_Tuning_getGunFireDelay, METH_VARARGS, "Return tuning result"},
	{"getShotgunFireDelay", API_Tuning_getShotgunFireDelay, METH_VARARGS, "Return tuning result"},
	{"getGrenadeFireDelay", API_Tuning_getGrenadeFireDelay, METH_VARARGS, "Return tuning result"},
	{"getLaserFireDelay", API_Tuning_getLaserFireDelay, METH_VARARGS, "Return tuning result"},
	{"getNinjaFireDelay", API_Tuning_getNinjaFireDelay, METH_VARARGS, "Return tuning result"},
	{"getHammerHitFireDelay", API_Tuning_getHammerHitFireDelay, METH_VARARGS, "Return tuning result"},
	{"getGroundElasticityX", API_Tuning_getGroundElasticityX, METH_VARARGS, "Return tuning result"},
	{"getGroundElasticityY", API_Tuning_getGroundElasticityY, METH_VARARGS, "Return tuning result"},
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
