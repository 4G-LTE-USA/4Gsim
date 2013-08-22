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

#include "PHYenb.h"
#include "LTEChannelControl.h"

Define_Module(PHYenb);

PHYenb::PHYenb() : PHY() {
    dlBandwith = 6;
    ulBandwith = 6;
}

PHYenb::~PHYenb() {

}

void PHYenb::initialize(int stage) {
    // TODO - Generated method body
	PHY::initialize(stage);

    if (stage == 2) {
        cc->setRadioChannel(myRadioRef, PRACH);
        cc->setRadioChannel(myRadioRef, PUSCH);
    }

    if (stage == 4) {
        fsm = cFSM("fsm-PHYenb");
        fsm.setState(IDLE);

        nb->subscribe(this, DLCONFIGRequest);
    }
}

void PHYenb::handleMessage(cMessage *msg) {
	if (msg->isSelfMessage()) {
		if (msg == symbolTimer) {
		    buildAndSendFrame();

            this->cancelEvent(symbolTimer);
            this->scheduleAt(simTime() + symbPeriod, symbolTimer);

            symb = (symb + 1) % nDLsymb;
            if (symb == 0) {
                slot = (slot + 1) % 20;
                sf = slot / 2;

                if (sf ==  0)
                    sfn++;

                if (slot % 2 == 0) {
                    SubframeIndication *sfInd = new SubframeIndication();
                    sfInd->setSf(sf);
                    sfInd->setSfn(sfn);
                    nb->fireChangeNotification(SUBFRAMEIndication, sfInd);
                    delete sfInd;
                }
            }
		}
	}
}

void PHYenb::handleUpperMessage(cMessage *msg) {
    EV << "LTE-PHYenb: Receiving message with id = " << msg->getKind() << " from upper layer. Buffering the received message.\n";
//    LTEControlInfo *ctrl = check_and_cast<LTEControlInfo*>(msg->removeControlInfo());
//    TransportBlock *tb = new TransportBlock();
//    tb->setName(msg->getName());
//    tb->encapsulate(PK(msg));
//    switch(ctrl->getChannel()) {
//        case BCH:
//        	// TODO repetition of MIB + get the tti rest at PHYue
//            tb->setChannelNumber(PBCH);
//            break;
//        case DLSCH0:
//            tb->setChannelNumber(PDSCH0);
//            break;
//        case DLSCH1:
//            tb->setChannelNumber(PDSCH1);
//            break;
//        default:
//            break;
//    }
//    buffer[msg->getKind()] = tb;
}

void PHYenb::handleRadioMessage(cMessage *msg) {
    PhysicalResourceBlock *prb = check_and_cast<PhysicalResourceBlock*>(msg);
    EV << "LTE-PHYenb: Receiving message from ";

//    if (prb->getChannelNumber() == PRACH) {
//    	EV << "PRACH.\n";
//    	RAPreamble *raPreamble = check_and_cast<RAPreamble*>(prb);
//    	RachIndication *rachInd = new RachIndication();
//    	rachInd->setTti(tti);
//    	rachInd->setPreamblesArraySize(1);
//    	RachPreamble rachPrbl = RachPreamble();
//    	rachPrbl.setPreamble(raPreamble->getRapid());
//    	rachPrbl.setRnti(raPreamble->getRnti());
//    	rachInd->setPreambles(0, rachPrbl);
//    	nb->fireChangeNotification(RACHIndication, rachInd);
//    }
}

void PHYenb::buildAndSendFrame() {
    PHYFrame *frame = new PHYFrame();
    frame->setResArraySize(nDLrb * nRBsc);
    frame->setChannelNumber(Downlink);

    unsigned char ssOffset = (nDLrb * nRBsc) / 2 - 31;
    unsigned char vShift = nCellId % 6;

    // PSS
    if ((slot == 0 || slot == 10) && (symb == (nDLsymb - 1))) {
        for (unsigned char k = ssOffset; k < ssOffset + 62; k++) {
            frame->setRes(k, PSS);
        }
        PSSSignal *pss = new PSSSignal();
        pss->setCellIdInGroup(n2id);
        setData(PSS, pss);
    }

    // SSS
    if ((slot == 0 || slot == 10) && (symb == (nDLsymb - 2))) {
        for (unsigned char k = ssOffset; k < ssOffset + 62; k++) {
            frame->setRes(k, SSS);
        }
        SSSSignal *sss = new SSSSignal();
        sss->setCellGroupId(n1id);
        sss->setSf(sf);
        setData(SSS, sss);
    }

    // RS
    if (symb == 0 || symb == (nDLsymb - 3)) {
        unsigned char v = symb == 0 ? 0 : 3;
        unsigned char rsOffset = (v + vShift) % 6;
        for (unsigned char i = 0; i < 2 * nDLrb - 1; i++) {
            frame->setRes(rsOffset + 6 * i, RS);
        }
        ReferenceSignal *refSig = new ReferenceSignal();
        refSig->setCellId(nCellId);
        refSig->setNcp(ncp);
        setData(RS, refSig);
    }

    // PBCH
    if (slot == 1 && symb < 4) {
        unsigned char pbchOffset = nDLrb * nRBsc / 2 - 36;
        for (unsigned char k = pbchOffset; k < pbchOffset + 72; k++) {
            frame->setRes(k, PBCH);
        }
    }

    sendToChannel(frame);
}

void PHYenb::setData(unsigned char channel, cMessage *msg) {
    if (dynamic_cast<LTEChannelControl*>(cc) != NULL) {
        dynamic_cast<LTEChannelControl*>(cc)->setData(channel, msg);
    }
}

void PHYenb::stateEntered(int category, const cPolymorphic *details) {
	PHY::stateEntered(category, details);

	if (fsm.getState() == RUNNING) {
		if (category == DLCONFIGRequest) {
			DlConfigRequest *dlCfgReq = check_and_cast<DlConfigRequest*>(details);
		    	for (unsigned i = 0; i < dlCfgReq->getPdusArraySize(); i++) {
		    		DlConfigRequestPduPtr pdu = dlCfgReq->getPdus(i)->dup();
		    		EV << "LTE-PHYenb: Received DLCONFIGRequest for PDU with type = " << (unsigned)pdu->getType() << ".\n";
		    		if (pdu->getType() == DciDlPdu) {
		    			sendDCIFormat(pdu);
		    		} else {
		    			dlCfgReqs[dlCfgReq->getTti()].push_back(pdu);
		    		}
		    	}
		} else if (category == TXRequest) {
			TxRequest *txReq = check_and_cast<TxRequest*>(details);
			for (unsigned i = 0; i < txReq->getPdusArraySize(); i++) {
				TxRequestPdu pdu = txReq->getPdus(i);
				if (dlCfgReqs.find(txReq->getTti()) != dlCfgReqs.end()) {
					for (unsigned j = 0; j < pdu.getMsgKindsArraySize(); j++) {
						unsigned short msgKind = pdu.getMsgKinds(j);
						EV << "LTE-PHYenb: Received TXRequest for PDU with id = " << msgKind << ".\n";
						txReqs[pdu.getPduIndex()].push_back(msgKind);
					}
				}
			}
		}
	}
	delete details;
}

void PHYenb::sendBufferedData() {
	unsigned short rnti = 0;

//	DlConfigRequests::iterator dlCfgReqsIt = dlCfgReqs.find(tti);
//	if (dlCfgReqsIt != dlCfgReqs.end()) {	// first check downlink configuration requests
//		EV << "LTE-PHYenb: Found DLConfigRequest for tti = " << tti << ".\n";
//
//		while(!dlCfgReqsIt->second.empty()) {
//			DlConfigRequestPduPtr dlCfgReqPdu = dlCfgReqsIt->second.front();
//
//			// find rnti to add to transport block
//			if (dlCfgReqPdu->getType() == DlschPdu) {
//				DlConfigRequestDlschPdu *dlCfgReqDlschPdu = check_and_cast<DlConfigRequestDlschPdu*>(dlCfgReqPdu);
//				rnti = dlCfgReqDlschPdu->getRnti();
//			}
//
//			dlCfgReqsIt->second.pop_front();
//			TxRequests::iterator txReqsIt = txReqs.find(dlCfgReqPdu->getPduIndex());
//			if (txReqsIt != txReqs.end()) {	// second check transmission requests
//				EV << "LTE-PHYenb: Found TxRequest for pduIndex = " << dlCfgReqPdu->getPduIndex() << ".\n";
//
//				while(!txReqsIt->second.empty()) {
//					short msgKind = txReqsIt->second.front();
//					txReqsIt->second.pop_front();
//
//					TransmissionBuffer::iterator bufferIt = buffer.find(msgKind);
//					if (bufferIt != buffer.end()) {	// send transportblock from the buffer
//
//						EV << "LTE-PHYenb: Sending buffered transport block with id = " << msgKind << " to the air.\n";
//						TransportBlock *tb = bufferIt->second;
//						tb->setRnti(rnti);
//						sendToChannel(tb);
//						buffer.erase(bufferIt);
//					}
//				}
//			}
//			delete dlCfgReqPdu;
//		}
//		dlCfgReqs.erase(dlCfgReqsIt);
//	}
}

void PHYenb::sendDCIFormat(DlConfigRequestPduPtr pdu) {
    DlConfigRequestDciDlPdu *dciPdu = check_and_cast<DlConfigRequestDciDlPdu*>(pdu);
    DCIFormat *dci = new DCIFormat();
    dci->setName("DCIFormat");
    dci->setRnti(dciPdu->getRnti());
//    dci->setRntiType(dciPdu->getRntiType());
    dci->setChannelNumber(PDCCH);
    sendToChannel(dci);
}
//
//bool PHYenb::findAndRemoveDlConfigRequestPdu(unsigned short pduIndex) {
//    DlConfigRequestPdus::iterator i = dlCfgReqPdus.find(pduIndex);
//    if (i != dlCfgReqPdus.end()) {
//    	delete i->second;
//        dlCfgReqPdus.erase(i);
//        return true;
//    } else {
//        return false;
//    }
//}
//
//bool PHYenb::findAndRemoveTxRequestPdu(unsigned msgId) {
//    TxRequestPdus::iterator i = txReqPdus.find(msgId);
//    if (i != txReqPdus.end()) {
//    	txReqPdus.erase(i);
//        return true;
//    } else {
//        return false;
//    }
//}
