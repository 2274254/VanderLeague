#include "ScriptUtils.h"

#include "Morgana.h"

#include <string>

IScript* GetScriptByChampionName(char*) {
}

IScript* ScriptUtils::GetScriptByChampionName(char* championName)
{
	if (strcmp(championName, "Morgana") == 0) {
		return new Morgana();
	}

	return nullptr;
}
