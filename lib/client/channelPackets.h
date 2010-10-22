
/* Copyright (c) 2005-2010, Stefan Eilemann <eile@equalizergraphics.com>
 *                    2010, Cedric Stalder  <cedric.stalder@gmail.com>
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

#ifndef EQ_CHANNELPACKETS_H
#define EQ_CHANNELPACKETS_H

#include <eq/client/packets.h> // base structs

#include <eq/client/statistic.h>     // member

/** @cond IGNORE */
namespace eq
{
    struct ChannelConfigInitPacket : public ChannelPacket
    {
        ChannelConfigInitPacket()
            {
                command = fabric::CMD_CHANNEL_CONFIG_INIT;
                size    = sizeof( ChannelConfigInitPacket );
            }

        uint32_t        initID;
    };

    struct ChannelConfigInitReplyPacket : public ChannelPacket
    {
        ChannelConfigInitReplyPacket()
            {
                command   = fabric::CMD_CHANNEL_CONFIG_INIT_REPLY;
                size      = sizeof( ChannelConfigInitReplyPacket );
            }

        bool result;
    };

    struct ChannelConfigExitPacket : public ChannelPacket
    {
        ChannelConfigExitPacket()
            {
                command = fabric::CMD_CHANNEL_CONFIG_EXIT;
                size    = sizeof( ChannelConfigExitPacket );
            }
    };

    struct ChannelConfigExitReplyPacket : public ChannelPacket
    {
        ChannelConfigExitReplyPacket( const uint32_t channelID, const bool res )
                : result( res )
            {
                command   = fabric::CMD_CHANNEL_CONFIG_EXIT_REPLY;
                size      = sizeof( ChannelConfigExitReplyPacket );
                objectID  = channelID;
            }

        const bool result;
    };



    struct ChannelFrameStartPacket : public ChannelTaskPacket
    {
        ChannelFrameStartPacket()
            {
                command        = fabric::CMD_CHANNEL_FRAME_START;
                size           = sizeof( ChannelFrameStartPacket );
            }

        uint32_t frameNumber;
        uint32_t version;
    };

    struct ChannelFrameFinishPacket : public ChannelTaskPacket
    {
        ChannelFrameFinishPacket()
            {
                command        = fabric::CMD_CHANNEL_FRAME_FINISH;
                size           = sizeof( ChannelFrameFinishPacket );
            }

        uint32_t frameNumber;
    };

    struct ChannelFrameFinishReplyPacket : public ChannelPacket
    {
        ChannelFrameFinishReplyPacket( )
            {
                command     = fabric::CMD_CHANNEL_FRAME_FINISH_REPLY;
                size        = sizeof( ChannelFrameFinishReplyPacket );
            }

        uint32_t frameNumber;
        uint32_t nStatistics;
        EQ_ALIGN8( Statistic statistics[1] );
    };
        

    struct ChannelFrameDrawFinishPacket : public ChannelPacket
    {
        ChannelFrameDrawFinishPacket()
            {
                command       = fabric::CMD_CHANNEL_FRAME_DRAW_FINISH;
                size          = sizeof( ChannelFrameDrawFinishPacket );
            }

        uint32_t frameID;
        uint32_t frameNumber;
    };
        
    struct ChannelFrameClearPacket : public ChannelTaskPacket
    {
        ChannelFrameClearPacket()
            {
                command       = fabric::CMD_CHANNEL_FRAME_CLEAR;
                size          = sizeof( ChannelFrameClearPacket );
            }
    };
        
    struct ChannelFrameDrawPacket : public ChannelTaskPacket
    {
        ChannelFrameDrawPacket()
            {
                command       = fabric::CMD_CHANNEL_FRAME_DRAW;
                size          = sizeof( ChannelFrameDrawPacket );
            }
    };
        
    struct ChannelFrameAssemblePacket : public ChannelTaskPacket
    {
        ChannelFrameAssemblePacket()
            {
                command       = fabric::CMD_CHANNEL_FRAME_ASSEMBLE;
                size          = sizeof( ChannelFrameAssemblePacket );
            }

        uint32_t             nFrames;
        EQ_ALIGN8( net::ObjectVersion frames[1] );
    };
        
    struct ChannelFrameReadbackPacket : public ChannelTaskPacket
    {
        ChannelFrameReadbackPacket()
            {
                command       = fabric::CMD_CHANNEL_FRAME_READBACK;
                size          = sizeof( ChannelFrameReadbackPacket );
            }

        uint32_t             nFrames;
        EQ_ALIGN8( net::ObjectVersion frames[1] );
    };
        
    struct ChannelFrameTransmitPacket : public ChannelTaskPacket
    {
        ChannelFrameTransmitPacket()
            {
                command       = fabric::CMD_CHANNEL_FRAME_TRANSMIT;
                size          = sizeof( ChannelFrameTransmitPacket );
            }

        
        net::ObjectVersion frame;
        uint32_t           nNodes;
        EQ_ALIGN8( net::NodeID nodes[1] );
    };

    struct ChannelFrameViewStartPacket : public ChannelTaskPacket
    {
        ChannelFrameViewStartPacket()
            {
                command       = fabric::CMD_CHANNEL_FRAME_VIEW_START;
                size          = sizeof( ChannelFrameViewStartPacket );
            }
    };
        
    struct ChannelFrameViewFinishPacket : public ChannelTaskPacket
    {
        ChannelFrameViewFinishPacket()
            {
                command       = fabric::CMD_CHANNEL_FRAME_VIEW_FINISH;
                size          = sizeof( ChannelFrameViewFinishPacket );
            }
    };
    
    inline std::ostream& operator << ( std::ostream& os, 
                                    const ChannelConfigInitReplyPacket* packet )
    {
        os << (net::ObjectPacket*)packet << " result " << packet->result;
        return os;
    }

    inline std::ostream& operator << ( std::ostream& os, 
                                       const ChannelFrameStartPacket* packet )
    {
        os << (net::ObjectPacket*)packet << " frame " << packet->frameNumber;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                   const ChannelFrameFinishReplyPacket* packet )
    {
        os << (net::ObjectPacket*)packet << " frame " << packet->frameNumber;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                    const ChannelFrameDrawFinishPacket* packet )
    {
        os << (net::ObjectPacket*)packet << " frame " << packet->frameNumber
           << " id " << packet->frameID;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                      const ChannelFrameReadbackPacket* packet )
    {
        os << (ChannelTaskPacket*)packet << " nFrames " << packet->nFrames;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                      const ChannelFrameTransmitPacket* packet )
    {
        os << (net::ObjectPacket*)packet << " frame " << packet->frame
           << " nNodes " << packet->nNodes;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                      const ChannelFrameAssemblePacket* packet )
    {
        os << (ChannelTaskPacket*)packet << " nFrames " << packet->nFrames;
        return os;
    }
}
#endif //EQ_CHANNELPACKETS_H