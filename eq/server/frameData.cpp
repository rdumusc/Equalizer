
/* Copyright (c) 2006-2009, Stefan Eilemann <eile@equalizergraphics.com>
 *                    2011, Daniel Nachbaur <danielnachbaur@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "frameData.h"

#include <co/dataIStream.h>
#include <co/dataOStream.h>

namespace eq
{
namespace server
{

FrameData::FrameData()
    : _frameNumber( 0 )
{
    _data.buffers = fabric::Frame::BUFFER_UNDEFINED;
    _data.frameType = fabric::Frame::TYPE_MEMORY;
}

void FrameData::getInstanceData( co::DataOStream& os )
{
    _data.serialize( os );
}

void FrameData::applyInstanceData( co::DataIStream& is )
{
    LBUNREACHABLE;
    _data.deserialize( is );
}

}
}
