#pragma once

#include <functional>
#include <qwidget.h>
#include <string>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>

// һ�����������һ������оƬ
class MFCConfigItem : public QWidget {
public:
    using task_cb = std::function<void()>;
    using ptr = std::shared_ptr< MFCConfigItem>;
    MFCConfigItem(const std::string& name, uint32_t id);

    // ���send��ť����ô˺���
    void send_cb();

    // �õ����б� item �ĳ�Ա����
    const std::string dump();
public:
    QLabel m_name;
    uint32_t m_id;

    QLabel m_rt;
    QLineEdit m_le;
    QPushButton m_ptn_send;
    uint8_t m_addr;
    
    std::function<void(uint8_t, uint16_t)> m_cb;
};

