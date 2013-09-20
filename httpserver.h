#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QMouseEvent>
#include <QKeyEvent>

class HttpServer : public QTcpServer
{
	Q_OBJECT
	QStringList m_commands;
	QTcpSocket *m_socket;

public:
	explicit HttpServer(QObject *parent = 0);

signals:
	receiveMouseEvent(QMouseEvent);
	receiveKeyEvent(QKeyEvent);

public slots:
	sendCommand(QByteArray commands);
};

#endif // HTTPSERVER_H
