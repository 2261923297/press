#pragma once
#include <QWidget>
#include "CANController.h"
#include "FlowController.h"
#include "LogPanel.h"

#include "logger.h"

class MainWindow : public QWidget
{
public:
	MainWindow();
	~MainWindow();

	static LogPanel::ptr GetLogPanel() { return s_lp; }
public:

	void init();
private:
	CANController::ptr		m_can;
	FlowController::ptr		m_flow;

	static LogPanel::ptr	s_lp;
};

