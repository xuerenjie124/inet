//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "inet/protocol/transceiver/PacketReceiver.h"

namespace inet {

Define_Module(PacketReceiver);

void PacketReceiver::initialize(int stage)
{
    PacketReceiverBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
        datarate = bps(par("datarate"));
}

void PacketReceiver::handleMessage(cMessage *message)
{
    if (message->isPacket())
        receiveSignal(check_and_cast<Signal *>(message));
    else
        throw cRuntimeError("Unknown message");
}

void PacketReceiver::receiveSignal(Signal *signal)
{
    auto packet = check_and_cast<Packet *>(signal->decapsulate());
    // TODO: check signal physical properties such as datarate, modulation, etc.
    pushOrSendPacket(packet, outputGate, consumer);
    delete signal;
}

} // namespace inet
