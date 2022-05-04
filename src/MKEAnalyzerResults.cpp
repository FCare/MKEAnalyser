#include "MKEAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "MKEAnalyzer.h"
#include "MKEAnalyzerSettings.h"
#include <iostream>
#include <sstream>

#define STATUS_DOOR_CLOSED   0x80
#define STATUS_DISK_IN       0x40
#define STATUS_SPINNING      0x20
#define STATUS_CMD_ERROR     0x10
#define STATUS_AUDIO_BUSY    0x08
#define STATUS_LOCKED        0x04
#define STATUS_DOUBLE_SPEED  0x02
#define STATUS_READY         0x01

#define	ERROR_NO_ERROR	     0x00
#define	ERROR_RECV_RETRY	   0x01
#define	ERROR_RECV_ECC	     0x02
#define	ERROR_NOT_READY	     0x03
#define	ERROR_TOC_ERROR	     0x04
#define	ERROR_UNRECV_ERROR	 0x05
#define	ERROR_SEEK_ERROR	   0x06
#define	ERROR_TRACK_ERROR	   0x07
#define	ERROR_RAM_ERROR	     0x08
#define	ERROR_DIAG_ERROR	   0x09
#define	ERROR_FOCUS_ERROR	   0x0a
#define	ERROR_CLV_ERROR	     0x0b
#define	ERROR_DATA_ERROR	   0x0c
#define	ERROR_ADDRESS_ERROR	 0x0d
#define	ERROR_CDB_ERROR	     0x0e
#define	ERROR_END_ADDRESS	   0x0f
#define	ERROR_MODE_ERROR	   0x10
#define	ERROR_MEDIA_CHANGED	 0x11
#define	ERROR_HARD_RESET	   0x12
#define	ERROR_ROM_ERROR	     0x13
#define	ERROR_CMD_ERROR	     0x14
#define	ERROR_DISC_OUT	     0x15
#define	ERROR_HARD_ERROR     0x16
#define	ERROR_ILLEGAL_REQ	   0x17

MKEAnalyzerResults::MKEAnalyzerResults(MKEAnalyzer* analyzer, std::shared_ptr<MKEAnalyzerSettings> settings)
    :   AnalyzerResults(),
        mSettings(settings),
        mAnalyzer(analyzer)
{
}

MKEAnalyzerResults::~MKEAnalyzerResults()
{
}

void MKEAnalyzerResults::getCmdRequestTabular(Frame &frame) {
  U8 Cmd = (frame.mData1>>0)&0xFF;
  U8 data[7];
  std::stringstream ss;
  switch(Cmd){
    case SEEK:
      AddTabularText("CMD: SEEK");
      data[1] = (frame.mData1>>8)&0xFF;
      data[2] = (frame.mData1>>16)&0xFF;
      data[3] = (frame.mData1>>24)&0xFF;
      data[4] = (frame.mData1>>32)&0xFF;
      if (data[1] == 0x00) {
        ss << ",LBA:" << (+data[2]) << ":" << (+data[3]) << ":" << (+data[4]);
        AddTabularText(ss.str().c_str());
      }
      if (data[1] == 0x02) {
        ss << ",MSF:" << (+data[2]) << ":" << (+data[3]) << ":" << (+data[4]);
        AddTabularText(ss.str().c_str());
      }
      break;
    case SPIN_UP:
      AddTabularText("CMD: SPIN UP");
      break;
    case SPIN_DOWN:
      AddTabularText("CMD: SPIN DOWN");
      break;
    case DIAG:
      AddTabularText("CMD: DIAG");
      break;
    case STATUS:
      AddTabularText("CMD: STATUS");
      break;
    case EJECT:
      AddTabularText("CMD: EJECT");
      break;
    case INJECT:
      AddTabularText("CMD: INJECT");
      break;
    case ABORT:
      AddTabularText("CMD: ABORT");
      break;
    case SET_MODE:
      AddTabularText("CMD: SET MODE");
      break;
    case RESET:
      AddTabularText("CMD: RESET");
      break;
    case FLUSH:
      AddTabularText("CMD: FLUSH");
      break;
    case LOCK:
      AddTabularText("CMD: LOCK");
      break;
    case PAUSE_RESUME:
      AddTabularText("CMD: PAUSE/RESUME");
      break;
    case PLAY_MSF:
      AddTabularText("CMD: PLAY MSF");
      data[1] = (frame.mData1>>8)&0xFF;
      data[2] = (frame.mData1>>16)&0xFF;
      data[3] = (frame.mData1>>24)&0xFF;
      data[4] = (frame.mData1>>32)&0xFF;
      data[5] = (frame.mData1>>40)&0xFF;
      data[6] = (frame.mData1>>48)&0xFF;
      ss << ",from:" << (+data[1]) << ":" << (+data[2]) << ":" << (+data[3]);
      ss << ",to:" << (+data[4]) << ":" << (+data[5]) << ":" << (+data[6]);
      AddTabularText(ss.str().c_str());
      break;
    case PLAY_TRACK:
      AddTabularText("CMD: PLAY TRACK");
      data[1] = (frame.mData1>>8)&0xFF;
      data[2] = (frame.mData1>>16)&0xFF;
      data[3] = (frame.mData1>>24)&0xFF;
      data[4] = (frame.mData1>>32)&0xFF;
      ss << ",from:" << (+data[1]) << ":" << (+data[2]);
      ss << ",to:" << (+data[3]) << ":" << (+data[4]);
      AddTabularText(ss.str().c_str());
      break;
    case READ_DATA:
      AddTabularText("CMD: READ DATA");
      data[1] = (frame.mData1>>8)&0xFF;
      data[2] = (frame.mData1>>16)&0xFF;
      data[3] = (frame.mData1>>24)&0xFF;
      data[4] = (frame.mData1>>32)&0xFF;
      if (data[4] == 0x00) {
        ss << ",MSF:" << (+data[1]) << ":" << (+data[2]) << ":" << (+data[3]);
        AddTabularText(ss.str().c_str());
      }
      if (data[4] == 0x01) {
        ss << ",LBA:" << (+data[1]) << ":" << (+data[2]) << ":" << (+data[3]);
        AddTabularText(ss.str().c_str());
      }
      break;
    case SUBCHANNEL_INFO:
      AddTabularText("CMD: SUBCHANNEL INFO");
      break;
    case PATH_CHECK:
      AddTabularText("CMD: PATH CHECK");
      break;
    case READ_ERROR:
      AddTabularText("CMD: READ ERROR");
      break;
    case READ_ID:
      AddTabularText("CMD: READ ID");
      break;
    case MODE_SENSE:
      AddTabularText("CMD: MODE SENSE");
      break;
    case READ_CAPACITY:
      AddTabularText("CMD: READ CAPACITY");
      break;
    case READ_HEADER:
      AddTabularText("CMD: READ HEADER");
      break;
    case READ_SUB_Q:
      AddTabularText("CMD: READ SUBQ");
      break;
    case READ_UPC:
      AddTabularText("CMD: READ UPC");
      break;
    case READ_ISRC :
      AddTabularText("CMD: READ ISRC");
      break;
    case READ_DISC_CODE:
      AddTabularText("CMD: READ DISC CODE");
      break;
    case READ_DISC_INFO:
      AddTabularText("CMD: READ DISC INFO");
      break;
    case READ_TOC:
      AddTabularText("CMD: READ TOC");
      break;
    case READ_SESSION:
      AddTabularText("CMD: READ SESSION");
      break;
    case READ_DRIVER:
      AddTabularText("CMD: WTF? READ DRIVER");
      break;
    case DRIVE_RESET:
      AddTabularText("CMD: WTF? DRIVE RESET");
      break;
    default:
      AddTabularText("CMD: Unknown CMD");
      ss << " 0x";
      ss << std::hex << +Cmd;
      AddTabularText(ss.str().c_str());
      break;
  }
}

void MKEAnalyzerResults::getCmdBubble(Frame &frame) {
  std::stringstream ss;
  U8 Cmd = (frame.mData1>>0)&0xFF;
  switch(Cmd){
    case SEEK:
      AddResultString("SEEK");
      break;
    case SPIN_UP:
      AddResultString("SPIN UP");
      break;
    case SPIN_DOWN:
      AddResultString("SPIN DOWN");
      break;
    case DIAG:
      AddResultString("DIAG");
      break;
    case STATUS:
      AddResultString("STATUS");
      break;
    case EJECT:
      AddResultString("EJECT");
      break;
    case INJECT:
      AddResultString("INJECT");
      break;
    case ABORT:
      AddResultString("ABORT");
      break;
    case SET_MODE:
      AddResultString("SET MODE");
      break;
    case RESET:
      AddResultString("RESET");
      break;
    case FLUSH:
      AddResultString("FLUSH");
      break;
    case LOCK:
      AddResultString("LOCK");
      break;
    case PAUSE_RESUME:
      AddResultString("PAUSE/RESUME");
      break;
    case PLAY_MSF:
      AddResultString("PLAY MSF");
      break;
    case PLAY_TRACK:
      AddResultString("PLAY TRACK");
      break;
    case READ_DATA:
      AddResultString("WTF ? READ DATA");
      break;
    case SUBCHANNEL_INFO:
      AddResultString("SUBCHANNEL INFO");
      break;
    case PATH_CHECK:
      AddResultString("PATH CHECK");
      break;
    case READ_ERROR:
      AddResultString("READ ERROR");
      break;
    case READ_ID:
      AddResultString("READ ID");
      break;
    case MODE_SENSE:
      AddResultString("MODE SENSE");
      break;
    case READ_CAPACITY:
      AddResultString("READ CAPACITY");
      break;
    case READ_HEADER:
      AddResultString("READ HEADER");
      break;
    case READ_SUB_Q:
      AddResultString("READ SUBQ");
      break;
    case READ_UPC:
      AddResultString("READ UPC");
      break;
    case READ_ISRC :
      AddResultString("READ ISRC");
      break;
    case READ_DISC_CODE:
      AddResultString("READ DISC CODE");
      break;
    case READ_DISC_INFO:
      AddResultString("READ DISC INFO");
      break;
    case READ_TOC:
      AddResultString("READ TOC");
      break;
    case READ_SESSION:
      AddResultString("READ SESSION");
      break;
    case READ_DRIVER:
      AddResultString("WTF? READ DRIVER");
      break;
    case DRIVE_RESET:
      AddResultString("WTF? DRIVE RESET");
      break;
    default:
      ss << "Unknown CMD 0x";
      ss << std::hex << +Cmd;
      AddResultString(ss.str().c_str());
      break;
  }
}

void MKEAnalyzerResults::getCmdResponseTabular(Frame &frame) {
  U8 Cmd = (frame.mData1>>0)&0xFF;
  std::stringstream ss;
  U8 Status = (frame.mData1>>8)&0xFF;;
  switch(Cmd){
    case SEEK:
      AddTabularText("RESP: SEEK");
      getStatusString(Status);
      break;
    case SPIN_UP:
      AddTabularText("RESP: SPIN UP");
      getStatusString(Status);
      break;
    case SPIN_DOWN:
      AddTabularText("RESP: SPIN DOWN");
      getStatusString(Status);
      break;
    case DIAG:
      AddTabularText("RESP: DIAG");
      getStatusString(Status);
      break;
    case STATUS:
      AddTabularText("RESP: STATUS");
      getStatusString(Status);
      break;
    case EJECT:
      AddTabularText("RESP: EJECT");
      getStatusString(Status);
      break;
    case INJECT:
      AddTabularText("RESP: INJECT");
      getStatusString(Status);
      break;
    case ABORT:
      AddTabularText("RESP: ABORT");
      getStatusString(Status);
      break;
    case SET_MODE:
      AddTabularText("RESP: SET MODE");
      getStatusString(Status);
      break;
    case RESET:
      AddTabularText("RESP: RESET");
      break;
    case FLUSH:
      AddTabularText("RESP: FLUSH");
      getStatusString(Status);
      break;
    case LOCK:
      AddTabularText("RESP: LOCK");
      getStatusString(Status);
      break;
    case PAUSE_RESUME:
      AddTabularText("RESP: PAUSE/RESUME");
      getStatusString(Status);
      break;
    case PLAY_MSF:
      AddTabularText("RESP: PLAY MSF");
      getStatusString(Status);
      break;
    case PLAY_TRACK:
      AddTabularText("RESP: PLAY TRACK");
      getStatusString(Status);
      break;
    case READ_DATA:
      AddTabularText("RESP: WTF ? READ DATA");
      break;
    case SUBCHANNEL_INFO:
      AddTabularText("RESP: SUBCHANNEL INFO");
      getStatusString(Status);
      break;
    case PATH_CHECK:
      AddTabularText("RESP: PATH CHECK");
      getStatusString(Status);
      break;
    case READ_ERROR:
      AddTabularText("RESP: READ ERROR");
      Status = (frame.mData2>>8)&0xFF;
      getErrorString((frame.mData1>>24)&0xFF);
      getStatusString(Status);
      break;
    case READ_ID:
      AddTabularText("RESP: READ ID");
      Status = (frame.mData2>>(8*3))&0xFF;
      getStatusString(Status);
      break;
    case MODE_SENSE:
      AddTabularText("RESP: MODE SENSE");
      Status = (frame.mData1>>(8*6))&0xFF;
      getStatusString(Status);
      break;
    case READ_CAPACITY:
      AddTabularText("RESP: READ CAPACITY");
      Status = (frame.mData1>>(8*6))&0xFF;
      getStatusString(Status);
      break;
    case READ_HEADER:
      AddTabularText("RESP: READ HEADER");
      Status = (frame.mData1>>(8*5))&0xFF;
      getStatusString(Status);
      break;
    case READ_SUB_Q:
      AddTabularText("RESP: READ SUBQ");
      Status = (frame.mData2>>(8*3))&0xFF;
      getStatusString(Status);
      break;
    case READ_UPC:
      AddTabularText("RESP: READ UPC");
      Status = (frame.mData2>>(8*1))&0xFF;
      getStatusString(Status);
      break;
    case READ_ISRC :
      AddTabularText("RESP: READ ISRC");
      Status = (frame.mData2>>(8*3))&0xFF;
      getStatusString(Status);
      break;
    case READ_DISC_CODE:
      AddTabularText("RESP: READ DISC CODE");
      Status = (frame.mData2>>(8*3))&0xFF;
      getStatusString(Status);
      break;
    case READ_DISC_INFO:
      AddTabularText("RESP: READ DISC INFO");
      Status = (frame.mData1>>(8*7))&0xFF;
      getStatusString(Status);
      break;
    case READ_TOC:
      AddTabularText("RESP: READ TOC");
      Status = (frame.mData2>>(8*1))&0xFF;
      getStatusString(Status);
      break;
    case READ_SESSION:
      AddTabularText("RESP: READ SESSION");
      Status = (frame.mData1>>(8*7))&0xFF;
      getStatusString(Status);
      break;
    case READ_DRIVER:
      AddTabularText("RESP: WTF? READ DRIVER");
      break;
    case DRIVE_RESET:
      AddTabularText("RESP: WTF? DRIVE RESET");
      break;
    default:
      ss << "Unknown CMD 0x";
      ss << std::hex << +Cmd;
      AddTabularText(ss.str().c_str());
      break;
  }
}

void MKEAnalyzerResults::getStatusString(U8 data){
  std::stringstream ss;
  AddTabularText(" Status: ");
  if (data & STATUS_DOOR_CLOSED) ss << "Door closed,";
  if (data & STATUS_DISK_IN) ss << "Disk in,";
  if (data & STATUS_SPINNING) ss << "Spinning,";
  if (data & STATUS_CMD_ERROR) ss << "Command error,";
  if (data & STATUS_AUDIO_BUSY) ss << "Busy with Audio,";
  if (data & STATUS_LOCKED) ss << "Drive locked,";
  if (data & STATUS_DOUBLE_SPEED) ss << "Double speed media,";
  if (data & STATUS_READY) ss << "Ready,";
  AddTabularText(ss.str().c_str());
}

void MKEAnalyzerResults::getErrorString(U8 data){
  std::stringstream ss;
  AddTabularText("Error code:");
  switch(data) {
    case ERROR_NO_ERROR:
      AddTabularText("No error");
    break;
    case ERROR_RECV_RETRY:
      AddTabularText("Soft read error after retry");
    break;
    case ERROR_RECV_ECC:
      AddTabularText("Soft read error after error correction");
    break;
    case ERROR_NOT_READY:
      AddTabularText("Not ready");
    break;
    case ERROR_TOC_ERROR:
      AddTabularText("Cannot read TOC");
    break;
    case ERROR_UNRECV_ERROR:
      AddTabularText("Hard read error");
    break;
    case ERROR_SEEK_ERROR:
      AddTabularText("Seek failed to complete");
    break;
    case ERROR_TRACK_ERROR:
      AddTabularText("Tracking servo failure");
    break;
    case ERROR_RAM_ERROR:
      AddTabularText("Drive RAM error");
    break;
    case ERROR_DIAG_ERROR:
      AddTabularText("Self test failure");
    break;
    case ERROR_FOCUS_ERROR:
      AddTabularText("Focusing servo failure");
    break;
    case ERROR_CLV_ERROR:
      AddTabularText("Spindle servo failure");
    break;
    case ERROR_DATA_ERROR:
      AddTabularText("Data path failure");
    break;
    case ERROR_ADDRESS_ERROR:
      AddTabularText("Illegal logical block address (LBA)");
    break;
    case ERROR_CDB_ERROR:
      AddTabularText("Illegal field in CDB");
    break;
    case ERROR_END_ADDRESS:
      AddTabularText("End of user encountered on track");
    break;
    case ERROR_MODE_ERROR:
      AddTabularText("Illegal data mode for track");
    break;
    case ERROR_MEDIA_CHANGED:
      AddTabularText("Media changed");
    break;
    case ERROR_HARD_RESET:
      AddTabularText("Power-on or reset occurred");
    break;
    case ERROR_ROM_ERROR:
      AddTabularText("Drive ROM failure");
    break;
    case ERROR_CMD_ERROR:
      AddTabularText("Illegal drive command");
    break;
    case ERROR_DISC_OUT:
      AddTabularText("Disc removed during operation");
    break;
    case ERROR_HARD_ERROR:
      AddTabularText("Drive hardware error");
    break;
    case ERROR_ILLEGAL_REQ:
      AddTabularText("Illegal request");
    break;
    default:
      ss << "unknown code: 0x" << std::hex << +data;
      AddTabularText(ss.str().c_str());
  }
}

void MKEAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base)    //unrefereced vars commented out to remove warnings.
{
  //we only need to pay attention to 'channel' if we're making bubbles for more than one channel (as set by AddChannelBubblesWillAppearOn)
  Frame frame = GetFrame(frame_index);
  ClearResultStrings();
  if ((channel == mSettings->mChannel[CDHRD]) && !(frame.mFlags & READ_WRITE_FLAG)){
    getCmdBubble(frame);
  }
  if ((channel == mSettings->mChannel[CDHWR]) && (frame.mFlags & READ_WRITE_FLAG)){
    getCmdBubble(frame);
  }

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

  Frame frame = GetFrame(frame_index);
  bool is_write_frame = false;
  if ((frame.mFlags & READ_WRITE_FLAG) != 0) {
      is_write_frame = true;
  }
  ClearTabularText();
  if (is_write_frame){
    getCmdRequestTabular(frame);
  } else {
    getCmdResponseTabular(frame);
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
