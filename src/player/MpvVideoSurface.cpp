#include "MpvVideoSurface.h"

#include <QQuickWindow>
#include <QWindow>

MpvVideoSurface::MpvVideoSurface(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, false);

    connect(this, &QQuickItem::windowChanged,
            this, &MpvVideoSurface::handleWindowChanged);
    connect(this, &QQuickItem::xChanged, this, &MpvVideoSurface::syncGeometry);
    connect(this, &QQuickItem::yChanged, this, &MpvVideoSurface::syncGeometry);
    connect(this, &QQuickItem::widthChanged, this, &MpvVideoSurface::syncGeometry);
    connect(this, &QQuickItem::heightChanged, this, &MpvVideoSurface::syncGeometry);
    connect(this, &QQuickItem::visibleChanged, this, &MpvVideoSurface::syncGeometry);
}

MpvVideoSurface::~MpvVideoSurface()
{
    delete m_container;
}

qulonglong MpvVideoSurface::windowId() const
{
    return m_windowId;
}

void MpvVideoSurface::handleWindowChanged(QQuickWindow *window)
{
    if (!window) {
        if (m_container) {
            m_container->hide();
            m_container->setParent(nullptr);
        }
        return;
    }

    ensureContainerWindow();
    m_container->setParent(window);
    syncGeometry();
}

void MpvVideoSurface::ensureContainerWindow()
{
    if (m_container) {
        return;
    }

    m_container = new QWindow;
    m_container->setFlags(Qt::FramelessWindowHint);
    m_container->create();

    const qulonglong id = static_cast<qulonglong>(m_container->winId());
    if (m_windowId != id) {
        m_windowId = id;
        emit windowIdChanged();
    }
}

void MpvVideoSurface::syncGeometry()
{
    if (!m_container || !window()) {
        return;
    }

    const QPointF scenePos = mapToScene(QPointF(0, 0));
    const QRect rect(qRound(scenePos.x()), qRound(scenePos.y()),
                     qMax(0, qRound(width())), qMax(0, qRound(height())));
    m_container->setGeometry(rect);

    if (isVisible() && rect.width() > 0 && rect.height() > 0) {
        m_container->show();
    } else {
        m_container->hide();
    }
}
