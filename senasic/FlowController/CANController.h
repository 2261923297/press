#pragma once

#include <QtWidgets/QMainWindow>
#include <qlabel.h>
#include <qlineedit.h>
#include <QTimer>
#include <memory>
#include <qwidget.h>

#include "LogPanel.h"
#include "ui_CANController.h"
#include "LogPanel.h"

class CANConfigItem : public QWidget {
public:
    CANConfigItem(QWidget* parent);

    void set_config_profix(const std::string& profix) 
        { m_config_profix.setText(profix.c_str()); }
    void init();
    void show_rt(const uint8_t* datas, size_t len);
    const std::vector<std::tuple<QLabel*, QLineEdit*>*>& 
    get_inputs() const
         { return m_inputs; }
private:
    
    uint32_t m_rt_size_w;
    uint32_t m_rt_size_h;

    QLabel m_config_profix;             // n#
    std::vector<QLabel*> m_rts;          // show data

    uint32_t m_input_w;
    uint32_t m_input_h;
    // 
    std::vector<std::tuple<QLabel*, QLineEdit*>*> m_inputs;  // up down

}; // class CANConfigItem

class CANController : public QWidget
{
    Q_OBJECT
public:
    using ptr = std::shared_ptr< CANController>;
    CANController(QWidget *parent = Q_NULLPTR);
    void init(int n_config_item = 3);

    void 
    show_data(int witem, const uint8_t* datas, int len)
        { m_config_items[witem]->show_rt(datas, len); }

    const std::vector<std::tuple<QLabel*, QLineEdit*>*>&
    get_input(int witem) const 
        { return m_config_items[witem]->get_inputs(); }
public:
    static LogPanel::ptr GetLogPanel() { return s_lp; }

private:
    void for_test();

private:
    Ui::CANControllerClass ui;
    QLabel m_title;
    uint32_t m_n_item;
    std::vector<CANConfigItem*> m_config_items;
    QTimer m_timer;
    static LogPanel::ptr s_lp;
};
