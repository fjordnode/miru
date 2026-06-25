#pragma once

#include <QObject>
#include <QByteArray>
#include <QElapsedTimer>
#include <QStringList>
#include <QVariantMap>

class QLocalSocket;
class QProcess;
class QJsonArray;

class ExternalMpvPlayer : public QObject
{
    Q_OBJECT

public:
    explicit ExternalMpvPlayer(QObject *parent = nullptr);

    Q_INVOKABLE bool play(const QString &url, const QString &title,
                          const QVariantMap &headers = {},
                          const QStringList &subtitleUrls = {},
                          bool enableHwdec = true,
                          bool enableGpuNext = false,
                          bool enableHdrHint = false,
                          const QStringList &extraArgs = {},
                          double startSeconds = 0.0,
                          qulonglong windowId = 0);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void setPaused(bool paused);
    Q_INVOKABLE void seek(double seconds);
    Q_INVOKABLE void seekRelative(double seconds);

signals:
    void playbackStarted();   // mpv launched; stream still opening/buffering
    void playbackPlaying();    // first frame decoded; playback actually underway
    void positionChanged(double position, double duration);
    void pauseChanged(bool paused);
    void playbackFinished(double position, double duration);
    void errorOccurred(const QString &message);

private:
    void resetWatcher(bool emitFinished);
    void startWatcher(const QString &socketPath);
    void retryConnect();
    void handleReadyRead();
    bool sendCommand(const QJsonArray &command);
    void emitProgressIfDue(bool force = false);
    void finishPlayback();

    QLocalSocket *m_socket = nullptr;
    QProcess *m_process = nullptr;
    QByteArray m_readBuffer;
    QString m_socketPath;
    int m_connectAttempts = 0;
    int m_generation = 0;
    double m_lastPosition = 0.0;
    double m_lastDuration = 0.0;
    bool m_paused = false;
    bool m_finishEmitted = false;
    bool m_playingEmitted = false;
    QElapsedTimer m_progressTimer;
};
