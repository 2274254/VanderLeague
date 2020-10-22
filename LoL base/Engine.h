#pragma once
#include "Offsets.h"
#include "Utils.h"
#include "Vector.h"
#include "Hooks.h"
#include "CObjectManager.h"

class Engine {
public:
	static float GetGameTime()
	{
		return *(float*)(baseAddr + offsets::global::oGameTime);
	}

	/*static int GetGameTimeTickCount()
	{
		return (int)(GetGameTime() * 1000);
	}*/

	static int GetPing()
	{
		return Functions.GetPing(*(void**)(baseAddr + offsets::global::oNetClient));
	}

	static int GetLatencyTicks(int extraTicks = 50) {
		return (extraTicks + GetPing() / 2);
	}

	static bool IsLeagueInForeground()
	{
		TCHAR title[500];
		GetWindowText(GetForegroundWindow(), title, 500);
		return wcscmp(title, L"League of Legends (TM) Client") == 0;
	}

	static Vector GetMouseWorldPosition() {
		DWORD MousePtr = baseAddr + offsets::global::oHudInstance;
		auto aux1 = *(DWORD*)MousePtr;
		aux1 += 0x14;
		auto aux2 = *(DWORD*)aux1;
		aux2 += 0x1C;

		float X = *(float*)(aux2 + 0x0);
		float Y = *(float*)(aux2 + 0x4);
		float Z = *(float*)(aux2 + 0x8);

		return Vector{ X, Y, Z };
	}

	static CObject* GetLocalObject() {
		auto retaddr = *(DWORD*)(baseAddr + offsets::global::oLocalPlayer);
		if (retaddr == NULL)
			return NULL;

		return (CObject*)retaddr;
	}
	static void PrintChat(const char* Message) {
		typedef void(__thiscall* tPrintChat)(DWORD ChatClient, const char* Message, int Color);
		tPrintChat fnPrintChat = (tPrintChat)(baseAddr + offsets::functions::oPrintChat);
		fnPrintChat(*(DWORD*)(baseAddr + offsets::global::oChatClientPtr), Message, 1);
	}

	static CObject* Engine::GetObjectByID(int ID)
	{
		if (ObjManager && ID >= 0 && ID <= ObjManager->GetHighestIndex()) {
			return ObjManager->GetObjByIndex(ID);
		}

		return nullptr;
	}

	static void MoveTo(Vector* pos) {
		Functions.IssueOrder(GetLocalObject(), 2, pos, NULL, false, false, false);

	}
		


	static void AttackTarget(CObject* obj) {
		Functions.IssueOrder(GetLocalObject(), 3, &obj->GetPos(), obj, true, false, false);
	}


	static void Engine::CastSpellSelf(int SlotID) {
		if (Engine::GetLocalObject()->IsAlive()) {
			auto spellbook = (DWORD)Engine::GetLocalObject() + offsets::cobject::oObjSpellBook;
			auto spellslot = Engine::GetLocalObject()->GetSpellBook()->GetSpellSlotByID(SlotID);
			Functions.CastSpell(spellbook, spellslot, SlotID, &Engine::GetLocalObject()->GetPos(), &Engine::GetLocalObject()->GetPos(), 0);
		}
	}

	static void Engine::CastSpellPos(int SlotID, Vector pos) {
		auto spellbook = (DWORD)Engine::GetLocalObject() + offsets::cobject::oObjSpellBook;
		auto spellslot = Engine::GetLocalObject()->GetSpellBook()->GetSpellSlotByID(SlotID);
		auto targetpos = &pos;

		Functions.CastSpell(spellbook, spellslot, SlotID, targetpos, new Vector(0, 0, 0), 0x0);
	}

	static void Engine::CastSpellPos(int SlotID, float x, float y, float z, CObject* obj) {
		auto spellbook = (DWORD)Engine::GetLocalObject() + offsets::cobject::oObjSpellBook;
		auto spellslot = Engine::GetLocalObject()->GetSpellBook()->GetSpellSlotByID(SlotID);
		Vector* pos = new Vector(x, y, z);
		Functions.CastSpell(spellbook, spellslot, SlotID, &obj->GetPos(), &Engine::GetLocalObject()->GetPos(), obj->GetNetworkID());
		delete pos;
	}

	static void Engine::CastSpellTargetted(int SlotID, CObject* obj) {
		auto spellbook = (DWORD)Engine::GetLocalObject() + offsets::cobject::oObjSpellBook;
		auto spellslot = Engine::GetLocalObject()->GetSpellBook()->GetSpellSlotByID(SlotID);

		Functions.CastSpell(spellbook, spellslot, SlotID, &obj->GetPos(), &Engine::GetLocalObject()->GetPos(), obj->GetNetworkID());
	}

	static float Engine::getCD(int slot, CObject* obj) {

		//Console.print("CD : %f", obj->GetSpellBook()->GetSpellSlotByID(slot)->GetCD() - Engine::GetGameTime());
		return obj->GetSpellBook()->GetSpellSlotByID(slot)->GetCD();
	}

	static bool Engine::IsReady(int slot, CObject* obj) {
		//Console.print("LEVEL : %i", obj->GetSpellBook()->GetSpellSlotByID(slot)->GetLevel());
		return obj->GetSpellBook()->GetSpellSlotByID(slot)->GetLevel() >= 1 && getCD(slot, obj) == 0.0f;
	}
};

