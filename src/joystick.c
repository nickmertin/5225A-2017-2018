CONFIGS
{
	CONFIG_INIT

	CONFIG(DRIVER(MANUAL_X_Y))
	{
		DEADZONE(Ch1, 10)
		DEADZONE(Ch3, 10)

		HIGH(Ch1, gArmTarget.x += (float) (value * value * sgn(value)) / 63_500.0;)
		HIGH(Ch3, gArmTarget.y += (float) (value * value * sgn(value)) / 63_500.0;)
		//FALLING(Ch1, currentArmPosition(gArmTarget);)
		//FALLING(Ch3, currentArmPosition(gArmTarget);)

		RISING(Btn6U, CHANGE_STATE(claw, clawClosed);)
		RISING(Btn6D, CHANGE_STATE(claw, clawOpen);)

		RISING(Btn5U, gArmPath = &gStack7_1; CHANGE_STATE(arm, armDoFollowPath);)
		RISING(Btn5D, CHANGE_STATE(arm, armMoveHoldToTarget);)
	}
}
