/*
** Copyright 2011-2013 Merethis
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

#include <exception>
#include "com/centreon/engine/error.hh"
#include "com/centreon/engine/modules/external_commands/commands.hh"
#include "com/centreon/engine/globals.hh"
#include "com/centreon/engine/logging/engine.hh"
#include "test/unittest.hh"

using namespace com::centreon::engine;

/**
 *  Run enable_hostgroup_passive_svc_checks test.
 */
static int check_enable_hostgroup_passive_svc_checks(int argc, char** argv) {
  (void)argc;
  (void)argv;

  init_object_skiplists();

  host* hst = add_host("name", NULL, NULL, "localhost", NULL, 0, 0.0, 0.0, 42,
                       0, 0, 0, 0, 0, 0.0, 0.0, NULL, 0, NULL, 0, 0, NULL, 0,
                       0, 0.0, 0.0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, NULL,
                       NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0.0, 0.0,
                       0.0, 0, 0, 0, 0, 0);
  if (!hst)
    throw (engine_error() << "create host failed.");

  service* svc = add_service("name", "description", NULL,
                             NULL, 0, 42, 0, 0, 0, 42.0, 0.0, 0.0, NULL,
                             0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, "command", 0, 0,
                             0.0, 0.0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL,
                             0, 0, NULL, NULL, NULL, NULL, NULL,
                             0, 0, 0);
  if (!svc)
    throw (engine_error() << "create service failed.");

  hostgroup* group = add_hostgroup("group", NULL, NULL, NULL, NULL);
  if (!group)
    throw (engine_error() << "create hostgroup failed.");

  hostsmember* hmember = add_host_to_hostgroup(group, "name");
  if (!hmember)
    throw (engine_error() << "host link to hostgroup.");
  hmember->host_ptr = hst;

  servicesmember* smember = add_service_link_to_host(hst, svc);
  if (!smember)
    throw (engine_error() << "service link to servicegroup.");
  smember->service_ptr = svc;

  svc->accept_passive_service_checks = false;
  char const* cmd("[1317196300] ENABLE_HOSTGROUP_PASSIVE_SVC_CHECKS;group");
  process_external_command(cmd);

  if (!svc->accept_passive_service_checks)
    throw (engine_error() << "enable_hostgroup_passive_svc_checks failed.");

  delete[] smember->host_name;
  delete[] smember->service_description;
  delete smember;

  delete[] hmember->host_name;
  delete hmember;

  delete[] group->group_name;
  delete[] group->alias;
  delete group;

  delete[] svc->host_name;
  delete[] svc->description;
  delete[] svc->service_check_command;
  delete[] svc->display_name;
  delete svc;

  delete[] hst->name;
  delete[] hst->display_name;
  delete[] hst->alias;
  delete[] hst->address;
  delete hst;

  free_object_skiplists();

  return (0);
}

/**
 *  Init unit test.
 */
int main(int argc, char** argv) {
  unittest utest(argc, argv, &check_enable_hostgroup_passive_svc_checks);
  return (utest.run());
}
