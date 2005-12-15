
/* Copyright (c) 2005, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#include "channel.h"

#include "commands.h"
#include "global.h"
#include "nodeFactory.h"
#include "packets.h"
#include "channel.h"

using namespace eq;
using namespace std;

Channel::Channel()
        : eqNet::Base( CMD_CHANNEL_ALL ),
          _window(NULL)
{
    registerCommand( CMD_CHANNEL_INIT, this, reinterpret_cast<CommandFcn>(
                         &eq::Channel::_cmdInit ));
    registerCommand( CMD_CHANNEL_EXIT, this, reinterpret_cast<CommandFcn>( 
                         &eq::Channel::_cmdExit ));
}

Channel::~Channel()
{
}

//---------------------------------------------------------------------------
// command handlers
//---------------------------------------------------------------------------
void Channel::_cmdInit( eqNet::Node* node, const eqNet::Packet* pkg )
{
    ChannelInitPacket* packet = (ChannelInitPacket*)pkg;
    INFO << "handle channel init " << packet << endl;

    ChannelInitReplyPacket reply( packet );
    reply.result = init(); // XXX push to channel thread
    node->send( reply );
}

void Channel::_cmdExit( eqNet::Node* node, const eqNet::Packet* pkg )
{
    ChannelExitPacket* packet = (ChannelExitPacket*)pkg;
    INFO << "handle channel exit " << packet << endl;

    exit();

    ChannelExitReplyPacket reply( packet );
    node->send( reply );
}
