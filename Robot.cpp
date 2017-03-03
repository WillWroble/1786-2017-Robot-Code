#include <iostream>
#include <memory>
#include <string>
#include <CANTalon.h>
#include <WPILib.h>
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

class Robot: public frc::IterativeRobot {
	CANTalon *climberController, *intakeController, *shooter, *lf, *lb, *rf, *rb;
	RobotDrive *mecanumDrive;
	Joystick *stick, *operatorStick;
	bool button1Pressed = false;
	bool opButton1Pressed = false;
	bool switchClimberOn = false;
	bool switchIntakeOn = false;
	int reverseThrottleNum = 1;
	int opReverseThrottleNum = 1;

public:
	void RobotInit() {
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);

		//calling constructers of previously defined classes
		stick = new Joystick(0);
		operatorStick = new Joystick(1);

		// same thing for CANTalons
		lf = new CANTalon(3);
		lb = new CANTalon(4);
		rf = new CANTalon(2);
		rb = new CANTalon(5);
		intakeController = new CANTalon(0);
		climberController = new CANTalon(1);
		shooter = new CANTalon(6);

		// getting the RobotDrive ready
		mecanumDrive = new RobotDrive(lf, lb, rf, rb);
		mecanumDrive->SetSafetyEnabled(true);
		mecanumDrive->SetExpiration(0.1);
		lf->SetInverted(true);
		lb->SetInverted(true);
	}

	/*
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString line to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional comparisons to the
	 * if-else structure below with additional strings. If using the
	 * SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit() override {
		autoSelected = chooser.GetSelected();
		// std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		mecanumDrive->MecanumDrive_Cartesian(0,1,0);


		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
		} else {
			// Default Auto goes here
		}
	}

	void AutonomousPeriodic() {
		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
		} else {
			// Default Auto goes here
		}
	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {



		// GETS THROTTLE FOR INTAKE
		float driveThrottle = ((stick->GetThrottle()-1)/-2);

		//GETS THROTTLE FOR CLIMBER
		float operatorThrottle = ((operatorStick->GetThrottle()-1)/-2);

		//DRIVE TRAIN
		mecanumDrive->MecanumDrive_Cartesian(stick->GetX(), stick->GetY(), stick->GetZ());

		//GET INTAKE AUTOMATIC BUTTON
		if(stick->GetRawButton(7))
		{
			switchIntakeOn = true;
		}
		else
		{
			switchIntakeOn = false;
		}

		//CONTROLS THROTTLE REVERSAL FOR INTAKE
		if(stick->GetRawButton(11) && !button1Pressed)
		{
			reverseThrottleNum = reverseThrottleNum * -1;
			button1Pressed = true;
		}
		if(!stick->GetRawButton(11))
		{
			button1Pressed = false;
		}

		//AUTOMATIC CONTROL FOR INTAKE
		if(switchIntakeOn && driveThrottle == 0)
		{
			driveThrottle = 0.5;
		}

		//PUTS CORRECT VALUE IN FOR INTAKE
		intakeController->Set(driveThrottle * reverseThrottleNum);

		//GET CLIMBER AUTOMATIC BUTTON
		if(operatorStick->GetRawButton(7))
		{
			switchClimberOn = true;
		}
		else
		{
			switchClimberOn = false;
		}

		//CONTOLS THROTTLE REVERSAL FOR CLIMBER
		if(operatorStick->GetRawButton(11) && !opButton1Pressed)
		{
			opReverseThrottleNum = opReverseThrottleNum * -1;
			opButton1Pressed = true;
		}
		if(!operatorStick->GetRawButton(11))
		{
			opButton1Pressed = false;
		}

		//AUTOMATIC CONTROL FOR CLIMBER
		if(switchClimberOn && operatorThrottle == 0)
		{
			operatorThrottle = 0.5;
		}

		//PUTS CORRECT VALUE IN FOR CLIMBER
		climberController->Set(operatorThrottle * opReverseThrottleNum);

		//SHOOTER
		//controlled by y axis of operator stick
		shooter->Set(operatorStick->GetY);




	}
private:
	frc::LiveWindow* lw = LiveWindow::GetInstance();
	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;
};

START_ROBOT_CLASS(Robot)
