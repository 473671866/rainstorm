#include "dispaly.h"

DispalyLabel::DispalyLabel(rfbClient *client, QWidget *parent)
    :QLabel(parent),m_client(client)
{
    setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);
}

int DispalyLabel::QtKeyToX11Key(int qtKey)
{
    switch (qtKey) {
    case Qt::Key_Escape: return XK_Escape;
    case Qt::Key_Tab: return XK_Tab;
    case Qt::Key_Backspace: return XK_BackSpace;
    case Qt::Key_Return: return XK_Return;
    case Qt::Key_Insert: return XK_Insert;
    case Qt::Key_Delete: return XK_Delete;
    case Qt::Key_Shift: return XK_Shift_L; // or XK_Shift_R
    case Qt::Key_Control: return XK_Control_L; // or XK_Control_R
    case Qt::Key_Alt: return XK_Alt_L; // or XK_Alt_R
    case Qt::Key_Home: return XK_Home;
    case Qt::Key_End: return XK_End;
    case Qt::Key_Left: return XK_Left;
    case Qt::Key_Up: return XK_Up;
    case Qt::Key_Right: return XK_Right;
    case Qt::Key_Down: return XK_Down;
    case Qt::Key_F1: return XK_F1;
    case Qt::Key_F2: return XK_F2;
    default: return qtKey;
    }
}

void DispalyLabel::keyPressEvent(QKeyEvent* event)  {

    if (!event->isAutoRepeat() && m_client) {  // Avoid repeating keys
        int key = QtKeyToX11Key(event->key());
        SendKeyEvent(m_client, key, true);  // Key down
    }

}

void DispalyLabel::keyReleaseEvent(QKeyEvent* event)  {
    if (!event->isAutoRepeat() && m_client) {  // Avoid repeating keys
        int key = QtKeyToX11Key(event->key());
        SendKeyEvent(m_client, key, false);  // Key up
    }
}

void DispalyLabel::mousePressEvent(QMouseEvent* event) {

    SendPointerEvent(m_client, event->x(), event->y(), 0);

    if (m_client && event) {
        int buttonMask = 0;
        SendPointerEvent(m_client, event->x(), event->y(), buttonMask);
        switch (event->button()) {
        case Qt::LeftButton:
            buttonMask = 1;
            break;
        case Qt::RightButton:
            buttonMask = 4;
            break;
        default:
            break;
        }

        SendPointerEvent(m_client, event->x(), event->y(), buttonMask);
    }
}

void DispalyLabel::mouseReleaseEvent(QMouseEvent* event) {
    if (m_client && event) {
        SendPointerEvent(m_client, event->x(), event->y(), 0);  // Button released
    }
}

void DispalyLabel::mouseMoveEvent(QMouseEvent* event) {
    if (m_client && event) {
        int buttonMask = 0;
        SendPointerEvent(m_client, event->x(), event->y(), buttonMask);
        if (event->buttons() & Qt::LeftButton){
             buttonMask |= 1;
        }
        if (event->buttons() & Qt::RightButton){
              buttonMask |= 4;
        }
        SendPointerEvent(m_client, event->x(), event->y(), buttonMask);
    }
}

void DispalyLabel::wheelEvent(QWheelEvent* event) {
    if (m_client && event) {
        int buttonMask = 0;
        QPoint position =  event->position().toPoint();

        if (event->angleDelta().y() > 0) {
            buttonMask = 8;
        } else if (event->angleDelta().y() < 0) {
            buttonMask = 16;
        }

        SendPointerEvent(m_client, position.x(), position.y(), buttonMask);
        SendPointerEvent(m_client, position.x(), position.y(), 0);
    }
}
