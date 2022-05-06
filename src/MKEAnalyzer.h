#ifndef MKE_ANALYZER_H
#define MKE_ANALYZER_H

#include <memory>

#include <Analyzer.h>
#include "MKEAnalyzerResults.h"
#include "MKEAnalyzerSettings.h"
#include "MKESimulationDataGenerator.h"

typedef enum {
  SEEK           = 0x01,
  SPIN_UP        = 0x02,
  SPIN_DOWN      = 0x03,
  DIAG           = 0x04,
  STATUS         = 0x05,
  EJECT          = 0x06,
  INJECT         = 0x07,
  ABORT          = 0x08,
  SET_MODE       = 0x09,
  RESET          = 0x0A,
  FLUSH          = 0x0B,
  LOCK           = 0x0C,
  PAUSE_RESUME   = 0x0D,
  PLAY_MSF       = 0x0E,
  PLAY_TRACK     = 0x0F,
  READ_DATA      = 0x10,
  SUBCHANNEL_INFO= 0x11,
  PATH_CHECK     = 0x80,
  READ_ERROR     = 0x82,
  READ_ID        = 0x83,
  MODE_SENSE     = 0x84,
  READ_CAPACITY  = 0x85,
  READ_HEADER    = 0x86,
  READ_SUB_Q     = 0x87,
  READ_UPC       = 0x88,
  READ_ISRC      = 0x89,
  READ_DISC_CODE = 0x8A,
  READ_DISC_INFO = 0x8B,
  READ_TOC       = 0x8C,
  READ_SESSION   = 0x8D,
  READ_DRIVER    = 0x8E,
  DRIVE_RESET    = 0x93
} commandCode;

class MKEAnalyzerSettings;

class ANALYZER_EXPORT MKEAnalyzer : public Analyzer
{
public:
    MKEAnalyzer();

    virtual ~MKEAnalyzer();
    virtual void SetupResults();
    virtual void WorkerThread();

    virtual U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channels);
    virtual U32 GetMinimumSampleRateHz();

    virtual const char *GetAnalyzerName() const;
    virtual bool NeedsRerun();

protected: //functions
	AnalyzerResults::MarkerType mMarker;

#ifdef _WIN32
    #pragma warning( push )
    #pragma warning( disable : 4251 ) //warning C4251: 'SerialAnalyzer::<...>' : class <...> needs to have dll-interface to be used by clients of class
#endif

protected:  //vars
    std::shared_ptr<MKEAnalyzerSettings> mSettings;
    std::shared_ptr<MKEAnalyzerResults> mResults;
    bool mSimulationInitilized;
    MKESimulationDataGenerator mSimulationDataGenerator;

    AnalyzerChannelData *mMKE[CD_MAX];

    // U64 mCurrentSample;
    AnalyzerResults::MarkerType mArrowMarker;
    std::vector<U64> mMarkerLocations;
private:
    void getDataBus(U8 *data);
    void advanceAllToEarlierNextEdge(input_s channelA, input_s channelB, U8 levelA, U8 levelB);
    void advanceAllToNextEdge(input_s channel, U8 level);
    void advanceAllTo(input_s channel);
    bool CDavailable(void);

#ifdef _WIN32
    #pragma warning( pop )
#endif

};

extern "C" ANALYZER_EXPORT const char *__cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer *__cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer *analyzer);

#endif //MKE_ANALYZER_H
