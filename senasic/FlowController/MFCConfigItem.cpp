#include "MFCConfigItem.h"
#include "FlowController.h"

#define def_appandar (LPWrap(FlowController::GetLogPanel().get()).get_ss())
#include "logger.h"


MFCConfigItem::MFCConfigItem(const std::string& name, uint32_t id)
{
    m_id = id;
    m_name.setText(name.c_str());
    m_ptn_send.setText("send");
    m_rt.setText("flow ins rt");

    m_name.setParent(this);
    m_rt.setParent(this);
    m_le.setParent(this);
    m_ptn_send.setParent(this);


    m_name.setGeometry(0, 0, 50, 20);
    m_rt.setGeometry(60, 0, 100, 20);
    m_le.setGeometry(180, 0, 100, 20);
    m_ptn_send.setGeometry(280, 0, 50, 20);

    QObject::connect(&m_ptn_send, &QPushButton::clicked
        , this, &MFCConfigItem::send_cb);
}
void MFCConfigItem::send_cb()
{
    __logger_debug << "send cb: \n"
        << dump()
        << std::endl;
    std::string val = m_le.text().toStdString();
    uint16_t real = 0;
    for (int i = 0; i < val.size(); i++) {
        real *= 10;
        real += val[i] - '0';
    }
    m_cb(m_addr, real);                     // FlowController::set_flow;
//    __logger_debug << str_val(real) << '\n';
}
const std::string MFCConfigItem::dump()
{
    std::string rt = "";
    rt = (m_name.text() + " "
        + std::to_string(m_id).c_str() + " "
        + m_rt.text() + " "
        + m_le.text()).toStdString();
    return rt;
}