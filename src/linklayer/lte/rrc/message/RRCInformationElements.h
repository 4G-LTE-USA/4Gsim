//
// Copyright (C) 2012 Calin Cerchez
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

#ifndef RRCINFORMATIONELEMENTS_H_
#define RRCINFORMATIONELEMENTS_H_

#include "ASNTypes.h"
#include "RRCClassDefinitions.h"

namespace rrc {

#define maxLogMeas_r10 4060

class VarLogMeasConfigr10 : public Sequence {
private:
	static const void *itemsInfo[3];
	static bool itemsPres[3];
public:
	static const Info theInfo;
	VarLogMeasConfigr10(): Sequence(&theInfo) {}
	VarLogMeasConfigr10(const LoggingDurationr10& loggingDurationr10, const LoggingIntervalr10& loggingIntervalr10);

	void setAreaConfigurationr10(const AreaConfigurationr10& areaConfigurationr10) { *static_cast<AreaConfigurationr10*>(items[0]) = areaConfigurationr10; }
	void setLoggingDurationr10(const LoggingDurationr10& loggingDurationr10) { *static_cast<LoggingDurationr10*>(items[1]) = loggingDurationr10; }
	void setLoggingIntervalr10(const LoggingIntervalr10& loggingIntervalr10) { *static_cast<LoggingIntervalr10*>(items[2]) = loggingIntervalr10; }
};

typedef OctetString<CONSTRAINED, 2, 2> VarLogMeasReportr10TraceRecordingSessionRefr10;

typedef OctetString<CONSTRAINED, 1, 1> VarLogMeasReportr10TceIdr10;

typedef SequenceOf<LogMeasInfor10, CONSTRAINED, 1, maxLogMeas_r10> LogMeasInfoList2r10;

class VarLogMeasReportr10 : public Sequence {
private:
	static const void *itemsInfo[6];
	static bool itemsPres[6];
public:
	static const Info theInfo;
	VarLogMeasReportr10(): Sequence(&theInfo) {}
	VarLogMeasReportr10(const TraceReferencer10& traceReferencer10, const VarLogMeasReportr10TraceRecordingSessionRefr10& varLogMeasReportr10TraceRecordingSessionRefr10, const VarLogMeasReportr10TceIdr10& varLogMeasReportr10TceIdr10, const PLMNIdentity& plmnIdentityr10, const AbsoluteTimeInfor10& absoluteTimeInfor10, const LogMeasInfoList2r10& logMeasInfoListr10);

	void setTraceReferencer10(const TraceReferencer10& traceReferencer10) { *static_cast<TraceReferencer10*>(items[0]) = traceReferencer10; }
	void setVarLogMeasReportr10TraceRecordingSessionRefr10(const VarLogMeasReportr10TraceRecordingSessionRefr10& varLogMeasReportr10TraceRecordingSessionRefr10) { *static_cast<VarLogMeasReportr10TraceRecordingSessionRefr10*>(items[1]) = varLogMeasReportr10TraceRecordingSessionRefr10; }
	void setVarLogMeasReportr10TceIdr10(const VarLogMeasReportr10TceIdr10& varLogMeasReportr10TceIdr10) { *static_cast<VarLogMeasReportr10TceIdr10*>(items[2]) = varLogMeasReportr10TceIdr10; }
	void setPlmnIdentityr10(const PLMNIdentity& plmnIdentityr10) { *static_cast<PLMNIdentity*>(items[3]) = plmnIdentityr10; }
	void setAbsoluteTimeInfor10(const AbsoluteTimeInfor10& absoluteTimeInfor10) { *static_cast<AbsoluteTimeInfor10*>(items[4]) = absoluteTimeInfor10; }
	void setLogMeasInfoListr10(const LogMeasInfoList2r10& logMeasInfoListr10) { *static_cast<LogMeasInfoList2r10*>(items[5]) = logMeasInfoListr10; }
};

typedef Integer<CONSTRAINED, -140, -44> VarMeasConfigSMeasure;

typedef Null VarMeasConfigSpeedStateParsRelease;

class VarMeasConfigSpeedStateParsSetup : public Sequence {
private:
	static const void *itemsInfo[2];
	static bool itemsPres[2];
public:
	static const Info theInfo;
	VarMeasConfigSpeedStateParsSetup(): Sequence(&theInfo) {}
	VarMeasConfigSpeedStateParsSetup(const MobilityStateParameters& mobilityStateParameters, const SpeedStateScaleFactors& timeToTriggerSF);

	void setMobilityStateParameters(const MobilityStateParameters& mobilityStateParameters) { *static_cast<MobilityStateParameters*>(items[0]) = mobilityStateParameters; }
	void setTimeToTriggerSF(const SpeedStateScaleFactors& timeToTriggerSF) { *static_cast<SpeedStateScaleFactors*>(items[1]) = timeToTriggerSF; }
};

class VarMeasConfigSpeedStatePars : public Choice {
private:
	static const void *choicesInfo[2];
public:
	enum VarMeasConfigSpeedStateParsChoices {
		varMeasConfigSpeedStateParsRelease = 0,
		varMeasConfigSpeedStateParsSetup = 1,
	};
	static const Info theInfo;
	VarMeasConfigSpeedStatePars(): Choice(&theInfo) {}
};

class VarMeasConfig : public Sequence {
private:
	static const void *itemsInfo[6];
	static bool itemsPres[6];
public:
	static const Info theInfo;
	VarMeasConfig(): Sequence(&theInfo) {}
	void setMeasIdList(const MeasIdToAddModList& measIdList) { *static_cast<MeasIdToAddModList*>(items[0]) = measIdList; }
	void setMeasObjectList(const MeasObjectToAddModList& measObjectList) { *static_cast<MeasObjectToAddModList*>(items[1]) = measObjectList; }
	void setReportConfigList(const ReportConfigToAddModList& reportConfigList) { *static_cast<ReportConfigToAddModList*>(items[2]) = reportConfigList; }
	void setQuantityConfig(const QuantityConfig& quantityConfig) { *static_cast<QuantityConfig*>(items[3]) = quantityConfig; }
	void setVarMeasConfigSMeasure(const VarMeasConfigSMeasure& varMeasConfigSMeasure) { *static_cast<VarMeasConfigSMeasure*>(items[4]) = varMeasConfigSMeasure; }
	void setVarMeasConfigSpeedStatePars(const VarMeasConfigSpeedStatePars& varMeasConfigSpeedStatePars) { *static_cast<VarMeasConfigSpeedStatePars*>(items[5]) = varMeasConfigSpeedStatePars; }
};

class CellsTriggeredList : public Sequence {
private:
	static const void *itemsInfo[0];
	static bool itemsPres[0];
public:
	static const Info theInfo;
	CellsTriggeredList(): Sequence(&theInfo) {}
};

typedef IntegerBase VarMeasReportNumberOfReportsSent;

class VarMeasReport : public Sequence {
private:
	static const void *itemsInfo[3];
	static bool itemsPres[3];
public:
	static const Info theInfo;
	VarMeasReport(): Sequence(&theInfo) {}
	VarMeasReport(const MeasId& measId, const VarMeasReportNumberOfReportsSent& varMeasReportNumberOfReportsSent);

	void setMeasId(const MeasId& measId) { *static_cast<MeasId*>(items[0]) = measId; }
	void setCellsTriggeredList(const CellsTriggeredList& cellsTriggeredList) { *static_cast<CellsTriggeredList*>(items[1]) = cellsTriggeredList; }
	void setVarMeasReportNumberOfReportsSent(const VarMeasReportNumberOfReportsSent& varMeasReportNumberOfReportsSent) { *static_cast<VarMeasReportNumberOfReportsSent*>(items[2]) = varMeasReportNumberOfReportsSent; }
};

typedef SequenceOf<VarMeasReport, CONSTRAINED, 1, maxMeasId> VarMeasReportList;

class VarRLFReportr10 : public Sequence {
private:
	static const void *itemsInfo[2];
	static bool itemsPres[2];
public:
	static const Info theInfo;
	VarRLFReportr10(): Sequence(&theInfo) {}
	VarRLFReportr10(const RLFReportr9& rlfReportr10, const PLMNIdentity& plmnIdentityr10);

	void setRlfReportr10(const RLFReportr9& rlfReportr10) { *static_cast<RLFReportr9*>(items[0]) = rlfReportr10; }
	void setPlmnIdentityr10(const PLMNIdentity& plmnIdentityr10) { *static_cast<PLMNIdentity*>(items[1]) = plmnIdentityr10; }
};

class VarShortMACInput : public Sequence {
private:
	static const void *itemsInfo[3];
	static bool itemsPres[3];
public:
	static const Info theInfo;
	VarShortMACInput(): Sequence(&theInfo) {}
	VarShortMACInput(const CellIdentity& cellIdentity, const PhysCellId& physCellId, const CRNTI& cRNTI);

	void setCellIdentity(const CellIdentity& cellIdentity) { *static_cast<CellIdentity*>(items[0]) = cellIdentity; }
	void setPhysCellId(const PhysCellId& physCellId) { *static_cast<PhysCellId*>(items[1]) = physCellId; }
	void setCRNTI(const CRNTI& cRNTI) { *static_cast<CRNTI*>(items[2]) = cRNTI; }
};

}

#endif /* RRCINFORMATIONELEMENTS_H_ */
