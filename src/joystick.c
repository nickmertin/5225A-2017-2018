CONFIGS
{
	CONFIG_INIT

	CONFIG(DRIVER(SINGLE_STICK))
	{
		DEADZONE(Ch4, 10)

		ANY(Ch4, gJoyTurn = MAP_DRIVE_INPUT(value);)
	}

	CONFIG(DRIVER(DUAL_STICK))
	{
		DEADZONE(Ch1, 10)

		ANY(Ch1, gJoyTurn = MAP_DRIVE_INPUT(value);)
	}

	CONFIG(DRIVER(SINGLE_STICK) | DRIVER(DUAL_STICK))
	{
		DEADZONE(Ch3, 10)

		ANY(Ch3, gJoyThrottle = MAP_DRIVE_INPUT(value);)

		RISING(Btn5D, CHANGE_STATE(lift, liftLowering);)
		RISING(Btn5U, CHANGE_STATE(lift, liftRaising);)

		FALLING(Btn5D, CHANGE_STATE_FROM_STATE(lift, liftLowering, liftHolding);)
		FALLING(Btn5U, CHANGE_STATE_FROM_STATE(lift, liftRaising, liftHolding);)

		RISING(Btn6D, CHANGE_STATE_FROM_STATE(mobile, mobileIdle, mobileLowering); CHANGE_STATE_FROM_STATE(mobile, mobileToHalfway, mobileLowering); CHANGE_STATE_FROM_STATE(mobile, mobileUp, mobileToHalfway); CHANGE_STATE_FROM_STATE(mobile, mobileRaising, mobileLowering);)
		RISING(Btn6U, CHANGE_STATE(mobile, mobileRaising);)

		RISING(Btn8R, CHANGE_STATE(intake, intakeIn);)
		RISING(Btn8D, CHANGE_STATE(intake, intakeOut);)

		FALLING(Btn8R, CHANGE_STATE_FROM_STATE(intake, intakeIn, intakeIdle);)
		FALLING(Btn8D, CHANGE_STATE_FROM_STATE(intake, intakeOut, intakeIdle);)
	}
}
