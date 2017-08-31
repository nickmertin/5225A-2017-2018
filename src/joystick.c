CONFIGS
{
	CONFIG_INIT

	CONFIG(SINGLE_STICK)
	{
		DEADZONE(Ch4, 10)

		ANY(Ch4, gJoyTurn = value)
	}

	CONFIG(DUAL_STICK)
	{
		DEADZONE(Ch1, 10)

		ANY(Ch1, gJoyTurn = value)
	}

	CONFIG(SINGLE_STICK | DUAL_STICK)
	{
		DEADZONE(Ch3, 10)

		ANY(Ch3, gJoyThrottle = value)

		RISING(Btn5D, CHANGE_STATE(lift, liftLowering);)
		RISING(Btn5U, CHANGE_STATE(lift, liftRaising);)

		FALLING(Btn5D, CHANGE_STATE_FROM_STATE(lift, liftLowering, liftHolding);)
		FALLING(Btn5U, CHANGE_STATE_FROM_STATE(lift, liftRaising, liftHolding);)
	}
}
