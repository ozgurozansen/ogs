/**
 * \file
 * \author Karsten Rink
 * \date   2012-09-27
 * \brief  Implementation of readMeshFromFile function.
 *
 * \copyright
 * Copyright (c) 2012-2015, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 * @file readMeshFromFile.cpp
 * @date 2012-09-27
 * @author Karsten Rink
 */

#include "readMeshFromFile.h"

#ifdef USE_PETSC
#include <petsc.h>
#endif

#include <boost/algorithm/string/erase.hpp>

#include <logog/include/logog.hpp>

#include "BaseLib/FileTools.h"
#include "BaseLib/StringTools.h"

#include "MeshLib/Mesh.h"

#include "FileIO/Legacy/MeshIO.h"
#include "FileIO/VtkIO/VtuInterface.h"

#ifdef USE_PETSC
#include "FileIO/MPI_IO/NodePartitionedMeshReader.h"
#include "MeshLib/NodePartitionedMesh.h"
#endif

namespace FileIO
{
MeshLib::Mesh* readMeshFromFile(const std::string &file_name)
{
#ifdef USE_PETSC
	NodePartitionedMeshReader read_pmesh(PETSC_COMM_WORLD);

	std::size_t pos = file_name.find_last_of(".");
	std::string file_name_ext = file_name.substr(pos, file_name.size()-1);

	std::string const file_name_base =
	    boost::erase_last_copy(file_name, file_name_ext);
	return read_pmesh.read(file_name_base);
#else
	if (BaseLib::hasFileExtension("msh", file_name))
	{
		Legacy::MeshIO meshIO;
		return meshIO.loadMeshFromFile(file_name);
	}

	if (BaseLib::hasFileExtension("vtu", file_name))
		return VtuInterface::readVTUFile(file_name);

	ERR("readMeshFromFile(): Unknown mesh file format in file %s.", file_name.c_str());
	return nullptr;
#endif
}
} // end namespace FileIO
