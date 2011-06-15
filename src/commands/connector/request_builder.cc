/*
** Copyright 2011      Merethis
**
** This file is part of Centreon Engine.
**
** Centreon Engine is free software: you can redistribute it and/or
** modify it under the terms of the GNU General Public License version 2
** as published by the Free Software Foundation.
**
** Centreon Engine is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Centreon Engine. If not, see
** <http://www.gnu.org/licenses/>.
*/

#include <QBuffer>
#include "error.hh"
#include "commands/connector/execute_query.hh"
#include "commands/connector/execute_response.hh"
#include "commands/connector/version_query.hh"
#include "commands/connector/version_response.hh"
#include "commands/connector/quit_query.hh"
#include "commands/connector/quit_response.hh"
#include "commands/connector/request_builder.hh"

using namespace com::centreon::engine::commands::connector;

request_builder& request_builder::instance() {
  static request_builder instance;
  return (instance);
}
#include <QDebug>
QSharedPointer<request> request_builder::build(QByteArray const& data) const {
  int pos = data.indexOf('\0');
  QByteArray tmp = data.left(pos < 0 ? data.size() : pos);

  bool ok;
  unsigned int req_id = tmp.toUInt(&ok);
  qDebug() << "res: " << data << " : " << data.indexOf('\0')  << " : " << tmp << " : " << req_id << " : " << ok << " : " << data.size();
  if (ok == false) {
    throw (engine_error() << "bad request id.");
  }

  QHash<unsigned int, QSharedPointer<request> >::const_iterator it = _list.find(req_id);
  if (it == _list.end()) {
    throw (engine_error() << "bad request id.");
  }

  QSharedPointer<request> ret((*it)->clone());
  ret->restore(data);
  return (ret);
}

request_builder::request_builder() {
  QSharedPointer<request> req(new version_query());
  _list.insert(req->get_id(), req);
  req = QSharedPointer<request>(new version_response());
  _list.insert(req->get_id(), req);
  req = QSharedPointer<request>(new execute_query());
  _list.insert(req->get_id(), req);
  req = QSharedPointer<request>(new execute_response());
  _list.insert(req->get_id(), req);

  req = QSharedPointer<request>(new quit_query());
  _list.insert(req->get_id(), req);
  req = QSharedPointer<request>(new quit_response());
  _list.insert(req->get_id(), req);
}

request_builder::~request_builder() throw() {

}
