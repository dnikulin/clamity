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




#include "ClamityConfig.hh"
#include "Clamity.hh"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QPluginLoader>


//Initialise with the default options
ClamityConfig::ClamityConfig()
{
    this->deviceGlobalMemory = 0;
    this->epsilonDelta = 1e-5;
    this->deviceMaxAllocation = 0;
    this->memDelta = 10;
    this->memPoolDivisor = 1;
    this->skipCPU = false;
    this->logLevel = LOG_INFO;

}

//Creates a config file with the default options set
void ClamityConfig::CreateDefault(const char * path) {
    std::string file(path);
    file.append("/clamity.xml");
    std::ofstream configFile(file.c_str());
    boost::archive::xml_oarchive oa(configFile);
    ClamityConfig defaultConfig;
    oa << BOOST_SERIALIZATION_NVP(defaultConfig);
    configFile.flush();
    configFile.close();
}
