#include "main_GUI.h"
#include <QFile>

main_GUI::main_GUI(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::main_GUI),
	qout(NULL),
	qerr(NULL)
{
	ui->setupUi(this);

	//QFile file("./styleSheet.qss");
	//file.open(QFile::ReadOnly);
	//QString myStyle = QLatin1String(file.readAll());
	//this->setStyleSheet(myStyle);

	// init the redirect buffer for messages
	qerr = new QDebugStream(std::cerr, ui->textEdit_cout);
	qout = new QDebugStream(std::cout, ui->textEdit_cout);
	this->setRedirect(true);
	qerr->setVerbose(true);
	qout->setVerbose(true);

	m_update_GUI = new QTimer();
	m_update_GUI->setInterval(100);// (m_base_time_step);
	connect(m_update_GUI,
		SIGNAL(timeout()), this,
		SLOT(updateGUI()));
	m_update_GUI->start();

	int argc = 0;
	char ** argv = NULL;
	parser = new ArArgumentParser(&argc,argv);
	parser->loadDefaultArguments();
	std::cout << " we are here " << std::endl;

	robot = new ArRobot("myRobot");
	sonar = new ArSonarDevice();
	//sonar = robot->findRangeDevice("sonar");
	//sonar->setMaxRange(1000);
	recover = new ArActionStallRecover();
	gyro = new ArAnalogGyro(robot);
	// Add the range device to the robot. You should add all the range
	// devices and such before you add actions
	robot->addRangeDevice(sonar);

	// Create instances of the actions defined above, plus ArActionStallRecover,
    // a predefined action from Aria.
	gotoPoseAction = new ArActionGoto("goto");
	gotoPoseAction->setSpeed(150);
	gotoPoseAction->setSpeedToTurnAt(75);
	gotoPoseAction->setCloseDist(100);
	stopAction = new ArActionStop("stop");
	limiterAction = new ArActionLimiterForwards ("speed limiter near", 25, 300, 50);
	limiterFarAction = new ArActionLimiterForwards ("speed limiter far", 50, 500, 150);
	tableLimiterAction = new ArActionLimiterTableSensor();

	connect(ui->actionConnectRobot,
		SIGNAL(triggered()), this,
		SLOT(connectRobotClicked()));
	connect(ui->actionConnectAgent,
		SIGNAL(triggered()), this,
		SLOT(connectClientClicked()));
	
	connect(ui->pushButton_sendMessage,
		SIGNAL(clicked()), this,
		SLOT(sendMessageToServerClicked()));
	connect(ui->horizontalSlider_longSpeed,
		SIGNAL(valueChanged(int)), this,
		SLOT(sliderLongVelChanged(int)));
	connect(ui->verticalSlider_lateralSpeed,
		SIGNAL(valueChanged(int)), this,
		SLOT(sliderLatVelChanged(int)));
	connect(ui->pushButton_stopRobot,
		SIGNAL(clicked()), this,
		SLOT(robotStopClicked()));
	connect(ui->pushButton_goto,
		SIGNAL(clicked()), this,
		SLOT(robotGoToPose()));
	
}


void main_GUI::connectRobotClicked()
{
	if (ui->actionConnectRobot->isEnabled() == true)
	{

	QApplication::setOverrideCursor(Qt::WaitCursor);
	Aria::init();

	// Connect to the robot, get some initial data from it such as type and name,
	// and then load parameter files for this robot.
	robotConnector = new ArRobotConnector(parser, robot);
	if (!robotConnector->connectRobot())
	{
		std::cout << "actionExample: Could not connect to the robot." << std::endl;
		//ArLog::log(ArLog::Terse, "actionExample: Could not connect to the robot.");
		if (parser->checkHelpAndWarnUnparsed())
		{
			// -help not given
			Aria::logOptions();
			//Aria::exit(1);
			QApplication::restoreOverrideCursor();    //close transform the cursor for waiting mode
			ui->actionConnectRobot->setChecked(false);
			return;
		}
	}

	if (!Aria::parseArgs() || !parser->checkHelpAndWarnUnparsed())
	{
		Aria::logOptions();
		//Aria::exit(1);
		QApplication::restoreOverrideCursor();    //close transform the cursor for waiting mode
		ui->actionConnectRobot->setChecked(false);
		return;
	}

	std::cout << "connected to robot" << std::endl;

	//startRobotActionsClicked();

	// starting robot actions
	// Add our actions in order. The second argument is the priority,
	// with higher priority actions going first, and possibly pre-empting lower
	// priority actions.
	robot->addAction(recover, 100);

	// Collision avoidance actions at higher priority
	robot->addAction(tableLimiterAction, 100);
	robot->addAction(limiterAction, 95);
	robot->addAction(limiterFarAction, 90);
	//ArActionGoto gotoPoseAction("goto");
	robot->addAction(gotoPoseAction, 50);

	// Stop action at lower priority, so the robot stops if it has no goal
	robot->addAction(stopAction, 40);

	// Enable the motors, disable amigobot sounds
	robot->enableMotors();

	//ArLog::log(ArLog::Normal, "actionExample: Connected to robot.");
	robot->runAsync();
	QApplication::restoreOverrideCursor();    //close transform the cursor for waiting mode
	}
	else
	{
		this->robotStopClicked();
		robotConnector->disconnectAll();
		ArLog::log(ArLog::Normal, "actionExample: Disconnected to robot.");
		Aria::uninit();
	}
}

void main_GUI::robotGoToPose()
{
	robot->resetTripOdometer();

	int gotoDistance = ui->spinBox_gotoDistance->value();
	int gotoHeading = ui->spinBox_gotoHeading->value();
	gotoPoseAction->setGoal(ArPose(gotoDistance, gotoHeading));
}

void main_GUI::robotActionTurnClicked()
{
	//if (robot->isRunning())
	int heading = 0;// ui->spinBox_turnSpeed->value();

	robot->lock();
	robot->setHeading(heading);
	robot->unlock();
}

void main_GUI::robotActionGoClicked()
{
	int distance = 0;// ui->spinBox_goSpeed->value();

	robot->lock();
	robot->move(distance);
	robot->unlock();
}

void main_GUI::robotStopClicked()
{
	robot->lock();
	robot->stop();
	robot->unlock();
	ui->spinBox_cLongVel->setValue(0);
	ui->spinBox_cLatVel->setValue(0);
	ui->horizontalSlider_longSpeed->setValue(0);
	ui->verticalSlider_lateralSpeed->setValue(0);
}


void main_GUI::connectClientClicked()
{
	if (ui->actionConnectAgent->isEnabled() == false)
	{
		disconnectClientClicked();
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	ui->centralwidget->setEnabled(false);
	myAgentClient = new AgentClient();
	if (myAgentClient->ConnectToLocalHostServer() == true)
	{
	
	connect(myAgentClient,
		&AgentClient::messageReceived, this,
		&main_GUI::messageReceivedFromAgent);
	}
	else
	{
		ui->actionConnectAgent->setChecked(false);
		delete myAgentClient;
	}

	ui->centralwidget->setEnabled(true);
	QApplication::restoreOverrideCursor();    //close transform the cursor for waiting mode
}

void main_GUI::messageReceivedFromAgent()
{
	//std::cout << message.toStdString() << std::endl;

	myCurrentMessage = myAgentClient->getMessageFromAgent();

	QString msg = QString::fromStdString(myCurrentMessage.message);
	QStringList msgList = msg.split(":");

	if (msgList.count() > 1)
	{
		QString command = msgList.at(0);
		QString argument = msgList.at(1);

		// interprete a message and run an action accordingly

		commandFromAgent cfa;
		std::string commandToStd = command.toStdString();
		//std::cout << commandToStd.compare(cfa.MOVE_TO_POSE_PREFIX) << std::endl;
		if (commandToStd.compare(cfa.MOVE_TO_POSE_PREFIX) == 0)
		{
			std::cout << " move to pose prefix triggered " << std::endl;
			// define the behaviour
		}
		if (commandToStd.compare(cfa.MOVE_TO_NAMED_LOCATION_PREFIX) == 0)
		{
			std::cout << " move to named location prefix triggered " << std::endl;

			std::string argumentToStd = argument.toStdString();

			if (argumentToStd.compare("Mattias' office") == 0)
			{
				std::cout << " move to mattias' office " << std::endl;
				if (robot->isRunning())
					gotoPoseAction->setGoal(ArPose(6400, 0)); // TODO: ugly defined in mm
			}
			if (argumentToStd.compare("Mauro's office") == 0)
			{
				std::cout << " move to Mauro's office " << std::endl;
				if (robot->isRunning())
					gotoPoseAction->setGoal(ArPose(0, 0));
			}

			// define the behaviour
		}
	}

	std::cout << "Received message from agent:"
		<< "  " << myCurrentMessage.sender << "  " << myCurrentMessage.message
		<< "  " << std::endl;
}


void main_GUI::sendMessageToServerClicked()
{
	myAgentClient->sendMessage(ui->lineEdit_writeData->text().toStdString());
}


void main_GUI::disconnectClientClicked()
{
	myAgentClient->DisconnectFromLocalHostServer();
	delete myAgentClient;
}


void main_GUI::updateGUI()
{
	if (robot->isConnected())
	{
		//ArRobot::ChargeState SoC  = robot->getChargeState();
		double vel = robot->getVel();
		ui->lineEdit_robotLongVel->setText(QString::number(vel));
		double omega = robot->getRotVel();
		ui->lineEdit_robotLatVel->setText(QString::number(omega));

		double compassHeading = robot->getCompass();
		ui->lineEdit_compassHeading->setText(QString::number(compassHeading));
		long rightEnc = robot->getRightEncoder();
		ui->lineEdit_rightEncoder->setText(QString::number(rightEnc));
		long leftEnc = robot->getLeftEncoder();
		ui->lineEdit_leftEncoder->setText(QString::number(leftEnc));
		double rightVel = robot->getRightVel();
		ui->lineEdit_rightWheelSpeed->setText(QString::number(rightVel));
		double leftVel = robot->getLeftVel();
		ui->lineEdit_leftWheelSpeed->setText(QString::number(leftVel));

		int sonar_0 = robot->getSonarRange(0);
		int sonar_1 = robot->getSonarRange(1);
		int sonar_2 = robot->getSonarRange(2);
		int sonar_3 = robot->getSonarRange(3);
		int sonar_4 = robot->getSonarRange(4);
		int sonar_5 = robot->getSonarRange(5);
		ui->lineEdit_sonar0->setText(QString::number(sonar_0));
		ui->lineEdit_sonar1->setText(QString::number(sonar_1));
		ui->lineEdit_sonar2->setText(QString::number(sonar_2));
		ui->lineEdit_sonar3->setText(QString::number(sonar_3));
		ui->lineEdit_sonar4->setText(QString::number(sonar_4));
		ui->lineEdit_sonar5->setText(QString::number(sonar_5));


		ArRobot::ChargeState chargeState = robot->getChargeState();
		ui->lineEdit_chargeState->setText(QString::number(chargeState));

		double voltage = robot->getBatteryVoltage();
		if (voltage < MIN_BATTERY_VOLTAGE)
		{
			ui->progressBar->setValue(0);
			ui->lineEdit_batteryVoltage->setText(QString::number(voltage));
		}
		else
		{
			double bv = 100.0*(voltage - MIN_BATTERY_VOLTAGE) / DELTA_BATTERY_VOLTAGE;
			int batteryLevel = int(bv);
			ui->progressBar->setValue(batteryLevel);
			ui->lineEdit_batteryVoltage->setText(QString::number(voltage));
		}

		int temperature = robot->getTemperature();
		ui->lineEdit_temperature->setText(QString::number(temperature));
		double odoDeg = robot->getOdometerDegrees();
		ui->lineEdit_odoHeading->setText(QString::number(odoDeg));

		double dist = robot->getOdometerDistance();
		ui->lineEdit_odoDist->setText(QString::number(dist));

	}
}

void main_GUI::sliderLongVelChanged(int _value)
{
	robot->lock();
	robot->setVel(_value);
	robot->unlock();
	ui->spinBox_cLongVel->setValue(_value);

}

void main_GUI::sliderLatVelChanged(int _value)
{
	robot->lock();
	robot->setRotVel(_value);
	robot->unlock();
	ui->spinBox_cLatVel->setValue(_value);
}



void main_GUI::setRedirect(bool _enable)
{
	// qerr and cout must be already initialized
	// the pointer are set to NULL by default in ctor
	if (qerr == NULL)
		return;
	if (qout == NULL)
		return;

	// errors will go always to GUI 
	qerr->redirectOutInGUI(true);

#ifndef _DEBUG
	// if we are not in debug 
	//we redirect normal messages to the GUI according to the settings
	qout->redirectOutInGUI(_enable);
	// and normal messages will not go to the terminal
	qout->copyOutToTerminal(false);
	// errors will not go the the terminal (we have no terminal window)
	qerr->copyOutToTerminal(true);
#else
	// otherwise, we are in debug hence 
	// all messages will go to the terminal
	qerr->copyOutToTerminal(true);
	qout->copyOutToTerminal(true);

	// out messages will go to GUI according to the settings
	qout->redirectOutInGUI(_enable);
#endif
}

void main_GUI::exit()
{
    close();
    qApp->quit();
    delete ui;
}

main_GUI::~main_GUI()
{
	Aria::exit(0);

	delete qout;
	delete qerr;
    delete ui;
}
