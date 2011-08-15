/***************************************************************************
 *
 * Project: libLAS -- C/C++ read/write library for LAS LIDAR data
 * Purpose: LAS translation with optional configuration
 * Author:  Howard Butler, hobu.inc at gmail.com
 ***************************************************************************
 * Copyright (c) 2010, Howard Butler, hobu.inc at gmail.com 
 *
 * See LICENSE.txt in this source distribution for more information.
 **************************************************************************/


#include <iostream>

#include <pdal/exceptions.hpp>
#include <pdal/FileUtils.hpp>
//#include <pdal/pdal_config.hpp>
//#include <pdal/Bounds.hpp>
//#include <pdal/Color.hpp>
//#include <pdal/Dimension.hpp>
//#include <pdal/Schema.hpp>
#include <pdal/filters/Chipper.hpp>

#include <pdal/filters/ReprojectionFilter.hpp>
#include <pdal/filters/ScalingFilter.hpp>

//#include <pdal/ColorFilter.hpp>
//#include <pdal/MosaicFilter.hpp>
//#include <pdal/FauxReader.hpp>
//#include <pdal/FauxWriter.hpp>
#include <pdal/drivers/las/Reader.hpp>
//#include <pdal/LasHeader.hpp>
#include <pdal/drivers/las/Writer.hpp>
#include <pdal/filters/CacheFilter.hpp>
#include <pdal/filters/ByteSwapFilter.hpp>

#ifdef PDAL_HAVE_LIBLAS
#include <pdal/drivers/liblas/Writer.hpp>
#include <pdal/drivers/liblas/Reader.hpp>
#endif

#ifdef PDAL_HAVE_ORACLE
#include <pdal/drivers/oci/Writer.hpp>
#include <pdal/drivers/oci/Reader.hpp>
#endif

#include <boost/property_tree/xml_parser.hpp>

#include "Application.hpp"

using namespace pdal;
namespace po = boost::program_options;


class Application_pc2pc : public Application
{
public:
    Application_pc2pc(int argc, char* argv[]);
    int execute();

private:
    void addOptions();
    bool validateOptions();

    std::string m_inputFile;
    std::string m_outputFile;
    std::string m_xml;
    std::string m_srs;
    bool m_bCompress;
    
};


Application_pc2pc::Application_pc2pc(int argc, char* argv[])
    : Application(argc, argv, "pc2pc")
{
}


bool Application_pc2pc::validateOptions()
{
    if (!hasOption("input"))
    {
        usageError("--input/-i required");
        return false;
    }

    if (!hasOption("output"))
    {
        usageError("--output/-o required");
        return false;
    }

    return true;
}


void Application_pc2pc::addOptions()
{
    po::options_description* file_options = new po::options_description("file options");

    file_options->add_options()
        ("input,i", po::value<std::string>(&m_inputFile), "input file name")
        ("output,o", po::value<std::string>(&m_outputFile), "output file name")
        ("native", "use native LAS classes (not liblas)")
        ("oracle-writer", "Read data from LAS file and write to Oracle")
        ("a_srs", po::value<std::string>(&m_srs)->default_value(""), "Assign output coordinate system")
        ("compress", po::value<bool>(&m_bCompress)->zero_tokens()->implicit_value(true),"Compress output data if available")
        ("xml", po::value<std::string>(&m_xml)->default_value("log.xml"), "XML file to load process (OCI only right now)")
        ;

    addOptionSet(file_options);
}

int Application_pc2pc::execute()
{
    if (!FileUtils::fileExists(m_inputFile))
    {
        runtimeError("file not found: " + m_inputFile);
        return 1;
    }

    std::ostream* ofs = FileUtils::createFile(m_outputFile);

    if (hasOption("native"))
    {
        pdal::drivers::las::LasReader reader(m_inputFile);
    
        const boost::uint64_t numPoints = reader.getNumPoints();

        pdal::drivers::las::LasWriter writer(reader, ofs);

        //BUG: handle laz writer.setCompressed(false);

        //writer.setPointFormat( reader.getPointFormatNumber() );

        writer.initialize();

        writer.write(numPoints);
    }

//     else if (hasOption("oracle-writer"))
//     {
// #ifdef PDAL_HAVE_ORACLE
//         try{
//         pdal::drivers::las::LasReader reader(m_inputFile);
//     
//         const boost::uint64_t numPoints = reader.getNumPoints();
// 
//         boost::property_tree::ptree load_tree;
//         
//         boost::property_tree::read_xml(m_xml, load_tree);
//         
//         boost::property_tree::ptree oracle_options = load_tree.get_child("pdal.drivers.oci.writer");
//         boost::property_tree::ptree las_options = load_tree.get_child("pdal.drivers.las");
//     
//         pdal::OptionsOld options(oracle_options);
//         
//         boost::property_tree::ptree in_srs_options = las_options.get_child("spatialreference");
//         std::string in_wkt = in_srs_options.get<std::string>("userinput");
//         boost::property_tree::ptree out_srs_options = oracle_options.get_child("spatialreference");
//         std::string out_wkt = out_srs_options.get<std::string>("userinput");
//         
//         pdal::SpatialReference in_ref(in_wkt);
//         pdal::SpatialReference out_ref(out_wkt);
//                 
//         boost::property_tree::ptree& tree = options.GetPTree();
//         
//         boost::uint32_t capacity = tree.get<boost::uint32_t>("capacity");
//         double scalex = oracle_options.get<double>("scale.x");
//         double scaley = oracle_options.get<double>("scale.y");
//         double scalez = oracle_options.get<double>("scale.z");
// 
//         double offsetx = oracle_options.get<double>("offset.x");
//         double offsety = oracle_options.get<double>("offset.y");
//         double offsetz = oracle_options.get<double>("offset.z");        
//         
//         pdal::filters::CacheFilter cache(reader, 1, capacity);
//         pdal::filters::Chipper chipper(cache, capacity);
//         pdal::filters::ScalingFilter scalingFilter(chipper);
//         pdal::filters::ReprojectionFilter reprojectionFilter(scalingFilter, in_ref, out_ref);
//         pdal::filters::DescalingFilter descalingFilter(reprojectionFilter,
//                                                        scalex, offsetx,
//                                                        scaley, offsety,
//                                                        scalez, offsetz);
//         
//         // pdal::filters::ByteSwapFilter swapper(descalingFilter);
//         pdal::drivers::oci::Writer writer(descalingFilter, options);
// 
//         writer.initialize();
// 
//         writer.write(numPoints);
//             
// 
// 
//         boost::property_tree::ptree output_tree;
//         // output_tree.put_child(writer.getName(), options.GetPTree());
//         // boost::property_tree::write_xml(m_xml, output_tree);
// 
//         } catch (pdal::pdal_error& e)
//         {
//             std::cerr << "Error writing oracle: " << e.what() << std::endl;
//             
//         }                    
// #else
//         throw configuration_error("PDAL not compiled with Oracle support");
// #endif
//     }
    //     else if (hasOption("oracle-reader"))
    //     {
    // #ifdef PDAL_HAVE_ORACLE
    //     try{
    //     boost::property_tree::ptree load_tree;
    //     
    //     boost::property_tree::read_xml(m_xml, load_tree);
    //     
    //     boost::property_tree::ptree oracle_options = load_tree.get_child("pdal.drivers.oci.reader");
    // 
    //     boost::property_tree::ptree las_options = load_tree.get_child("pdal.drivers.las");
    //     boost::property_tree::ptree srs_options = las_options.get_child("spatialreference");
    //     
    //     double scalex = las_options.get<double>("scale.x");
    //     double scaley = las_options.get<double>("scale.y");
    //     double scalez = las_options.get<double>("scale.z");
    // 
    //     double offsetx = las_options.get<double>("offset.x");
    //     double offsety = las_options.get<double>("offset.y");
    //     double offsetz = las_options.get<double>("offset.z");
    //     
    //     bool compress = las_options.get<bool>("compress");
    //     
    //     std::string out_wkt = srs_options.get<std::string>("userinput");
    //     pdal::OptionsOld options(oracle_options);
    // 
    // 
    // 
    //     pdal::drivers::oci::Reader reader_srs(options);
    //     reader_srs.initialize();
    //     
    //     pdal::drivers::oci::Reader reader(options);
    // 
    //     pdal::drivers::las::LasWriter* writer;
    // 
    //     pdal::SpatialReference out_ref(out_wkt);
    //     pdal::SpatialReference in_ref(reader_srs.getSpatialReference());
    //     if (!(in_ref == out_ref)) 
    //     {
    //         // pdal::filters::ByteSwapFilter swapper(reader);
    //         pdal::filters::ScalingFilter scalingFilter(reader);
    //         pdal::filters::ReprojectionFilter reprojectionFilter(scalingFilter, in_ref, out_ref);
    //         pdal::filters::DescalingFilter descalingFilter(reprojectionFilter, 
    //                                                        scalex, offsetx,
    //                                                        scaley, offsety,
    //                                                        scalez, offsetz);
    // 
    //         writer = new pdal::drivers::las::LasWriter(descalingFilter, ofs);
    //         if (compress)
    //             writer->setCompressed(true);
    //         writer->setChunkSize(oracle_options.get<boost::uint32_t>("capacity"));
    //         writer->setPointFormat(pdal::drivers::las::PointFormat3);
    //     
    //         writer->initialize();
    // 
    //         writer->write(0);
    //         delete writer;
    //     }
    //     else
    //     {
    //         writer = new pdal::drivers::las::LasWriter(reader, ofs);
    //         if (compress)
    //             writer->setCompressed(true);
    //         writer->setChunkSize(oracle_options.get<boost::uint32_t>("capacity"));
    //         writer->setPointFormat(pdal::drivers::las::PointFormat3);
    //     
    //         writer->initialize();
    // 
    //         writer->write(0);
    //         delete writer;
    //     }
    //     
    // 
    // 
    // 
    // 
    //         
    //     } catch (pdal::pdal_error& e)
    //     {
    //         std::cerr << "Error reading oracle: " << e.what() << std::endl;
    //         return 1;
    //         
    //     }
    //     
    //         
    // 
    // #else
    //         throw configuration_error("PDAL not compiled with Oracle support");
    // #endif
    //     }



    else
    {
#ifdef PDAL_HAVE_LIBLAS
        pdal::drivers::liblas::LiblasReader reader(m_inputFile);
    
        const boost::uint64_t numPoints = reader.getNumPoints();

        pdal::drivers::liblas::LiblasWriter writer(reader, ofs);

        //BUG: handle laz writer.setCompressed(false);

        writer.setPointFormat( reader.getPointFormat() );

        writer.initialize();

        writer.write(numPoints);
#endif
    }

    FileUtils::closeFile(ofs);

    return 0;
}

int main(int argc, char* argv[])
{
    Application_pc2pc app(argc, argv);
    return app.run();
}



