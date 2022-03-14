#pragma once

#include <QtWidgets/QWidget>
#include <QListWidget>
#include <sstream>
#include <iostream>

class LogPanel : public QListWidget {
    Q_OBJECT
public:
    using ptr = std::shared_ptr<LogPanel>;

    LogPanel(QWidget *parent = Q_NULLPTR) 
        : QListWidget(parent) { }

    ~LogPanel() { }
    void writeLine(const std::string& str) { this->addItem(str.c_str()); }
    
    LogPanel& operator<<(const std::string& t) {
        std::stringstream ss;
        writeLine(t);
        return *this;
    }

private:

};

class LPWrap {
public:
    LPWrap(LogPanel* lp) { m_ss.clear();  set_lp(lp); }
    ~LPWrap() { m_lp->writeLine(m_ss.str()); }

    void set_lp(LogPanel* lp) { m_lp = lp; }

    LPWrap& operator<<(const std::string&t) {
        m_ss << t;
        return *this;
    }
    std::stringstream& get_ss() { return m_ss;  }
private:
    std::stringstream m_ss;
    LogPanel* m_lp;
}; // class LPWrap

