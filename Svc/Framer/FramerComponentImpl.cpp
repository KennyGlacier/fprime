// ======================================================================
// \title  FramerComponentImpl.cpp
// \author mstarch
// \brief  cpp file for Framer component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include <Svc/Framer/FramerComponentImpl.hpp>
#include "Fw/Logger/Logger.hpp"
#include "Fw/Types/BasicTypes.hpp"
#include "Utils/Hash/Hash.hpp"

namespace Svc {



// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

FramerComponentImpl ::FramerComponentImpl(const char* const compName) : FramerComponentBase(compName), FramingProtocolInterface(),
m_protocol(NULL) {}

void FramerComponentImpl ::init(const NATIVE_INT_TYPE instance) {
    FramerComponentBase::init(instance);
}

FramerComponentImpl ::~FramerComponentImpl(void) {}

void FramerComponentImpl ::setup(FramingProtocol& protocol) {
    FW_ASSERT(m_protocol == NULL);
    m_protocol = &protocol;
    protocol.setup(*this);
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void FramerComponentImpl ::comIn_handler(const NATIVE_INT_TYPE portNum, Fw::ComBuffer& data, U32 context) {
    FW_ASSERT(m_protocol != NULL);
    m_protocol->frame(data.getBuffAddr(), data.getBuffLength(), Fw::ComPacket::FW_PACKET_UNKNOWN);
}

void FramerComponentImpl ::bufferIn_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer) {
    FW_ASSERT(m_protocol != NULL);
    m_protocol->frame(fwBuffer.getData(), fwBuffer.getSize(), Fw::ComPacket::FW_PACKET_FILE);
    bufferReturn_out(0, fwBuffer);
}

void FramerComponentImpl ::send(Fw::Buffer& outgoing) {
    Drv::SendStatus sendStatus = framedOut_out(0, outgoing);
    if (sendStatus != Drv::SEND_OK) {
        // Note: if there is a data sending problem, an EVR likely wouldn't make it down. Log the issue in hopes that
        // someone will see it.
        Fw::Logger::logMsg("[ERROR] Failed to send framed data: %d\n", sendStatus);
    }
}

Fw::Buffer FramerComponentImpl ::allocate(const U32 size) {
    return allocate_out(0, size);
}

Fw::Time FramerComponentImpl ::time() {
    Fw::Time time_val;  // TODO: does this get time?
    return time_val;
}

}  // end namespace Svc