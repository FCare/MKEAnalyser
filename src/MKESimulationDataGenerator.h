#ifndef MKE_SIMULATION_DATA_GENERATOR
#define MKE_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>

class MKEAnalyzerSettings;

class MKESimulationDataGenerator
{
public:
    MKESimulationDataGenerator();
    ~MKESimulationDataGenerator();

    void Initialize(U32 simulation_sample_rate, MKEAnalyzerSettings *settings);
    U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channels);

protected:
    MKEAnalyzerSettings *mSettings;
    U32 mSimulationSampleRateHz;
    U64 mValue;

protected: //MKE specific
    ClockGenerator mClockGenerator;

  //  void CreateMKETransaction(U8 type);
  //  void OutputNibble(U64 lad_data, bool start);

    SimulationChannelDescriptorGroup mMKESimulationChannels;
    SimulationChannelDescriptor *mMKE_SimulationData[16];  //if we had more than one channel to simulate, they would need to be in an array


};
#endif //MKE_SIMULATION_DATA_GENERATOR
