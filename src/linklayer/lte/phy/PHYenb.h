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

#ifndef PHYENB_H_
#define PHYENB_H_

#include "PHY.h"

class PHYenb : public PHY {
private:
    unsigned dlBandwith;
    unsigned ulBandwith;

    bool hasBCHPdu;

    typedef std::map<unsigned short /* pduIndex */, PHYFramePtr> DlConfigRequests;
    DlConfigRequests dlCfgReqs;

    typedef std::map<short /* msgKind */, PHYFramePtr> TxRequests;
    TxRequests txReqs;

//    bool findAndRemoveDlConfigRequestPdu(unsigned short pduIndex);
//    bool findAndRemoveTxRequestPdu(unsigned msgId);

    typedef std::list<DlConfigRequestDciDlPdu*> DCIBuffer;
    DCIBuffer dciBuffer;

    void sendDCIFormat(DlConfigRequestPduPtr pdu);

    void buildSubframe();
    void sendSymbol();
    void cleanup();

    void setData(unsigned short k, PHYFrame *frame);

    virtual void stateEntered(int category, const cPolymorphic *details);

    virtual void receiveChangeNotification(int category, const cPolymorphic *details);
public:
    PHYenb();
    virtual ~PHYenb();

    virtual void initialize(int stage);

    virtual void handleMessage(cMessage *msg);

    virtual void handleUpperMessage(cMessage *msg);
    virtual void handleRadioMessage(cMessage *msg);

    virtual void sendBufferedData();
};

#endif /* PHYENB_H_ */

