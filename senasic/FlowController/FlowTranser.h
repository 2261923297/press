#pragma once
#include <memory>
#include <string>
#include <qserialport.h>

class FlowTranser {
public:
    using ptr = std::shared_ptr<FlowTranser>;
    FlowTranser();
    ~FlowTranser();

    void init(const std::string& com_name);
    int set(const uint8_t* datas, uint32_t len);
    int get(uint8_t* rt, uint32_t& size);


private:
    QSerialPort* m_com;
}; // class FlowTranser

