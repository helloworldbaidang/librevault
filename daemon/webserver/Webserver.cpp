/* Copyright (C) 2017 Alexander Shishenko <alex@shishenko.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
#include "Webserver.h"

#include "UndefinedSession.h"
#include "control/Config.h"
#include <QDebug>
#include <QLoggingCategory>
#include <QTcpSocket>

namespace librevault {

Q_LOGGING_CATEGORY(log_webserver, "webserver")

Webserver::Webserver(QObject* parent) : QObject(parent) { server_ = new QTcpServer(this); }

void Webserver::start() {
  connect(server_, &QTcpServer::newConnection, this, &Webserver::handleConnection);

  server_->listen(QHostAddress::Any, Config::get()->getGlobals().control_listen);
  qCDebug(log_webserver) << "Started listening on port:" << server_->serverPort();
}

void Webserver::handleConnection() {
  QTcpSocket* sock = server_->nextPendingConnection();
  auto session =
      new UndefinedSession(sock, this);
  connect(session, &UndefinedSession::haveHttp, this, &Webserver::handleHttpSession);
  connect(
      session, &UndefinedSession::haveWebSocket, this, &Webserver::handleWebsocketSession);
  undefined_sessions_.insert(sock, session);
}

void Webserver::handleHttpSession(QTcpSocket* sock) {
  sock->setParent(this);
  if(undefined_sessions_.contains(sock)) {
    undefined_sessions_[sock]->deleteLater();
    undefined_sessions_.remove(sock);
  }

  qCDebug(log_webserver) << "Got HTTP session from" << sock->peerAddress();
  // handle
}

void Webserver::handleWebsocketSession(QTcpSocket* sock) {
  sock->setParent(this);
  undefined_sessions_.remove(sock);

  qCDebug(log_webserver) << "Got WebSocket session from" << sock->peerAddress();
  // handle
}

}  // namespace librevault
