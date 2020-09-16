//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#include "inet/common/packet/Packet.h"
#include "inet/physicallayer/ethernet/EthernetSignalUtil.h"

namespace inet {
namespace physicallayer {

void cutEthernetSignalEnd(EthernetSignalBase* signal, simtime_t duration)
{
    ASSERT(duration <= signal->getDuration());
    if (duration == signal->getDuration())
        return;
    signal->setDuration(duration);
    int64_t newBitLength = duration.dbl() * signal->getBitrate();
    if (auto packet = check_and_cast_nullable<Packet*>(signal->decapsulate())) {
        //TODO: removed length calculation based on the PHY layer (parallel bits, bit order, etc.)
        if (newBitLength < packet->getBitLength()) {
            packet->trimFront();
            packet->setBackOffset(b(newBitLength));
            packet->trimBack();
            packet->setBitError(true);
        }
        signal->encapsulate(packet);
    }
    signal->setBitError(true);
    signal->setBitLength(newBitLength);
}

} // namespace physicallayer
} // namespace inet

