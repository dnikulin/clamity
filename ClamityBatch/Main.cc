// Copyright 2010-2011 Dmitri Nikulin, Enzo Reyes.
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

#include "TestSuite.hh"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QPluginLoader>

#include <fstream>
#include <iostream>
#include <sstream>

Clamity::Clamity(std::ostream &_logfile, cl::Device &_device)
    : logfile(_logfile), device(_device) {

    // Retrieve platform from device
    platform = device.getInfo<CL_DEVICE_PLATFORM>();

    // Create context containing only this device
    std::vector<cl::Device> devices;
    devices.push_back(device);
    context = cl::Context(devices);

    // Create command queue
    queue = cl::CommandQueue(context, device);
    log = makeStreamLogger(LOG_DEBUG,"CLAMITY",&_logfile);
}

static void testDevice(Clamity &subject, TestSuites &suites) {
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
            log(LOG_INFO, str(format("  Suite %s") % suite->suiteName()));
            suite->runTests(subject, level.level);
        }
    }

    log(LOG_INFO, "Clamity testing complete");
}

static void loadSuite(TestSuites *suites, QObject *object, const QString &path) {
    TestSuite *suite = qobject_cast<TestSuite *>(object);
    if (suite != NULL) {
        suites->push_back(suite);

        std::cerr
            << "Loaded suite '" << suite->suiteName()
            << "' from '" << path.toStdString()
            << "'" << std::endl << std::flush;
    }
}

static void loadDynamicSuites(TestSuites *suites) {
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

static void loadStaticSuites(TestSuites *suites) {
    foreach (QObject *object, QPluginLoader::staticInstances())
        loadSuite(suites, object, "static plugin");
}

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    Q_UNUSED(app);

    // Vector of all test suites found via plugins.
    TestSuites suites;

    // Load static and dynamic test suite plugins.
    loadStaticSuites(&suites);
    loadDynamicSuites(&suites);

    // Discover all OpenCL platforms
    // In general, each platform belongs to a specific vendor
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    try {
        for (size_t nplatform = 0; nplatform < platforms.size(); nplatform++) {
            cl::Platform platform = platforms.at(nplatform);

            // Discover all OpenCL devices of any type
            std::vector<cl::Device> devices;
            platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

            for (size_t ndevice = 0; ndevice < devices.size(); ndevice++) {
                cl::Device device = devices.at(ndevice);

                // Invoke tests in order, log to stdout
                Clamity subject(std::cout, device);
                testDevice(subject, suites);
            }
        }
    } catch (cl::Error error) {
         std::cout << "Test Failed --- ";
         std::cout << error.what() << "(" << error.err() << ")" << std::endl;
    }

    return 0;
}
