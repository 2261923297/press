#include "FlowController.h"
#include <Windows.h>

#define def_appandar (LPWrap(FlowController::GetLogPanel().get()).get_ss())
#include "logger.h"
#include "structure.h"

#include <qobject.h>
#include <qserialportinfo.h>
#include <qthread.h>
#include <chrono>
#include <functional>
#include <sstream>





FlowController::FlowController(const std::string name
    , LogPanel* lp
    , int item_num
    , QWidget* parent)
{
//    ui.setupUi(this);
    m_flow_transer = FlowTranser::ptr(new FlowTranser);
    m_flow_transer->init("COM13");
    this->setParent(parent);

    m_item_num = item_num;
    this->resize(1200, 1000);
    m_title = new QLabel;
    m_title->setText("FlowController");
    m_title->setParent(this);
    m_title->setGeometry(0, 0, 200, 10);

    std::vector<std::string> mp = { "Air", "N2", "NO" };
    for (int i = 0; i < m_item_num; i++) {
        m_items.push_back(CtrItem::ptr(
            new CtrItem(std::string("flow_") 
                + mp[i])));
    }

    QObject::connect(&m_timer, &QTimer::timeout
        , this, &FlowController::show_cur_flow);
    init_window();
    m_timer.start(500);
}

FlowController::~FlowController() {
    if(nullptr != m_title)
        delete m_title;
}

void framing(uint8_t* datas, int len) {
    uint32_t crc = 0;
    for (int i = 0; i < len; i++) {
        crc += datas[i];
    }
    datas[len] = (uint8_t)crc;
    printf("crc = 0x%x\n", (uint8_t)crc);
}


void get_head(MFCHead* mfch) {
    mfch->addr =        0x20;
    mfch->stx =         0x02;
    mfch->rw =          0x81;
    mfch->len =         0x05;
    mfch->cmd_class =   0x69;
    mfch->instance =    0x01;
    mfch->attribute =   0xA4;
}

void mk_pkg_set_flow(MFCHead* mfch, uint16_t val, uint32_t insert) {
    val = 32768 * ((val + 0.00) / 10000);
    val += 0x4000;
    printf("vla = 0x%x\n", val);
    memcpy((uint8_t*)mfch + insert, &val, 2);
}

void FlowController::show_cur_flow()
{
    float flow = 0 ;
    for (auto it : m_configs) {
        float flow = 0;
        for (auto it : m_configs) {
            flow = 0;
            get_cur_flow(it->m_addr, flow);
            if (flow <= 0.00 || flow > 100.00) continue;
            std::stringstream ss;
            ss.precision(2);//¸²¸ÇÄ¬ÈÏ¾«¶È
            ss.setf(std::ios::fixed);
            ss << flow << "%";
            it->m_rt.setText(ss.str().c_str());
            //        QThread::usleep(10);
        }
    }
}


void get_cmd_set_flow(uint8_t* buffer, uint8_t addr, uint16_t val) {
    MFCHead mfc_h = { addr, 0x02, 0x81, 0x05, 0x69, 0x01, 0xA4 };
    memcpy(buffer, (uint8_t*)&mfc_h, 7);
    mk_pkg_set_flow((MFCHead*)buffer, val, 7);
    framing(buffer, 10);
}
void FlowController::set_flow(uint8_t addr, uint16_t val)
{
    uint32_t nRecv = 0;
    memset(m_send_buffer, 0, 100);
    get_cmd_set_flow(m_send_buffer, addr, val);
    print_bytes(m_send_buffer, 11);

//    __logger_debug << str_val(val) << "\n";
    m_flow_transer->set(m_send_buffer, 11);
}

void FlowController::enable_dig_mode()
{
    MFCHead mfch;
    mfch.addr = 0x20;
    mfch.stx = 0x02;
    mfch.rw = 0x81;
    mfch.len = 0x04;
    mfch.cmd_class = 0x6A;
    mfch.instance = 0x01;
    mfch.attribute = 0x01;
    memset(m_send_buffer, 0, 512);
    memcpy(m_send_buffer, &mfch, 8);

    framing(m_send_buffer, mfch.len + 5);
    uint32_t recv_len = 0;
    m_flow_transer->set(m_send_buffer, mfch.len + 6);
    print_bytes(m_send_buffer, mfch.len + 6);

    m_flow_transer->get(m_recv_buffer, recv_len);
    print_bytes(m_recv_buffer, recv_len);
}



void FlowController::init_window() {
    int i = 0;
    for (auto it : m_items) {
        auto new_config = 
            MFCConfigItem::ptr(new MFCConfigItem(it->m_name, i));
        new_config->m_addr = 0x20 + i;
        new_config->setParent(this);
        new_config->setGeometry(20, (int)(MFCCFG_H * i) + 20, MFCCFG_W, MFCCFG_H);
        m_configs.push_back(new_config);
        i++;

        std::function<void(uint8_t, uint16_t)> cb =
            std::bind(&FlowController::set_flow, this
                , std::placeholders::_1, std::placeholders::_2);
        new_config->m_cb = cb;
        
//        __logger_debug << str_val(i) << '\n';
    }
}

void FlowController::get_cur_flow(uint8_t addr, float& flow)
{
    const int cmd_len = 9;
    uint32_t n_recv;
    uint8_t cmd_get_flow[9] = { 
        addr, 0x02, 0x80
        , 0x03, 0x68, 0x01, 0xB9, 0x00, 0x0 };
    framing(cmd_get_flow, 8);
    MFC_ACK* rest;
    uint16_t n_flow = 0;
    float b_flow = 0.00;

//    print_bytes(cmd_get_flow, cmd_len);
    m_flow_transer->set(cmd_get_flow, cmd_len);
//    __logger_debug << "what set: \n";
//    print_bytes(cmd_get_flow, cmd_len);


    memset(m_recv_buffer, 0, 128);
    m_flow_transer->get(m_recv_buffer, n_recv);
    rest = (MFC_ACK*)m_recv_buffer;
    if (rest->ack == 0x06) {
        n_flow = (uint16_t)(*(uint16_t*)rest->datas - 0x4000);
        b_flow = ((n_flow) + 0.00) / m_flow_down;
        flow = b_flow * 100;
    }
}

void FlowController::set_valve(uint8_t open2_close1)
{
    MFCHead mfch;
    mfch.addr =         0x20;
    mfch.stx =          0x02;
    mfch.rw =           0x81;
    mfch.len =          0x04;
    mfch.cmd_class =    0x6A;
    mfch.instance =     0x01;
    mfch.attribute =    0x01;
    memset(m_send_buffer, 0, 512);
    memcpy(m_send_buffer, &mfch, 8);
    m_send_buffer[8] = open2_close1;

    framing(m_send_buffer, mfch.len + 5);
    uint32_t recv_len = 0;
    m_flow_transer->set(m_send_buffer, mfch.len + 6);
    print_bytes(m_send_buffer, mfch.len + 6);

    m_flow_transer->get(m_recv_buffer, recv_len);
    print_bytes(m_recv_buffer, recv_len);
}

void FlowController::get_all_addr()
{
    uint8_t cmd[] = { 0xff, 0x02, 0x80, 0x03, 0x03, 0x01, 0x01 };
    memset(m_send_buffer, 0, 100);
    memcpy(m_send_buffer, cmd, cmd[4] + 4);
    framing(m_send_buffer, cmd[4] + 5);
    uint32_t nrecv = 0;
    m_flow_transer->set(m_send_buffer, cmd[4] + 6);
    m_flow_transer->get(m_recv_buffer, nrecv);
    print_bytes(m_recv_buffer, nrecv);
    m_flow_transer->get(m_recv_buffer, nrecv);
    print_bytes(m_recv_buffer, nrecv);
    m_flow_transer->get(m_recv_buffer, nrecv);
    print_bytes(m_recv_buffer, nrecv);
}


CtrItem::CtrItem(const std::string& name
    , const std::string& rt
    , const std::string& input)
{
    m_name = name;
    m_rt = rt;
    m_input = input;
}

