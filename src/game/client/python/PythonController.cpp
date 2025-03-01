#include "PythonController.h"
#include "Python.h"
#include "game/client/python/api/api.h"

PythonController::PythonController()
{
	PyImport_AppendInittab("API", &PyInit_API);
	Py_Initialize();
}

void PythonController::AutoloadAdd(PythonScript *pythonScript)
{
	if(this->isScriptAutoloading(pythonScript))
		return;

	this->autoLoadPythonScripts.push_back(pythonScript);
}

void PythonController::AutoloadRemove(PythonScript *pythonScript)
{
	for(auto i = this->autoLoadPythonScripts.begin(); i != this->autoLoadPythonScripts.end(); i++)
	{
		auto autoLoadScript = *i;
		if(autoLoadScript->filepath == pythonScript->filepath)
		{
			this->autoLoadPythonScripts.erase(i);
			return;
		}
	}
}

bool PythonController::isScriptAutoloading(PythonScript *pythonScript)
{
	for(auto PythonScript : this->autoLoadPythonScripts)
	{
		if(PythonScript->filepath == pythonScript->filepath)
			return true;
	}
	return false;
}
bool PythonController::OnChatMessage(int MsgType, void *pRawMsg)
{
	CNetMsg_Sv_Chat *pMsg = (CNetMsg_Sv_Chat *)pRawMsg;

	for (auto ExecutedPythonScript : this->executedPythonScripts) {
		if (!PyObject_HasAttrString(ExecutedPythonScript->module, "onMessage")) {
			ExecutedPythonScript->updateExceptions();
			continue;
		}

		PyObject* Function = PyObject_GetAttrString(ExecutedPythonScript->module, "onMessage");

		if (Function != nullptr && PyCallable_Check(Function)) {
			GameClient()->pythonRender.SetScriptRender(ExecutedPythonScript->filepath);
			PyObject* KeyCodeObject = PyLong_FromLong(pMsg->m_ClientId);
			PyObject* KeyFlagsObject = PyLong_FromLong( pMsg->m_Team);
			PyObject* KeyNameObject = PyUnicode_DecodeUTF8(string(pMsg->m_pMessage).c_str(), string(pMsg->m_pMessage).size(), "strict");
			PyObject* args = PyTuple_Pack(3, KeyCodeObject, KeyFlagsObject, KeyNameObject);
			PyObject* result = PyObject_CallObject(Function, args);

			PyOS_InterruptOccurred();
			Py_XDECREF(args);
			Py_XDECREF(KeyCodeObject);
			Py_XDECREF(KeyFlagsObject);
			Py_XDECREF(KeyNameObject);
		} else {
			PyErr_Clear();
		}

		ExecutedPythonScript->updateExceptions();
	}

	return false;
}

void PythonController::StartExecuteScript(PythonScript* pythonScript)
{
	if(m_pClient->m_Snap.m_pLocalInfo)
	{
		CServerInfo CurrentServerInfo;
		Client()->GetServerInfo(&CurrentServerInfo);

		for (auto pythonIgnoreServer : GameClient()->user.userData.pythonBlacklistIp)
		{
			if (pythonIgnoreServer == string(CurrentServerInfo.m_aAddress))
			{
				return;
			}
		}
	}

	GameClient()->pythonRender.SetScriptRender(pythonScript->filepath);
	ResetInput();

	pythonScript->init();

	if (!pythonScript->isInitialized() || this->isExecutedScript(pythonScript)) {
		return;
	}

	PyObject* function = nullptr;

	if (PyObject_HasAttrString(pythonScript->module, "onScriptStarted")) {
		function = PyObject_GetAttrString(pythonScript->module, "onScriptStarted");
	} else {
		pythonScript->updateExceptions();
	}

	if (function != nullptr && PyCallable_Check(function)) {
		PyObject* args = PyTuple_Pack(0);
		PyObject_CallObject(function, args);
		Py_XDECREF(args);
	} else {
		PyErr_Clear();
	}

	pythonScript->updateExceptions();

	this->executedPythonScripts.push_back(pythonScript);
}

void PythonController::StopExecuteScript(PythonScript* pythonScript)
{
	GameClient()->pythonRender.ResetScriptObjects(pythonScript->filepath);
	pythonScript->fileExceptions = vector<string>(0);

	for (auto iterator = this->executedPythonScripts.begin(); iterator != this->executedPythonScripts.end(); iterator++) {
		auto executedPythonScript = *iterator;

		if (executedPythonScript->filepath == pythonScript->filepath) {
			this->executedPythonScripts.erase(iterator);

			PyObject* function = nullptr;

			if (PyObject_HasAttrString(executedPythonScript->module, "onScriptStopped")) {
				function = PyObject_GetAttrString(pythonScript->module, "onScriptStopped");
			} else {
				executedPythonScript->updateExceptions();
			}

			if (function != nullptr && PyCallable_Check(function)) {
				PyObject* args = PyTuple_Pack(0);
				PyObject_CallObject(function, args);
				PyOS_InterruptOccurred();
				Py_XDECREF(args);
			} else {
				PyErr_Clear();
			}

			pythonScript->updateExceptions();
			ResetInput();

			return;
		}
	}
}

bool PythonController::OnInput(const IInput::CEvent &Event)
{
	std::string KeyName = this->m_pClient->Input()->KeyName(Event.m_Key);
	bool NeedBrakeInput = false;

	for (auto ExecutedPythonScript : this->executedPythonScripts) {
		if (!PyObject_HasAttrString(ExecutedPythonScript->module, "onInput")) {
			ExecutedPythonScript->updateExceptions();
			continue;
		}

		PyObject* Function = PyObject_GetAttrString(ExecutedPythonScript->module, "onInput");

		if (Function != nullptr && PyCallable_Check(Function)) {
			GameClient()->pythonRender.SetScriptRender(ExecutedPythonScript->filepath);
			PyObject* KeyCodeObject = PyLong_FromLong(Event.m_Key);
			PyObject* KeyFlagsObject = PyLong_FromLong(Event.m_Flags);
			PyObject* KeyNameObject = PyUnicode_DecodeUTF8(KeyName.c_str(), KeyName.size(), "strict");
			PyObject* args = PyTuple_Pack(3, KeyCodeObject, KeyFlagsObject, KeyNameObject);
			PyObject* result = PyObject_CallObject(Function, args);

			if (result != nullptr && PyObject_IsTrue(result)) {
				NeedBrakeInput = true;
			}

			PyOS_InterruptOccurred();
			Py_XDECREF(args);
			Py_XDECREF(KeyCodeObject);
			Py_XDECREF(KeyFlagsObject);
			Py_XDECREF(KeyNameObject);
		} else {
			PyErr_Clear();
		}

		ExecutedPythonScript->updateExceptions();
	}

	return NeedBrakeInput;
}

bool PythonController::isExecutedScript(PythonScript *pythonScript)
{
	for (auto ExecutedPythonScript : this->executedPythonScripts) {
		if (ExecutedPythonScript->filepath == pythonScript->filepath) {
			return true;
		}
	}

	return false;
}

bool PythonController::needForceInput(int inputId)
{
	if (inputId != g_Config.m_ClDummy) {
		return enableDummyControl;
	}

	return this->inputs[inputId].m_Direction != 0 ||
	       this->inputs[inputId].m_Jump != 0 ||
	       this->inputs[inputId].m_Hook != 0 ||
	       this->inputs[inputId].m_Fire != 0 ||
	       this->inputs[inputId].m_TargetX != 0 ||
	       this->inputs[inputId].m_TargetY != 0 ||
	       this->inputs[inputId].m_WantedWeapon != -1 ||
		blockUserInput;
}

int PythonController::SnapInput(int *pData, int inputId)
{
	int SizeData;
	CNetObj_PlayerInput Input;

	if (inputId == g_Config.m_ClDummy) {
		SizeData = this->m_pClient->m_Controls.SnapInput(pData);
		mem_copy(&Input, pData, SizeData);
	} else {
		Input.m_Direction = 0;
		Input.m_TargetX = 0;
		Input.m_TargetY = 0;
		Input.m_Jump = 0;
		Input.m_Fire = 0;
		Input.m_Hook = 0;
		Input.m_PlayerFlags = 0;
		Input.m_WantedWeapon = 0;
		Input.m_NextWeapon = 0;
		Input.m_PrevWeapon = 0;
		SizeData = sizeof(Input);
	}

	if (Input.m_Direction == 0 || this->blockUserInput) {
		Input.m_Direction = this->inputs[inputId].m_Direction;
	}

	if (Input.m_Jump == 0 || this->blockUserInput) {
		if (this->inputs[inputId].m_Jump > 0) {
			Input.m_Jump = 1;
		}
	}

	if (Input.m_Hook == 0 || this->blockUserInput) {
		Input.m_Hook = this->inputs[inputId].m_Hook;
	}

	int HookState = this->m_pClient->m_aClients[this->m_pClient->m_aLocalIds[inputId]].m_Predicted.m_HookState;

	if (HookState == -1) {
		this->inputs[inputId].m_Hook = 0;
	}

	if (this->inputs[inputId].m_Fire > 0 || this->blockUserInput) {
		Input.m_Fire = this->inputs[inputId].m_Fire;
		if (this->inputs[inputId].m_Fire > 0 && !this->blockUserInput) {
			this->m_pClient->m_Controls.m_aInputData[inputId].m_Fire = (this->m_pClient->m_Controls.m_aInputData[inputId].m_Fire + 1) % 64;
		}
		this->inputs[inputId].m_Fire = 0;
//		this->m_pClient->m_Chat.AddLine(inputId, 0, ("On snap input" + std::to_string(input.m_Fire)).c_str());
	}

	if (this->inputs[inputId].m_TargetX != 0 || this->inputs[inputId].m_TargetY != 0) {
		Input.m_TargetX = this->inputs[inputId].m_TargetX;
		Input.m_TargetY = this->inputs[inputId].m_TargetY;
	}

	if (this->inputs[inputId].m_WantedWeapon != -1) {
		Input.m_WantedWeapon = this->inputs[inputId].m_WantedWeapon;
		this->m_pClient->m_Controls.m_aInputData[inputId].m_WantedWeapon = Input.m_WantedWeapon;
	}

	mem_copy(pData, &Input, sizeof(Input));
	if (this->inputs[inputId].m_Jump > 0) {
		this->inputs[inputId].m_Jump--;
	}


	return SizeData;
}

void PythonController::InputFire(int id)
{
	if (GameClient()->m_Controls.m_aInputData[id].m_Fire % 2 == 1) {
		return;
	}

	GameClient()->m_Controls.m_aInputData[id].m_Fire = (GameClient()->m_Controls.m_aInputData[id].m_Fire + 1) % 64;
	GameClient()->pythonController.inputs[id].m_Fire = GameClient()->m_Controls.m_aInputData[id].m_Fire;
}

void PythonController::OnUpdate()
{
	// static int s_LastUpdateId = -1;
	// static int s_LastUpdateGameTick = Client()->PrevGameTick(g_Config.m_ClDummy);
	//
	// if (s_LastUpdateId == g_Config.m_ClDummy && s_LastUpdateGameTick == Client()->GameTick(g_Config.m_ClDummy))
	// {
	// 	return;
	// }
	//
	// s_LastUpdateId = g_Config.m_ClDummy;
	// s_LastUpdateGameTick = Client()->GameTick(g_Config.m_ClDummy);

	if(m_pClient->m_Snap.m_pLocalInfo && this->executedPythonScripts.size() > 0)
	{
		CServerInfo CurrentServerInfo;
		Client()->GetServerInfo(&CurrentServerInfo);

		for (auto pythonIgnoreServer : GameClient()->user.userData.pythonBlacklistIp)
		{
			if (pythonIgnoreServer == string(CurrentServerInfo.m_aAddress))
			{
				for (auto executedPythonScript : this->executedPythonScripts)
				{
					this->StopExecuteScript(executedPythonScript);
				}
			}
		}
	}

	for (auto executedPythonScript : this->executedPythonScripts) {
		executedPythonScript->updateExceptions();
		if (!PyModule_Check(executedPythonScript->module) || !PyObject_HasAttrString(executedPythonScript->module, "onUpdate")) {
			executedPythonScript->updateExceptions();
			continue;
		}

		PyObject* function = PyObject_GetAttrString(executedPythonScript->module, "onUpdate");

		if (function != nullptr && PyCallable_Check(function)) {
			GameClient()->pythonRender.SetScriptRender(executedPythonScript->filepath);
			PyObject* args = PyTuple_Pack(0);
			PyObject* result = PyObject_CallObject(function, args);
			executedPythonScript->updateExceptions();
			Py_XDECREF(args);
			Py_XDECREF(result);
		}
		else {
			PyErr_Clear();
		}
		Py_XDECREF(function);
	}
}

void PythonController::ResetInput(int id)
{
	if (id == -1) {
		for (int i = 0; i < 2; i++) {
			this->inputs[i].m_Fire = 0;
			this->inputs[i].m_Direction = 0;
			this->inputs[i].m_Jump = 0;
			this->inputs[i].m_Hook = 0;
			this->inputs[i].m_TargetX = 0;
			this->inputs[i].m_TargetY = 0;
			this->inputs[i].m_WantedWeapon = -1;
			GameClient()->m_Controls.m_aInputData[i].m_Fire = 0;
		}

		GameClient()->humanLikeMouse.removeMoving();
	} else {
		this->inputs[id].m_Fire = 0;
		this->inputs[id].m_Direction = 0;
		this->inputs[id].m_Jump = 0;
		this->inputs[id].m_Hook = 0;
		this->inputs[id].m_TargetX = 0;
		this->inputs[id].m_TargetY = 0;
		this->inputs[id].m_WantedWeapon = -1;
		GameClient()->m_Controls.m_aInputData[id].m_Fire = 0;

		if (id == g_Config.m_ClDummy) {
			GameClient()->humanLikeMouse.removeMoving();
		}
	}
}
