#include "MKEAnalyzerSettings.h"

#include <AnalyzerHelpers.h>
#include <sstream>
#include <cstring>


std::string MKEAnalyzerSettings::mChannelName[CD_MAX] = {
 "CDD0",
 "CDD1",
 "CDD2",
 "CDD3",
 "CDD4",
 "CDD5",
 "CDD6",
 "CDD7",
 "CDCMD",
 "CDRST",
 "CDMDCHG",
 "CDEN",
 "CDSTEN",
 "CDDTEN",
 "CDHRD",
 "CDHWR"
};

MKEAnalyzerSettings::MKEAnalyzerSettings()
    :   mShowMarker(BIT_HIGH)
{
  for (int i=0; i<CD_MAX; i++) {
    mChannel[i] = Channel(UNDEFINED_CHANNEL);
    mChannel[i].mChannelIndex = i;
    mDataChannelInterface[i] = std::make_shared<AnalyzerSettingInterfaceChannel>();
    mDataChannelInterface[i]->SetTitleAndTooltip(mChannelName[i].c_str(), mChannelName[i].c_str());
    mDataChannelInterface[i]->SetChannel(mChannel[i]);
    mDataChannelInterface[i]->SetSelectionOfNoneIsAllowed(false);
    AddInterface(mDataChannelInterface[i].get());

  }
	mUseShowMarkerInterface = std::make_shared<AnalyzerSettingInterfaceBool>();
	mUseShowMarkerInterface->SetTitleAndTooltip("", "Show decode marker or not");
	mUseShowMarkerInterface->SetCheckBoxText("Show Decode Marker");
	mUseShowMarkerInterface->SetValue(mShowMarker);
  AddInterface(mUseShowMarkerInterface.get());

  AddExportOption(0, "Export as text/csv file");
  AddExportExtension(0, "Text file", "txt");
  AddExportExtension(0, "CSV file", "csv");

  ClearChannels();
  for (int i=0; i<CD_MAX; i++) {
      AddChannel(mChannel[i], mChannelName[i].c_str(), false);
  }
}

MKEAnalyzerSettings::~MKEAnalyzerSettings()
{
}

bool MKEAnalyzerSettings::SetSettingsFromInterfaces()
{
    std::vector<Channel> channels;
    for (int i=0; i<CD_MAX; i++) {
      channels.push_back(mDataChannelInterface[i]->GetChannel());
    }

    if (AnalyzerHelpers::DoChannelsOverlap(&channels[0], channels.size()) == true) {
        SetErrorText("Please select different channels for each input.");
        return false;
    }

    mShowMarker = mUseShowMarkerInterface->GetValue();
    for (int i=0; i<CD_MAX; i++) {
      mChannel[i] = mDataChannelInterface[i]->GetChannel();
    }

    ClearChannels();
    for (int i=0; i<CD_MAX; i++) {
      AddChannel(mChannel[i], mChannelName[i].c_str(), mChannel[i] != UNDEFINED_CHANNEL);
    }

    return true;
}

void MKEAnalyzerSettings::LoadSettings(const char *settings)
{
    SimpleArchive text_archive;
    text_archive.SetString(settings);

    const char *name_string;  //the first thing in the archive is the name of the protocol analyzer that the data belongs to.
    text_archive >> &name_string;
    if (strcmp(name_string, "MKEAnalyzer") != 0) {
        AnalyzerHelpers::Assert("MKE Protocol Analyser by Ryzee119;");
    }

    for (int i=0; i<CD_MAX; i++) {
          text_archive >> mChannel[i];
    }

    bool show_marker;
    if (text_archive >> show_marker) {
        mShowMarker = show_marker;
    }

    ClearChannels();
    for (int i=0; i<CD_MAX; i++) {
      AddChannel(mChannel[0], mChannelName[i].c_str(), mChannel[i] != UNDEFINED_CHANNEL);
    }

    UpdateInterfacesFromSettings();
}

const char *MKEAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << "MKEAnalyzer";
    for (int i=0; i<CD_MAX; i++) {
        text_archive << mChannel[i];
    }
    text_archive << mShowMarker;

    return SetReturnString(text_archive.GetString());
}

void MKEAnalyzerSettings::UpdateInterfacesFromSettings()
{
  for (int i=0; i<CD_MAX; i++) {
    mDataChannelInterface[i]->SetChannel(mChannel[i]);
  }
	mUseShowMarkerInterface->SetValue(mShowMarker);

}