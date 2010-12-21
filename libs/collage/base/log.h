
/* Copyright (c) 2005-2010, Stefan Eilemann <eile@equalizergraphics.com> 
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

/**
 * @file base/log.h
 * 
 * This file contains the logging classes for Equalizer. The macros EQERROR,
 * EQWARN, EQINFO and EQVERB output messages at their respective logging level,
 * if the level is active. They use a per-thread base::Log instance, which is a
 * std::ostream. EQVERB is always inactive in release builds.
 */

#ifndef EQBASE_LOG_H
#define EQBASE_LOG_H

#include <co/base/os.h>
#include <co/base/clock.h>

#include <assert.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <time.h>

#ifdef WIN32_API
#  include <process.h>
#  define getpid _getpid
#endif

namespace eq
{
namespace base
{
    /** The logging levels. @version 1.0 */
    enum LogLevel
    {
        LOG_ERROR = 1, //!< Output critical errors
        LOG_WARN,      //!< Output potentially critical warnings
        LOG_INFO,      //!< Output informational messages
        LOG_VERB,      //!< Be noisy
        LOG_ALL
    };

    /** 
     * The logging topics.
     * 
     * @sa net/log.h, client/log.h
     * @version 1.0
     */
    enum LogTopic
    {
        LOG_PLUGIN = 0x1,        //!< Plugin usage (1)
        LOG_CUSTOM = 0x10,       //!< Log topics for other namespaces start here
        LOG_ANY    = 0xffffu     //!< Log all Equalizer topics
    };

    /** The string buffer used for logging. @internal */
    class LogBuffer : public std::streambuf
    {
    public:
        LogBuffer( std::ostream& stream )
                : _line(0), _indent(0), _blocked(0), _noHeader(0), 
                  _newLine(true), _stream(stream)
            {}
        virtual ~LogBuffer() {}

        void indent() { ++_indent; }
        void exdent() { --_indent; }

        void disableFlush() { ++_blocked; } // use counted variable to allow
        void enableFlush()                  //   nested enable/disable calls
            { 
                assert( _blocked );// Too many enableFlush on log stream
                --_blocked;
            }

        void disableHeader() { ++_noHeader; } // use counted variable to allow
        void enableHeader()  { --_noHeader; } //   nested enable/disable calls

#ifdef _WIN32
        void setLogInfo( const char* subdir, const char* file, const int line )
            { _file = file; _line = line; } // SUBDIR not needed on WIN32
#else
        void setLogInfo( const char* subdir, const char* file, const int line )
            { _file = std::string( subdir ) + '/' + file; _line = line; }
#endif

    protected:
        virtual int_type overflow( LogBuffer::int_type c );
        
        virtual int sync() 
            {
                if( !_blocked )
                {
                    const std::string& string = _stringStream.str();
                    _stream.write( string.c_str(), string.length( ));
                    _stringStream.str( "" );
                }
                _newLine = true;
                return 0;
            }

    private:
        LogBuffer( const LogBuffer& );
        LogBuffer& operator = ( const LogBuffer& );

        /** The current file logging. */
        std::string _file;

        /** The current line logging. */
        int _line;

        /** The current indentation level. */
        int _indent;

        /** Flush reference counter. */
        int _blocked;

        /** The header disable counter. */
        int _noHeader;

        /** The flag that a new line has started. */
        bool _newLine;

        /** The temporary buffer. */
        std::ostringstream _stringStream;

        /** The wrapped ostream. */
        std::ostream& _stream;
    };

    /** The logging class. @internal */
    class Log : public std::ostream
    {
    public:

        Log() : std::ostream( &_logBuffer ), _logBuffer( getOutput( )){}
        virtual ~Log() { _logBuffer.pubsync(); }

        void indent() { _logBuffer.indent(); }
        void exdent() { _logBuffer.exdent(); }
        void disableFlush() { _logBuffer.disableFlush(); }
        void enableFlush()  { _logBuffer.enableFlush();  }
        void forceFlush()  { _logBuffer.pubsync();  }
        void disableHeader() { _logBuffer.disableHeader(); }
        void enableHeader()  { _logBuffer.enableHeader();  }

        /** The current log level. */
        static EQBASE_API int level;

        /** The current log topics. */
        static EQBASE_API unsigned topics;

        /** The per-thread logger. */
        static EQBASE_API Log& instance( const char* subdir, const char* file,
                                        const int line );

        /** Exit the log instance for the current thread. */
        static EQBASE_API void exit();

        /** The string representation of the current log level. */
        static std::string& getLogLevelString();

        /** Change the output stream. */
        static EQBASE_API void setOutput( std::ostream& stream );

        /** Get the current output stream. @internal */
        static EQBASE_API std::ostream& getOutput ();

        /**
         * Set the reference clock.
         *
         * The clock will be used instantly by all log outputs. Use 0 to reset
         * the clock to the default clock.
         *
         * @param clock the reference clock.
         */
        static EQBASE_API void setClock( Clock* clock );

    private:
        LogBuffer _logBuffer; 

        Log( const Log& );
        Log& operator = ( const Log& );

        void setLogInfo( const char* subdir, const char* file, const int line )
            { _logBuffer.setLogInfo( subdir, file, line ); }

    };

    /** 
     * Increases the indentation level of the Log stream, causing subsequent
     * lines to be intended by four characters.
     * @version 1.0
     */
    EQBASE_API std::ostream& indent( std::ostream& os );
    /** Decrease the indent of the Log stream. @version 1.0 */
    EQBASE_API std::ostream& exdent( std::ostream& os );

    /** Disable flushing of the Log stream. @version 1.0 */
    EQBASE_API std::ostream& disableFlush( std::ostream& os );
    /** Re-enable flushing of the Log stream. @version 1.0 */
    EQBASE_API std::ostream& enableFlush( std::ostream& os );
    /** Flush the Log stream regardless of the auto-flush state. @version 1.0 */
    EQBASE_API std::ostream& forceFlush( std::ostream& os );

    /** Disable printing of the Log header for subsequent lines. @version 1.0 */
    EQBASE_API std::ostream& disableHeader( std::ostream& os );
    /** Re-enable printing of the Log header. @version 1.0 */
    EQBASE_API std::ostream& enableHeader( std::ostream& os );

}
}

#ifndef SUBDIR
#  define SUBDIR ""
#endif

/** Output an error message to the per-thread Log stream. @version 1.0 */
#define EQERROR (eq::base::Log::level >= eq::base::LOG_ERROR) &&    \
    eq::base::Log::instance( SUBDIR, __FILE__, __LINE__ )
/** Output a warning message to the per-thread Log stream. @version 1.0 */
#define EQWARN  (eq::base::Log::level >= eq::base::LOG_WARN)  &&    \
    eq::base::Log::instance( SUBDIR, __FILE__, __LINE__ )
/** Output an informational message to the per-thread Log. @version 1.0 */
#define EQINFO  (eq::base::Log::level >= eq::base::LOG_INFO)  &&    \
    eq::base::Log::instance( SUBDIR, __FILE__, __LINE__ )

#ifdef NDEBUG
#  define EQVERB if( false )                                    \
        eq::base::Log::instance( SUBDIR, __FILE__, __LINE__ )
#else
/** Output a verbatim message to the per-thread Log stream. @version 1.0 */
#  define EQVERB (eq::base::Log::level >= eq::base::LOG_VERB)  &&    \
    eq::base::Log::instance( SUBDIR, __FILE__, __LINE__ )
#endif

/**
 * Output a message pertaining to a topic to the per-thread Log stream.
 * @version 1.0
 */
#define EQLOG(topic)  (eq::base::Log::topics & (topic))  &&  \
    eq::base::Log::instance( SUBDIR, __FILE__, __LINE__ )

#endif //EQBASE_LOG_H