#ifndef DISPALY_H
#define DISPALY_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include "rfbclient.h"

class DispalyLabel : public QLabel {
    Q_OBJECT

public:
    explicit DispalyLabel(rfbClient* client, QWidget *parent = nullptr);

public:
    int QtKeyToX11Key(int qtKey);
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
private:
    rfbClient* m_client;
};

#endif // VNCDISPLAYLABEL_H

// vncdisplaylabel.cpp
