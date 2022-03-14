#include "FlowTranser.h"
#include "FlowController.h"

#define def_appandar (LPWrap(FlowController::GetLogPanel().get()).get_ss())
#include "logger.h"
#include <qserialportinfo.h>
#include <QThread>

FlowTranser::FlowTranser()
{
    m_com = nullptr;
}

FlowTranser::~FlowTranser()
{
    if (m_com != nullptr) {
        delete m_com;
        m_com = nullptr;
    }
}

void FlowTranser::init(const std::string& com_name)
{
    __logger_debug << "init port" << "\n";
    for (auto& info : QSerialPortInfo::availablePorts()) {
        __logger_debug << "Name        : " << info.portName().toStdString()
            << "\nDescription : " << info.description().toStdString()
            << "\nManufacturer: " << info.manufacturer().toStdString();
    }

    m_com = new QSerialPort;
    m_com->setPortName(QString(com_name.c_str()));
    __logger_debug << str_val(com_name) << "\n";
    m_com->setBaudRate(QSerialPort::Baud19200);
    m_com->open(QIODevice::ReadWrite);
}

int FlowTranser::set(const uint8_t* datas, uint32_t len)
{
    int nWrite = m_com->write((const char*)datas, len);
    m_com->waitForBytesWritten();
    m_com->flush();
//    print_bytes(datas, len);
    if (nWrite != len) {
        fprintf(stderr, "com should write %d byte, but write %d byte"
            , len, nWrite);
        return -1;
    }
    else {
        
    }
    
    
    return 0;
}

int FlowTranser::get(uint8_t* rt, uint32_t& size)
{
    QByteArray tmp_qa, read_buffer;
    read_buffer.clear();
    do {
//        m_com->waitForReadyRead(50);
        QThread::usleep(100);
        tmp_qa = m_com->readAll();
        if (tmp_qa.size() <= 0) {
            break;
        }
        read_buffer.push_back(tmp_qa);
    } while (1);

    m_com->flush();
    size = read_buffer.size();
    
    if (size <= 0) {
//        __logger_error << "com cant read data\n";
        return -1;
    }
    else {
        memcpy(rt, read_buffer.constData(), size);
    }
//    print_bytes(rt, size);
    return 0;
}
