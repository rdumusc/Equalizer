
/* Copyright (c) 2007-2009, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#include "compoundInitVisitor.h"

#include "config.h"
#include "frame.h"

#include <eq/client/log.h>

using namespace std;
using namespace eq::base;

namespace eq
{
namespace server
{
CompoundInitVisitor::CompoundInitVisitor()
{}

VisitorResult CompoundInitVisitor::visit( Compound* compound )
{
    Channel* channel = compound->getChannel();
    if( channel )
    {
        channel->refUsed();

        if( compound->getLatestView() != eq::View::TYPE_NONE )
        {
            const eq::View& view = compound->getView();
            EQASSERT( view.getID() != EQ_ID_INVALID );
            EQASSERTINFO( !channel->getView(),
                          "Multiple views per channel are not supported" );
            channel->setView( &view );
        }
    }

    Config*        config  = compound->getConfig();
    const uint32_t latency = config->getLatency();
    EQASSERT( config );
    
    const std::vector< Frame* >& outputFrames = compound->getOutputFrames();
    for( vector<Frame*>::const_iterator i = outputFrames.begin(); 
         i != outputFrames.end(); ++i )
    {
        Frame* frame = *i;
        config->registerObject( frame );
        frame->setAutoObsolete( latency );
        EQLOG( eq::LOG_ASSEMBLY ) << "Output frame \"" << frame->getName() 
                                  << "\" id " << frame->getID() << endl;
    }

    const std::vector< Frame* >& inputFrames = compound->getInputFrames();
    for( vector<Frame*>::const_iterator i = inputFrames.begin(); 
         i != inputFrames.end(); ++i )
    {
        Frame* frame = *i;
        config->registerObject( frame );
        frame->setAutoObsolete( latency );
        EQLOG( eq::LOG_ASSEMBLY ) << "Input frame \"" << frame->getName() 
                                  << "\" id " << frame->getID() << endl;
    }

    compound->updateInheritData( 0 ); // set up initial values
    if( channel )
        channel->addTasks( compound->getInheritTasks( ));

    return TRAVERSE_CONTINUE;    
}

}
}
