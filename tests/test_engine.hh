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

#ifndef TEST_ENGINE_HH
#define TEST_ENGINE_HH

#include <gtest/gtest.h>
#include "com/centreon/engine/configuration/contact.hh"
#include "com/centreon/engine/configuration/host.hh"
#include "com/centreon/engine/configuration/service.hh"

using namespace com::centreon::engine;

class TestEngine : public ::testing::Test {
 public:
  configuration::contact valid_contact_config() const;
  configuration::host new_configuration_host(std::string const& hostname,
                                             std::string const& contacts);
  configuration::service new_configuration_service(
      std::string const& hostname,
      std::string const& description,
      std::string const& contacts);
};

#endif /* !TEST_ENGINE_HH */