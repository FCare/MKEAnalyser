#ifndef MKE_ANALYZER_RESULTS
#define MKE_ANALYZER_RESULTS

#include <AnalyzerResults.h>

#define READ_WRITE_FLAG (0x1<<2)
#define BYTE_TYPE_FLAG (0x3<<0)

#define CMD_MODE (0x0 << 0)
#define DATA_MODE (0x1 << 0)
#define STATUS_MODE (0x2 << 0)

class MKEAnalyzer;
class MKEAnalyzerSettings;

class MKEAnalyzerResults : public AnalyzerResults
{
public:
    MKEAnalyzerResults(MKEAnalyzer* analyzer, std::shared_ptr<MKEAnalyzerSettings> settings);
    virtual ~MKEAnalyzerResults();

    virtual void GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base);
    virtual void GenerateExportFile(const char *file, DisplayBase display_base, U32 export_type_user_id);

    virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base);
    virtual void GeneratePacketTabularText(U64 packet_id, DisplayBase display_base);
    virtual void GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base);

protected: //functions

protected: //vars
    std::shared_ptr<MKEAnalyzerSettings> mSettings;
    MKEAnalyzer* mAnalyzer;
};

#endif //MKE_ANALYZER_RESULTS
