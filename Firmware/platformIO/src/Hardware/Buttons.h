// Button code

#ifndef __BUTTONS_H
#define __BUTTONS_H

class MagicShifterButtons { 

	private:

	public:

	// TODO: private state
	// state for button timing
	long msBtnAPressTime = 0;
	long msBtnPwrPressTime = 0;
	long msBtnBPressTime = 0;

	// state for double click timing
	long msBtnATTL = 0;
	long msBtnPwrTTL = 0;
	long msBtnBTTL = 0;

	bool msLongClickOK = true;

	// todo public properties? Logic for consuming buttons?
	// events for consumers true/false;
	bool msBtnAHit = false;
	bool msBtnPwrHit = false;
	bool msBtnBHit = false;

	bool msBtnALongHit = false;
	bool msBtnPwrLongHit = false;
	bool msBtnBLongHit = false;

	bool msBtnADoubleHit = false;
	bool msBtnPwrDoubleHit = false;
	bool msBtnBDoubleHit = false;

	long deltaMicros = 0; // !J! todo: init? (msGlobals.ggCurrentMicros - msGlobals.ggLastMicros);

	//  button activity 
	bool msBtnActive = false;

	bool powerButtonPressed(void)
	{
		// had unusual high values on one of the 100x PCBs so this threshold is bigger now
		return analogRead(A0) > 970;
	}

	void setup()
	{
	    // init pin modes
	    pinMode(PIN_BUTTON_A, INPUT);
	    pinMode(PIN_BUTTON_B, INPUT);
	}

	void step() 
	{
		msBtnActive = false;

	    deltaMicros = (msGlobals.ggCurrentMicros - msGlobals.ggLastMicros);

	// handle Buttons:
		pinMode(PIN_BUTTON_A, INPUT);
		pinMode(PIN_BUTTON_B, INPUT);

		if (!digitalRead(PIN_BUTTON_A))
		{
			msBtnActive = true;

			if (msBtnAPressTime)
				msBtnAPressTime += deltaMicros;
			else
				msBtnAPressTime = 1;
		}
		else
		{
			if (msLongClickOK && msBtnAPressTime >= MIN_TIME_LONG_CLICK)
			{
	    // logln("We gots LOON clicks A.");
				msBtnALongHit = true;
			}
			else 
			if (msBtnAPressTime >= MIN_TIME_CLICK)
			{
	// logln("We gots clicks A.");
				msBtnAHit = true;
			}

			msBtnAPressTime = 0;
		}

		if (!digitalRead(PIN_BUTTON_B))
		{

			msBtnActive = true;

			if (msBtnBPressTime)
				msBtnBPressTime += deltaMicros;
			else
				msBtnBPressTime = 1;
		}
		else
		{
			if (msLongClickOK && msBtnBPressTime >= MIN_TIME_LONG_CLICK)
			{
			    // logln("We gots LOON clicks B .");
				msBtnBLongHit = true;
			}
			else
			if (msBtnBPressTime >= MIN_TIME_CLICK)
			{
			    // logln("We gots clicks B.");
				msBtnBHit = true;
			}

			msBtnBPressTime = 0;
		}

		if (powerButtonPressed())
		{

			msBtnActive = true;

			if (msBtnPwrPressTime)
				msBtnPwrPressTime += deltaMicros;
			else
				msBtnPwrPressTime = 1;
		}
		else
		{
			if (msBtnPwrPressTime >= MIN_TIME_LONG_CLICK)
			{
				msBtnPwrLongHit = true;
	// logln("Btn Pwr Looong Hit");
			}
			else 
			if (msBtnPwrPressTime >= MIN_TIME_CLICK)
			{
	// logln("We gots clicks Pwr.");
				msBtnPwrHit = true;
			}

			msBtnPwrPressTime = 0;
		}
	}
};

#endif
