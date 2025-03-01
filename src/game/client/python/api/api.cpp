#include "api.h"
#include "api_player.h"
#include "api_vector2.h"
#include "api_tee.h"
#include "api_collision.h"
#include "api_console.h"
#include "api_input.h"
#include "api_dummy_input.h"
#include "api_time.h"
#include "api_predict.h"
#include "api_character.h"
#include "api_graphics.h"
#include "api_tuning.h"
#include "api_chat.h"
#include "api_world.h"
#include "game/client/gameclient.h"
#include <thread>

// ============ API Module ============ //

static PyObject* API_LocalID(PyObject* self, PyObject* args) {
	int clientId;
	PyArg_ParseTuple(args, "i", &clientId);

	if(clientId == -1)
		clientId = g_Config.m_ClDummy;
	else if (clientId == -2)
		clientId = (g_Config.m_ClDummy + 1) % 2;

	auto localId = PythonAPI_GameClient->m_aLocalIds[clientId];

	PyObject* localIdObject = PyLong_FromLong(localId);

	return localIdObject;
}

// Таймер-обертка для вызова callback через заданное время
static PyObject* API_Timeout(PyObject* self, PyObject* args) {
	double timeout;
	PyObject* pyCallback;

	// Разбор аргументов: ожидание в секундах (float) и callback (callable)
	if (!PyArg_ParseTuple(args, "dO:Timeout", &timeout, &pyCallback)) {
		return NULL;
	}

	// Проверка, что передан callback
	if (!PyCallable_Check(pyCallback)) {
		PyErr_SetString(PyExc_TypeError, "Expected a callable as the second argument");
		return NULL;
	}

	// Увеличиваем ссылку на callback, чтобы Python не удалил его
	Py_INCREF(pyCallback);

	// Создаём новый поток для таймера
	std::thread([timeout, pyCallback]() {
	    // Задержка
	    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(timeout * 1000)));

	    // Выполняем callback в главном потоке Python
	    PyGILState_STATE gilState = PyGILState_Ensure(); // Захватываем GIL
	    PyObject* result = PyObject_CallObject(pyCallback, NULL); // Вызываем callback

	    if (!result) {
		PyErr_Print(); // Печатаем ошибку, если вызов не удался
	    } else {
		Py_DECREF(result); // Уменьшаем ссылку на результат
	    }

	    Py_DECREF(pyCallback); // Уменьшаем ссылку на callback
	    PyGILState_Release(gilState); // Освобождаем GIL
	}).detach(); // Отсоединяем поток

	Py_RETURN_NONE;
}

static PyMethodDef APIMethods[] = {
	{"LocalID", API_LocalID, METH_VARARGS, "Get Local ID"},
	{"Timeout", API_Timeout, METH_VARARGS, "Call a function after a timeout"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef API = {
	PyModuleDef_HEAD_INIT,
	"API",
	NULL,
	-1,
	APIMethods
};

PyMODINIT_FUNC PyInit_API(void) {
	PyObject* APIModule = PyModule_Create(&API);
	PyModule_AddObject(APIModule, "Console", PyInit_API_Console());
	PyModule_AddObject(APIModule, "Input", PyInit_API_Input());
	PyModule_AddObject(APIModule, "DummyInput", PyInit_API_DummyInput());
	PyModule_AddObject(APIModule, "Collision", PyInit_API_Collision());
	PyModule_AddObject(APIModule, "Time", PyInit_API_Time());
	PyModule_AddObject(APIModule, "Predict", PyInit_API_Predict());
	PyModule_AddObject(APIModule, "Graphics", PyInit_API_Graphics());
	PyModule_AddObject(APIModule, "Tuning", PyInit_API_Tuning());
	PyModule_AddObject(APIModule, "Chat", PyInit_API_Chat());

	while (PyType_Ready(&Vector2Type) < 0 || PyType_Ready(&PlayerType) < 0 || PyType_Ready(&TeeType) < 0 || PyType_Ready(&CharacterType) < 0 || PyType_Ready(&WorldType) < 0)
	{
	}

	PyModule_AddObject(APIModule, "Vector2", (PyObject *)&Vector2Type);
	PyModule_AddObject(APIModule, "Player", (PyObject *)&PlayerType);
	PyModule_AddObject(APIModule, "Tee", (PyObject *)&TeeType);
	PyModule_AddObject(APIModule, "Character", (PyObject *)&CharacterType);
	PyModule_AddObject(APIModule, "World", (PyObject *)&WorldType);

	return APIModule;
}