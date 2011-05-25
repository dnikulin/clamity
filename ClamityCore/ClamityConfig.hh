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



#ifndef CLAMITYCONFIG_HH
#define CLAMITYCONFIG_HH

#include <fstream>
#include <iostream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

class ClamityConfig
{
private:
    friend class boost::serialization::access;
      template<class Archive> void serialize(Archive & ar,
             const unsigned int version) {
             using boost::serialization::make_nvp;
             ar & make_nvp("logLevel", logLevel);
             ar & make_nvp("availMem", deviceGlobalMemory);
             ar & make_nvp("maxAlloc", deviceMaxAllocation);
             ar & make_nvp("memDiv", memPoolDivisor);
             ar & make_nvp("memDelta", memDelta);
             ar & make_nvp("floatError", epsilonDelta);
             ar & make_nvp("skipCPU", skipCPU);
     }

public:
    static void CreateDefault(const char *);
    ClamityConfig();
    bool logToFile;
    unsigned int logLevel;
    unsigned int deviceGlobalMemory;
    unsigned int deviceMaxAllocation;
    unsigned int memPoolDivisor;
    unsigned int memDelta;
    double epsilonDelta;
    bool skipCPU;
};

#endif // CLAMITYCONFIG_HH
