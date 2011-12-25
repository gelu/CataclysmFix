/*
SD%Complete: 80%
TO DO: Check Timer and implement event
*/

#include "ScriptPCH.h"

#define YELL_AGGRO "Forest just setback!"
#define YELL_DEAD "Yiiipe!"

enum spell
{
	SPELL_VICIOUS_SLICE     = 86604,
	SPELL_MADDENING_CALL    = 86620, // spell doesn't work 
	SPELL_ENRAGE            = 86736,
};

class boss_hogger_stockade : public CreatureScript
{
public:
    boss_hogger_stockade() : CreatureScript("boss_hogger_stockade") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_hogger_stockadeAI (pCreature);
    }

    struct boss_hogger_stockadeAI : public ScriptedAI
    {
        boss_hogger_stockadeAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;

		uint32 uiTimerViciousSlice;
		uint32 uiTimerMaddeningCall;
		bool bIsEnraged;

		void Reset()
		{
			uiTimerViciousSlice    = 6000;
			uiTimerMaddeningCall    = 10000;
			bIsEnraged = false;

		}

		void EnterCombat(Unit * /*who*/)
		{
			me->MonsterYell(YELL_AGGRO, LANG_UNIVERSAL, NULL);
		}

		void JustDied(Unit* /*Killer*/)
		{
			me->MonsterYell(YELL_DEAD, LANG_UNIVERSAL, NULL);
		}


		void UpdateAI(uint32 const diff)
		{

			if (!UpdateVictim())
				return;

			if (uiTimerViciousSlice <= diff)
			{

				DoCast(me->getVictim(), SPELL_VICIOUS_SLICE);
					uiTimerViciousSlice = 16000;
			} else uiTimerViciousSlice -= diff;


			if(uiTimerMaddeningCall <= diff)
			{
				DoCast(me, SPELL_MADDENING_CALL);
				uiTimerMaddeningCall = 30000;
			} else uiTimerMaddeningCall -= diff;


            if (!bIsEnraged && !HealthAbovePct(30) && !me->IsNonMeleeSpellCasted(false))
            {
                bIsEnraged = true;
                DoCast(me, SPELL_ENRAGE);
            }

			DoMeleeAttackIfReady();
		}
	};
};

void AddSC_boss_hogger_stockade()
{
    new boss_hogger_stockade();
}