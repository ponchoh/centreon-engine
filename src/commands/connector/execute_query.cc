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

#include "error.hh"
#include "commands/connector/execute_query.hh"

using namespace com::centreon::engine::commands::connector;

execute_query::execute_query(unsigned long cmd_id,
		 QString const& cmd,
		 QDateTime const& start_time,
		 int timeout)
  : request(request::execute_q),
    _cmd(cmd),
    _start_time(start_time),
    _cmd_id(cmd_id),
    _timeout(timeout) {

}

execute_query::execute_query(execute_query const& right)
  : request(right) {
  operator=(right);
}

execute_query::~execute_query() throw() {

}

execute_query& execute_query::operator=(execute_query const& right) {
  if (this != &right) {
    request::operator=(right);

    _cmd = right._cmd;
    _start_time = right._start_time;
    _id = right._id;
    _timeout = right._timeout;
  }
  return (*this);
}

request* execute_query::clone() const {
  return (new execute_query(*this));
}

QByteArray execute_query::build() {
  QByteArray query =
    QByteArray().setNum(_id) + '\0' +
    QByteArray().setNum(static_cast<qulonglong>(_cmd_id)) + '\0' +
    QByteArray().setNum(_timeout) + '\0' +
    QByteArray().setNum(_start_time.toMSecsSinceEpoch()) + '\0';
  query += _cmd;
  return (query + cmd_ending());
}

void execute_query::restore(QByteArray const& data) {
  QList<QByteArray> list = data.split('\0');
  if (list.size() < 5) {
    throw (engine_error() << "bad request argument.");
  }

  bool ok;
  unsigned int id = list[0].toUInt(&ok);
  if (ok == false || id != _id) {
    throw (engine_error() << "bad request id.");
  }

  _cmd_id = list[1].toULong(&ok);
  if (ok == false) {
    throw (engine_error() << "bad request argument, invalid cmd_id.");
  }

  _timeout = list[2].toInt(&ok);
  if (ok == false) {
    throw (engine_error() << "bad request argument, invalid timeout.");
  }

  qint64 timestamp = list[3].toLongLong(&ok);
  if (ok == false) {
    throw (engine_error() << "bad request argument, invalid start_time.");
  }
  _start_time.setMSecsSinceEpoch(timestamp);

  _cmd = list[4];
}

QString const& execute_query::get_command() const throw() {
 return (_cmd);
}

QStringList execute_query::get_args() const throw() {
  return (_cmd.simplified().split(' '));
}

QDateTime const& execute_query::get_start_time() const throw() {
 return (_start_time);
}

unsigned long execute_query::get_command_id() const throw() {
 return (_cmd_id);
}

int execute_query::get_timeout() const throw() {
 return (_timeout);
}