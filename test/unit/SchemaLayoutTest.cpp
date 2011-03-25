/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
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
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
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

#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>

#include <libpc/SchemaLayout.hpp>

using namespace libpc;

BOOST_AUTO_TEST_SUITE(SchemaLayoutTest)

BOOST_AUTO_TEST_CASE(test_layout)
{
    Dimension d1(Dimension::Field_X, Dimension::Uint32);
    Dimension d2(Dimension::Field_Y, Dimension::Uint32);

    Schema s1;
    s1.addDimension(d1);
    s1.addDimension(d2);
    BOOST_CHECK(s1.getDimensionIndex(Dimension::Field_X, Dimension::Uint32) == 0);
    BOOST_CHECK(s1.getDimensionIndex(Dimension::Field_Y, Dimension::Uint32) == 1);

    BOOST_CHECK(s1.hasDimension(Dimension::Field_X, Dimension::Uint8) == false);
    BOOST_CHECK(s1.hasDimension(Dimension::Field_Y, Dimension::Uint8) == false);

    Schema s2;
    s2.addDimension(d1);
    BOOST_CHECK(s2.hasDimension(Dimension::Field_X, Dimension::Uint32) == true);
    BOOST_CHECK(s2.getDimensionIndex(Dimension::Field_X, Dimension::Uint32) == 0);
    BOOST_CHECK(s2.hasDimension(Dimension::Field_Y, Dimension::Uint32) == false);

    SchemaLayout l1(s1);
    SchemaLayout l2(l1);
    SchemaLayout l3 = l1;
    SchemaLayout l4(s2);

    BOOST_CHECK(l1==l1);
    BOOST_CHECK(l1==l2);
    BOOST_CHECK(l2==l1);
    BOOST_CHECK(l1==l3);
    BOOST_CHECK(l3==l1);
    BOOST_CHECK(l1!=l4);
    BOOST_CHECK(l4!=l1);
}


BOOST_AUTO_TEST_CASE(test_layout_size)
{
    Dimension d1(Dimension::Field_X, Dimension::Uint32);
    Dimension d2(Dimension::Field_Y, Dimension::Uint32);
    Schema s1;
    s1.addDimension(d1);
    s1.addDimension(d2);
    SchemaLayout sl1(s1);

    const DimensionLayout& dl1 = sl1.getDimensionLayout(0);
    BOOST_CHECK(dl1.getDimension() == d1);
    BOOST_CHECK(dl1.getPosition() == 0);
    BOOST_CHECK(dl1.getByteOffset() == 0);

    const DimensionLayout& dl2 = sl1.getDimensionLayout(1);
    BOOST_CHECK(dl2.getDimension() == d2);
    BOOST_CHECK(dl2.getPosition() == 1);
    BOOST_CHECK(dl2.getByteOffset() == 4);
}


BOOST_AUTO_TEST_SUITE_END()
