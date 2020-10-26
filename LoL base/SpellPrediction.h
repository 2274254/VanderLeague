#pragma once

#include "Vector.h"
#include "CObject.h"
#include "CSpell.h"
#include "ESpellSlot.h"
#include "Autokey.h"
#include "ImRender.hpp"
#include "Prediction.h"

#include "Debug.h"

class SpellPrediction {
public:
	Vector PredictSkillshot(CObject* target, ESpellSlot slot) {
		auto spell = Engine::GetLocalObject()->GetSpellBook()->GetSpellByID(static_cast<int>(slot));
		auto spellData = spell->GetSpellInfo()->GetSpellData();
		
		//float timeToTravel = (target->GetPos() - Engine::GetLocalObject()->GetPos()).length() / spellData->GetMissileSpeed();
		//Vector vec1 = target->GetAIManager()->GetVelocity().Normalize() * target->GetMoveSpeed() * timeToTravel;
	 	//auto vec = target->GetPos() + vec1;

		static LinePrediction linePrediction;
		auto vec = linePrediction.Calculate(target, spellData->GetSpellRange(), spellData->GetMissileSpeed(), 0.f);

		if (!(vec == Vector(0.f, 0.f, 0.f))) {
			Autokey::MoveMouse(vec);
		}

		return vec;
	}

	/*Vector PrediticTest(CObject* target, float radius, float spell_range, float spell_missile_speed, float spell_cast_delay) {
		int ping = Engine::GetPing();
		float flytime_max = (spell_missile_speed != 0) ? spell_range / spell_missile_speed : 0.f;
		float t_min = spell_cast_delay + ping / 2000.f;
		float t_max = flytime_max + spell_cast_delay + ping / 1000.f;
		float path_time = 0.f;

		auto aiMgr = target->GetAIManager();
		Vector* arr[2] = { aiMgr->GetNavBegin(), aiMgr->GetNavEnd() };
		auto ppNavStart = &arr[0];
		auto ppNavEnd = &arr[1];

		Vector next = (**ppNavEnd).switchYZ();
		Vector cur = (**ppNavStart).switchYZ();
		Vector direction = Vector{ (next - cur) }.Normalize();

		float extender = target->GetBoundingRadius();
		float distance = radius;

		int steps = (int)floor(cur.DistTo(next) / distance);

		if (steps > 0 && steps < 1000)
		{
			for (int i = 1; i < steps - 1; i++)
			{
				Vector center = Vector{ cur + direction * distance * i };
				Vector pt_a = Vector{ center - direction * extender };
				Vector pt_b = Vector{ center + direction * extender };

				float flytime = (spell_missile_speed != 0) ? Engine::GetLocalObject()->GetPos().DistTo(target->GetPos()) / spell_missile_speed : 0.f;
				float t = flytime + spell_cast_delay + ping / 2000.f;

				float arrive_time_a = target->GetPos().DistTo(pt_a) / target->GetMoveSpeed();
				float arrive_time_b = target->GetPos().DistTo(pt_b) / target->GetMoveSpeed();

				Vector predicted_pos = Vector{center.X, 0.f, center.Y};

				Vector enemy_position_w2s;
				Functions.WorldToScreen(&target->GetPos(), &enemy_position_w2s);
				Vector predicted_position_w2s;
				Functions.WorldToScreen(&predicted_pos, &predicted_position_w2s);

				debug::flog("arrive_time_a %.6f arrive_time_b %.6f t %.6f\n", arrive_time_a, arrive_time_b, t);
				if (min(arrive_time_a, arrive_time_b) <= t && max(arrive_time_a, arrive_time_b) >= t)
				{
					render.draw_line(enemy_position_w2s.X, enemy_position_w2s.Y, predicted_position_w2s.X, predicted_position_w2s.Y, ImColor(0, 255, 0, 255), 5.f);
					Autokey::MoveMouse(predicted_pos);
					return predicted_pos;
				}
				else
				{
					render.draw_line(enemy_position_w2s.X, enemy_position_w2s.Y, predicted_position_w2s.X, predicted_position_w2s.Y, ImColor(255, 0, 0, 255), 5.f);
				}
			}
		}
		
		return Vector();
	}*/
};