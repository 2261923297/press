#pragma once

#include <functional>
#include <qwidget.h>
#include <string>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>

// 一个配置项控制一个流量芯片
class MFCConfigItem : public QWidget {
public:
    using task_cb = std::function<void()>;
    using ptr = std::shared_ptr< MFCConfigItem>;
    MFCConfigItem(const std::string& name, uint32_t id);

    // 点击send按钮会调用此函数
    void send_cb();

    // 得到所有本 item 的成员内容
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

