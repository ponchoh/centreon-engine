/*
 * Copyright 2019 Centreon (https://www.centreon.com/)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For more information : contact@centreon.com
 *
 */

#include <com/centreon/clib.hh>
#include <com/centreon/engine/checks/checker.hh>
#include <com/centreon/engine/configuration/applier/host.hh>
#include <com/centreon/engine/configuration/applier/state.hh>
#include <com/centreon/engine/configuration/parser.hh>
#include <com/centreon/engine/hostescalation.hh>
#include <com/centreon/engine/macros/grab_host.hh>
#include <com/centreon/engine/macros/process.hh>
#include <com/centreon/engine/macros.hh>
#include <com/centreon/engine/timezone_manager.hh>
#include <com/centreon/engine/broker/loader.hh>
#include <fstream>
#include <gtest/gtest.h>
#include "../timeperiod/utils.hh"

using namespace com::centreon;
using namespace com::centreon::engine;

extern configuration::state* config;

class Macro : public ::testing::Test {
 public:
  void SetUp() override {
    clib::load();
    com::centreon::logging::engine::load();
    if (config == nullptr)
      config = new configuration::state;
    timezone_manager::load();
    configuration::applier::state::load();  // Needed to create a contact
    broker::loader::load();
  }

  void TearDown() override {
    broker::loader::unload();
    configuration::applier::state::unload();
    checks::checker::unload();
    timezone_manager::unload();
    delete config;
    config = nullptr;
    com::centreon::logging::engine::unload();
    clib::unload();
  }
};

// Given host configuration without host_id
// Then the applier add_object throws an exception.
TEST_F(Macro, TotalServicesOkZero) {
  std::string out;
  nagios_macros mac;
  process_macros_r(&mac, "$TOTALSERVICESOK$", out, 0);
  ASSERT_EQ(out, "0");
}

// Given host configuration without host_id
// Then the applier add_object throws an exception.
TEST_F(Macro, TotalHostOk) {
  configuration::applier::host hst_aply;
  configuration::service svc;
  configuration::host hst;
  ASSERT_TRUE(hst.parse("host_name", "test_host"));
  ASSERT_TRUE(hst.parse("address", "127.0.0.1"));
  ASSERT_TRUE(hst.parse("_HOST_ID", "12"));
  ASSERT_NO_THROW(hst_aply.add_object(hst));
  ASSERT_EQ(1u, host::hosts.size());
  init_macros();

  nagios_macros mac;
  std::string out;
  host::hosts["test_host"]->set_current_state(host::state_up);
  host::hosts["test_host"]->set_has_been_checked(true);
  process_macros_r(&mac, "$TOTALHOSTSUP$", out, 1);
  ASSERT_EQ(out, "1");
}

// Given host configuration without host_id
// Then the applier add_object throws an exception.
TEST_F(Macro, TotalHostServicesCritical) {
  configuration::applier::host hst_aply;
  configuration::service svc;
  configuration::host hst;
  ASSERT_TRUE(hst.parse("host_name", "test_host"));
  ASSERT_TRUE(hst.parse("address", "127.0.0.1"));
  ASSERT_TRUE(hst.parse("_HOST_ID", "12"));
  ASSERT_NO_THROW(hst_aply.add_object(hst));
  ASSERT_EQ(1u, host::hosts.size());
  init_macros();

  nagios_macros mac;
  std::string out;
  host::hosts["test_host"]->set_current_state(host::state_up);
  host::hosts["test_host"]->set_has_been_checked(true);
  process_macros_r(&mac, "$TOTALHOSTSERVICESCRITICAL:test_host$", out, 1);
  ASSERT_EQ(out, "0");
}

// Given host configuration without host_id
// Then the applier add_object throws an exception.
TEST_F(Macro, TotalHostServicesCriticalError) {
  configuration::applier::host hst_aply;
  configuration::service svc;
  configuration::host hst;
  ASSERT_TRUE(hst.parse("host_name", "test_host"));
  ASSERT_TRUE(hst.parse("address", "127.0.0.1"));
  ASSERT_TRUE(hst.parse("_HOST_ID", "12"));
  ASSERT_NO_THROW(hst_aply.add_object(hst));
  ASSERT_EQ(1u, host::hosts.size());
  init_macros();

  nagios_macros mac;
  std::string out;
  host::hosts["test_host"]->set_current_state(host::state_up);
  host::hosts["test_host"]->set_has_been_checked(true);
  /* The call of this variable needs a host name */
  process_macros_r(&mac, "$TOTALHOSTSERVICESCRITICAL$", out, 1);
  ASSERT_EQ(out, "");
}

TEST_F(Macro, TimeT) {
  configuration::applier::host hst_aply;
  configuration::host hst;
  ASSERT_TRUE(hst.parse("host_name", "test_host"));
  ASSERT_TRUE(hst.parse("address", "127.0.0.1"));
  ASSERT_TRUE(hst.parse("_HOST_ID", "12"));
  ASSERT_NO_THROW(hst_aply.add_object(hst));
  ASSERT_EQ(1u, host::hosts.size());

  int now{500000000};
  set_time(now);
  init_macros();

  std::string out;
  nagios_macros* mac(get_global_macros());
  process_macros_r(mac, "$TIMET:test_host$", out, 0);
  ASSERT_EQ(out, "500000000");
}
