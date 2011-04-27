/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS Schema implementation for C++ libLAS
 * Author:   Howard Butler, hobu.inc@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2010, Howard Butler
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following
 * conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department
 *       of Natural Resources nor the names of its contributors may be
 *       used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 ****************************************************************************/

#ifndef LIBPC_SCHEMA_HPP_INCLUDED
#define LIBPC_SCHEMA_HPP_INCLUDED

#include <libpc/libpc.hpp>

#include <vector>

#include <libpc/Dimension.hpp>


namespace libpc
{


/// Schema definition
class LIBPC_DLL Schema
{
public:
    typedef std::vector<Dimension> Dimensions;
    typedef std::vector<Dimension>::iterator DimensionsIter;
    typedef std::vector<Dimension>::const_iterator DimensionsCIter;

public:
    Schema();
    Schema(Schema const& other);

    Schema& operator=(Schema const& rhs);

    bool operator==(const Schema& other) const;
    bool operator!=(const Schema& other) const;

    void addDimension(Dimension const& dim);
    void addDimensions(const std::vector<Dimension>& dims);

    void removeDimension(Dimension const& dim);

    const Dimension& getDimension(std::size_t index) const;
    Dimension& getDimension(std::size_t index);
    const Dimensions& getDimensions() const;

    // returns the index of the field
    //
    // This function assumes the field is present and valid.  If not, it will throw.
    // (This behaviour is okay because looking up the diemsnion index is not expected 
    // to be on the critical path anywhere.)
    int getDimensionIndex(Dimension::Field field, Dimension::DataType datatype) const;
    int getDimensionIndex(const Dimension& dim) const;

    bool hasDimension(Dimension::Field field, Dimension::DataType datatype) const;
    bool hasDimension(const Dimension& dim) const;

    boost::property_tree::ptree getPTree() const;

    void dump() const;

private:
    std::vector<Dimension> m_dimensions;

    // this is a mapping from field name to index position in the
    // m_dimensions array (or -1 if field not present)
    int m_indexTable[Dimension::Field_LAST];
};


LIBPC_DLL std::ostream& operator<<(std::ostream& os, Schema const&);


} // namespace liblas

#endif // LIBPC_SCHEMA_HPP_INCLUDED
