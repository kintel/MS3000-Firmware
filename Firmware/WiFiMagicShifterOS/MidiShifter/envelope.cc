#include "envelope.h"

void adsr_envelope_init(adsr_envelope * env) {
	env->stages[ENV_START].level = 0;
	env->stages[ENV_START].next = &env->stages[ENV_START];

	env->stages[ENV_ATTACK].level = 1;
	env->stages[ENV_ATTACK].next = &env->stages[ENV_DECAY];

	env->stages[ENV_DECAY].duration = -1;
	env->stages[ENV_DECAY].coeff = 0;
	env->stages[ENV_DECAY].next = &env->stages[ENV_SUSTAIN];

	env->stages[ENV_SUSTAIN].next = &env->stages[ENV_RELEASE];

	env->stages[ENV_RELEASE].duration = -1;
	env->stages[ENV_RELEASE].coeff = 0;
	env->stages[ENV_RELEASE].level = 0;
	env->stages[ENV_RELEASE].next = &env->stages[ENV_START];

	env->timer = 0;
	env->level = 0;
	env->current = &env->stages[ENV_RELEASE]; // inactive
	env->is_idle = true;
}

uint16_t adsr_envelope_tick(adsr_envelope * env) {
	estage * es = env->current;

	msSystem.log("Current Stage:");
	msSystem.logln((unsigned int)es, HEX);
	msSystem.log("Timer:");
	msSystem.logln(env->timer, DEC);
	msSystem.log("Duration:");
	msSystem.logln(es->duration, DEC);

	if (env->timer >= es->duration) { // next stage
		env->current = (estage *)es->next;
		es = env->current;
		env->timer = 0;
		env->level = es->level;
		if (es == &(env->stages[ENV_RELEASE]))
			env->is_idle = true;
		adsr_envelope_tick(env);
	} else {
		env->level += es->coeff;
		if (env->level < 0)
			env->level = 0;
	 	env->timer++;
	}
	return env->level;
}
