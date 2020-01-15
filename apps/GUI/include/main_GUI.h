#ifndef MAIN_GUI_H_
#define MAIN_GUI_H_

#include "ui_main_GUI.h"

#include <QMainWindow>
#include <QTranslator>
#include <QDateTime>
#include <QTimer>

#include "Q_DebugStream.h"
#include "Aria.h"
#include "AgentClient.h"


class AgentClient;

class main_GUI : public  QMainWindow
{
	Q_OBJECT


public:
	explicit main_GUI(QWidget *parent = nullptr);

	~main_GUI();

	void exit();

	private slots:
		void connectRobotClicked();

		void robotActionGoClicked();

		void robotActionTurnClicked();

		void robotStopClicked();

		void connectClientClicked();

		void disconnectClientClicked();

		void sendMessageToServerClicked();

		void robotGoToPose();

		void updateGUI();

		void sliderLongVelChanged(int _value = 0);

		void sliderLatVelChanged(int _value = 0);

		void messageReceivedFromAgent();

private:

	/** \brief Set debug to terminal
	*/
	void dumpToTerminal(int _state) {
		qout->copyOutToTerminal(_state);
		qerr->copyOutToTerminal(_state);
	};

	void setRedirect(bool _enable = true);

	QDebugStream *qout;                 //!< redirect cout for messages into the GUI
	QDebugStream *qerr;                 //!< redirect cerr for messages into the GUI

	ArArgumentParser *parser;
	ArRobot *robot;
	ArRobotConnector *robotConnector;
	ArSonarDevice *sonar;
	ArActionStallRecover *recover;
	ArAnalogGyro *gyro;

	ArActionGoto *gotoPoseAction;
	ArActionStop *stopAction;
	ArActionLimiterForwards *limiterAction;
	ArActionLimiterForwards *limiterFarAction;
	ArActionLimiterTableSensor *tableLimiterAction;

	AgentClient *myAgentClient;
	messageFromAgent myCurrentMessage;

	QTimer *m_update_GUI;

	const double MIN_BATTERY_VOLTAGE = 12.0;
	const double MAX_BATTERY_VOLTAGE = 13.5;
	const double DELTA_BATTERY_VOLTAGE = 1.5;

protected:
	Ui::main_GUI *ui;    //--> the user interface
};

#endif /* MAIN_GUI_H_ */