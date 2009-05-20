
/* Copyright (c) 2008-2009, Stefan Eilemann <eile@equalizergraphics.com> 
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

#include "viewport.h"

#include <eq/client/pixelViewport.h>

EQ_EXPORT const eq::Viewport eq::Viewport::FULL;

namespace eq
{
void Viewport::applyView( const Viewport& segmentVP, const Viewport& viewVP,
                      const PixelViewport& pvp, const vmml::Vector4i& overdraw )
{
    // part of view covered by segment
    Viewport contribution( segmentVP );
    contribution.intersect( viewVP );
    contribution.transform( viewVP );
        
    // extend by overdraw percentage
    EQASSERT( pvp.hasArea( ));

    const float xDelta(( static_cast< float >( overdraw.x + pvp.w ) /
                         static_cast< float >( pvp.w ) - 1.0f ) * 
                       contribution.w );
    contribution.x -= xDelta;
    contribution.w += (( static_cast< float >( overdraw.z + pvp.w ) /
                         static_cast< float >( pvp.w ) - 1.0f ) * 
                       contribution.w ); 
    contribution.w += xDelta;

    const float yDelta(( static_cast< float >( overdraw.y + pvp.h ) /
                         static_cast< float >( pvp.h ) - 1.0f ) *
                       contribution.h ); 
    contribution.y -= yDelta;
    contribution.h += (( static_cast< float >( overdraw.w + pvp.h ) /
                         static_cast< float >( pvp.h ) - 1.0f ) *
                       contribution.h ); 
    contribution.h += yDelta;
    
    apply( contribution );
}


}
