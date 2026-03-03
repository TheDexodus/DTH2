#ifndef DDNET_PYTHONCONTROLLER_H
#define DDNET_PYTHONCONTROLLER_H

#include "engine/client.h"
#include "engine/input.h"
#include "game/client/component.h"
#include "game/client/python/PythonScript.h"
#include <map>

class PythonController : public CComponent
{
public:
	PythonController();
	void StartExecuteScript(PythonScript* pythonScript);
	void StopExecuteScript(PythonScript* pythonScript);
	bool isExecutedScript(PythonScript* pythonScript);
	void AutoloadAdd(PythonScript* pythonScript);
	void AutoloadRemove(PythonScript* pythonScript);
	bool isScriptAutoloading(PythonScript* pythonScript);
	bool OnChatMessage(int MsgType, void *pRawMsg);

	void InputFire(int id);
	void ResetInput(int id = -1);

	void OnUpdate();

	virtual int Sizeof() const override { return sizeof(*this); }

	int SnapInput(int* pData, int inputId);
	bool needForceInput(int inputId);
	void SetScriptShowMenuCursor(const std::string &scriptId, bool show);
	void RemoveScriptShowMenuCursor(const std::string &scriptId);
	vec2 GetScriptCursorPos() const { return m_ScriptCursorPos; }
	CNetObj_PlayerInput inputs[NUM_DUMMIES];
	bool blockUserInput = false;
	bool showMenuCursor = false;

	std::vector<PythonScript*> autoLoadPythonScripts;
	std::vector<PythonScript*> executedPythonScripts;
	bool enableDummyControl = false;
	std::map<std::string, bool> scriptShowMenuCursor;
	vec2 m_ScriptCursorPos = vec2(0.0f, 0.0f);
	bool m_ScriptCursorPosInitialized = false;

protected:
	bool OnCursorMove(float x, float y, IInput::ECursorType CursorType) override;
	bool OnInput(const IInput::CEvent &Event);
};

#endif // DDNET_PYTHONCONTROLLER_H
