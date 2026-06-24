#pragma once

#include <QQuickItem>

class QWindow;

class MpvVideoSurface : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qulonglong windowId READ windowId NOTIFY windowIdChanged)

public:
    explicit MpvVideoSurface(QQuickItem *parent = nullptr);
    ~MpvVideoSurface() override;

    qulonglong windowId() const;

signals:
    void windowIdChanged();

private:
    void handleWindowChanged(QQuickWindow *window);
    void ensureContainerWindow();
    void syncGeometry();

    QWindow *m_container = nullptr;
    qulonglong m_windowId = 0;
};
