
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
	for (U32 i = CDD0; i <= CDD7; i++) {
		if (mMKE[i]->GetBitState() == BIT_HIGH)
			*data |= 1 << (i-CDD0);
	}
}

void MKEAnalyzer::advanceAllToNextEdge(input_s channel) {
	U64 sampleNb = mMKE[channel]->GetSampleOfNextEdge();
	for (int i = 0; i< CD_MAX; i++) {
		if (i != channel)
			mMKE[i]->AdvanceToAbsPosition(sampleNb - 1 );
		else
			mMKE[i]->AdvanceToAbsPosition(sampleNb);
	}
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
				      advanceAllToNextEdge(CDHWR);
				  }

	        //we're now at the beginning of the start bit.  We can start collecting the data.
	        U64 frame_starting_sample = mMKE[CDHWR]->GetSampleNumber();

	        U8 data[16] = {0};

					advanceAllToNextEdge(CDHWR);
					getDataBus(&data[0]);

					if (mMKE[CDCMD]->GetBitState() == BIT_LOW) mFlags |= CMD_MODE;
					else mFlags |= DATA_MODE;

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
						advanceAllToNextEdge(CDHWR);
						advanceAllToNextEdge(CDHWR);
						getDataBus(&data[i]);
					}

					switch(data[0]) {
						case READ_DATA:
							nbDataPacket = data[5];
						default:
						break;
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

					//Get answers
					memset(data, 0, 16);
					mFlags = 0;
					while (mMKE[CDHRD]->GetBitState() == BIT_HIGH){
						advanceAllToNextEdge(CDHRD);
					}
					frame.mStartingSampleInclusive = mMKE[CDHRD]->GetSampleNumber();

					for (int i = 0; i < nbStatusPacket; i++) {
						if (mMKE[CDHRD]->GetBitState() == BIT_HIGH) advanceAllToNextEdge(CDHRD);
						advanceAllToNextEdge(CDHRD);
						getDataBus(&data[i]);

						mFlags |= STATUS_MODE;

						if (mMKE[CDCMD]->GetBitState() == BIT_LOW) mFlags |= CMD_MODE;
						else mFlags |= DATA_MODE;

						frame.mEndingSampleInclusive = mMKE[CDHRD]->GetSampleNumber();
					}

					advanceAllToNextEdge(CDHWR);
					memcpy(&frame.mData1, &data[0], 8);
					memcpy(&frame.mData2, &data[8], 8);
					frame.mFlags = mFlags;
					mResults->AddFrame(frame);
					//Go to next command

	        mResults->CommitResults();

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
