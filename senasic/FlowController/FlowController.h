#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <qtimer.h>

#include <QtWidgets/QMainWindow>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qserialport.h>

#include "FlowTranser.h"
#include "MFCConfigItem.h"
#include "LogPanel.h"
#include "ui_FlowController.h"
#include "structure.h"
#include <memory>

#pragma comment(lib, "ws2_32.lib")


void framing(uint8_t* datas, int len);

class CtrItem {
public:
    using ptr = std::shared_ptr<CtrItem>;
   
public:
    std::string m_name;
    std::string m_rt;
    std::string m_input;
public:
    CtrItem(const std::string& name
        , const std::string& rt = ""
        , const std::string& input = "");
    
};

#define MFCCFG_W 400
#define MFCCFG_H 30

class FlowController : public QMainWindow
{
    Q_OBJECT

public:
    using ptr = std::shared_ptr<FlowController>;
    FlowController(const std::string name
        , LogPanel* lp = nullptr
        , int item_num = 3
        , QWidget *parent = Q_NULLPTR);
    ~FlowController();

signals:
    void start_get_flow();
public:
    static LogPanel::ptr GetLogPanel() { return s_lp; }
public:
    void init_window();
    void set_flow(uint8_t addr, uint16_t val);
    void show_cur_flow();
private:
    void get_cur_flow(uint8_t addr, float& flow);

    void enable_dig_mode();
    void set_valve(uint8_t open2_close1);
    void set_fclose() { set_valve(1); }
    void set_fopen() { set_valve(2); }

    void get_all_addr();
private:

    uint8_t  m_send_buffer[512];
    uint8_t  m_recv_buffer[512];

    Ui::FlowControllerClass ui;
    std::string m_name;
    uint32_t m_item_num;

    static LogPanel::ptr s_lp;
    QLabel* m_title;
    std::list<CtrItem::ptr> m_items;

    std::list<MFCConfigItem::ptr> m_configs;

    const uint16_t m_flow_down = 32768;
    FlowTranser::ptr m_flow_transer;
    HandleMFCFrame::ptr m_mfc_frame_handler;
    QTimer           m_timer;
};



