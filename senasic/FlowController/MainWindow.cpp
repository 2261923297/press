#include "MainWindow.h"

MainWindow::MainWindow()
{
	m_can = CANController::ptr(new CANController(this));
	m_flow = FlowController::ptr(new FlowController(
		"FlowController"
		, nullptr, 3, this));

	
	m_flow->init_window();
	m_flow->setGeometry(10, 20, 400, 300);
	m_can->setGeometry(10, 220, 400, 300);
	m_can->setParent(this);
	m_can->init(3);

	s_lp->setParent(this);
	s_lp->setGeometry(500, 0, 600, 600);
	this->resize(1300, 600);
}

MainWindow::~MainWindow()
{

}

void MainWindow::init()
{
}
