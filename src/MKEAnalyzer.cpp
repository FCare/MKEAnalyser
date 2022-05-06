
#include "MKEAnalyzer.h"
#include <AnalyzerChannelData.h>
#include <string.h>

MKEAnalyzer::MKEAnalyzer()
	: Analyzer(),
    mSettings(std::make_shared<MKEAnalyzerSettings>()),
	mSimulationInitilized(false)

{
	SetAnalyzerSettings(mSettings.get());
}

MKEAnalyzer::~MKEAnalyzer()
{
    KillThread();
}

void MKEAnalyzer::SetupResults()
{
	mResults = std::make_shared<MKEAnalyzerResults>(this, mSettings);
	SetAnalyzerResults(mResults.get());
	mResults->AddChannelBubblesWillAppearOn(mSettings->mChannel[CDHWR]);
	mResults->AddChannelBubblesWillAppearOn(mSettings->mChannel[CDHRD]);
}

void MKEAnalyzer::getDataBus(U8 *data) {
	*data = 0x0;
	for (U32 i = CDD0; i <= CDD7; i++) {
		if (mMKE[i]->GetBitState() == BIT_HIGH)
			*data |= 1 << (i-CDD0);
	}
}

bool MKEAnalyzer::CDavailable(void) {
	return (mMKE[CDMDCHG]->GetBitState() == BIT_LOW);
}

void MKEAnalyzer::advanceAllToEarlierNextEdge(input_s channelA, input_s channelB, U8 levelA, U8 levelB) {
	bool isAdvancing = true;
	while (isAdvancing) {
		U64 sampleNbA = mMKE[channelA]->GetSampleOfNextEdge();
		U64 sampleNbB = mMKE[channelB]->GetSampleOfNextEdge();
		U8 level = (sampleNbA < sampleNbB)?levelA:levelB;
		input_s earlierChannel = (sampleNbA < sampleNbB)?channelA:channelB;

		U64 sampleNb = (sampleNbA < sampleNbB)?sampleNbA:sampleNbB;
		for (int i = 0; i< CD_MAX; i++) {
			if ((i != earlierChannel) && (i != CDMDCHG))
			mMKE[i]->AdvanceToAbsPosition(sampleNb - 1 );
			else
			mMKE[i]->AdvanceToAbsPosition(sampleNb);
		}
		isAdvancing = !CDavailable() || (mMKE[earlierChannel]->GetBitState() != level);
	}
}

void MKEAnalyzer::advanceAllToNextEdge(input_s channel, U8 level) {
	bool isAdvancing = true;
	while (isAdvancing) {
		U64 sampleNb = mMKE[channel]->GetSampleOfNextEdge();
		for (int i = 0; i< CD_MAX; i++) {
			if ((i != channel)&& (i != CDMDCHG))
				mMKE[i]->AdvanceToAbsPosition(sampleNb - 1 );
			else
				mMKE[i]->AdvanceToAbsPosition(sampleNb);
		}
		isAdvancing = !CDavailable();
	}
}

void MKEAnalyzer::advanceAllTo(input_s channel) {
	U64 sampleNb = mMKE[channel]->GetSampleNumber();
	for (int i = 0; i< CD_MAX; i++) {
		if (i != channel)
			mMKE[i]->AdvanceToAbsPosition(sampleNb);
	}
}

bool MKEAnalyzer::isLongHigh(input_s channel) {
	if (mMKE[channel]->GetBitState() == BIT_HIGH) {
		U64 currentSample = mMKE[channel]->GetSampleNumber();
		U64 nextEdge = mMKE[channel]->GetSampleOfNextEdge();
		if ((nextEdge - currentSample)>100) return true;
	}
	return false;
}

void MKEAnalyzer::WorkerThread()
{
	mMarker = AnalyzerResults::Dot;

	for (U32 i = 0; i < CD_MAX; i++) {
    mMKE[i] = GetAnalyzerChannelData(mSettings->mChannel[i]);
  }

	mResults->CommitPacketAndStartNewPacket();
	mResults->CommitResults();

	    for (; ;) {
	        //we're starting high.  (we'll assume that we're not in the middle of a byte.)
	        U8 mFlags = 0;

					while ( (mMKE[CDHWR]->GetBitState() == BIT_HIGH)    ||
									(mMKE[CDMDCHG]->GetBitState() == BIT_HIGH) ||
									(mMKE[CDRST]->GetBitState() == BIT_LOW)    ||
									(mMKE[CDEN]->GetBitState() == BIT_HIGH)
								) {
				      advanceAllToNextEdge(CDHWR, BIT_LOW);
				  }

	        //we're now at the beginning of the start bit.  We can start collecting the data.
	        U64 frame_starting_sample = mMKE[CDHWR]->GetSampleNumber();

	        U8 data[16] = {0};

					advanceAllToNextEdge(CDHWR, BIT_HIGH);
					getDataBus(&data[0]);

					mFlags |= CMD_MODE;

					int nbSentPacket = 6;
					int nbDataPacket = 0;
					int nbStatusPacket = 2;
					switch(data[0]) {
						case FLUSH:
						case RESET:
							nbStatusPacket = 1;
						case DIAG:
						case STATUS:
							nbSentPacket = 0;
							break;
						case PATH_CHECK:
							nbStatusPacket = 4;
							break;
						case READ_HEADER:
							nbStatusPacket = 6;
							break;
						case MODE_SENSE:
						case READ_CAPACITY:
							nbStatusPacket = 7;
							break;
						case READ_DISC_INFO:
						case READ_SESSION:
							nbStatusPacket = 8;
							break;
						case READ_ERROR:
						case READ_UPC:
						case READ_TOC:
							nbStatusPacket = 10;
							break;
						case READ_ID:
						case READ_SUB_Q:
						case READ_ISRC:
						case READ_DISC_CODE:
							nbStatusPacket = 12;
							break;
						default:
						break;
					}
					for (int i = 1; i < (nbSentPacket + 1); i++) {
						advanceAllToNextEdge(CDHWR, BIT_LOW);
						advanceAllToNextEdge(CDHWR, BIT_HIGH);
						getDataBus(&data[i]);
					}

	        // if (mMKE[CDCMD]->GetBitState() == BIT_LOW)
	        mFlags |= READ_WRITE_FLAG;

					U64 frame_endinging_sample = mMKE[CDHWR]->GetSampleNumber();

	        //note that we're not using the mData2 or mType fields for anything, so we won't bother to set them.
	        Frame frame;
	        frame.mStartingSampleInclusive = frame_starting_sample;
	        frame.mEndingSampleInclusive = frame_endinging_sample;
	        memcpy(&frame.mData1, &data[0], 8);
					memcpy(&frame.mData2, &data[8], 8);
	        frame.mFlags = mFlags;


					//CMD is sent

					mResults->AddFrame(frame);
				  mResults->CommitResults();

					//Get answers
					memset(data, 0, 16);
					mFlags = 0;
					while ((mMKE[CDHRD]->GetBitState() == BIT_HIGH) && (mMKE[CDDTEN]->GetBitState() == BIT_HIGH)){
						advanceAllToEarlierNextEdge(CDHRD, CDDTEN, BIT_LOW, BIT_LOW);
					}
					frame.mStartingSampleInclusive = mMKE[CDHRD]->GetSampleNumber();

					if ((mMKE[CDSTEN]->GetBitState() == BIT_LOW) && (mMKE[CDDTEN]->GetBitState() == BIT_HIGH)) {
						for (int i = 0; i < nbStatusPacket; i++) {
							if (mMKE[CDHRD]->GetBitState() == BIT_HIGH) advanceAllToNextEdge(CDHRD, BIT_LOW);
							advanceAllToNextEdge(CDHRD, BIT_HIGH);
							getDataBus(&data[i]);

							mFlags |= CMD_MODE;

							frame.mEndingSampleInclusive = mMKE[CDHRD]->GetSampleNumber();
						}
						memcpy(&frame.mData1, &data[0], 8);
						memcpy(&frame.mData2, &data[8], 8);
						frame.mFlags = mFlags;
						mResults->AddFrame(frame);
						mResults->CommitResults();
						advanceAllTo(CDHRD);
					}

					if (mMKE[CDDTEN]->GetBitState() == BIT_LOW) {
						//Read DATA case. Make only one packet with XOR of all values
						frame.mStartingSampleInclusive = mMKE[CDDTEN]->GetSampleNumber();
						frame.mData1 = 0;
						bool dataStarted = false;
						while (!dataStarted || !isLongHigh(CDSTEN) || !CDavailable()) {
							if ((mMKE[CDHRD]->GetBitState() == BIT_HIGH) && (mMKE[CDEN]->GetBitState() == BIT_LOW)) advanceAllToEarlierNextEdge(CDHRD, CDEN, BIT_LOW, BIT_HIGH); //LOW
							if(mMKE[CDHRD]->GetBitState() == BIT_HIGH) {
								//Interrupted due to CDEN HIGH
								if (!dataStarted) {
									//Data transmission did not start yet. Wait for CDEN LOW and resume
									while (mMKE[CDHRD]->GetBitState() == BIT_HIGH) advanceAllToNextEdge(CDHRD, BIT_LOW); //LOW
								} else {
									//Data transmission did start. Abort the data frame
									break;
								}
							}
							advanceAllToEarlierNextEdge(CDHRD, CDEN, BIT_HIGH, BIT_HIGH); // CDHRD HI
							if(mMKE[CDHRD]->GetBitState() == BIT_LOW){
								//Interrupted due to CDEN HIGH
								if (!dataStarted) {
									//Data transmission did not start yet. Wait for CDEN LOW and resume
									while (mMKE[CDHRD]->GetBitState() == BIT_LOW)  advanceAllToNextEdge(CDHRD, BIT_HIGH); //HIGH
								} else {
									//Data transmission did start. Abort the data frame
									break;
								}
							}
							if (mMKE[CDSTEN]->GetBitState() == BIT_LOW) dataStarted = true;
							getDataBus(&data[0]);
							if (mMKE[CDDTEN]->GetBitState() == BIT_HIGH){
								//Status
								frame.mData2 = data[0];
							} else {
								frame.mData1 ^= data[0];
							}
							advanceAllTo(CDHRD); //sync values
						}

						if(mMKE[CDEN]->GetBitState() == BIT_HIGH) {
							//Advance to get CDEN available again
							advanceAllToNextEdge(CDEN, BIT_LOW);
							//In that case, status is sent after resume
							//Now get the STATUS
							if (mMKE[CDHRD]->GetBitState() == BIT_HIGH) advanceAllToNextEdge(CDHRD, BIT_LOW); //Low
							advanceAllToNextEdge(CDHRD, BIT_HIGH); //High
							advanceAllToNextEdge(CDHRD, BIT_LOW); //Low
							advanceAllToNextEdge(CDHRD, BIT_HIGH); //High
							getDataBus(&data[0]);
							frame.mData2 = data[0];
							mFlags |= ABORTED_DATA;
						}

						frame.mEndingSampleInclusive = mMKE[CDHRD]->GetSampleNumber();
						frame.mFlags = mFlags;
						mResults->AddFrame(frame);
						mResults->CommitResults();
					}

					advanceAllToNextEdge(CDHWR, BIT_LOW);
					//Go to next command

	        ReportProgress(frame.mEndingSampleInclusive);
	        CheckIfThreadShouldExit();
	    }

}

bool MKEAnalyzer::NeedsRerun()
{
    return false;
}

U32 MKEAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor **simulation_channels)
{
    if (mSimulationInitilized == false) {
        mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
        mSimulationInitilized = true;
    }

    return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
}


U32 MKEAnalyzer::GetMinimumSampleRateHz()
{
    return 20000000; //Doesnt really matter
}

const char *MKEAnalyzer::GetAnalyzerName() const
{
    return "MKE";
}

const char *GetAnalyzerName()
{
    return "MKE";
}

Analyzer *CreateAnalyzer()
{
    return new MKEAnalyzer();
}

void DestroyAnalyzer(Analyzer *analyzer)
{
    delete analyzer;
}
