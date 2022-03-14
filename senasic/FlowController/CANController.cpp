#include "CANController.h"
#include "structure.h"
#include "logger.h"
#include <qobject.h>
#include <time.h>
#include <tuple>
#include <QThread>
#include <iostream>

#define def_appandar (LPWrap(FlowController::GetLogPanel().get()).get_ss())
#include "logger.h"

CANController::CANController(QWidget *parent)
    : QWidget(parent)
{
//    ui.setupUi(this);

    m_title.setParent(this);
    m_title.setText("CANContraller");
    m_title.setGeometry(0, 0, 100, 20);
    m_n_item = 0;
    QObject::connect(&m_timer, &QTimer::timeout
        , this, &CANController::for_test);
    m_timer.start(500);
}
void CANController::for_test()
{
    static uint8_t show_waht[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
//    printf("for_test\n");
    for (int i = 0; i < m_n_item; i++) {
        for (int i = 0; i < 8; i++) {
            show_waht[i] = (uint8_t)(clock() % 128) + i;
        }
        show_data(i, show_waht, 8);
        const std::vector<std::tuple<QLabel*, QLineEdit*>*>&
            what_iput = get_input(i);
    }
}
void CANController::init(int n_config_item)
{
    m_n_item = n_config_item;
    for (int i = 0; i < n_config_item; i++) {
        m_config_items.push_back(new CANConfigItem(this));
        m_config_items[i]->init();
        m_config_items[i]->setGeometry(20, i * 60 + 30, 320, 60);
        m_config_items[i]->
            set_config_profix(std::to_string(i) + "#");
    }
}



CANConfigItem::CANConfigItem(QWidget* parent)
{
    this->setParent(parent);
}

void CANConfigItem::init()
{
    this->resize(800, 600);
    uint8_t init_show[] = { 0,0,0,0,0,0,0,0 };
    show_rt(init_show, 8);
    m_config_profix.setParent(this);
    m_config_profix.setGeometry(100, 10, 30, 20);

    std::vector<std::string> airs = { "O2", "NOx" };
    std::tuple<QLabel, QLineEdit> cur_item;
    for (int i = 0; i < 2; i++) {
        m_inputs.push_back(new std::tuple<QLabel*, QLineEdit*>);
        std::get<0>(*m_inputs[i]) = new QLabel(this);
        std::get<0>(*m_inputs[i])->setParent(this);
        std::get<0>(*m_inputs[i])->setText(m_config_profix.text() 
            + airs[i].c_str());
        printf("profix = %s\n", m_config_profix.text()
            .toStdString().c_str());
        std::get<0>(*m_inputs[i])->setGeometry(240 + i * 30 + 10, 0
                                                 , 30, 20);
        std::get<1>(*m_inputs[i]) = new QLineEdit(this);
        std::get<1>(*m_inputs[i])->setParent(this);
        std::get<1>(*m_inputs[i])->setGeometry(240 + i * 30 + 10, 30
            , 30, 20);
    }

}

void CANConfigItem::show_rt(const uint8_t* datas, size_t len)
{
    uint8_t tmp_hex[3] = { 0 };
//    printf("rts.size = %d\n", m_rts.size());
    for (int i = 0; i < m_rts.size(); i++) {
        
        CharToHex(datas[i], tmp_hex);
        QString txt((char*)tmp_hex);
        m_rts[i]->setText(txt);
//        std::cout << txt.toStdString() << ", ";
    }
    for (int i = m_rts.size(); i < len; i++) {
        m_rts.push_back(new QLabel(this));
        CharToHex(datas[i], tmp_hex);
       // printf("0x%x, ", datas[i]);
        
        m_rts[i]->setParent(this);
//        m_rts[i]->setText(txt);
        
        m_rts[i]->setGeometry(i * 30, 30, 30, 20);
    }
    QThread::usleep(0); 
}
