#include <iostream>
#include <sstream>
#include "CondCore/Utilities/interface/PayloadInspector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/standard.h"
#include "FWCore/PluginManager/interface/SharedLibrary.h"
#include "FWCore/ServiceRegistry/interface/ServiceRegistry.h"
#include "CondFormats/PPSObjects/interface/PPSTimingCalibration.h"
#include "CondCore/CTPPSPlugins/interface/PPSTimingCalibrationPayloadInspectorHelper.h"

int main(int argc, char** argv) {
  Py_Initialize();

  edmplugin::PluginManager::Config config;
  edmplugin::PluginManager::configure(edmplugin::standard::config());

  std::vector<edm::ParameterSet> psets;
  edm::ParameterSet pSet;
  pSet.addParameter("@service_type", std::string("SiteLocalConfigService"));
  psets.push_back(pSet);
  edm::ServiceToken servToken(edm::ServiceRegistry::createSet(psets));
  edm::ServiceRegistry::Operate operate(servToken);

  std::string connectionString("frontier://FrontierProd/CMS_CONDITIONS");

  // PPSTimimg
  if (argc == 4) {
    std::string tag = argv[1];
    cond::Time_t start = static_cast<unsigned long long>(std::strtol(argv[2], &argv[2], 10));
    cond::Time_t end = static_cast<unsigned long long>(std::strtol(argv[3], &argv[3], 10));

    edm::LogPrint("testPPSCalibrationPI") << "## Exercising TimingCalibration plots ";

    ParametersPerChannel<PPSTimingCalibrationPI::db0,
                         PPSTimingCalibrationPI::plane0,
                         PPSTimingCalibrationPI::parameter0,
                         PPSTimingCalibration>
        test;
    test.process(connectionString, PI::mk_input(tag, start, end));
    edm::LogPrint("testparametersPerChannel") << test.data();
  } else {
    edm::LogPrint("testparametersPerChannel") << "Wrong arguments";
  }

  Py_Finalize();
}
