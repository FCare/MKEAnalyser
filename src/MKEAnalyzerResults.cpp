#include "MKEAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "MKEAnalyzer.h"
#include "MKEAnalyzerSettings.h"
#include <iostream>
#include <sstream>

MKEAnalyzerResults::MKEAnalyzerResults(MKEAnalyzer* analyzer, std::shared_ptr<MKEAnalyzerSettings> settings)
    :   AnalyzerResults(),
        mSettings(settings),
        mAnalyzer(analyzer)
{
}

MKEAnalyzerResults::~MKEAnalyzerResults()
{
}

void MKEAnalyzerResults::getCmdRequestString(Frame &frame, std::string &out) {
  U8 Cmd = (frame.mData1>>0)&0xFF;
  std::stringstream ss;
  U8 Status = 0;
  switch(Cmd){
    case SEEK:
      out.append("SEEK");
      break;
    case SPIN_UP:
      out.append("SPIN UP");
      break;
    case SPIN_DOWN:
      out.append("SPIN DOWN");
      break;
    case DIAG:
      out.append("DIAG");
      break;
    case STATUS:
      out.append("STATUS");
      break;
    case EJECT:
      out.append("EJECT");
      break;
    case INJECT:
      out.append("INJECT");
      break;
    case ABORT:
      out.append("ABORT");
      break;
    case SET_MODE:
      out.append("SET MODE");
      break;
    case RESET:
      out.append("RESET");
      break;
    case FLUSH:
      out.append("FLUSH");
      break;
    case LOCK:
      out.append("LOCK");
      break;
    case PAUSE_RESUME:
      out.append("PAUSE/RESUME");
      break;
    case PLAY_MSF:
      out.append("PLAY MSF");
      break;
    case PLAY_TRACK:
      out.append("PLAY TRACK");
      break;
    case READ_DATA:
      out.append("WTF ? READ DATA");
      break;
    case SUBCHANNEL_INFO:
      out.append("SUBCHANNEL INFO");
      break;
    case PATH_CHECK:
      out.append("PATH CHECK");
      break;
    case READ_ERROR:
      out.append("READ ERROR");
      break;
    case READ_ID:
      out.append("READ ID");
      break;
    case MODE_SENSE:
      out.append("MODE SENSE");
      break;
    case READ_CAPACITY:
      out.append("READ CAPACITY");
      break;
    case READ_HEADER:
      out.append("READ HEADER");
      break;
    case READ_SUB_Q:
      out.append("READ SUBQ");
      break;
    case READ_UPC:
      out.append("READ UPC");
      break;
    case READ_ISRC :
      out.append("READ ISRC");
      break;
    case READ_DISC_CODE:
      out.append("READ DISC CODE");
      break;
    case READ_DISC_INFO:
      out.append("READ DISC INFO");
      break;
    case READ_TOC:
      out.append("READ TOC");
      break;
    case READ_SESSION:
      out.append("READ SESSION");
      break;
    case READ_DRIVER:
      out.append("WTF? READ DRIVER");
      break;
    case DRIVE_RESET:
      out.append("WTF? DRIVE RESET");
      break;
    default:
      out.append("Unknown CMD");
      ss << " 0x";
      ss << std::hex << +Cmd;
      out.append(ss.str());
      break;
  }
}

void MKEAnalyzerResults::getCmdResponseString(Frame &frame, std::string &out) {
  U8 Cmd = (frame.mData1>>0)&0xFF;
  std::stringstream ss;
  U8 Status = 0;
  switch(Cmd){
    case SEEK:
      out.append("SEEK");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case SPIN_UP:
      out.append("SPIN UP");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case SPIN_DOWN:
      out.append("SPIN DOWN");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case DIAG:
      out.append("DIAG");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case STATUS:
      out.append("STATUS");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case EJECT:
      out.append("EJECT");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case INJECT:
      out.append("INJECT");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case ABORT:
      out.append("ABORT");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case SET_MODE:
      out.append("SET MODE");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case RESET:
      out.append("RESET");
      break;
    case FLUSH:
      out.append("FLUSH");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case LOCK:
      out.append("LOCK");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case PAUSE_RESUME:
      out.append("PAUSE/RESUME");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case PLAY_MSF:
      out.append("PLAY MSF");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case PLAY_TRACK:
      out.append("PLAY TRACK");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case READ_DATA:
      out.append("WTF ? READ DATA");
      break;
    case SUBCHANNEL_INFO:
      out.append("SUBCHANNEL INFO");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case PATH_CHECK:
      out.append("PATH CHECK");
      Status = (frame.mData1>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case READ_ERROR:
      out.append("READ ERROR");
      Status = (frame.mData2>>8)&0xFF;
      getStatusString(Status, out);
      break;
    case READ_ID:
      out.append("READ ID");
      Status = (frame.mData2>>(8*3))&0xFF;
      getStatusString(Status, out);
      break;
    case MODE_SENSE:
      out.append("MODE SENSE");
      Status = (frame.mData1>>(8*6))&0xFF;
      getStatusString(Status, out);
      break;
    case READ_CAPACITY:
      out.append("READ CAPACITY");
      Status = (frame.mData1>>(8*6))&0xFF;
      getStatusString(Status, out);
      break;
    case READ_HEADER:
      out.append("READ HEADER");
      Status = (frame.mData1>>(8*5))&0xFF;
      getStatusString(Status, out);
      break;
    case READ_SUB_Q:
      out.append("READ SUBQ");
      Status = (frame.mData2>>(8*3))&0xFF;
      getStatusString(Status, out);
      break;
    case READ_UPC:
      out.append("READ UPC");
      Status = (frame.mData2>>(8*1))&0xFF;
      getStatusString(Status, out);
      break;
    case READ_ISRC :
      out.append("READ ISRC");
      Status = (frame.mData2>>(8*3))&0xFF;
      getStatusString(Status, out);
      break;
    case READ_DISC_CODE:
      out.append("READ DISC CODE");
      Status = (frame.mData2>>(8*3))&0xFF;
      getStatusString(Status, out);
      break;
    case READ_DISC_INFO:
      out.append("READ DISC INFO");
      Status = (frame.mData1>>(8*7))&0xFF;
      getStatusString(Status, out);
      break;
    case READ_TOC:
      out.append("READ TOC");
      Status = (frame.mData2>>(8*1))&0xFF;
      getStatusString(Status, out);
      break;
    case READ_SESSION:
      out.append("READ SESSION");
      Status = (frame.mData1>>(8*7))&0xFF;
      getStatusString(Status, out);
      break;
    case READ_DRIVER:
      out.append("WTF? READ DRIVER");
      break;
    case DRIVE_RESET:
      out.append("WTF? DRIVE RESET");
      break;
    default:
      out.append("Unknown CMD");
      ss << " 0x";
      ss << std::hex << +Cmd;
      out.append(ss.str());
      break;
  }
}

void MKEAnalyzerResults::getStatusString(U8 data, std::string &out){
  std::stringstream ss;
  ss << " Status: 0x";
  ss << std::hex << +data;
  out.append(ss.str());
}

void MKEAnalyzerResults::getErrorString(U8 data, std::string &out){
  std::stringstream ss;
  ss << " Error: 0x";
  ss << std::hex << +data;
  out.append(ss.str());
}

void MKEAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base)    //unrefereced vars commented out to remove warnings.
{
  //we only need to pay attention to 'channel' if we're making bubbles for more than one channel (as set by AddChannelBubblesWillAppearOn)
  Frame frame = GetFrame(frame_index);
  std::string out;
  ClearResultStrings();
  if ((channel == mSettings->mChannel[CDHRD]) && !(frame.mFlags & READ_WRITE_FLAG)){
    getCmdResponseString(frame, out);
    AddResultString(out.c_str());
  }
  if ((channel == mSettings->mChannel[CDHWR]) && (frame.mFlags & READ_WRITE_FLAG)){
    getCmdRequestString(frame, out);
    AddResultString(out.c_str());
  }
    //
    // U8 frame_mode = frame.mFlags & BYTE_TYPE_FLAG;
    //
    // bool is_write_frame = false;
    // if ((frame.mFlags & READ_WRITE_FLAG) != 0) {
    //     is_write_frame = true;
    // }
    //
    // if (is_write_frame) {
    //   AddResultString("Write ");
    // } else {
    //   AddResultString("Read ");
    // }
    //
    // switch(frame_mode) {
    //   case CMD_MODE:
    //     AddResultString("CMD ");
    //     for (int i = 0; i<8; i++) {
    //       U8 val = frame.mData1>>(8*i) & 0xFF;
    //       char val_str[3];
    //       AnalyzerHelpers::GetNumberString(val, display_base, 8, val_str, 3);
    //       AddResultString(" 0x%s", val_str);
    //     }
    //     break;
    //   case DATA_MODE:
    //     AddResultString("DATA ");
    //     break;
    //   case STATUS_MODE:
    //     AddResultString("STATUS ");
    //     break;
    // }

}

void MKEAnalyzerResults::GenerateExportFile(const char *file, DisplayBase display_base, U32 /*export_type_user_id*/)
{
  /*
    export_type_user_id is only important if we have more than one export type.

    std::stringstream ss;
    void *f = AnalyzerHelpers::StartFile(file);

    U64 trigger_sample = mAnalyzer->GetTriggerSample();
    U32 sample_rate = mAnalyzer->GetSampleRate();

    ss << "Time [ms],Transaction separated by nibble LAD[3:0]" << std::endl;

    U64 num_frames = GetNumFrames();
    for (U32 i = 0; i < num_frames-1; i++) {
        Frame frame = GetFrame(i);
		MKEAnalyzer::MKE_STATE state = (MKEAnalyzer::MKE_STATE)frame.mData2;
		if (state == MKEAnalyzer::MKE_STATE::START) {
			char time_str[128];
			AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128);
			S32 MKE_nibble_cnt = -1;
			ss << time_str << ",";
			while (state != MKEAnalyzer::MKE_STATE::TAR && MKE_nibble_cnt < 20) {
				Frame nextFrame = GetFrame(MKE_nibble_cnt + i + 1);
				state = (MKEAnalyzer::MKE_STATE)nextFrame.mData2;
				char nibble_str[128];
				snprintf(nibble_str, 7, "0b%c%c%c%c", NIBBLE_TO_BINARY(nextFrame.mData1));
				ss << nibble_str << ",";
				MKE_nibble_cnt++;
			}
			ss << std::endl;
			AnalyzerHelpers::AppendToFile((U8*)ss.str().c_str(), ss.str().length(), f);
			ss.str(std::string());
			i += MKE_nibble_cnt;
		}

        if (UpdateExportProgressAndCheckForCancel(i, num_frames) == true) {
            AnalyzerHelpers::EndFile(f);
            return;
        }
    }

    UpdateExportProgressAndCheckForCancel(num_frames, num_frames);
    AnalyzerHelpers::EndFile(f);
    */
}

void MKEAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
  //Text displayed in the right frame
  ClearTabularText();

  Frame frame = GetFrame(frame_index);

  U8 frame_mode = frame.mFlags & BYTE_TYPE_FLAG;

  bool is_write_frame = false;
  if ((frame.mFlags & READ_WRITE_FLAG) != 0) {
      is_write_frame = true;
  }

  if (is_write_frame) {
    AddTabularText("Write ");
  } else {
    AddTabularText("Read ");
  }

  switch(frame_mode) {
    case CMD_MODE:
      AddTabularText("CMD ");
        // AddTabularText(val_str);
      for (int i = 0; i<8; i++) {
        U8 val = frame.mData1>>(8*i) & 0xFF;
      }
//         char val_str[3];
//         AnalyzerHelpers::GetNumberString(val, display_base, 8, val_str, 3);
//         // AddTabularText(val_str);
//       }
      break;
    case DATA_MODE:
      // AddTabularText("DATA ");
      break;
    case STATUS_MODE:
      AddTabularText("STATUS ");
      break;
    default:
      AddTabularText("UNKNOWN");
      break;
  }
  /*
#define NIBBLE_TO_BINARY_PATTERN "%c%c%c%c"
#define NIBBLE_TO_BINARY(nibble)  \
	  (nibble & 0x08 ? '1' : '0'), \
	  (nibble & 0x04 ? '1' : '0'), \
	  (nibble & 0x02 ? '1' : '0'), \
	  (nibble & 0x01 ? '1' : '0')


	ClearTabularText();
	Frame frame = GetFrame(frame_index);
	Frame prevFrame = GetFrame(frame_index - 1);
	MKEAnalyzer::MKE_STATE prevState = (MKEAnalyzer::MKE_STATE)prevFrame.mData2;
	Frame nextFrame[8]; //Get the next bunch of frames. Used to merged Address and Data nibbles.
	for (U8 i = 0; i < 8; i++) {
		nextFrame[i]= GetFrame(frame_index + i);
	}


	char state_str[128];
	char tab_str[128];

	MKEAnalyzer::MKE_STATE state = (MKEAnalyzer::MKE_STATE)frame.mData2;

	switch (state) {
	case MKEAnalyzer::MKE_STATE::START:
		memcpy(state_str, "START", 6);
		break;

	case MKEAnalyzer::MKE_STATE::CYCTYPE:
		switch (frame.mData1) {
		case 0b0000:
			memcpy(state_str, "IO READ", 8);
			break;
		case 0b0010:
			memcpy(state_str, "IO WRITE", 9);
			break;
		case 0b0100:
			memcpy(state_str, "MEM READ", 9);
			break;
		case 0b0110:
			memcpy(state_str, "MEM WRITE", 10);
			break;
		}

		break;

	case MKEAnalyzer::MKE_STATE::IO_READ_ADD:
	case MKEAnalyzer::MKE_STATE::IO_WRITE_ADD:
		if (prevState != MKEAnalyzer::MKE_STATE::IO_READ_ADD &&
			prevState != MKEAnalyzer::MKE_STATE::IO_WRITE_ADD) {
			//IO Read/Write address are 4 nibbles long
			//Addresses are Most Significant Nibble First as per MKE spec
			uint32_t address = nextFrame[0].mData1 << 12 |
				     nextFrame[1].mData1 << 8 |
				     nextFrame[2].mData1 << 4 |
				     nextFrame[3].mData1 << 0;
			snprintf(tab_str, 100, "ADDRESS 0x%04x", address);
			AddTabularText(tab_str);
		}
		else {
			ClearTabularText();
		}
		return;
		break;
	case MKEAnalyzer::MKE_STATE::MEM_READ_ADD:
	case MKEAnalyzer::MKE_STATE::MEM_WRITE_ADD:
		if (prevState != MKEAnalyzer::MKE_STATE::MEM_READ_ADD &&
			prevState != MKEAnalyzer::MKE_STATE::MEM_WRITE_ADD) {
			//Mem Read/Write address are 8 nibbles long
			//Addresses are Most Significant Nibble First as per MKE spec
			uint32_t address = nextFrame[0].mData1 << 28 |
                     nextFrame[1].mData1 << 24 |
                     nextFrame[2].mData1 << 20 |
                     nextFrame[3].mData1 << 16 |
                     nextFrame[4].mData1 << 12 |
                     nextFrame[5].mData1 << 8 |
                     nextFrame[6].mData1 << 4 |
                     nextFrame[7].mData1 << 0;

			snprintf(tab_str, 100, "ADDRESS 0x%08x", address);
			AddTabularText(tab_str);
		}
		return;
		break;
	case MKEAnalyzer::MKE_STATE::IO_READ_DATA:
	case MKEAnalyzer::MKE_STATE::IO_WRITE_DATA:
	case MKEAnalyzer::MKE_STATE::MEM_READ_DATA:
	case MKEAnalyzer::MKE_STATE::MEM_WRITE_DATA:
		memcpy(state_str, "DATA", 5);
		break;

	case MKEAnalyzer::MKE_STATE::IO_READ_TAR:
	case MKEAnalyzer::MKE_STATE::IO_WRITE_TAR:
	case MKEAnalyzer::MKE_STATE::MEM_READ_TAR:
	case MKEAnalyzer::MKE_STATE::MEM_WRITE_TAR:
	case MKEAnalyzer::MKE_STATE::TAR:
		memcpy(state_str, "TAR", 4);
		break;

	case MKEAnalyzer::MKE_STATE::IO_READ_SYNC:
	case MKEAnalyzer::MKE_STATE::IO_WRITE_SYNC:
	case MKEAnalyzer::MKE_STATE::MEM_READ_SYNC:
	case MKEAnalyzer::MKE_STATE::MEM_WRITE_SYNC:
		memcpy(state_str, "SYNC", 5);
		break;

	case MKEAnalyzer::MKE_STATE::COMPLETE:
		memcpy(state_str, "COMPLETE", 9);
		break;

	default:
		memcpy(state_str, "UNKNOWN", 8);
		break;
	}

	if (frame.mFlags != 0) {
		snprintf(tab_str, 30, "ERROR");
	}
	else {
		snprintf(tab_str, 30, "%s (0b%c%c%c%c)", state_str, NIBBLE_TO_BINARY(frame.mData1));
	}
	AddTabularText(tab_str);
  */
}

void MKEAnalyzerResults::GeneratePacketTabularText(U64 /*packet_id*/, DisplayBase /*display_base*/)    //unrefereced vars commented out to remove warnings.
{
  printf("GeneratePacketTabularText\n");
    ClearResultStrings();
    AddResultString("not supported");
}

void MKEAnalyzerResults::GenerateTransactionTabularText(U64 /*transaction_id*/, DisplayBase /*display_base*/)    //unrefereced vars commented out to remove warnings.
{
  printf("GenerateTransactionTabularText\n");
    ClearResultStrings();
    AddResultString("not supported");
}
