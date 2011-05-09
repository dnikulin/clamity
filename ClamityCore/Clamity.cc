// Copyright 2010 Dmitri Nikulin, Enzo Reyes.
//
// This file is part of clamity.
//
// clamity is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// clamity is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with clamity.  If not, see <http://www.gnu.org/licenses/>.

#include "Clamity.hh"
#include "TestSuite.hh"

#include <boost/program_options.hpp>
#include <boost/asio/ip/host_name.hpp>


#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QPluginLoader>



Clamity::Clamity(std::ostream &_logfile,std::ofstream &_reportfile, cl::Device &_device,
                 unsigned int _maxDiv, double _epsilon, unsigned int _memDelta,
                 unsigned int _loglevel, unsigned int _maxalloc,
                 unsigned int _maxmem)
    :logfile(_logfile),reportfile(_reportfile), device(_device),
      memoryPoolFraction(_maxDiv), epsilonErrorMargin(_epsilon),
      memorySizeDelta(_memDelta),maxalloc(_maxalloc), maxmem(_maxmem) {

    // Retrieve platform from device
    platform = device.getInfo<CL_DEVICE_PLATFORM>();

    using boost::format;
    using boost::str;

    // Create context containing only this device
    std::vector<cl::Device> devices;
    devices.push_back(device);
    context = cl::Context(devices);

    // Create logger

    log = makeStreamLogger((LogLevel)_loglevel,"clamity",&_logfile);
    log(LOG_INFO, "Clamity OpenCL testing framework has started");
    log(LOG_INFO,str(format("Epsilon Error Margin set to : %f Memory Pool divisor: 1/%d") % epsilonErrorMargin % memoryPoolFraction));

    //Once we have created the logging system
    //We create the report system which will only output a
    //file based on clamity.<hostname>.report
    log(LOG_INFO,str(format("Creating Report file: clamity.%s.report") % boost::asio::ip::host_name()));

    //Create the instances of the report files
    testrun = makeTestOutput( &reportfile );
    testdiag = makeLineOutout( &reportfile );

}

void testDevice(Clamity &subject, TestSuites &suites) {
    using boost::format;
    using boost::str;

    Logger &log = subject.log;

    std::string name = subject.device.getInfo<CL_DEVICE_NAME>();

    log(LOG_INFO, str(format("Clamity testing started for '%s'") % name));

    // Log device info
    // In case of a bug report, this is critical
    subject.logInfo();

    // Run suites once per level, in decreasing order of importance.
    for (size_t nlevel = 0; nlevel < TestLevelCount; nlevel++) {
        const TestLevelData &level = TestLevels[nlevel];
        log(LOG_INFO, str(format("Starting %s tests") % level.name));
        foreach(TestSuite *suite, suites) {
            try {
                log(LOG_INFO, str(format("  Suite %s") % suite->suiteName()));
                suite->runTests(subject, level.level);
            } catch (cl::Error error) {
                 std::cout << "Test Failed --- ";
                 std::cout << error.what() << "(" << error.err() << ")" << std::endl;
                 subject.testrun(str(format("Suite %s ")% suite->suiteName()),false);
                 subject.testdiag(str(format("Error as follows: %s") % error.what()));
            }
        }
    }

    log(LOG_INFO, str(format("Clamity testing complete for '%s") % name));
}

void loadSuite(TestSuites *suites, QObject *object, const QString &path) {
    TestSuite *suite = qobject_cast<TestSuite *>(object);
    if (suite != NULL) {
        suites->push_back(suite);

        std::cerr
            << "Loaded suite '" << suite->suiteName()
            << "' from '" << path.toStdString()
            << "'" << std::endl << std::flush;
    }
}

void loadDynamicSuites(TestSuites *suites) {
    // Start from binary directory
    QDir root(qApp->applicationDirPath());

    // Move down to Plugins directory
    root.cd("Plugins");

    foreach (QString name, root.entryList(QDir::Files)) {
        QString path(root.absoluteFilePath(name));

        std::cerr
            << "Loading '" << path.toStdString()
            << "'" << std::endl << std::flush;

        QPluginLoader loader(path);

        if (loader.load() == false) {
            std::cerr
                << "  " << loader.errorString().toStdString()
                << std::endl << std::flush;
            continue;
        }

        QObject *object = loader.instance();
        if (object != NULL)
            loadSuite(suites, object, path);
    }
}

void loadStaticSuites(TestSuites *suites) {
    foreach (QObject *object, QPluginLoader::staticInstances())
        loadSuite(suites, object, "static plugin");
}
