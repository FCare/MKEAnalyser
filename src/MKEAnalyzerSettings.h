#ifndef MKE_ANALYZER_SETTINGS
#define MKE_ANALYZER_SETTINGS

#include <memory>

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

typedef enum {
  CDD0 = 0,
  CDD1,
  CDD2,
  CDD3,
  CDD4,
  CDD5,
  CDD6,
  CDD7,
  CDCMD,
  CDRST,
  CDMDCHG,
  CDEN,
  CDSTEN,
  CDDTEN,
  CDHRD,
  CDHWR,
  CD_MAX
} input_s;

class MKEAnalyzerSettings : public AnalyzerSettings
{
public:
    MKEAnalyzerSettings();
    virtual ~MKEAnalyzerSettings();

    virtual bool SetSettingsFromInterfaces();
    virtual void LoadSettings(const char *settings);
    virtual const char *SaveSettings();
    void UpdateInterfacesFromSettings();

    Channel mChannel[CD_MAX];
    bool  mShowMarker;
    static std::string mChannelName[CD_MAX];

protected:
  std::shared_ptr< AnalyzerSettingInterfaceChannel >    mDataChannelInterface[CD_MAX];
  std::shared_ptr< AnalyzerSettingInterfaceBool >       mUseShowMarkerInterface;
};

#endif //MKE_ANALYZER_SETTINGS
