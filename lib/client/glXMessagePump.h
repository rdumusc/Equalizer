
/* Copyright (c) 2007-2010, Stefan Eilemann <eile@equalizergraphics.com> 
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

#ifndef EQ_GLXMESSAGEPUMP_H
#define EQ_GLXMESSAGEPUMP_H

#include <eq/client/messagePump.h> // base class
#include <eq/client/os.h>          // X11

#include <eq/net/connectionSet.h>  // member

namespace eq
{
    /**
     * Implements a message pump for the X11 window system.
     */
    class GLXMessagePump : public MessagePump
    {
    public:
        GLXMessagePump();
        virtual ~GLXMessagePump();

        /** Wake up dispatchOne(). */
        virtual void postWakeup();

        /** Get and dispatch all pending system events, non-blocking. */
        virtual void dispatchAll();

        /** Get and dispatch at least one pending system event, blocking. */
        virtual void dispatchOne();
        
        /** Register a new Display connection for event dispatch. */
        void register_( Display* display );

        /** Deregister a Display connection from event dispatch. */
        void deregister( Display* display );

    private:
        net::ConnectionSet _connections; //!< Registered Display connections
        stde::hash_map< void*, size_t > _referenced; //!< # of registrations
    };
}

#endif //EQ_GLXMESSAGEPUMP_H
