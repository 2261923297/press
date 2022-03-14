#include "FlowController.h"
#include <QtWidgets/QApplication>
#include <QThread>

#include <functional>
#include <thread>
#include "CANController.h"
#include "LogPanel.h"
#include "Instance.h"
#include "MainWindow.h"

#define def_appandar (LPWrap(MainWindow::GetLogPanel().get()).get_ss())
#include "logger.h"

int a;
QApplication for_init(a, nullptr);
LogPanel::ptr FlowController::s_lp =
    LogPanel::ptr(tt::pattern::GetPoint<LogPanel>());

LogPanel::ptr MainWindow::s_lp =
    LogPanel::ptr(tt::pattern::GetPoint<LogPanel>());

LogPanel::ptr CANController::s_lp =
    LogPanel::ptr(tt::pattern::GetPoint<LogPanel>());

void for_test_cmd() {
    char addr = 0x20, stx = 0x02, service_type = 0x80;
    std::vector<const int8_t*> cmds;

    FlowTranser ft;
    uint8_t read_buffer[128] = { 0 };
    uint8_t get_current_flow[] =
    { 0x20, 0x02, 0x80, 0x03, 0x68, 0x01, 0xB9, 0x00, };
    framing(get_current_flow, 8);
    // rt: 06 00 02 80 05 68 01 B9 3D 4F 00 35

    uint32_t n_read = 0;
    
    ft.init("COM12");
    ft.set(get_current_flow, 9);
    ft.get(read_buffer, n_read);

    printf("what recv: ");
    
    for (int i = 0; i < n_read; i++) {
        printf("0x%x, ", (uint8_t)read_buffer[i]);
    }
    printf("\n");
    
}
void test_tpl() {

}

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    for_test_cmd();
//    FlowController w("FlowController", nullptr, 3);
    MainWindow w;
//    CANController w;
    w.init();
//    __logger_debug << "test_logger" << "\n";
//    w.get_all_addr();
    w.show();

    return for_init.exec();
}
